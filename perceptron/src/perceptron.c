#include <perceptron.h>

float randomFloat() {
	return (float)rand() / (float)RAND_MAX;
}

float heaviside(float x){
	return (x < 0) ? 0 : 1;
}