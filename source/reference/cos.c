#include <stdio.h>
#include <math.h>
#include<time.h>

int main(){
    clock_t start, end;
    double execution_time = 1;
    puts("math.h version:");
    
    start = clock();
    for (int i = 0; i < 5000000; i += 1)
        cos((float)i/5000000);
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n", execution_time);
    return 0;   
}
