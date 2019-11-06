#ifndef FUZZY_H
#define FUZZY_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<struct.h>
#include<list.h>
#include<unistd.h>

fuzzy insert_fuzzy_varaibles(linguistic_variable* input, linguistic_variable* output, int nb_input, int nb_output);
liste fuzzyfication(linguistic_variable l_variable, float input);
void fuzzy_all(fuzzy* fuzzy_set, crispy_fuzzy_value* input, int nb_input);
void clearfuzzy(fuzzy* fuzzy_set);

#endif