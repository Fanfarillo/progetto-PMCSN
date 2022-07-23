gcc -c ./algorithms/servicenode1alg.c
gcc -c ./algorithms/servicenode2alg.c
gcc -c ./algorithms/servicenode3alg.c
gcc -c ./algorithms/servicenode4alg.c
gcc -c ./algorithms/servicenode5alg.c
gcc -c simulation.c
gcc simulation.o servicenode1alg.o servicenode2alg.o servicenode3alg.o servicenode4alg.o servicenode5alg.o -o simulation
./simulation
