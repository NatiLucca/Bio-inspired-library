#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include"../include/bee.hpp"

using namespace std;


Bee::Bee(){
}

/*
Inicializa para cada abelha sua posição aleátoria D-imensional no espaço de busca
*/
Bee::Bee(int dim, double max, double min){
  long double x=0, random=0;
  for(int i = 0;  i < dim; i++){ /* inicializa toda a fonte de alimento(abelhas) D-imensional*/
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0,1);
            random = dis(gen);
            x = min + (random * (max-min));
            this->position.push_back(x);
  }
  this->fit = 0; /* calcular o fitness inicial*/
  this->f = 0;  /* calcular a função objetivo inicial */
  this->best_fitness = 0; /* melhor posição da abelha*/
  this->update = 0; /* limite de iterações sem melhor uma solução */
  this->probabilitie = 1; /* probabilidades de fontes de alimento (soluções) serem escolhidas */
  }

  Bee::Bee(int dim, double max, double min, ostream& arq){
    long double x=0, random=0;
    for(int i = 0;  i < dim; i++){ /* inicializa toda a fonte de alimento(abelhas) D-imensional*/
              random_device rd;
              mt19937 gen(rd());
              uniform_real_distribution<> dis(0,1);
              random = dis(gen);
              x = min + (random * (max-min));
              arq << x << endl;
              this->position.push_back(x);
    }
    this->fit = 0; /* calcular o fitness inicial*/
    this->f = 0;  /* calcular a função objetivo inicial */
    this->best_fitness = 0; /* melhor posição da abelha*/
    this->update = 0; /* limite de iterações sem melhor uma solução */
    this->probabilitie = 0; /* probabilidades de fontes de alimento (soluções) serem escolhidas */
}

Bee::Bee(int dim, double max, double min, istream& arq){
  long double x=0;
  for(int i = 0;  i < dim; i++){ /* inicializa toda a fonte de alimento(abelhas) D-imensional*/
            arq >> x;
            this->position.push_back(x);
  }
  this->fit = 0; /* calcular o fitness inicial*/
  this->f = 0;  /* calcular a função objetivo inicial */
  this->best_fitness = 0; /* melhor posição da abelha*/
  this->update = 0; /* limite de iterações sem melhor uma solução */
  this->probabilitie = 0; /* probabilidades de fontes de alimento (soluções) serem escolhidas */
}

void Bee::newBee(vector<double> b){
    this->position = b;
}
double Bee::getGlobalBest(){
  return this->best_fitness;
}

void Bee::setFit(double fit){
  this->fit = fit;
}

double Bee::getFit(){
  return this->fit;
}

void Bee::setF(double f){
  this->f = f;
}

double Bee::getF(){
  return this->f;
}

void Bee::updateIncrement(){
    this->update = this->update + 1;
}

int Bee::getUpdate(){
  return this->update;
}

void Bee::updateClean(){
  this->update = 0;
}

void Bee::setProbabilities(double p){
    this->probabilitie = p;
}

double Bee::getProbabilitie(){
  return this->probabilitie;
}
