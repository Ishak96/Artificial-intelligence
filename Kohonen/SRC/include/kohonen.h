#ifndef KOHONEN_H
#define KOHONEN_H

#include <stdlib.h>
#include<data.h>

typedef struct {
    double *weights;
    int x, y;
    int num_weights;
}Neuron;

typedef struct {
    Neuron *lattice;
    int latice_size;
    double mapRadius;
    int sideX, sideY; 
    int scale;
}Map;

double min_dbl(double a, double b);

double max_dbl(double a, double b);

void init_neuron(Neuron *n, int x, int y, int num_weights);

Map* init_map(int xSize, int ySize, int num_Weights, int scale);

void distroyNeuron(Neuron *n);

void distroyMap(Map* map);

double neuron_distance(Neuron *n, double *inputs);

int neuron_distance_to(Neuron *src, Neuron *dst);

Neuron* find_bmu(Map *m, double *inputs);

void adjust_weights(Neuron *n, double *inputs, double epsilon, double theta);

double distance_between_weights(Neuron *src, Neuron *dst);

#endif