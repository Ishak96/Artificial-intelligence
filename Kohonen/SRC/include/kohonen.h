#ifndef KOHONEN_H
#define KOHONEN_H

#include <stdlib.h>
#include <math.h>
#include <data.h>

#define LEARNING_RATE  0.3

#define LAMBDA		   0.5
#define BETA		   0.2

#define DVP			   1
#define DVN			   2
#define NEIGHBOUR_DIST 4

#define MIN_R		   50
#define MAX_R		   750

typedef struct {
    double *weights;
    int x, y;
    int num_weights;
    double pot,act;
}Neuron;

typedef struct {
    Neuron *lattice;
    int latice_size;
    int sideX, sideY;
}Map;

double min_dbl(double a, double b);

double max_dbl(double a, double b);

double Q1(int dist);

void init_neuron(Neuron *n, int x, int y, int num_weights);

Map* init_map(int xSize, int ySize, int num_Weights);

void distroyNeuron(Neuron *n);

void distroyMap(Map* map);

void dump_NEURON(Neuron neuron);

void dump_MAP(Map* map);

double neuron_distance(Neuron n, double *inputs);

void MAP_activation(Map* map, DATA data);

int find_bmu(Map *m);

void adjust_weights(Map* map, int bmu, DATA data);

#endif