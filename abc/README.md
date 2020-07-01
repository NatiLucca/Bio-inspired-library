
## Compilar

g++ -std=c++11 -o abc main/main.cpp main/benchmarking.cpp main/benchmarking.hpp src/bee.cpp src/abc.cpp include/bee.hpp include/abc.hpp -fopenmp

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

dim: (int) valor inteiro que corresponde a dimensão do enxame.

colony: (int) valor inteiro que corresponde ao número de agentes da população.

lim: (int) valor inteiro que corresponde ao limite de tentativas de melhor uma solução.

sn: (int) valor inteiro que corresponde ao número de fontes de alimento.

stepsMax: (int) valor inteiro que corresponde ao número máximo de iterações do algoritmo.

dMin: (double) valor decimal que corresponde ao limite inferior do espaço de busca.

dMax: (double) valor decimal que corresponde ao limite superior do espaço de busca.

typeF: (string) campo de texto [min|max] que corresponde a função de minimização (min) ou maximização (max).

## Variáveis: Construtor ABC()

**`ABC abc(dim, colony, lim, sn, stepsMax, dMin, dMax, typeF);`**

Exemplo:

**'ABC abc(50, 20, 100, 10, 3000, -100, 100, min);`**

#### Execução Detalhada:

**`runDetails(function)`** – método que inicializa a execução detalhada da função especificada.

Exemplo:
**`abc.runDetails(function);`**
**`abc.runDetails(griewank);**`

#### Execução Sequencial:
**`run(function)`** – método que inicializa a execução sequencial da função especificada.

Exemplo:
**`abc.run(function);`**
**`abc.run(sphere);`**

#### Execução Paralela
**`runParallel(function); `** – método que inicializa a execução paralela da função especificada.

Exemplo:
**`abc.runParallel(function); `**
**`abc.runParallel(rastringin);`**
