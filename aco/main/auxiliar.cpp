#include <iostream>
#include <omp.h>
#include <vector>
#include <math.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <omp.h>
#include"../include/aco.hpp"

using namespace std;

extern string nameArq;

#pragma omp declare simd
void ACO::calculateWP( ){
    double x=0, y = 0, z = 0;
    //#pragma omp simd private(x, y, z)
    for(int l = 0; l < this->col; l++){
            y = (- pow((l - 1), 2)) / (2 * this->k * this->q);
            x = (1 / (this->q * this->k * sqrt(2 * M_PI)));
            z = pow( x, y);
            this->colony[l].setW(z);
    }
}

#pragma omp declare simd
void ACO::calculateProbabilitieP( ){
      double sum = 0;
    // #pragma omp simd reduction(+: sum)
      for(int l = 0; l < this->col; l++){
          sum += this->colony[l].getW();
      }
      ///             -> //#pragma omp simd
      for(int l = 0; l < this->col; l++){
          this->colony[l].setP(this->colony[l].getW() / sum);
      }
}
void ACO::initAntsDET(){
  ofstream archiveAnt;
  archiveAnt.open("Inputs/valuesAntInit" + nameArq + ".txt", ios::ate);
    for(int i = 0; i < this->col;  i++) {
              Ant a(this->dim, this->max, this->min, archiveAnt);
              this->colony.push_back(a);
    }
    archiveAnt.close();
}

void ACO::initValuesAnt(){
  ifstream archiveAnt;
  archiveAnt.open("Inputs/valuesAntInit" + nameArq + ".txt");
  for (int i =0; i<this->col; i++){
      Ant a(this->dim, this->max, this->min, archiveAnt);
      this->colony.push_back(a);
  }
    archiveAnt.close();
}

void ACO::initSolutionDET(){
  ofstream archiveAnt;
  archiveAnt.open("Inputs/valuesSolution" + nameArq + ".txt", ios::ate);
  for(int i = 0; i < this->k;  i++) {
            Ant a(this->dim, this->max, this->min, archiveAnt);
            this->solution.push_back(a);
  }
  archiveAnt.close();
}

void ACO::initSolutionMOD(){
  ifstream archiveAnt;
  archiveAnt.open("Inputs/valuesSolution" + nameArq + ".txt");
  for(int i = 0; i < this->k;  i++) {
            Ant a(this->dim, this->max, this->min, archiveAnt);
            this->solution.push_back(a);
  }
  archiveAnt.close();
}

int ACO::selectionAntDET(ostream& arq){
  double s=0, r=0;
  random_device rd;
  mt19937 gen(rd());
  for(int i=0; i < this->col; i++){
      s += this->colony[i].getP();
  }
  uniform_real_distribution<> dis(0,s);
  r = dis(gen);
  arq << r << endl;

  for(int i=0; i < this->col; i++){
        r -=  this->colony[i].getP();
        if(r <= 0){
            return i;
        }
    }
    return 1;
}

int ACO::selectionAntMOD(istream& arq){
  double s=0, r=0;
  for(int i=0; i < this->col; i++){
      s += this->colony[i].getP();
  }
  arq >> r;
  //r = dis(gen);
  for(int i=0; i < this->col; i++){
        r -=  this->colony[i].getP();
        if(r <= 0){
            return i;
        }
    }
    return 1;
}

int ACO::selectionAntMODP(istream& arq){
  double s=0, r=0;
  //#pragma omp simd reduction(+: s)
  for(int i=0; i < this->col; i++){
      s += this->colony[i].getP();
  }
  arq >> r;
  for(int i=0; i < this->col; i++){
        r -=  this->colony[i].getP();
        if(r <= 0){
            return i;
        }
    }
    return 1;
}
void ACO::advanceAntDET( double (*costFunction)(vector<double> ), ostream& arq, ostream& arqS){
  int l = 0;
  double sigma_sum = 0, sigma = 0, r=0;
  random_device rd;
  mt19937 gen(rd());

  for(int i = 0 ; i < this->col; i++){
        l = selectionAntDET(arqS);
         for(int j = 0; j < this->dim; j++){
                 sigma_sum = 0;
                for(int t = 0; t < this->k; t++){
                      sigma_sum += abs(this->solution[t].position[j] - this->solution[l].position[j]);
                }

              sigma = this->evaporation * (sigma_sum / (this->k - 1));
              uniform_real_distribution<> dist(this->solution[l].position[j], sigma);
              r = dist(gen);
              this->colony[i].position[j] = r;
              arq << r << endl;

              /* Verifica os Limites da Nova Solução */
              if ((this->colony[i].position[j] < this->min)  || (this->colony[i].position[j] > this->max)) {
                  uniform_real_distribution<> dis(this->min, this->max);
                  r = dist(gen);
                  this->colony[i].position[j] = r;
                  arq << r << endl;
              }
        }
        this->colony[i].setCost(costFunction(this->colony[i].position));
  }
}

void ACO::advanceAntMOD( double (*costFunction)(vector<double> ), istream& arq, istream& arqS){
  int l = 0;
  double sigma_sum = 0, sigma = 0;

  for(int i = 0 ; i < this->col; i++){
        l = selectionAntMOD(arqS);
         for(int j = 0; j < this->dim; j++){
                 sigma_sum = 0;
                for(int t = 0; t < this->k; t++){
                      sigma_sum += abs(this->solution[t].position[j] - this->solution[l].position[j]);
                }

              sigma = this->evaporation * (sigma_sum / (this->k - 1));
              arq >> this->colony[i].position[j]; //= dist(gen);

              /* Verifica os Limites da Nova Solução */
              if ((this->colony[i].position[j] < this->min)  || (this->colony[i].position[j] > this->max)) {
                //  uniform_real_distribution<> dis(this->min, this->max);
                  arq >> this->colony[i].position[j] ; //= dis(gen);
              }
        }
        this->colony[i].setCost(costFunction(this->colony[i].position));
  }
}

void ACO::advanceAntMODP( double (*costFunction)(vector<double> ), istream& arq, istream& arqS){
  int l = 0;
  double sigma_sum = 0, sigma = 0;

  for(int i = 0 ; i < this->col; i++){
        l = selectionAntMODP(arqS);
         for(int j = 0; j < this->dim; j++){
                 sigma_sum = 0;
                // #pragma omp simd reduction(+: sigma_sum)
                for(int t = 0; t < this->k; t++){
                      sigma_sum += abs(this->solution[t].position[j] - this->solution[l].position[j]);
                }

              sigma = this->evaporation * (sigma_sum / (this->k - 1));
              arq >> this->colony[i].position[j]; //= dist(gen);

              /* Verifica os Limites da Nova Solução */
              if ((this->colony[i].position[j] < this->min)  || (this->colony[i].position[j] > this->max)) {
                //  uniform_real_distribution<> dis(this->min, this->max);
                  arq >> this->colony[i].position[j] ; //= dis(gen);
              }
        }
        this->colony[i].setCost(costFunction(this->colony[i].position));
  }
}
