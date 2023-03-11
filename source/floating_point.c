#include <stdio.h>
#include <math.h>
#include "atan.h"
#include<time.h>

/* CORDIC COSINE - FLOAT
 * uses atan implementation from stackoverflow to compensate 
 * for tangent approx error
 */
float cordic_cos_float(float theta){
    float x = 0.60725293;                   // cosine product
    float y = 0;
    float z = 0;
    
    for (int i = 0; i < 10; i++){
        
        float d = (z > theta) ? 1 : -1;     // "binary search" direction
        float x_temp = x;
        float pseudotan = powf(2, -i);      // tangent approx
        
        x = x - y * d * pseudotan;
        y = y + x_temp * d * pseudotan;
        z = z - d * my_atan(pseudotan);     // compensates tangent approx
    }
    
    return x;
}

/* CORDIC COSINE - FLOAT
 * returns a (rather bad) approximation of the cosine
 * does not compensate for the tangent approx
 */
float cordic_cos_float_approx(float theta){
    float x = 0.60725293;                   // cosine product
    float y = 0;
    float z = 0;   
    
    float rotation_angle = 0.78539816;      //45 degrees in radians
    
    for (int i = 0; i < 10; i++){
        
        float d = (z > theta) ? 1 : -1;     // "binary search" direction
        float x_temp = x;
        float pseudotan = powf(2, -i);      // tangent approx
        
        x = x - y * d * pseudotan;
        y = y + x_temp * d * pseudotan;
        
        z = z - d * rotation_angle;         // does not compensate tan approx
        rotation_angle /= 2;
        
    }
    
    return x;
}



int main(){

    /***************** (GNUPLOT) ******************/
    
    FILE * temp = fopen("./data/float1.data", "w");
    
    for (int i = -12000; i < 12000; i += 1){
        
        float x = (float)i / 5000;
        float y = cordic_cos_float (x);
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    temp = fopen("./data/float2.data", "w");
    
    for (int i = -12000; i < 12000; i += 1){
        
        float x = (float)i / 5000;
        float y = cordic_cos_float_approx (x);
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    
    
    
    
    /***************** (METERING) ******************/
    
    clock_t start, end;
    double execution_time;
    
    puts("approx version:");
    start = clock();
    for (int i = 0; i < 500000; i += 1)
        cordic_cos_float_approx((float)i/500000);
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n\n", execution_time);
    
    puts("non-approx version:");
    start = clock();
    for (int i = 0; i < 500000; i += 1)
        cordic_cos_float((float)i/500000);
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n", execution_time);
}
