#include<stdlib.h>
#include <string>
#include <time.h>
#include"../include/ant.hpp"
#include"../include/aco.hpp"
#include"../include/benchmarking.hpp"

using namespace std;

string nameArq;

int help(){
  cout << "Error!\n" << "Report\n  ant colony size, Limit Min, Limit Max, Dimension,  Alpha, Beta, Evaporation, StepsMax, Function, Type function [min|max] e Flag [0|1]" << endl;
  exit(0);
}

int main(int argc, char** argv){


  if(argc != 12){
      help();
  }

  int col  = atoi(argv[1]);      /* Tamanho da colônia de formigas */
  double dMax = atof(argv[2]); /* Limite Superior do Espaço de Busca*/
  double dMin = atof(argv[3]);  /* Limite Inferior do Espaço de Busca*/
  int dim = atoi(argv[4]);  /* Dimensão da Abelha */
  int k  = atoi(argv[5]);      /*   K = Tamanho do Arquivo Solução*/
  double q  = atof(argv[6]);      /* Q = Pesquisa */
  double evap  = atof(argv[7]);      /* Taxa de Evaporação */
  //int stepsMax = atoi(argv[8]);  /* Número de execuções */
  string function = (argv[8]);  /* Nome da Função Objetivo  */
  string typeFunction = (argv[9]); /* Tipo da Função Objetivo [Min ou Max] */
  nameArq = (argv[10]); /* Número da Execução - usado para gerar arquivos de saída  */
  int flag = atoi(argv[11]); /* Número da Flag . 0 - detalhes; 1 - Sequencial; 2 - Paralelo */

  if(typeFunction != "min" && typeFunction != "max"){
    help();
  }

  clock_t start_t, end_t;
  double total_t;
  start_t = clock();

  //ACO* aco = new ACO(k, q, evap, dim, col, 1, dMax, dMin, function);
  ACO  aco(k, q, evap, dim, col, 100, dMax, dMin, typeFunction);

  /* GERA ARQUIVO DE INPUTS*/
  if( flag == 0){
              if (function == "griewank"){
                            aco.runDetails(griewank);
              }else if (function == "alpine"){
                            aco.runDetails(alpine);
              }else if (function == "booth"){
                            aco.runDetails(booth);
               }else if (function == "easom"){
                             aco.runDetails(easom);
              }else if (function == "rosenbrock"){
                            aco.runDetails(rosenbrock);
              }else if (function == "rastringin"){
                            aco.runDetails(rastringin);
              }else if (function == "sphere"){
                            aco.runDetails(sphere);
              }else{
                  cout << "\nUnidentified Function" << endl;
            }
  }  else  if (flag == 1){ /* Versão Sequencial */
          if (function == "griewank"){
                        aco.run(griewank);
          }else if (function == "alpine"){
                        aco.run(alpine);
          }else if (function == "booth"){
                        aco.run(booth);
           }else if (function == "easom"){
                        aco.run(easom);
          }else if (function == "rosenbrock"){
                        aco.run(rosenbrock);
          }else if (function == "rastringin"){
                        aco.run(rastringin);
          }else if (function == "sphere"){
                       aco.run(sphere);
          }else{
              cout << "\nUnidentified Function" << endl;
          }

  }else if (flag == 2){ /* Versão Paralela */
                if (function == "griewank"){
                              aco.runParallel(griewank2);
                }else if (function == "alpine"){
                              aco.runParallel(alpine2);
                }else if (function == "booth"){
                              aco.runParallel(booth);
                 }else if (function == "easom"){
                            aco.runParallel(easom);
                }else if (function == "rosenbrock"){
                              aco.runParallel(rosenbrock2);
                }else if (function == "rastringin"){
                              aco.runParallel(rastringin2);
                }else if (function == "sphere"){
                              aco.runParallel(sphere2);
                }else{
                    cout << "\nUnidentified Function" << endl;
                }
  }

  end_t = clock();
  total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

  /* Saida formato CSV . Melhor Solução , tempo de execução */
  cout << aco.getGlobalBest() << " ; " << total_t << endl;

  return 0;
}
