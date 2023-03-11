#include <stdio.h>
#include <math.h>
#include "atan.h"
#include<time.h>

#define PI 3.14159265
#define UAT PI/2
#define TAU PI*2



/* ATAN LOOKUP
 * contains values fot atan, used for compensating the tangent approx
 */
float atan_lookup (float tan){
    
    if(tan == 1)                return 0.7853981633974483;
    if(tan == .5)               return 0.4636476090008061;
    if(tan == (.25))            return 0.24497866312686414;
    if(tan == (0.125))          return 0.12435499454676144;
    if(tan == (0.0625))         return 0.06241880999595735;
    if(tan == (0.03125))        return 0.031239833430268277;
    if(tan == (0.015625))       return 0.015623728620476831;
    if(tan == (0.0078125))      return 0.007812341060101111;
    if(tan == (0.00390625))     return 0.0039062301319669718;
    if(tan == (0.001953125))    return 0.0019531225164788188;
    
    
    return 1;
}



/* CORDIC COSINE - FLOAT
 * uses atan implementation from stackoverflow to compensate 
 * for tangent approx error
 */
float cordic_cos_float(float theta){

    float sign = (float)((int)floor(theta/PI + .5) % 2 == 0 ? 1 : -1);

    if((int)(2 * theta / PI) != 0) {        // extend to all inputs angles
        theta += UAT;
        theta = fabs(fmod(theta, PI));
        theta -= UAT;

    }

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
    
    return x * sign;
}



/* CORDIC COSINE - FLOAT
 * returns a (rather bad) approximation of the cosine
 * does not compensate for the tangent approx
 */
float cordic_cos_float_approx(float theta){
    
    float sign = (float)((int)floor(theta/PI + .5) % 2 == 0 ? 1 : -1);

    if((int)(2 * theta / PI) != 0) {        // extend to all inputs angles
        theta += UAT;
        theta = fabs(fmod(theta, PI));
        theta -= UAT;

    }
    
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
    
    return x * sign ;
}

/* CORDIC COSINE - FLOAT
 * uses a lookup table to compensate for the tan error, nice!
 */
float cordic_cos_float_lookup(float theta){
    
    float sign = (float)((int)floor(theta/PI + .5) % 2 == 0 ? 1 : -1);

    if((int)(2 * theta / PI) != 0) {        // extend to all inputs angles
        theta += UAT;
        theta = fabs(fmod(theta, PI));
        theta -= UAT;

    }
    
    float x = 0.60725293;                   // cosine product
    float y = 0;
    float z = 0;   
    
    for (int i = 0; i < 10; i++){
        
        float d = (z > theta) ? 1 : -1;     // "binary search" direction
        float x_temp = x;
        float pseudotan = powf(2, -i);      // tangent approx
        
        x = x - y * d * pseudotan;
        y = y + x_temp * d * pseudotan;
        
        z = z - d * atan_lookup(pseudotan);
        
    }
    
    return x * sign ;
}


int main(){
    /***************** (GNUPLOT) ******************/
    
#if 1   // metering toggle
    FILE * temp = fopen("./data/float1.data", "w");
    
    for (int i = -34000; i < 34000; i += 1){
        
        float x = (float)i / 5000;
        float y = cordic_cos_float (x);
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    temp = fopen("./data/float2.data", "w");
    
    for (int i = -34000; i < 34000; i += 1){
        
        float x = (float)i / 5000;
        float y = cordic_cos_float_approx (x);
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    temp = fopen("./data/float3.data", "w");
    
    for (int i = -34000; i < 34000; i += 1){
        
        float x = (float)i / 5000;
        float y = cordic_cos_float_lookup (x);
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    puts("GNUPLOT data generated");
    
#else
     puts("GNUPLOT data not generated");
#endif
    
    
    /***************** (METERING) ******************/
    
#if 1   // metering toggle
    clock_t start, end;
    double execution_time;
    
    puts("approx version:");
    start = clock();
    for (int i = 0; i < 5000000; i += 1)
        cordic_cos_float_approx((float)i/5000000);
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n\n", execution_time);
    
    puts("non-approx version:");
    start = clock();
    for (int i = 0; i < 5000000; i += 1)
        cordic_cos_float((float)i/5000000);
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n", execution_time);
    
    puts("lookup version:");
    start = clock();
    for (int i = 0; i < 5000000; i += 1)
        cordic_cos_float_lookup((float)i/5000000);
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n", execution_time);
#else
    puts("execution time not measured");
#endif
}
