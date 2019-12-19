#include<kohonen.h>

double min_dbl(double a, double b) { return a < b ? a : b; }
double max_dbl(double a, double b) { return a > b ? a : b; }

void init_neuron(Neuron *n, int x, int y, int num_weights){
	n->x = x; n->y = y;
	n->num_weights = num_weights;
	n->weights = malloc(num_weights * sizeof(double));
	
	for(int i = 0; i < num_weights; ++i) {
		double r = (double)rand() / (double)RAND_MAX;
		n->weights[i] = r;
	}
}

Map* init_map(int sideX, int sideY, int num_weights, int scale) {

	Map *map = malloc(sizeof(Map));
	map->latice_size = sideX * sideY;
	map->mapRadius   = max_dbl(sideX, sideY) / 2;
	map->sideX       = sideX; 
	map->sideY       = sideY; 
	map->scale       = scale;
	map->lattice     = malloc(map->latice_size * sizeof(Neuron));

	for(int y = 0; y < sideY; ++y) {
		for(int x = 0; x < sideX; ++x) {
			init_neuron(map->lattice + y * sideX + x, x, y, num_weights);
		}
	}
	
	return map;
}

void distroyNeuron(Neuron *n){
	free(n->weights);
}

void distroyMap(Map* map){
	int sideX = map->sideX;
	int sideY = map->sideY;

	for(int y = 0; y < sideY; y++){
		for(int x = 0; x < sideX; x++){
			distroyNeuron(map->lattice + y * sideX +x);
		}
	}

	free(map->lattice);
	free(map);
}