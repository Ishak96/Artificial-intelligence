#include <perceptron.h>

float randomWeight(float min, float max) {
	return (max - min) * ((float)rand() / RAND_MAX) + min;
}

float heaviside(float x){
	printf("%f -> (x >= 0) = %d\n", x, x >= 0);
	return x >= 0;
}

float neutral(float x){
	return x;
}

perceptron initialize_perceptron(float weight_interval[2], float* desired_output, 
								 int output_layer_size, int input_layer_size, outActivation funcActivation){
	perceptron network;

	network.output_layer_size = output_layer_size;
	network.input_layer_size = input_layer_size;
	network.desired_output = desired_output;
	network.weight_interval[2] = weight_interval[2];
	network.funcActivation = funcActivation;

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

void learn_perceptron(char* file, perceptron* network){
	FILE* fp;
	int line;

	if(file == NULL){
		printf("learn:invalide file name entering %s %d\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	fp = fopen(file, "r+");

	if(fp == NULL){
		printf("learn:invalide file entering %s %d\n", __FUNCTION__, __LINE__);
		exit(0);
	}


	int error = 0;
	int size_input = network->input_layer_size;
	int size_output = network->output_layer_size;

	float* input = (float *)malloc(size_input * sizeof(float));
	fscanf(fp, "line = %d\n", &line);

	for(int k = 0; k < line && !error; k++){
		for(int l = 0; l < size_input && !error; l++){
			fscanf(fp, "%f", input+l);
		}
		
		for(int i = 0; i < size_output && !error; i++){
			float weight_som = 0;
			
			for(int j = 0; j < size_input && !error; j++){
				weight_som += (network->weight[i*size_input+j] * input[j]) - BIAIS;
			}
			
			network->output[i] = network->funcActivation(weight_som);
			float ei = network->desired_output[i]-network->output[i];
			error = (ei >= -DELTA && ei <= DELTA);
			
			for(int j = 0; j < size_input && !error; j++){
				network->weight[i*size_input+j] = network->weight[i*size_input+j] + LEARNING_RATE * ei * input[j];
			}
		}
	}

	free(input);
	fclose(fp);
}

void test_phase(int* input, perceptron* network){
	int size_input = network->input_layer_size;
	int size_output = network->output_layer_size;

	for(int i = 0; i < size_output; i++){
		float weight_som = 0;
		for(int j = 0; j < size_input; j++){
			weight_som += (network->weight[i*size_input+j] * input[j]) - BIAIS;
		}
		printf("weight_som = %f\n",weight_som);
		network->output[i] = network->funcActivation(weight_som);
	}
}