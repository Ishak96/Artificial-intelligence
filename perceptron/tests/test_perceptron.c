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
	float weight_interval[2] = {0.0, 1.0};

	perceptron network = initialize_perceptron(weight_interval, output_layer_size, input_layer_size, heaviside);

	char* fileC = "data/dataC.dat";
	float desired_outputC[OUTPUT_NB] = {0.0, 1.0};

	learn_perceptron(fileC, &network, desired_outputC);

	char* fileA = "data/dataA.dat";
	float desired_outputA[OUTPUT_NB] = {1.0, 0.0};

	learn_perceptron(fileA, &network, desired_outputA);

	for(int i = 0; i < output_layer_size; i++){
		printf("out put[%d] :\n", i);
		for(int j = 0; j < input_layer_size; j++){
			printf("weight[%d] = %f\n", j, network.weight[i*input_layer_size+j]);
		}
	}

	test_phase(inputA, &network);

	for(int i = 0; i < OUTPUT_NB; i++){
		printf("output layer [%d] = %f\n", i, network.output[i]);
	}

	test_phase(inputC, &network);

	for(int i = 0; i < OUTPUT_NB; i++){
		printf("output layer [%d] = %f\n", i, network.output[i]);
	}

	destroy_perceptron(&network);

	return 0;
}