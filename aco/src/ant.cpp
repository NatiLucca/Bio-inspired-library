#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <ostream>
#include <cstdlib>
#include"../include/ant.hpp"

using namespace std;

Ant::Ant(){
}

/* Inicializa para cada formiga k sua posição aleátoria D-imensional no espaço de busca */
Ant::Ant(int dim, double max, double min){
  long double x=0, random=0;
  for(int i = 0;  i < dim; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0,1);
            random = dis(gen);
            x = min + (random * (max-min));
            this->position.push_back(x);
  }
  this->cost = 10000000000000;  /* calcular a função objetivo inicial */
  this->w = 0;
  this->p = 0;
}

Ant::Ant(int dim, double max, double min, int i){
  for(int j = 0;  j < dim; j++){
            this->position.push_back(i);
  }
  this->cost = 1000000000000000000;  /* calcular a função objetivo inicial */
  this->w = 0;
  this->p = 0;
}

Ant::Ant(int dim, double max, double min, ostream& arq){
  long double x=0, random=0;
  for(int i = 0;  i < dim; i++){
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<> dis(0,1);
            random = dis(gen);
            x = min + (random * (max-min));
            this->position.push_back(x);
            arq << x << endl;
  }
  this->cost = 100000000000000;  /* calcular a função objetivo inicial */
  this->w = 0;
  this->p = 0;
}

Ant::Ant(int dim, double max, double min, istream& arq){
  long double x=0, random=0;
  for(int i = 0;  i < dim; i++){
            arq >> x;
            this->position.push_back(x);
  }
  this->cost = 1000000000000000;  /* calcular a função objetivo inicial */
  this->w = 0;
  this->p = 0;
}

void Ant::setCost(double c){
    this->cost = c;
}

double Ant::getCost(){
    return this->cost;
}

void Ant::setW(double c){
    this->w = c;
}

double Ant::getW(){
  return this->w;
}

void Ant::setP(double p){
  this->p = p;
}

double Ant::getP(){
    return this->p;
}
