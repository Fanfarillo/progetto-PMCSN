gcc -c ./algorithms/servicenode1alg.c
gcc -c ./algorithms/servicenode2alg.c
gcc -c ./algorithms/servicenode3alg.c
gcc -c ./algorithms/servicenode4alg.c
gcc -c ./algorithms/servicenode5alg.c
gcc -c ./algorithms/rngs.c
gcc -c ./algorithms/rvms.c
gcc -c ./algorithms/randomGeneratorFunctions.c
gcc -c simulation.c
gcc simulation.o servicenode1alg.o servicenode2alg.o servicenode3alg.o servicenode4alg.o servicenode5alg.o rngs.o rvms.o randomGeneratorFunctions.o -o simulation -lm
#./simulation 2 3 4 5 6 1
