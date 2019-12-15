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

float sigmoid(float x){
	if (fabs(x)<1e-10)
		return 0.5;
	else
		return (1.0 / (1.0 + exp(-x)));
}

float gaussianNoise(int z){
	float e = -(pow((z - MEAN), 2) / pow((2 * S_DEVIATION), 2));
	float dividend = exp(e);
	float divider = 1 / S_DEVIATION * sqrt(2 * PI);

	return dividend * divider;
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

void destroy_perceptron(perceptron* network, DATA* data){
	if(network->output) 
		free(network->output);
	if(network->weight) 
		free(network->weight);

	free_matrix(data->line, data->input);
	free_matrix(data->line, data->output);
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

int error_check(float* error, int size){
	int err = 1;
	for(int i = 0; i < size; i++)
		err = err && (error[i] >= -DELTA && error[i] <= DELTA);
	return err;
}

DATA get_data(const char* file, int input_layer_size, int output_layer_size){
	FILE* fp;
	DATA data;

	if(file == NULL){
		printf("learn:invalide file name entering %s %d\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	fp = fopen(file, "r+");

	if(fp == NULL){
		printf("learn:invalide file entering %s %d\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	data.size_input = input_layer_size;
	data.size_output = output_layer_size;

	fscanf(fp, "line = %d\n", &(data.line));

	float** input = alloc_matrix(data.line, data.size_input);
	float** output = alloc_matrix(data.line, data.size_output);

	for(int k = 0; k < data.line; k++){
		for(int l = 0; l < data.size_input; l++)
			fscanf(fp, "%f", input[k]+l);

		for(int l = 0; l < data.size_output; l++)
			fscanf(fp, "%f", output[k]+l);
	}

	data.input = input;
	data.output = output;

	fclose(fp);
	return data;
}

void learn_perceptron(DATA data, perceptron* network){
	
	float* error = (float *)malloc(data.line * sizeof(float));

	int index = 0;
	//int index = 1;
	for(int k = 0; k < MAX_ITERATION; k++){
		float ei;

		float weight_som = 0;
			
		//index = (k >= MAX_ITERATION / 2) ? 1 - index : index;
		index = rand() % data.line;

		for(int i = 0; i < data.size_output; i++){
			for(int j = 0; j < data.size_input; j++){
				weight_som += (network->weight[i*data.size_input+j] 
							   * gaussianNoise(data.input[index][j]));
			}
				
			network->output[i] = network->funcActivation(weight_som - BIAIS);
			ei = data.output[index][i]-network->output[i];

			error[index] = ei;
				
			for(int j = 0; j < data.size_input; j++){
				network->weight[i*data.size_input+j] = network->weight[i*data.size_input+j] 
													   + LEARNING_RATE * ei * data.input[index][j];
			}
		}
	}

	free(error);
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