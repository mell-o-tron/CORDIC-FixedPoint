#include <stdio.h>
#include<time.h>


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


/* CORDIC COS FUNCTION - FIXED POINT
 * returns a (rather bad) approximation of the cosine, but faster! 
 */
int cordic_cos_fixed_point_8dec_approx(int theta){
    int x = 60725293;                   // tangent factor
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
    
    return x;
}



int main(){
    
    /***************** (GNUPLOT) ******************/
    
    FILE * temp = fopen("./data/fixed.data", "w");
    
    int initial = float_to_fixed_point_8dec(-5);
    int final =   float_to_fixed_point_8dec(5);
    
    for (int i = initial; i < final; i += 10000){
        float x = fixed_point_8dec_to_float(i);
        int c = cordic_cos_fixed_point_8dec_approx(i);
        float y = fixed_point_8dec_to_float(c);
        
        fprintf(temp, "%lf %lf \n", x, y);
        
    }
    
    fclose (temp);
    
    /***************** (METERING) ******************/
    
    clock_t start, end;
    
    start = clock();
    
    for (int i = 0; i < 500000; i += 1){
        float x = fixed_point_8dec_to_float((float)i / 500000);
        cordic_cos_fixed_point_8dec_approx(i);
    }
    
    end = clock();
    double execution_time = ((double)(end - start))/CLOCKS_PER_SEC;
    
    printf("execution time in seconds : %f\n", execution_time);
    
}
