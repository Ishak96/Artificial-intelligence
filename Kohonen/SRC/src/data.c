#include<data.h>

double randomDouble(float min, float max){
	return (max - min) * ((double)rand() / RAND_MAX) + min;
}

void init_DATA(DATA* data, int size){
	data->size = size;
	data->drawn = 0;

	data->input = (double *) malloc(size * sizeof(double));
}

void init_TRAINING_DATA(TRAINING_DATA* dataSet, int size, int num_in){
	dataSet->size = size;
	dataSet->num_in = num_in;

	dataSet->entries = (DATA *) malloc(size * sizeof(DATA));
	for(int i = 0; i < size; i++){
		init_DATA(dataSet->entries+i, num_in);
	}
}

void distroyDATA(DATA* data){
	free(data->input);
	data->size = 0;
}

void distroyTRAINING_DATA(TRAINING_DATA* dataSet){
	for(int i = 0; i < dataSet->size; i++){
		distroyDATA(dataSet->entries+i);
	}

	free(dataSet->entries);
	dataSet->size = 0;
	dataSet->num_in = 0;
}

void create_DATA(DATA* data, int size, double min, double max){
	if(data->input == NULL){
		fprintf(stderr, "Invalid pointer\n");
		exit(0);
	}

	for(int i = 0; i < size; i++){
		data->input[i] = randomDouble(min, max);
	}
}

void InitialiseSet(TRAINING_DATA* dataSet, double min, double max){
	if(dataSet->entries == NULL){
		fprintf(stderr, "Invalid pointer\n");
		exit(0);
	}

	for(int i = 0; i < dataSet->size; i++){
		create_DATA(dataSet->entries+i, dataSet->num_in, min, max);
	}
}

DATA SortData(TRAINING_DATA* dataSet){
	int findDATA = 0;
	int index = 0;
	DATA dataS = dataSet->entries[index];

	srand(time(NULL));

	while(!findDATA){
		int i = rand()%dataSet->size;

		if(!dataSet->entries[i].drawn){
			dataSet->entries[i].drawn = 1;
			findDATA = 1;
			index = i;
		}
	}

	dataS = dataSet->entries[index];
	return dataS;
}