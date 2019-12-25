#include<kohonen.h>

double min_dbl(double a, double b){ 
	return a < b ? a : b;
}

double max_dbl(double a, double b) {
	return a > b ? a : b;
}

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
			init_neuron(map->lattice+y*sideX+x, x, y, num_weights);
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

double neuron_distance(Neuron *n, double *inputs){
    double val = 0;
    for(int i = 0; i < n->num_weights; ++i){
        val += (inputs[i] - n->weights[i]) * (inputs[i] - n->weights[i]);
    }
    return val;
}

int neuron_distance_to(Neuron *src, Neuron *dst){
    return (dst->x - src->x) * (dst->x - src->x)
        +  (dst->y - src->y) * (dst->y - src->y);
}

Neuron* find_bmu(Map *m, double *inputs){
    Neuron *n = m->lattice;
    double min_val = neuron_distance(n, inputs);

    for(int i = 1; i < m->latice_size; ++i){
        double curr = neuron_distance(&(m->lattice[i]), inputs);
        min_val = min_dbl(curr, min_val);
        if(min_val == curr){ 
        	n = &(m->lattice[i]);
        }
    }
    return n;
}

void adjust_weights(Neuron *n, double *inputs, double epsilon, double theta){
    
    for(int i = 0; i < n->num_weights; ++i){
        n->weights[i] += epsilon * theta * (inputs[i] - n->weights[i]);
    }
}

double distance_between_weights(Neuron *src, Neuron *dst){
    double dist = 0;
    for(int i = 0; i < src->num_weights; ++i){
        dist += dst->weights[i] - src->weights[i];
    }
    return dist < 0 ? -dist : dist;
}