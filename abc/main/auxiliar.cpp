#include <iostream>
#include <omp.h>
#include <vector>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <omp.h>
#include"../include/abc.hpp"

using namespace std;

extern string nameArq;

void ABC::initBeesDet(){ /*Gera Arquivo de Saída */
   ofstream archiveBee;
   archiveBee.open("Inputs/valuesBeeInit" + nameArq + ".txt", ios::ate);
        for (int i =0; i<this->colony; i++){
            Bee B(this->dim, this->max, this->min, archiveBee);
            this->swarm.push_back(B);
        }
   archiveBee.close();
}

void ABC::initValuesBees(){
  ifstream archiveBee;
  archiveBee.open("Inputs/valuesBeeInit" + nameArq + ".txt");
  for (int i =0; i<this->colony; i++){
      Bee B(this->dim, this->max, this->min, archiveBee);
      this->swarm.push_back(B);
  }
    archiveBee.close();
}

/* Uma fonte de alimento é escolhida com a probabilidade proporcional à sua qualidade */
void ABC::calculateProbabilitiesDET(ostream& arq) {
    double fitn= this->swarm[0].getFit();
  	for ( int i = 1; i < this->SN; i++) {
        if(this->swarm[i].getFit() > fitn){
            fitn = this->swarm[i].getFit();
        }
  	}
  	for ( int i = 0; i < this->SN; i++){
		    this->swarm[i].setProbabilities((0.9 * (this->swarm[i].getFit() / fitn))+0.1);
        arq <<  this->swarm[i].getProbabilitie() << endl;
  	}
}

void ABC::calculateProbabilitiesMOD(istream& arq) {
  long double r=0;
  	for ( int i = 0; i < this->SN; i++){
        arq >> r ;
        this->swarm[i].setProbabilities(r);
  	}
}

void ABC::sendTrackBeesDET(double (*costFunction)(vector<double> ),  ostream& archiveBee){
  int indiceJ=0, k=0, t=0;
  long double valueFunction=0, valueFitness = 0;
  long double r=0;
  vector<double> newSolution;
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0,1);
  int i = 0;
while( t< this->SN){
     r = dis(gen);
     archiveBee << r << endl;
    if(r < this->swarm[i].getProbabilitie()) { /*escolha uma fonte de alimento dependendo da probabilidade*/
                    t++;
                    r = dis(gen); //cout <<  this->swarm[i].getProbabilitie() << endl;
                    archiveBee << r << endl;
                    indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/
                     do{   /* A solução k deve ser diferente da solução i */
                            r = dis(gen);
                            k = (int)(r*this->SN);
                      }while(k == i);
                      archiveBee << r << endl;
                      newSolution = this->swarm[i].position; /* solução  mutante */
                      r = dis(gen);
                      archiveBee << r << endl;  /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
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
        }else{
              i++;
              if(i ==this->SN){
                  i = 0;
              }
        }/* end else */
    }/* end while */
}

void ABC::sendTrackBeesMOD(double (*costFunction)(vector<double> ), istream& archive){
  int indiceJ=0, k=0, i=0, t=0;
  long  double valueFunction=0, valueFitness = 0;
  long double r=0;
  vector<double> newSolution;

  while( t< this->SN){
      archive >> r;
    	if(r < this->swarm[i].getProbabilitie()) { /*escolha uma fonte de alimento dependendo da probabilidade*/
                      t++;
                      archive >> r; //cout <<  this->swarm[i].getProbabilitie() << endl;
                      indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/
                      archive >> r;
                      k = (int)(r*this->SN);
                      newSolution = this->swarm[i].position; /* solução  mutante */
                      archive >> r;
                      newSolution[indiceJ] = this->swarm[i].position[indiceJ] + (( this->swarm[i].position[indiceJ] - this->swarm[k].position[indiceJ]) * ((r  - 0.5) * 2));
                        /* se o valor do parâmetro gerado estiver fora dos limites, ele é deslocado para os limites */
                      if (newSolution[indiceJ] < min){
                              newSolution[indiceJ] = min;
                      }else if (newSolution[indiceJ] > max){
                              newSolution[indiceJ] = max;
                      }
                      valueFunction = costFunction(newSolution); /* Valor da Função Objetivo */
                      valueFitness = CalculateFitness(valueFunction); /* Valor de Fitness */
                      /* Verifica se o Fitness da Solução Mutante é melhor que da Atual Solução  */
                      if (valueFitness > this->swarm[i].getFit()) {//cout << "i " << i << endl;
                                  this->swarm[i].updateClean(); /* Zerar o contador de teste */
                                  this->swarm[i].position = newSolution; /* Atualizar Solução i pela Mutante */
                                  this->swarm[i].setFit(valueFitness);
                                  this->swarm[i].setF(valueFunction);
                      } else {   /* Se a solução mutante é pior incrementa o contador de teste */
                                  this->swarm[i].updateIncrement();
                      }
		      }else{
                i++;
                if(i ==this->SN){
                    i = 0;
                }
          }
      }// end while
}

