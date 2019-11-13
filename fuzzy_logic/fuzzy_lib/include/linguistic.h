#ifndef LINGUISTIC_H
#define LINGUISTIC_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<math.h>
#include<struct.h>
#include<list.h>
#include<stdarg.h>

linguistic_value initLinguistic_value(char* name, coordinates* trapez);
linguistic_variable initLinguistic_variable(char* name, float* univers_discourse);
void creatlinguistic_variable(int nb, char* name, float* univers_discourse, linguistic_variable* l_variable, ...);
void afflinguistic_variable(linguistic_variable l_variable, print_element aff_function);
void clearlinguistic_variable(linguistic_variable* l_variable);

#endif