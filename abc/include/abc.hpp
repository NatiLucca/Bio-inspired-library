#ifndef SWARM_H
#define SWARM_H

#include<math.h>
#include<stdlib.h>
#include<vector>
#include <string>
#include"../include/bee.hpp"

using namespace std;

class ABC{

  private :
    vector<Bee> swarm;
    long double global_best;
    vector<double> global_best_swarm;
    string typeFunction;
    int dim;        // Dimensão
    int colony;   // Tamanho da colônia de abelhas (COL)
    int BN;       // Nº de abelhas campeiras (BN);
    int SN;       // N° de fontes de alimento (SN), que é igual a BN;
    int BC;       // N° de abelhas seguidoras -  é igual à diferença entre COL e BN;
    int BE;       // N° de abelhas escudeiras
    int lim;      // N° de tentativas de liberar uma fonte de alimento
    int stepsMax; // Nº de repetições
    double max;
    double min;

    void initBees();
    void sendBees(double (*costFunction)(vector<double> )); /* envia abelhas campeiras*/
    void memorizeBest();
    long double CalculateFitness(double fun);
    void calculateProbabilities();
    void sendTrackBees(double (*costFunction)(vector<double> )); /* enviar abelhas seguidoras */
    void sendScoutBees(double (*costFunction)(vector<double> )); /* abelhas escudeiras*/
    void printBees();
    void initValuesBees();
    void initBeesDet();
    void sendTrackBeesDET(double (*costFunction)(vector<double> ), ostream& archiveBee);
    void sendTrackBeesMOD(double (*costFunction)(vector<double> ),  istream& archive);
    void sendBeesDET(double (*costFunction)(vector<double> ),  ostream& arq);
    void sendBeesMOD(double (*costFunction)(vector<double> ),  istream& arq);
    void calculateProbabilitiesParallel();
    void calculateProbabilitiesDET(ostream& arq);
    void calculateProbabilitiesMOD(istream& arq) ;
    void sendScoutBeesDET(double (*costFunction)(vector<double> ), ostream& arq);
    void sendScoutBeesMOD(double (*costFunction)(vector<double> ), istream& arq);
  public :
    ABC();
    ABC(int dim, int col, int lim, int sn, int stepsMax , double max, double min);
    void run(double (*costFunction)(vector<double> ));
    void runDetails(double (*costFunction)(vector<double> ));
    double getGlobalBest();
    void runParallel(double (*costFunction)(vector<double> ));

};

#endif
