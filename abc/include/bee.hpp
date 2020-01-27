#ifndef BEE_H
#define BEE_H

#include<math.h>
#include<stdlib.h>
#include<ostream>
#include<vector>

using namespace std;

class Bee{
  private :
    vector<double> position; /* posição da abelha */
    long double fit; /* avaliação da fonte fitness */
    long double f; /* valor da função objetivo */
    double best_fitness; /* melhor posição da abelha*/
    long double probabilitie;
    int update; /* contador para identificar o limite de iterações sem melhor uma solução */

  public :
    friend class ABC;
    Bee();
    Bee(int dim, double max, double min);
    Bee(int dim, double max, double min, ostream& arq);
    Bee(int dim, double max, double min, istream& arq);
    double getGlobalBest();
    void setFit(double fit);
    void setF(double f);
    double getFit();
    void updateIncrement();
    void updateClean();
    void setProbabilities(double p);
    double getF();
    int getUpdate();
    double getProbabilitie();
    void newBee(vector<double> b);
};

#endif
