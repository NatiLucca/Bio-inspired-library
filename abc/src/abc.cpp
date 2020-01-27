#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <omp.h>
#include"../include/abc.hpp"
#include"../main/benchmarking.hpp"
#include"../main/auxiliar.cpp"

using namespace std;

extern string nameArq;

ABC::ABC(int dim, int col, int lim, int sn, int stepsMax , double max, double min){
  this->dim = dim;
  this->colony = col;
  this->BN = col/2;
  this->SN = sn;
  this->BC = col - sn;
  this->lim = lim;
  this->stepsMax = stepsMax;
  this->global_best = 0;
  this->max = max;
  this->min = min;
}

double ABC::getGlobalBest(){
      return this->global_best;
}

void ABC::initBees(){
  /* Inicializa todas as abelhas em posições aleatorias */
        for (int i =0; i<this->colony; i++){
            Bee B(this->dim, this->max, this->min);
            this->swarm.push_back(B);
        }
}

void ABC::printBees(){
  /* Inicializa todas as abelhas em posições aleatorias */
        for (int i =0; i<this->SN; i++){
            for(int j=0; j<this->dim; j++){
                  cout << (int) this->swarm[i].position[j] << "  " ;
            }
            cout << endl;
        }
}

long double ABC::CalculateFitness(double fun) {
	 long double result=0;
	 if(fun >= 0){
		 result = 1 / (fun+1);
	 }else{
		 result = 1 + fabs(fun);
	 }
	 return result;
}

/* Verifica  a melhor solução calculada pelas abelha campeiras*/
void ABC::memorizeBest() {
  	for(int i = 0; i < this->SN; i++){
          		if (this->swarm[i].getF() < this->global_best) {
                  		this->global_best = this->swarm[i].getF();
              }
  	}
}

void ABC::sendTrackBees(double (*costFunction)(vector<double> )){
    int indiceJ=0, k=0, t=0;
    double valueFunction=0, valueFitness = 0, r=0;
    vector<double> newSolution;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0,1);
    int i = 0;
    for(int t = 0; t < this->SN; t++){
  	//while(t < this->SN) {
  		 r = dis(gen);
    	if(r < this->swarm[i].getProbabilitie()) { /*escolha uma fonte de alimento dependendo da probabilidade*/
                  		//t++;
                      r = dis(gen);
                      indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/

                       do{   /* A solução k deve ser diferente da solução i */
                              r = dis(gen);
                              k = (int)(r*this->SN);
                        }while(k == i);

                        newSolution = this->swarm[i].position; /* solução  mutante */

                        /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
                        r = dis(gen);
                        newSolution[indiceJ] = this->swarm[i].position[indiceJ] + (( this->swarm[i].position[indiceJ] - this->swarm[k].position[indiceJ]) * (( r - 0.5) * 2));

                        /* se o valor do parâmetro gerado estiver fora dos limites, ele é deslocado para os limites */
                        if (newSolution[indiceJ] < min){
                              newSolution[indiceJ] = min;
                        }else if (newSolution[indiceJ] > max){
                              newSolution[indiceJ] = max;
                        }

                        valueFunction = costFunction(newSolution); /* Valor da Função Objetivo */
                        valueFitness = CalculateFitness(valueFunction); /* Valor de Fitness */

                        /* Verifica se o Fitness da Solução Mutante é melhor que da Atual Solução  */
                        if (valueFitness > this->swarm[i].getFit()) {
                                    this->swarm[i].updateClean(); /* Zerar o contador de teste */
                                    this->swarm[i].position = newSolution; /* Atualizar Solução i pela Mutante */
                                    this->swarm[i].setFit(valueFitness);
                                    this->swarm[i].setF(valueFunction);
                        } else {   /* Se a solução mutante é pior incrementa o contador de teste */
                                    this->swarm[i].updateIncrement();
                        }
		      }/* end if*/
          i++;
          if(i ==this->SN){
              i = 0;
          }
      }// end for
}

