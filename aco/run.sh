#!/bin/bash
chmod +x run.sh

if [ ! -d "Resultados" ]; then
	mkdir Resultados
else
	rm Resultados/*.csv
fi
#
if [ ! -d "Inputs" ]; then
	mkdir Inputs
else
	rm Inputs/*.txt
fi

g++ -std=c++11 -o aco main/main.cpp main/benchmarking.cpp include/benchmarking.hpp src/ant.cpp src/aco.cpp include/ant.hpp include/aco.hpp -fopenmp

pop=20
typeF="min"
it=1
k=50
d=10

# BENCHMARK Alpine##
for i in $(seq 1 $it);
do
	./aco $pop 0 10 $d $k 0.0001 0.85  alpine  $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop 0 10 $d $k 0.0001 0.85 alpine  $typeF $i 1 >> Resultados/AlpineS.csv
done
for i in $(seq 1 $it);
do
	./aco  $pop 0 10 $d $k 0.0001 0.85 alpine  $typeF $i 2 >>Resultados/AlpineP.csv
done

## BENCHMARK Booth##
for i in $(seq 1 $it);
do
	./aco $pop -10 10 2 $k 0.0001 0.85  booth $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -10 10 2 $k 0.0001 0.85  booth $typeF $i 1 >> Resultados/BoothS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -10 10 2 $k 0.0001 0.85  booth $typeF $i 2 >> Resultados/BoothP.csv
done

## BENCHMARK Easom ##
for i in $(seq 1 $it);
do
	./aco $pop -100 100 2 $k 0.0001 0.85  easom $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -100 100 2 $k 0.0001 0.85  easom $typeF $i 1 >> Resultados/EasomS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -100 100 2  $k 0.0001 0.85 easom $typeF $i 2 >> Resultados/EasomP.csv
done

## GRIEWANK ##
for i in $(seq 1 $it);
do
	./aco $pop -600 600  50 $k 0.0001 0.85 griewank  $typeF  $i  0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -600 600  50 $k 0.0001 0.85 griewank  $typeF  $i 1 >> Resultados/GriewankS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -600 600  50 $k 0.0001 0.85  griewank  $typeF  $i 2 >> Resultados/GriewankP.csv
done

## BENCHMARK Rastringin##
for i in $(seq 1 $it);
do
	./aco $pop -5.12 5.12 50 $k 0.0001 0.85 rastringin $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -5.12 5.12 50 $k 0.0001 0.85 rastringin $typeF $i 1 >> Resultados/RastringinS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -5.12 5.12 50 $k 0.0001 0.85 rastringin $typeF $i 2 >> Resultados/RastringinP.csv
done

## BENCHMARK Rosenbrock##
for i in $(seq 1 $it);
do
	./aco $pop -5 10  50  $k  0.0001 0.85 rosenbrock $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -5 10 50  $k  0.0001 0.85 rosenbrock $typeF $i 1 >>  Resultados/RosenbrockS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -5 10 50  $k  0.0001 0.85 rosenbrock $typeF $i 2 >>  Resultados/RosenbrockP.csv
done

## BENCHMARK Sphere ##
for i in $(seq 1 $it);
do
	./aco $pop -5.12 5.12 6  $k  0.0001 0.85 sphere $typeF $i 0 >> temp.csv
done
for i in $(seq 1 $it);
do
	./aco  $pop -5.12 5.12  6  $k  0.0001 0.85 sphere $typeF $i 1 >> Resultados/SphereS.csv
done
for i in $(seq 1 $it);
do
	./aco $pop -5.12 5.12 6 $k  0.0001 0.85 sphere $typeF $i 2 >> Resultados/SphereP.csv
done

rm temp.csv

exit
