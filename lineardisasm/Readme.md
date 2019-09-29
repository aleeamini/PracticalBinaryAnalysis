**for compile in linux by g++:**  
  1- create a object file from loader.h  
    ```g++ -c loader.c ```  
  2- create executable file from linear.c:  
    ```g++ -o linear linear.c loader.o  -std=c++11 -lbfd -lcapstone```  
