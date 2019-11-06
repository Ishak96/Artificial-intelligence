#ifndef LINGUISTIC_H
#define LINGUISTIC_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<struct.h>
#include<list.h>

linguistic_value initLinguistic_value(char* name);
linguistic_variable initLinguistic_variable(char* name, float* univers_discourse);
liste get_coordinates(coordinates A, coordinates B, function_eval function, int linear);
linguistic_value get_trapezoidal_function(char* name, coordinates trapez[4]);
linguistic_variable creatlinguistic_variable(char* name, linguistic_value* l_values, 
	float* univers_discourse , int nb_l_value);
void afflinguistic_variable(linguistic_variable l_variable);
void clearlinguistic_variable(linguistic_variable *l_variable);

#endif