#include <perceptron.h>

float randomWeight(float min, float max) {
	return (max - min) * ((float)rand() / RAND_MAX) + min;
}

float heaviside(float x){
	return x > 0;
}

float neutral(float x){
	return x;
}

perceptron initialize_perceptron(float min, float max, int output_layer_size, 
									int input_layer_size, outActivation funcActivation){
	perceptron network;

	network.output_layer_size = output_layer_size;
	network.input_layer_size = input_layer_size;
	network.funcActivation = funcActivation;

	network.output = (float *)malloc(output_layer_size * sizeof(float));

	network.weight = (float *)malloc((output_layer_size * input_layer_size) * sizeof(float));

	for(int i = 0; i < output_layer_size; i++){
		for(int j = 0; j < input_layer_size; j++){
			network.weight[i*input_layer_size+j] = randomWeight(min, max);
		}
	}

	return network;
}

void destroy_perceptron(perceptron* network){
	if(network->output) free(network->output);
	if(network->weight) free(network->weight);
}

float** alloc_matrix(int rows, int cols){
	
	float** matrix = (float **)malloc(rows * sizeof(float *));
	for(int i = 0; i < rows; i++) 
		matrix[i] = (float *)malloc(cols * sizeof(float));

	return matrix;
}

void free_matrix(int rows, float** matrix){
	for(int i = 0; i < rows; i++)
    	free(matrix[i]);
    free(matrix);
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

	int size_input = network->input_layer_size;
	int size_output = network->output_layer_size;

	fscanf(fp, "line = %d\n", &line);

	float** input = alloc_matrix(line, size_input);
	float** output = alloc_matrix(line, size_output);

	for(int k = 0; k < line; k++){
		for(int l = 0; l < size_input; l++)
			fscanf(fp, "%f", input[k]+l);

		for(int l = 0; l < size_output; l++)
			fscanf(fp, "%f", output[k]+l);
	}
	
	for(int k = 0; k < MAX_ITERATION; k++){
		float ei;
		int it = 0;
		while(it < MAX_ITERATION){

			float weight_som = 0;
			
			int index = rand() % line;

			for(int i = 0; i < size_output; i++){
				for(int j = 0; j < size_input; j++){
					weight_som += (network->weight[i*size_input+j] * input[index][j]) - BIAIS;
				}
				
				network->output[i] = network->funcActivation(weight_som);
				ei = output[index][i]-network->output[i];
				
				for(int j = 0; j < size_input; j++){
					network->weight[i*size_input+j] = network->weight[i*size_input+j] 
														+ LEARNING_RATE * ei * input[index][j];
				}
			}
			it++;
		}
	}

	free_matrix(line, input);
	free_matrix(line, output);
	fclose(fp);
}

float* test_phase(int* input, perceptron network){
	int size_input = network.input_layer_size;
	int size_output = network.output_layer_size;
	float* output = (float *)malloc(size_input * sizeof(float));

	for(int i = 0; i < size_output; i++){
		float weight_som = 0;
		for(int j = 0; j < size_input; j++){
			weight_som += (network.weight[i*size_input+j] * input[j]) - BIAIS;
		}
		output[i] = network.funcActivation(weight_som);
	}

	return output;
}