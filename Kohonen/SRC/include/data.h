#ifndef DATA_H
#define DATA_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct{
	int size;
	double* input;
	int drawn;
}DATA;

typedef struct{
   int size;
   int num_in;
   DATA* entries;
}TRAINING_DATA;

double randomDouble(float min, float max);

void init_DATA(DATA* data, int size);

void init_TRAINING_DATA(TRAINING_DATA* dataSet, int size, int num_in);

void distroyDATA(DATA* data);

void distroyTRAINING_DATA(TRAINING_DATA* dataSet);

void create_DATA(DATA* data, int size, double min, double max);

void InitialiseSet(TRAINING_DATA* dataSet, double min, double max);

DATA SortData(TRAINING_DATA* dataSet);

#endif