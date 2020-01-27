
#include<iostream>
#include<stdlib.h>
#include <string>
#include <time.h>
#include"../include/bee.hpp"
#include"../include/abc.hpp"
#include"benchmarking.hpp"
#include <sys/time.h>

using namespace std;

string nameArq;

int help(){
  cout << "Error!\n" << "Report\n  bee colony size, min,  max, Dimension,  Limit, StepsMax, Function, Type function [min|max] e Flag [0|1]" << endl;
  exit(0);
}

int main(int argc, char** argv){

  if(argc != 11){
      help();
  }

  int COL  = atoi(argv[1]);      /* Tamanho da colônia de abelhas (COL) */
  int BN  =  COL/2;   /* Nº de abelhas campeiras (BN)*/
  int SN =  BN ;   /* N° de fontes de alimento (SN), que é igual a BN*/
  int BC = (COL - BN);       /* N° de abelhas seguidoras -  é igual à diferença entre COL e BN*/
  double dMax = atof(argv[3]); /* Limite Superior do Espaço de Busca*/
  double dMin = atof(argv[2]);  /* Limite Inferior do Espaço de Busca*/
  int dim = atoi(argv[4]);  /* Dimensão da Abelha */
  int lim  = atoi(argv[5]);      /* N° de tentativas de liberar uma fonte de alimento*/
  int stepsMax = atoi(argv[6]);  /* Número de execuções */
  string function = (argv[7]);  /* Nome da Função Objetivo  */
  string typeFunction = (argv[8]); /* Tipo da Função Objetivo [Min ou Max] */
  nameArq = (argv[9]); /* Número da Execução - usado para gerar arquivos de saída  */
  int flag = atoi(argv[10]); /* Número da Flag -> 0 - detalhes; 1 - Sequencial; 2 - Paralelo */


  if(typeFunction != "min" && typeFunction != "max"){
    help();
  }

  clock_t start_t, end_t;
  double total_t;
  start_t = clock();

  ABC* abc = new ABC(dim, COL, lim, SN, stepsMax, dMax, dMin);

  /* GERA ARQUIVO DE INPUTS*/
  if( flag == 0){
              if (function == "griewank"){
                            abc->runDetails(griewank);
              }else if (function == "alpine"){
                            abc->runDetails(alpine);
              }else if (function == "booth"){
                            abc->runDetails(booth);
               }else if (function == "easom"){
                             abc->runDetails(easom);
              }else if (function == "rosenbrock"){
                            abc->runDetails(rosenbrock);
              }else if (function == "rastringin"){
                            abc->runDetails(rastringin);
              }else if (function == "sphere"){
                            abc->runDetails(sphere);
              }else{
                  cout << "\nUnidentified Function" << endl;
            }
  }else  if (flag == 1){ /* Versão Sequencial */
          if (function == "griewank"){
                        abc->run(griewank);
          }else if (function == "alpine"){
                        abc->run(alpine);
          }else if (function == "booth"){
                        abc->run(booth);
           }else if (function == "easom"){
                        abc->run(easom);
          }else if (function == "rosenbrock"){
                        abc->run(rosenbrock);
          }else if (function == "rastringin"){
                        abc->run(rastringin);
          }else if (function == "sphere"){
                      abc->run(sphere);
          }else{
              cout << "\nUnidentified Function" << endl;
          }

  }else if (flag == 2){ /* Versão Paralela */
                if (function == "griewank"){
                              abc->runParallel(griewank2);
                }else if (function == "alpine"){
                              abc->runParallel(alpine2);
                }else if (function == "booth"){
                              abc->runParallel(booth);
                 }else if (function == "easom"){
                              abc->runParallel(easom);
                }else if (function == "rosenbrock"){
                              abc->runParallel(rosenbrock2);
                }else if (function == "rastringin"){
                              abc->runParallel(rastringin2);
                }else if (function == "sphere"){
                              abc->runParallel(sphere2);
                }else{
                    cout << "\nUnidentified Function" << endl;
                }
  }

  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

  /* Saida formato CSV -> Melhor Solução , tempo de execução */
  cout << abc->getGlobalBest() << " ; " << total_t << endl;

  return 0;
}
