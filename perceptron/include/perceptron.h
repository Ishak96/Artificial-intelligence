#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    1
#define BIAIS	    	 0
#define MAX_ITERATION    1000
#define DELTA			 0.000001

typedef float (*outActivation)(float x);

typedef struct {
	int output_layer_size;
	int input_layer_size;
	float* output;
	float* weight;
	outActivation funcActivation;
}perceptron;

float randomWeight(float min, float max);
float heaviside(float x);
float neutral(float x);
perceptron initialize_perceptron(float min, float max, int output_layer_size, 
									int input_layer_size, outActivation funcActivation);
void destroy_perceptron(perceptron* network);
float** alloc_matrix(int rows, int cols);
void free_matrix(int rows, float** matrix);
void learn_perceptron(char* file, perceptron* network);
float* test_phase(int* input, perceptron network);

#endif