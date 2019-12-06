#ifndef PERCEPTRON_H
#define PERCEPTRON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define LEARNING_RATE    0.1
#define MAX_ITERATION    100

typedef float (*outEvaluate)(float x);

typedef struct {
	int output_layer_size;
	int input_layer_size;
	float* output;
	float* desired_output;
	float* weight;
	float weight_interval[2];
	outEvaluate funcEvaluation;
}perceptron;

float randomWeight(float min, float max);
float heaviside(float x);
perceptron initialize_perceptron(float weight_interval[2], float* desired_output, 
								 int output_layer_size, int input_layer_size);
void destroy_perceptron(perceptron* network);

#endif