
## Compilar

g++ -std=c++11 -o aco main/main.cpp main/benchmarking.cpp include/benchmarking.hpp src/ant.cpp src/aco.cpp include/ant.hpp include/aco.hpp -fopenmp

## Observação
Para a execução paralela é obrigatório o uso de  "-fopenmp", para as demais execuções é opcional.

## Opções de Funções de Teste
<ul>
  <li>alpine</li>
  <li>booth</li>
  <li>easom</li>
  <li>griewank</li>
  <li>rastringin</li>
  <li>rosenbrock</li>
  <li>sphere</li>
</ul>

## Variáveis

k: (int) valor inteiro que corresponde ao tamanho do arquivo de soluções (k >= colony).
q: (double) Coeficiente da Equação Omega.
evap: (double) valor decimal que corresponde a taxa de evaporação do feromônio.
dim: (int) valor inteiro que corresponde a dimensão do enxame.
colony: (int) valor inteiro que corresponde ao número de agentes da população.
stepsMax: (int) valor inteiro que corresponde ao número máximo de iterações do algoritmo.
dMin: (double) valor decimal que corresponde ao limite inferior do espaço de busca.
dMax: (double) valor decimal que corresponde ao limite superior do espaço de busca.
typeF: (string) campo de texto [min|max] que corresponde a função de minimização (min) ou maximização (max).

## Variáveis: Construtor ACO()

**`ACO aco(k, q, evap, dim, colony, stepsMax, min, max, typeF);`**

Exemplo:

**'ACO aco(50, 0.001, 0.85, 50, 20, 3000, -100, 100, min);`**

#### Execução Detalhada:

**`runDetails(function)`** – método que inicializa a execução detalhada da função especificada.

Exemplo:
**`aco.runDetails(function);`**
**`aco.runDetails(griewank);**`

#### Execução Sequencial:
**`run(function)`** – método que inicializa a execução sequencial da função especificada.

Exemplo:
**`aco.run(function);`**
**`aco.run(sphere);`**

#### Execução Paralela
**`runParallel(function); `** – método que inicializa a execução paralela da função especificada.

Exemplo:
**`aco.runParallel(function); `**
**`aco.runParallel(rastringin);`**
