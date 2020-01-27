#include <vector>
#include <random>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include"../include/aco.hpp"
#include"../include/benchmarking.hpp"
#include"../main/auxiliar.cpp"

using namespace std;

extern string nameArq;

ACO::ACO(int k, double q,double evap, int dim, int col, int stepsMax , double max, double min, string tf){
  		this->k = k;
  		this->q = q;
  		this->evaporation = evap;
  		this->dim = dim;
      this->col = col;
      this->stepsMax = stepsMax;
      this->max = max;
      this->min = min;
      this->typeFunction = tf;
      this->global_best = 0;
}

double ACO::getGlobalBest(){
      return this->global_best;
}
  /* Iniciliza Matriz de Solução  com a colônia aleatoria inicial */
void ACO::initSolution(){
  for(int i = 0; i < this->k;  i++) {
            Ant a(this->dim, this->max, this->min);
            this->solution.push_back(a);
  }
}

/* Inicializa as Formigas Aleatóriamente pelo espaço de Busca */
void ACO::initAnts(){
    for(int i = 0; i < this->col;  i++) {
              Ant a(this->dim, this->max, this->min);
              this->colony.push_back(a);
    }
}
/* Avalia o custo (Função Objetivo) de cada Formiga da Colônia*/
void ACO::costAnt(double (*costFunction)(vector<double> )) {
  for(int i = 0; i < this->col; i++){
        this->colony[i].setCost(costFunction(this->colony[i].position));
  }
}

/* Verifica  a melhor solução que está no Arquivo */
void ACO::memorizeBest(double (*costFunction)(vector<double> )) {
  for(int i = 0; i < this->k; i++){
        /* Calcula a Função Objetivo*/
        this->solution[i].setCost(costFunction(this->solution[i].position));
  }
  /* Verifica a melhor Função Objetivo */
  for(int i = 0; i < this->k; i++){
          		if (this->solution[i].getCost() < this->global_best) {
                  		this->global_best = this->solution[i].getCost();
              }
  }
}

void ACO::evaporationPheromone(){
  // Insere as novas Soluções no Arquivo
  for(int i = 0; i < this->col; i++){
    this->solution.push_back(this->colony[i]);
  }
  // Ordena as Soluções de acordo com a Função Objetivo
  sort(this->solution.begin(), this->solution.end(), [](Ant a, Ant b) {
       return (a.getCost() < b.getCost());
  });
  // Remove as Piores Soluções
  while( this->solution.size() < this->k){
        this->solution.pop_back();
  }
}

// Retorna um indice de acordo com a probabilidade
int ACO::selectionAnt(){
  double s=0, r=0;
  random_device rd;
  mt19937 gen(rd());
  for(int i=0; i < this->col; i++){
      s += this->colony[i].getP();
  }
  uniform_real_distribution<> dis(0,s);
  r = dis(gen);
  for(int i=0; i < this->col; i++){
        r -=  this->colony[i].getP();
        if(r <= 0){
            return i;
        }
    }
    return 1;
}

void ACO::advanceAnt( double (*costFunction)(vector<double> )){
  int l = 0;
  double sigma_sum = 0, sigma = 0;
  random_device rd;
  mt19937 gen(rd());

  for(int i = 0 ; i < this->col; i++){
        l = selectionAnt();
         for(int j = 0; j < this->dim; j++){
                 sigma_sum = 0;
                for(int t = 0; t < this->k; t++){
                      sigma_sum += abs(this->solution[t].position[j] - this->solution[l].position[j]);
                }

              sigma = this->evaporation * (sigma_sum / (this->k - 1));
              uniform_real_distribution<> dist(this->solution[l].position[j], sigma);
              this->colony[i].position[j] = dist(gen);

              /* Verifica os Limites da Nova Solução */
              if ((this->colony[i].position[j] < this->min)  || (this->colony[i].position[j] > this->max)) {
                  uniform_real_distribution<> dis(this->min, this->max);
                  this->colony[i].position[j] = dis(gen);
              }
        }
        this->colony[i].setCost(costFunction(this->colony[i].position));
  }

}

void ACO::calculateW( ){
    double x=0, y = 0, z = 0;
    for(int l = 0; l < this->col; l++){
            y = (- pow((l - 1), 2)) / (2 * this->k * this->q);
            x = (1 / (this->q * this->k * sqrt(2 * M_PI)));
            z = pow( x, y);
            this->colony[l].setW(z);
    }
}

void ACO::calculateProbabilitie( ){
      double sum = 0;
      for(int l = 0; l < this->col; l++){
          sum += this->colony[l].getW();
      }
      for(int l = 0; l < this->col; l++){
          this->colony[l].setP(this->colony[l].getW() / sum);
      }
}

