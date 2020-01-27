#!/bin/bash
# chmod +x run.sh

if [ ! -d "Resultados" ]; then
	mkdir Resultados
else
	rm Resultados/*.csv
fi

if [ ! -d "Inputs" ]; then
	mkdir Inputs
else
	rm Inputs/*.txt
fi

g++ -std=c++11 -o abc main/main.cpp main/benchmarking.cpp main/benchmarking.hpp src/bee.cpp src/abc.cpp include/bee.hpp include/abc.hpp -fopenmp

pop=10
typeF="min"
it=5
stepsMax=100

# ## GRIEWANK ##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -600 600  50 $stepsMax 5000 griewank  $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -600 600  50 $stepsMax 5000 griewank  $typeF $i 1 >> Resultados/GriewankS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -600 600  50 $stepsMax 5000 griewank  $typeF $i 2 >> Resultados/GriewankP.csv
# done

## BENCHMARK Alpine##
for i in $(seq 1 $it);
do
	./abc $pop 0 10 10 $stepsMax 3000 alpine  $typeF $i 0  >> temp.csv
done
echo "Seq"
for i in $(seq 1 $it);
do
	./abc $pop 0 10 10 $stepsMax 3000 alpine  $typeF $i 1   >> Resultados/AlpineS.csv
done
echo "Paralelo"
for i in $(seq 1 $it);
do
	./abc $pop 0 10 10 $stepsMax 3000 alpine  $typeF $i 2 >>Resultados/AlpineP.csv
done

# ## BENCHMARK Booth##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -10 10 2 $stepsMax 1000 booth $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -10 10 2 $stepsMax 1000 booth $typeF $i 1 >> Resultados/BoothS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -10 10 2 $stepsMax 1000 booth $typeF $i 2 >> Resultados/BoothP.csv
# done
#
# ## BENCHMARK Easom ##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -100 100 2 $stepsMax 1000 easom $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -100 100 2 $stepsMax 1000 easom $typeF $i 1 >> Resultados/EasomS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -100 100 2 $stepsMax 1000 easom $typeF $i 2 >> Resultados/EasomP.csv
# done
#
# ## BENCHMARK Rosenbrock##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5 10 50 $stepsMax 5000 rosenbrock $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5 10 50 $stepsMax 5000 rosenbrock $typeF $i 1 >>  Resultados/RosenbrockS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5 10 50 $stepsMax 5000 rosenbrock $typeF $i 2 >>  Resultados/RosenbrockP.csv
# done
#
# ## BENCHMARK Rastringin##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 50 $stepsMax 5000 rastringin $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 50 $stepsMax 5000 rastringin $typeF $i 1 >> Resultados/RastringinS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 50 $stepsMax 5000 rastringin $typeF $i 2 >> Resultados/RastringinP.csv
# done
#
# ## BENCHMARK Sphere ##
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 6 $stepsMax 3000 sphere $typeF $i 0 >> temp.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 6 $stepsMax 3000 sphere $typeF $i 1 >> Resultados/SphereS.csv
# done
# for i in $(seq 1 $it);
# do
# 	./abc $pop -5.12 5.12 6 $stepsMax 3000 sphere $typeF $i 2 >> Resultados/SphereP.csv
# done

rm temp.csv

exit