/*determinar as fontes estagnadas */
/*determine as fontes de alimentos cujo contador experimental excede o valor "limite".*/
void ABC::sendScoutBees(double (*costFunction)(vector<double> )) {
	int index = 0;
	for (int i = 0; i < this->SN; i++) {
    		if (this->swarm[i].getUpdate() > this->swarm[index].getUpdate()){
              index = i;
        }
  }
  long double fit = 0, val = 0;
  if(this->swarm[index].getUpdate() >= this->lim) {
          Bee B(this->dim, this->max, this->min);
          this->swarm[index].newBee(B.position);
           val  = costFunction(this->swarm[index].position);
           this->swarm[index].setF(val);
           fit = CalculateFitness(val);
           this->swarm[index].setFit(fit);
  }
}

   /*Solução escolhida aleatoriamente é usada na produção de uma solução mutante da solução i */
void ABC::sendBees(double (*costFunction)(vector<double> )) {
  int indiceJ=0, k=0;
  double valueFunction=0, valueFitness = 0, r=0;
  vector<double> newSolution;
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0,1);

	for ( int i = 0; i < this->SN; i++) {

      r = dis(gen);
      indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/

       do{   /* A solução k deve ser diferente da solução i */
            	r = dis(gen);
            	k = (int)(r*this->SN);
        }while(k == i);

        newSolution = this->swarm[i].position; /* solução  mutante */

        /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
        r = dis(gen);
        newSolution[indiceJ] = this->swarm[i].position[indiceJ] + (( this->swarm[i].position[indiceJ] - this->swarm[k].position[indiceJ]) * (( r - 0.5) * 2));

        /* se o valor do parâmetro gerado estiver fora dos limites, ele é deslocado para os limites */
        if (newSolution[indiceJ] < min){
              newSolution[indiceJ] = min;
        }else if (newSolution[indiceJ] > max){
              newSolution[indiceJ] = max;
        }

        valueFunction = costFunction(newSolution); /* Valor da Função Objetivo */
        valueFitness = CalculateFitness(valueFunction); /* Valor de Fitness */

        /* Verifica se o Fitness da Solução Mutante é melhor que da Atual Solução  */
        if (valueFitness > this->swarm[i].getFit()) {
                    this->swarm[i].updateClean(); /* Zerar o contador de teste */
                    this->swarm[i].position = newSolution; /* Atualizar Solução i pela Mutante */
                    this->swarm[i].setFit(valueFitness);
                    this->swarm[i].setF(valueFunction);
        } else {   /* Se a solução mutante é pior incrementa o contador de teste */
                    this->swarm[i].updateIncrement();
        }
  }
}

/* Uma fonte de alimento é escolhida com a probabilidade proporcional à sua qualidade */
void ABC::calculateProbabilities() {
    double fitn= this->swarm[0].getFit();
  	for ( int i = 1; i < this->SN; i++) {
        if(this->swarm[i].getFit() > fitn){
            fitn = this->swarm[i].getFit();
        }
  	}
  	for ( int i = 0; i < this->SN; i++){
		    this->swarm[i].setProbabilities((0.9 * (this->swarm[i].getFit() / fitn))+0.1);
  	}
}

void ABC::run(double (*costFunction)(vector<double> )){
  ifstream arq;
  arq.open("Inputs/sendBees" + nameArq + ".txt", ios::in);
  ifstream archiveBee;
  archiveBee.open("Inputs/sendTrackBees" + nameArq + ".txt", ios::in);
  ifstream arqS;
  arqS.open("Inputs/scoutBee" + nameArq + ".txt", ios::in);
  if(!arq || !archiveBee || !arqS){
    cout << "Erro Leitura de Arquivo" << endl;
    exit(0);
  }

      //initBees();
      initValuesBees();

      /* iniciliza o melhor solução global*/
      this->global_best = costFunction(this->swarm[0].position);

      long double cust = 0,valueFitness=0;
      for (int i =0; i<this->SN; i++){
                 cust = costFunction(this->swarm[i].position);
                 valueFitness = CalculateFitness(cust); /* Valor de Fitness */
                 this->swarm[i].setF(cust);  /* inicializa o valor da função objetivo */
                 this->swarm[i].setFit(valueFitness);  /* inicializa o fitness */
       }

      /* Percorre a colonia e verifica o menor valor encontrado na função objetivo */
      memorizeBest();

  for (int i=0; i < this->stepsMax; i++){ /* repete tantas iterações */
              sendBeesMOD(costFunction, arq);
              calculateProbabilities();
              sendTrackBeesMOD(costFunction, archiveBee);
              memorizeBest();
             sendScoutBeesMOD(costFunction, arqS);
    }/* end for steps */
    arq.close();
    archiveBee.close();
}

