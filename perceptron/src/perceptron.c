#include <perceptron.h>

float randomWeight(float min, float max) {
	return (max - min) * ((float)rand() / RAND_MAX) + min;
}

float heaviside(float x){
	return (x < 0) ? 0 : 1;
}

perceptron initialize_perceptron(float weight_interval[2], float* desired_output, 
								 int output_layer_size, int input_layer_size){
	perceptron network;

	network.output_layer_size = output_layer_size;
	network.input_layer_size = input_layer_size;
	network.desired_output = desired_output;
	network.weight_interval[2] = weight_interval[2];

	network.output = (float *)malloc(output_layer_size * sizeof(float));

	network.weight = (float *)malloc((output_layer_size * input_layer_size) * sizeof(float));

	for(int i = 0; i < output_layer_size; i++){
		for(int j = 0; j < input_layer_size; j++){
			network.weight[i*input_layer_size+j] = randomWeight(weight_interval[0], weight_interval[1]);
		}
	}

	return network;
}

void destroy_perceptron(perceptron* network){
	if(network->output) free(network->output);
	if(network->weight) free(network->weight);
}