g++ -Ofast -march=native -mtune=native LDasm.c rock2.cpp -o rocket
gcc -Ofast -march=native -mtune=native optim.c -o optiserver
./rocket
