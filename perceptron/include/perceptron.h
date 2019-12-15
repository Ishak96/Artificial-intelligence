#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE	 0.3
#define BIAIS			 0
#define MAX_ITERATION	 500
#define DELTA			 0.000001
#define INFINI			 1000000
#define PI 				 3.14159265358979323846
#define MEAN 			 0.5
#define S_DEVIATION		 0.25

typedef float (*outActivation)(float x);

typedef struct {
	int output_layer_size;
	int input_layer_size;
	float* output;
	float* weight;
	outActivation funcActivation;
}perceptron;

typedef struct{
	int size_input, size_output;
	int line;
	float** input;
	float** output;
}DATA;

float randomWeight(float min, float max);

float heaviside(float x);

float neutral(float x);

float sigmoid(float x);

float gaussianNoise(int z);

perceptron initialize_perceptron(float min, float max, int output_layer_size, 
									int input_layer_size, outActivation funcActivation);

void destroy_perceptron(perceptron* network, DATA* data);

float** alloc_matrix(int rows, int cols);

void free_matrix(int rows, float** matrix);

int error_check(float* error, int size);

DATA get_data(const char* file, int input_layer_size, int output_layer_size);

void learn_perceptron(DATA data, perceptron* network);

float* test_phase(int* input, perceptron network);

#endif