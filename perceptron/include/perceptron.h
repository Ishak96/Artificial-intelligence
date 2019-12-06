#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    0.1
#define BIAIS	    	 0
#define MAX_ITERATION    100
#define DELTA			 0.000001

typedef float (*outActivation)(float x);

typedef struct {
	int output_layer_size;
	int input_layer_size;
	float* output;
	float* desired_output;
	float* weight;
	float weight_interval[2];
	outActivation funcActivation;
}perceptron;

float randomWeight(float min, float max);
float heaviside(float x);
float neutral(float x);
perceptron initialize_perceptron(float weight_interval[2], float* desired_output, 
								 int output_layer_size, int input_layer_size, outActivation funcActivation);
void destroy_perceptron(perceptron* network);
void learn_perceptron(char* file, perceptron* network);
void test_phase(int* input, perceptron* network);

#endif