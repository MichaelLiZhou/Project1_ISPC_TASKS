#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#pragma warning (disable: 4244)
#pragma warning (disable: 4305)
#endif

#include <stdio.h>
#include <algorithm>
#include <string.h>
#include "../timing.h"
#include "mandelbrot_ispc.h"
#include <iostream>
using namespace ispc;

extern void mandelbrot_serial(float random_array[], float output_array[]);

int main(int argc, char *argv[]) {

    float random_array[10000];
    float output_array[10000];
    float random;
    double minISPC = 1e30;
    for(int k = 0; k < 2000; k++){
        for(int i = 0; i < 10000; i++){
            random = (float) (rand()) / ((float) (RAND_MAX/3));
            random_array[i] = random;
        }
        reset_and_start_timer();
        mandelbrot_ispc(random_array,output_array);
        double dt = get_elapsed_mcycles();
        minISPC = std::min(minISPC, dt);
    }
    printf("[mandelbrot ispc+tasks]:\t[%.3f] million cycles\n", minISPC);

    double minSerial = 1e30;
    for(int k = 0; k < 2000; k++){
        for(int i = 0; i < 10000; i++){
            random = (float) (rand()) / ((float) (RAND_MAX/3));
            random_array[i] = random;
        }
        reset_and_start_timer();
        mandelbrot_serial(random_array,output_array);
        double dt = get_elapsed_mcycles();
        minSerial = std::min(minSerial, dt);
    }
    for (int i = 0 ; i < 10 ; i++) {
      std::cout<<random_array[i]<<" "<<output_array[i]<<'\n';
    }
    printf("[mandelbrot serial]:\t\t[%.3f] millon cycles\n", minSerial);
    printf("\t\t\t\t(%.2fx speedup from ISPC)\n", minSerial/minISPC);
    return 0;
}