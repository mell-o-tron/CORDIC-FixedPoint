#include <stdio.h>
#include<time.h>
#include<math.h>

#define FPI 3.14159265
#define PI 314159265
#define UAT PI/2
#define TAU PI*2


/************************ UTILITIES ************************/

int float_to_fixed_point_8dec(float f){
    return (int)(f * 100000000);
}

float fixed_point_8dec_to_float(int i){
    return (float) i / (100000000);
}

void print_as_float(int x){
    printf("%f\n", fixed_point_8dec_to_float(x));
}

/***********************************************************/


/* ATAN LOOKUP
 * contains values fot atan, used for compensating the tangent approx
 */
int atan_lookup(int tan){
    if(tan == 100000000)    return 78539816;
    if(tan == 50000000)     return 46364760;
    if(tan == (25000000))   return 24497866;
    if(tan == (12500000))   return 12435499;
    if(tan == (6250000))    return 6241880;
    if(tan == (3125000))    return 3123983;
    if(tan == (1562500))    return 1562372;
    if(tan == (781250))     return 781234;
    if(tan == (390625))     return 390623;
    if(tan == (195312))     return 195312;
    
    return 100000000;
}


int my_abs(int x){
 return x >= 0 ? x : -x;
}

/* CORDIC COS FUNCTION - FIXED POINT
 * returns a (rather bad) approximation of the cosine, but faster! 
 */
int cordic_cos_fixed_point_8dec_approx(int theta){
    
    float ftheta = fixed_point_8dec_to_float (theta);
    int sign = ((int)floor(ftheta/FPI + .5) % 2 == 0 ? 1 : -1);

    if((int)(2 * theta / PI) != 0) {    // extend to all inputs angles
        theta += UAT;
        theta = my_abs(theta % (int)PI);
        theta -= UAT;
    }
    
    int x = 60725293;                   // cosine product
    int y = 0;
    int z = 0;   
    
    int rotation_angle = 78539816;      // 45 deg2rad
    
    for (int i = 0; i < 10; i++){
        
        int d = (z > theta) ? 1 : -1;   // "binary search" direction
        
        int x_temp = x;

        x = x - ((y * d) >> i);         // tan approx with bit shift
        y = y + ((x_temp * d) >> i);
        
        z = z - d * rotation_angle;     // tan approx is not compensated
        rotation_angle /= 2;
        
    }
    
    return x * sign;
}

/* CORDIC COSINE - FIXED POINT
 * uses a lookup table to compensate for the tan error, but faster; hyper nice!
 */
int cordic_cos_fixed_point_8dec_lookup(int theta){
    
    float ftheta = fixed_point_8dec_to_float (theta);
    int sign = ((int)floor(ftheta/FPI + .5) % 2 == 0 ? 1 : -1);

    if((int)(2 * theta / PI) != 0) {    // extend to all inputs angles
        theta += UAT;
        theta = my_abs(theta % (int)PI);
        theta -= UAT;
    }
    
    int x = 60725293;                   // cosine product
    int y = 0;
    int z = 0;   
    
    for (int i = 0; i < 10; i++){
        int pseudotan = 100000000 >> i;
        int d = (z > theta) ? 1 : -1;   // "binary search" direction
        
        int x_temp = x;

        x = x - ((y * d) >> i);         // tan approx with bit shift
        y = y + ((x_temp * d) >> i);
        
        z = z - d * atan_lookup(pseudotan);
        
    }
    
    return x * sign;
}


int main(){
    
    /***************** (GNUPLOT) ******************/
#if 1
    FILE * temp = fopen("./data/fixed1.data", "w");
    
    int initial = float_to_fixed_point_8dec(-5);
    int final =   float_to_fixed_point_8dec(5);
    
    for (int i = initial; i < final; i += 10000){
        float x = fixed_point_8dec_to_float(i);
        int c = cordic_cos_fixed_point_8dec_approx(i);
        float y = fixed_point_8dec_to_float(c);
        
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    temp = fopen("./data/fixed2.data", "w");
    
    initial = float_to_fixed_point_8dec(-5);
    final =   float_to_fixed_point_8dec(5);
    
    for (int i = initial; i < final; i += 10000){
        float x = fixed_point_8dec_to_float(i);
        int c = cordic_cos_fixed_point_8dec_lookup(i);
        float y = fixed_point_8dec_to_float(c);
        
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    puts("GNUPLOT data generated");
#else
    puts("GNUPLOT data not generated");
#endif
    /***************** (METERING) ******************/
    
#if 1 // metering toggle 
    clock_t start, end;
    double execution_time = 1;
    start = clock();
    
    for (int i = 0; i < 5000000; i += 1){
        float x = fixed_point_8dec_to_float((float)i / 5000000);
        cordic_cos_fixed_point_8dec_approx(i);
    }
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds (approx) : %f\n", execution_time);
    
    start = clock();
    
    for (int i = 0; i < 5000000; i += 1){
        float x = fixed_point_8dec_to_float((float)i / 5000000);
        cordic_cos_fixed_point_8dec_lookup(i);
    }
    
    end = clock();
    execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds (lookup) : %f\n", execution_time);
    
#else
    puts("execution time not measured");
    
#endif
}
