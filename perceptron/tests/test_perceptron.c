#include <perceptron.h>

#define OUTPUT_NB 2
#define INPUT_NB 20

int main(int argc, char const *argv[]){
	
	int output_layer_size = OUTPUT_NB;
	int input_layer_size = INPUT_NB;

	float weight_interval[2] = {0.0, 1.0};
	float desired_output[OUTPUT_NB] = {1.0, 0.0};

	perceptron network = initialize_perceptron(weight_interval, desired_output, 
												output_layer_size, input_layer_size);

	for(int i = 0; i < output_layer_size; i++){
		printf("out put[%d] :\n", i);
		for(int j = 0; j < input_layer_size; j++){
			printf("weight[%d] = %f\n", j, network.weight[i*input_layer_size+j]);
		}
	}

	destroy_perceptron(&network);

	return 0;
}