void ABC::sendScoutBeesDET(double (*costFunction)(vector<double> ), ostream& arq) {
	int index = 0;
	for (int i = 0; i < this->SN; i++) {
    		if (this->swarm[i].getUpdate() > this->swarm[index].getUpdate()){
              index = i;
        }
  }
  long double fit = 0, val = 0;
  if(this->swarm[index].getUpdate() >= this->lim) {
          Bee B(this->dim, this->max, this->min, arq);
          this->swarm[index].newBee(B.position);
           val  = costFunction(this->swarm[index].position);
           this->swarm[index].setF(val);
           fit = CalculateFitness(val);
           this->swarm[index].setFit(fit);
  }
}
void ABC::sendScoutBeesMOD(double (*costFunction)(vector<double> ), istream& arq){
  int index = 0;
	for (int i = 0; i < this->SN; i++) {
    		if (this->swarm[i].getUpdate() > this->swarm[index].getUpdate()){
              index = i;
        }
  }
    long double fit = 0, val = 0;
    if(this->swarm[index].getUpdate() >= this->lim) {
          Bee B(this->dim, this->max, this->min, arq);
          this->swarm[index].newBee(B.position);
           val  = costFunction(this->swarm[index].position);
           this->swarm[index].setF(val);
           fit = CalculateFitness(val);
           this->swarm[index].setFit(fit);
    }
}


void ABC::sendBeesDET(double (*costFunction)(vector<double> ), ostream& arq) {
  int indiceJ=0, k=0;
  long double valueFunction=0, valueFitness = 0;
  long double r=0;
  vector<double> newSolution;
  random_device rd;
  mt19937 gen(rd());
  uniform_real_distribution<> dis(0,1);

 for (int i = 0; i < this->SN; i++) {
      r = dis(gen);
      indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/
      arq << r << endl;

       do{   /* A solução k deve ser diferente da solução i */
             r = dis(gen);
             k = (int)(r*this->SN);
        }while(k == i);
        arq << r << endl;

        newSolution = this->swarm[i].position; /* solução  mutante */

        /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
        r = dis(gen);
        newSolution[indiceJ] = this->swarm[i].position[indiceJ] + (( this->swarm[i].position[indiceJ] - this->swarm[k].position[indiceJ]) * (( r - 0.5) * 2));
        arq << r << endl;

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

void ABC::sendBeesMOD(double (*costFunction)(vector<double> ),  istream& arq) {
  int indiceJ=0, k=0;
  long double valueFunction=0, valueFitness = 0;
  long double r=0;
  vector<double> newSolution;

 for ( int i = 0; i < this->SN; i++) {
      arq >> r;
      indiceJ = (int)(r*this->dim);   /* O indice J da equação é determinado aleatoriamente*/

      arq >> r;
      k = (int)(r*this->SN);

      newSolution = this->swarm[i].position; /* solução  mutante */

        /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
      arq >> r;
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

void ABC::calculateProbabilitiesParallel() {
    double fitn= this->swarm[0].getFit();
    for ( int i = 1; i < this->SN; i++) {
        if(this->swarm[i].getFit() > fitn){
            fitn = this->swarm[i].getFit();
        }
    }
    #pragma omp simd
    for ( int i = 0; i < this->SN; i++){
        this->swarm[i].setProbabilities((0.9 * (this->swarm[i].getFit() / fitn))+0.1);
    }
}