void ABC::runDetails(double (*costFunction)(vector<double> )){
  ofstream arq;
  arq.open("Inputs/sendBees" + nameArq + ".txt", ios::ate);
  ofstream archiveBee;
  archiveBee.open("Inputs/sendTrackBees" + nameArq + ".txt", ios::ate);
  ofstream arqS;
  arqS.open("Inputs/scoutBee" + nameArq + ".txt", ios::ate);
  if(!arq || !archiveBee || !arqS){
    cout << "Erro Leitura de Arquivo" << endl;
    exit(0);
  }

  initBeesDet();

      /* iniciliza o melhor solução global*/
  this->global_best = costFunction(this->swarm[0].position);

  long double cust = 0,valueFitness=0;
  for (int i =0; i<this->SN; i++){
             cust = costFunction(this->swarm[i].position);
             valueFitness = CalculateFitness(cust); /* Valor de Fitness */
             this->swarm[i].setF(cust);  /* inicializa o valor da função objetivo */
             this->swarm[i].setFit(valueFitness);  /* inicializa o fitness */
   }

      /* Percorre a colonia e verifica o menor valor encontrado na função objetivo */
  memorizeBest();

  for (int i=0; i < this->stepsMax; i++){ /* repete tantas iterações */
                 sendBeesDET(costFunction, arq);
         			   //calculateProbabilitiesDET(arqProb);
                 calculateProbabilities();
        			  sendTrackBeesDET(costFunction, archiveBee);
                 memorizeBest();
                 sendScoutBeesDET(costFunction, arqS);
  }/* end for steps */
  arq.close();
  archiveBee.close();
}

void ABC::runParallel(double (*costFunction)(vector<double> )){
  ifstream arq;
  arq.open("Inputs/sendBees" + nameArq + ".txt", ios::in);
  ifstream archiveBee;
  archiveBee.open("Inputs/sendTrackBees" + nameArq + ".txt", ios::in);
  ifstream arqS;
  arqS.open("Inputs/scoutBee" + nameArq + ".txt", ios::in);
  if(!arq || !archiveBee || !arqS){
    cout << "Erro Leitura de Arquivo" << endl;
    exit(0);
  }

  initValuesBees();

      /* iniciliza o melhor solução global*/
  this->global_best = costFunction(this->swarm[0].position);

  long double cust = 0,valueFitness=0;
  #pragma omp simd
  for (int i =0; i<this->SN; i++){
                 cust = costFunction(this->swarm[i].position);
                 valueFitness = CalculateFitness(cust); /* Valor de Fitness */
                 this->swarm[i].setF(cust);  /* inicializa o valor da função objetivo */
                 this->swarm[i].setFit(valueFitness);  /* inicializa o fitness */
  }

      /* Percorre a colonia e verifica o menor valor encontrado na função objetivo */
  memorizeBest();

  for (int i=0; i < this->stepsMax; i++){ /* repete tantas iterações */
              sendBeesMOD(costFunction, arq);
              calculateProbabilitiesParallel();
              sendTrackBeesMOD(costFunction, archiveBee);
              memorizeBest();
              sendScoutBeesMOD(costFunction, arqS);
    }/* end for steps */
    arq.close();
    archiveBee.close();
}
