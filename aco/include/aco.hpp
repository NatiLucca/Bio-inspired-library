#ifndef COLONY_H
#define COLONY_H

#include<stdlib.h>
#include<vector>
#include <string>
#include"../include/ant.hpp"

using namespace std;

class ACO{

  private :
    vector<Ant> colony;       /* Colônia de Formigas */
    vector<Ant>  solution;       /* Grafo T -> Feromonio = SA*/
    long double global_best; /* Melhor Valor da Função Objetivo */
  	double evaporation;			   /* Taxa de Evaporação */
    int k;								   /* importância do nível de feromônio*/
	  double q;								   /*importância da visibilidade */
    string typeFunction;      /* Tipo da Função [MIN | Max]*/
    int dim;                          /* Dimensão da Colônia */
    int col;                          /* Tamanho da colônia de formigas */
    int stepsMax;                /* Nº de repetições */
    double max;                  /* Limite Superior do Espaço de Busca */
    double min;                  /* Limite Inferior do Espaço de Busca */

    void initAnts();
    void initSolution();
    void memorizeBest(double (*costFunction)(vector<double> ));
    void evaporationPheromone();
    int selectionAnt();
    void advanceAnt( double (*costFunction)(vector<double> ));
    void calculateW();
    void calculateProbabilitie();
    void costAnt(double (*costFunction)(vector<double> ));
    void printF();

    void initAntsDET();
    void initValuesAnt();
    void calculateWP();
    void calculateProbabilitieP();
    void initSolutionMOD();
    void initSolutionDET();
    int selectionAntDET(ostream& arq);
    int selectionAntMOD(istream& arq);
      int selectionAntMODP(istream& arq);
    void advanceAntDET( double (*costFunction)(vector<double> ), ostream& arq, ostream& arqS);
    void advanceAntMOD( double (*costFunction)(vector<double> ), istream& arq, istream& arqS);
    void advanceAntMODP( double (*costFunction)(vector<double> ), istream& arq, istream& arqS);
    
  public :
    ACO();
    ACO(int k,double q,double evap, int dim, int col, int stepsMax , double max, double min, string tf);
    void run(double (*costFunction)(vector<double> ));
    void runDetails(double (*costFunction)(vector<double> ));
    double getGlobalBest();
    void runParallel(double (*costFunction)(vector<double> ));

};

#endif
