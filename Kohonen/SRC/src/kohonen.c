#include<kohonen.h>

double min_dbl(double a, double b){ 
	return a < b ? a : b;
}

double max_dbl(double a, double b) {
	return a > b ? a : b;
}

double Q1(int dist){
	if(dist == 0){
		return 1;
	} else if(dist < 2){
		return LAMBDA;
	} else if(dist < 3){
		return BETA;
	} else if(dist < 4){
		return BETA/4;
	}

	return 0;
}

void init_neuron(Neuron *n, int x, int y, int num_weights, float MIN_R, float MAX_R){
	n->x = x; n->y = y;
	n->num_weights = num_weights;
	n->weights = malloc(num_weights * sizeof(double));
	n->pot = 0;
	n->act = 0;
	
	for(int i = 0; i < num_weights; i++) {
		double r = randomDouble(MIN_R, MAX_R);
		n->weights[i] = r;
	}
}

Map* init_map(int sideX, int sideY, int num_weights, float MIN_R, float MAX_R){

	Map *map = malloc(sizeof(Map));
	map->latice_size = sideX * sideY;
	map->sideX       = sideX; 
	map->sideY       = sideY; 
	map->lattice     = malloc(map->latice_size * sizeof(Neuron));

	for(int y = 0; y < sideY; ++y) {
		for(int x = 0; x < sideX; ++x) {
			init_neuron(map->lattice+y*sideX+x, x, y, num_weights, MIN_R, MAX_R);
		}
	}
	
	return map;
}

void distroyNeuron(Neuron *n){
	free(n->weights);
}

void distroyMap(Map* map){
	
	for(int i = 0; i < map->latice_size; ++i) {
		free(map->lattice[i].weights);
	}
	free(map->lattice);
	free(map);
}

void dump_NEURON(Neuron neuron){
	printf("       pot = %f\n", neuron.pot);
	printf("       act = %f\n", neuron.act);
	for(int i = 0; i < neuron.num_weights; i++){
		printf("%f\t", neuron.weights[i]);
	}
	
	printf("\n");
	for(int j = 0; j < neuron.num_weights; j++){
		printf("----------\t");
	}
	printf("\n\n");
}

void dump_MAP(Map* map){
	int sideX = map->sideX;
	int sideY = map->sideY;
	Neuron* lattice = map->lattice;

	for(int y = 0; y < sideY; ++y){
		for(int x = 0; x < sideX; ++x){
			printf("       Neuron[%d, %d]\n", x, y);
			dump_NEURON(lattice[y*sideX+x]);
		}
	}
}

/*ALGORITHME*/
double topologicalDistance(int row_size, int a, int b){
	int ax = a%row_size;
	int ay = (int)a/row_size;

	int bx = b%row_size;
	int by = (int)b/row_size;

	return fabs(ax - bx) + fabs(ay - by);
}

double NEURON_distance(Neuron n, double *inputs){
	double distance = 0;
	for(int i = 0; i < n.num_weights; i++) {
		distance += fabs(inputs[i] - n.weights[i]);
	}
	return distance;
}

void MAP_activation(Map* map, DATA data){
	Neuron n;
	double* inputs;
	double pot,act;

	for(int y = 0; y < map->latice_size; y++){
		n = map->lattice[y];
		inputs = data.input;
		
		pot = NEURON_distance(n, inputs);
		act = 1/(1+pot);
		
		map->lattice[y].pot = pot;
		map->lattice[y].act = act;
	}
}

int find_bmu(Map *m){
	int index = 0;

    for(int i = 1; i < m->latice_size; i++) {
    	if(m->lattice[i].act > m->lattice[index].act)
    		index = i;
    }
    return index;
}

void adjust_weights(Map* map, int bmu, DATA data){
    int dist;
    double Q;
    double* inputs;
    
    for(int i = 0; i < map->latice_size; i++){
       	dist = topologicalDistance(map->sideX, i, bmu);
        
        Q = Q1(dist);

        inputs = data.input;

        Neuron *n = map->lattice+i;
		for(int j = 0; j < n->num_weights; j++){
			n->weights[j] += LEARNING_RATE * Q * (inputs[j] - n->weights[j]);
		}
    }
}