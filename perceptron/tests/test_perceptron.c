#include <perceptron.h>

#define OUTPUT_NB 2
#define INPUT_NB 20

int main(int argc, char const *argv[]){
	
	int inputA[INPUT_NB] = {0,0,0,0
						   ,0,1,0,0
						   ,1,0,1,0
						   ,1,1,1,0
						   ,1,0,1,0};
	
	int inputC[INPUT_NB] = {0,0,0,0
						   ,0,0,0,0
						   ,0,0,1,1
						   ,0,0,1,0
						   ,0,0,1,1};

	int output_layer_size = OUTPUT_NB;
	int input_layer_size = INPUT_NB;

	perceptron network = initialize_perceptron(0, 1, output_layer_size, input_layer_size, heaviside);

	char* file = "data/data.dat";

	learn_perceptron(file, &network);

	for(int i = 0; i < output_layer_size; i++){
		printf("out put[%d] :\n", i);
		for(int j = 0; j < input_layer_size; j++){
			printf("weight[%d] = %f\n", j, network.weight[i*input_layer_size+j]);
		}
	}

	float* output = test_phase(inputA, network);
	for(int i = 0; i < OUTPUT_NB; i++){
		printf("output layer [%d] = %f\n", i, output[i]);
	}
	free(output);

	output = test_phase(inputC, network);
	for(int i = 0; i < OUTPUT_NB; i++){
		printf("output layer [%d] = %f\n", i, network.output[i]);
	}
	free(output);

	destroy_perceptron(&network);

	return 0;
}