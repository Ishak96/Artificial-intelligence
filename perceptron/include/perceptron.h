#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    0.6
#define BIAIS	    	 0
#define DELTA			 0.000001

typedef float (*outActivation)(float x);

typedef struct {
	int output_layer_size;
	int input_layer_size;
	float* output;
	float* weight;
	float weight_interval[2];
	outActivation funcActivation;
}perceptron;

float randomWeight(float min, float max);
float heaviside(float x);
float neutral(float x);
perceptron initialize_perceptron(float weight_interval[2], int output_layer_size, 
									int input_layer_size, outActivation funcActivation);
void destroy_perceptron(perceptron* network);
void learn_perceptron(char* file, perceptron* network, float* desired_output);
void test_phase(int* input, perceptron* network);

#endif