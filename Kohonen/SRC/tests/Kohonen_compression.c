#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <ppm.h>
#include <kohonen.h>

#define NUM_WEIGHT 3
#define MIN_R	   0
#define MAX_R	   255

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

Image* convertMapToImage(Map* n, int x, int y){
	int latice_size = n->latice_size;

	Image* image = (Image *)malloc(sizeof(Image));
	image->data  = (Pixel *)malloc(latice_size * sizeof(Pixel));
	image->x 	 = x;
	image->y 	 = y;

	for(int i = 0; i < y; i++) {
		for(int j = 0; j < x; j++) {
			image->data[i*x+j].r = n->lattice[i*x+j].weights[0];
			image->data[i*x+j].g = n->lattice[i*x+j].weights[1];
			image->data[i*x+j].b = n->lattice[i*x+j].weights[2];
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

	NeuronSet = init_map(imageOriginal->x, imageOriginal->y, NUM_WEIGHT, MIN_R, MAX_R);
	Dataset = convertImageToDataTrain(imageOriginal);

	for(int i = 0; i < imageOriginal->y; i++){
		DATA sDATA = SortUnselectedData(&Dataset);

		MAP_activation(NeuronSet, sDATA);

		int i_bum = find_bmu(NeuronSet);

		adjust_weights(NeuronSet, i_bum, sDATA);
	}

	Image* imageCompressee = convertMapToImage(NeuronSet, imageOriginal->x, imageOriginal->y);

	writePPM("imageCompressee.ppm", imageCompressee);

	free(imageOriginal->data);
	free(imageCompressee->data);
	free(imageOriginal);
	free(imageCompressee);
	return 0;
}
