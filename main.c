#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double integrator(double* head_x, double* head_y, int size){
    if (size <= 1){
        return -1;
    }
    double* current_x = head_x;
    double* current_y = head_y;
    double* next_x = head_x + 1;
    double* next_y = head_y + 1;
    double current_area; 
    double output = 0;
    int counter = 0;
    for (int i = 0; i < size - 1; i++){
        current_area = ((*current_y + *next_y) * (*next_x - *current_x)) / 2;
        output += current_area;
        current_x ++;
        current_y ++;
        next_x ++;
        next_y ++;
    }
    
    return output;
}

void sine_map(double* head, double* destination_head,double* arr_head, int size, double T_multiple){
    double* current = head;
    double* current_dest = destination_head;
    double* current_impulse = arr_head;
    for (int i = 0; i < size; i++){
        *current_dest = sin(*current * T_multiple) * (*current_impulse);
        current ++;
        current_dest ++;
        current_impulse ++;
    }
}

void cosine_map(double* head, double* destination_head,double* arr_head, int size, double T_multiple){
    double* current = head;
    double* current_dest = destination_head;
    double* current_impulse = arr_head;
    for (int i = 0; i < size; i++){
        *current_dest = cos(*current * T_multiple) * (*current_impulse);
        current ++;
        current_dest ++;
        current_impulse ++;
    }
}


void fourier(double* time_head, double* destination_head, double* arr_head, int size){
    double* current_time = time_head;
    double* current_impulse = arr_head;
    int NUM_COEFFICIENTS = 8;
    double* current_coef = destination_head;
    double temp;
    double* sine_head = malloc(size * sizeof(double));
    double* cosine_head = malloc(size * sizeof(double));
    
    
    double T = (*(time_head + size - 1) - *(time_head));
    double w_0 = 2 * M_PI / T;
    
    
    double a_0 =  (1/T) * integrator(time_head, arr_head, size);
    *current_coef = a_0;
    current_coef ++;
    for (int i = 0; i < NUM_COEFFICIENTS; i++){
        sine_map(time_head, sine_head, arr_head,size, w_0 * (i + 1));
        cosine_map(time_head, cosine_head,arr_head,size, w_0 * (i + 1));
        temp = (2/T) * integrator(time_head, sine_head, size);
        *current_coef = temp;
        temp = (2/T) * integrator(time_head, cosine_head, size);
        current_coef += NUM_COEFFICIENTS;
        *current_coef = temp;
        current_coef -= (NUM_COEFFICIENTS - 1);
    }
    free(sine_head);
    free(cosine_head);
}

int main() {
    // Write C code here
    double impulses[] = {0, sin(2* M_PI * 0.1), sin(2* M_PI * 0.2), sin(2* M_PI * 0.3), sin(2* M_PI * 0.4), sin(2* M_PI * 0.5), sin(2* M_PI * 0.6), sin(2* M_PI * 0.7), sin(2* M_PI * 0.8), sin(2* M_PI * 0.9), sin(2* M_PI * 1)};
    double times[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1};
    double *coefficients_head = malloc(sizeof(double) * 17);
    fourier(&times[0], coefficients_head, &impulses[0], 11);
    double *current_coef = coefficients_head;
    printf("a_0: %f\n\n", *current_coef);
    current_coef ++;
    
    printf("Sine Coefficients:\n");
    for (int i = 0; i < 9; i++){
        printf("%i x w_0: %f\n", i+1, *current_coef);
        current_coef ++;
    }
    printf("\nCosine Coefficients:\n");
    for (int i = 0; i < 9; i++){
        printf("%i x w_0: %f\n", i+1, *current_coef);
        current_coef ++;
    }
    
    
    return 0;
}
