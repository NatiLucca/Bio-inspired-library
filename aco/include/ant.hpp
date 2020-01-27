#ifndef ANT_H
#define ANT_H

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<vector>

using namespace std;

class Ant{
  private :
    vector<double> position; /* posição ou rota da abelha */
    double w; /* componente \omega*/
    double p; /* probabilidade de escolha*/
    double cost; /* avaliação da do caminho */

  public :
    friend class ACO;
    Ant();
    Ant(int dim, double max, double min);
    Ant(int dim, double max, double min, int i);
    Ant(int dim, double max, double min, ostream& arq);
    Ant(int dim, double max, double min, istream& arq);
    void setCost(double c);
    double getCost();
    void setW(double c);
    double getW();
    void setP(double p);
    double getP();

};

#endif
