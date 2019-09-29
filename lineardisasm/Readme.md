for compile in linux by g++:\n
  1- create a object file from loader.h\n
    g++ -c loader.c\n
  2- create executable file from linear.c:\n
    g++ -o linear linear.c loader.o  -std=c++11 -lbfd -lcapstone\n