void ACO::printF(){
  for(int i=0; i <this->k; i++){
      for(int j=0; j <this->k; j++){
    cout << this->solution[i].position[j] << endl;
  }
  }
}

void ACO::runDetails(double (*costFunction)(vector<double> )){
          ofstream arq;
          arq.open("Inputs/selectionAnt" + nameArq + ".txt", ios::ate);
          ofstream archiveAnt;
          archiveAnt.open("Inputs/advanceAnt" + nameArq + ".txt", ios::ate);
          if(!arq || !archiveAnt){
            cout << "Erro Leitura de Arquivo" << endl;
            exit(0);
          }
          // initAnts(); /* inicializa as formigas */
           initAntsDET(); /* inicializa as formigas */
           costAnt(costFunction); /* Iniciliza o custo de cada Formiga */
           calculateW(); /* Calcula o Peso W  das Soluções as Formigas*/
           calculateProbabilitie(); /* Calcula a Probabilidade de Cada formiga*/

          // initSolution(); /* inicializa arquivo de soluções com a colonia inicial */
           initSolutionDET();
           sort(this->solution.begin(), this->solution.end(), [](Ant a, Ant b) {
		            return (a.getCost() < b.getCost());
              });
           this->global_best = costFunction(this->colony[0].position); /* Iniciliza o Melhor Custo */

            memorizeBest(costFunction);

            for(int i = 0; i <this->stepsMax; i++){
                //  advanceAnt(costFunction);
                  advanceAntDET(costFunction, archiveAnt, arq);
                  calculateW();
                  calculateProbabilitie();
                  evaporationPheromone();
                  memorizeBest(costFunction); //cout << i << endl;
         }
          arq.close();
          archiveAnt.close();
}

void ACO::run(double (*costFunction)(vector<double> )){
  ifstream arq;
  arq.open("Inputs/selectionAnt" + nameArq + ".txt", ios::in);
  ifstream archiveAnt;
  archiveAnt.open("Inputs/advanceAnt" + nameArq + ".txt", ios::in);
  if(!arq || !archiveAnt){
    cout << "Erro Leitura de Arquivo" << endl;
    exit(0);
  }

  initValuesAnt(); /* inicializa as formigas */
  costAnt(costFunction); /* Iniciliza o custo de cada Formiga */
  calculateW(); /* Calcula o Peso W  das Soluções as Formigas*/
  calculateProbabilitie(); /* Calcula a Probabilidade de Cada formiga*/

 // initSolution(); /* inicializa arquivo de soluções com a colonia inicial */
  initSolutionMOD();
  sort(this->solution.begin(), this->solution.end(), [](Ant a, Ant b) {
       return (a.getCost() < b.getCost());
     });
  this->global_best = costFunction(this->colony[0].position); /* Iniciliza o Melhor Custo */

   memorizeBest(costFunction);

   for(int i = 0; i <this->stepsMax; i++){
         advanceAntMOD(costFunction, archiveAnt, arq);
         calculateW();
         calculateProbabilitie();
         evaporationPheromone();
         memorizeBest(costFunction); //cout << i << endl;
 }
  arq.close();
  archiveAnt.close();
}

void ACO::runParallel(double (*costFunction)(vector<double> )){
  ifstream arq;
  arq.open("Inputs/selectionAnt" + nameArq + ".txt", ios::in);
  ifstream archiveAnt;
  archiveAnt.open("Inputs/advanceAnt" + nameArq + ".txt", ios::in);
  if(!arq || !archiveAnt){
    cout << "Erro Leitura de Arquivo" << endl;
    exit(0);
  }

  initValuesAnt(); /* inicializa as formigas */
  costAnt(costFunction); /* Iniciliza o custo de cada Formiga */
  calculateWP(); /* Calcula o Peso W  das Soluções as Formigas*/
  calculateProbabilitieP(); /* Calacula a Probabilidade de Cada formiga*/

  initSolutionMOD();

  sort(this->solution.begin(), this->solution.end(), [](Ant a, Ant b) {
       return (a.getCost() < b.getCost());
     });
  this->global_best = costFunction(this->colony[0].position); /* Iniciliza o Melhor Custo */

   memorizeBest(costFunction);

   for(int i = 0; i <this->stepsMax; i++){
         advanceAntMODP(costFunction, archiveAnt, arq);
         calculateWP();
         calculateProbabilitieP();
         evaporationPheromone();
         memorizeBest(costFunction); //cout << i << endl;
 }
  arq.close();
  archiveAnt.close();
}
