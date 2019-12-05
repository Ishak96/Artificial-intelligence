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
	int weight_size;
	float* output;
	float* weight;
	outEvaluate funcEvaluation;
}perceptron;

float randomFloat();
float heaviside(float x);

#endif