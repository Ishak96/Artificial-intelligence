#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <ppm.h>
#include <kohonen.h>

#define NUM_WEIGHT 	   3
#define MIN_R	   	   0
#define MAX_R	   	   255

#define LATICE_SIZE	   16

#define MAP_SIZE_X	   LATICE_SIZE
#define MAP_SIZE_Y	   1

TRAINING_DATA convertImageToDataTrain(Image* image){
	int sizeX = image->x;
	int sizeY = image->y;
	TRAINING_DATA Dataset;

	init_TRAINING_DATA(&Dataset, sizeX*sizeY, NUM_WEIGHT);
	for(int i = 0; i < sizeY; i++){
		for(int j = 0; j < sizeX; j++){
			Dataset.entries[i*sizeX+j].input[0] = image->data[i*sizeX+j].r;
			Dataset.entries[i*sizeX+j].input[1] = image->data[i*sizeX+j].g;
			Dataset.entries[i*sizeX+j].input[2] = image->data[i*sizeX+j].b;
		}
	}

	return Dataset;
}

double distance(double* x, double* y){
	return sqrt(pow(x[0]-y[0], 2) + pow(x[1]-y[1], 2) + pow(x[2]-y[2], 2));
}

Pixel MIN_RGB(Map* n, double* data){
	int latice_size = n->latice_size;
	double min_dist = 100000;
	double* data_n;
	Pixel result;

	for(int i = 0; i < latice_size; i++){
		data_n = n->lattice[i].weights;
		if(distance(data, data_n) < min_dist){
			min_dist = distance(data, data_n);
			result.r = data_n[0];
			result.g = data_n[1];
			result.b = data_n[2];
		}
	}

	return result;
}

Image* IMAGECompressee(Map* n, Image* imageOriginal){

	Image* image = (Image *)malloc(sizeof(Image));
	
	image->x 	 = imageOriginal->x;
	image->y 	 = imageOriginal->y;

	image->data  = (Pixel *)malloc(image->x * image->y * sizeof(Pixel));

	for(int i = 0; i < image->y; i++){
		for(int j = 0; j < image->x; j++){
			double data[3];
			data[0] = imageOriginal->data[i*image->x+j].r;
			data[1] = imageOriginal->data[i*image->x+j].g;
			data[2] = imageOriginal->data[i*image->x+j].b;

			image->data[i*image->x+j] = MIN_RGB(n, data);
		}
	}

	return image;
}

int main(int argc, char **argv){
	TRAINING_DATA Dataset;
	Map* NeuronSet;

	if (argc != 2) 
		EXIT_ON_ERROR("You must specified a .ppm file");
	Image* imageOriginal = readPPM(argv[1]);

	NeuronSet = init_map(MAP_SIZE_X, MAP_SIZE_Y, NUM_WEIGHT, MIN_R, MAX_R);
	Dataset = convertImageToDataTrain(imageOriginal);

	for(int i = 0; i < 1000; i++){
		DATA sDATA = SortUnselectedData(&Dataset);

		MAP_activation(NeuronSet, sDATA);

		int i_bum = find_bmu(NeuronSet);

		adjust_weights(NeuronSet, i_bum, sDATA);
	}

	Image* imageCompressee = IMAGECompressee(NeuronSet, imageOriginal);

	writePPM("imageCompressee.ppm", imageCompressee);

	free(imageOriginal->data);
	free(imageCompressee->data);
	free(imageOriginal);
	free(imageCompressee);
	return 0;
}
