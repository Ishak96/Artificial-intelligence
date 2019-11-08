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
#include<stdarg.h>

fuzzy init_fuzzy();
void insert_fuzzy_varaibles(int nb, int output_intput, fuzzy* fuzzy_varaibles, ...);
liste fuzzyfication(linguistic_variable l_variable, float input);
void fuzzy_all(int nb, fuzzy* fuzzy_set, ...);
void clipping(linguistic_value l_value, float clip, float* univers_discourse);
void clipping_process(fuzzy fuzzy_set, fuzzy_controler result);
void aggregation(fuzzy fuzzy_set, liste fuzzy_result_liste);
float trapez_area(coordinates* trapez);
float center_of_trapez_area(coordinates* trapez);
liste defuzzification(fuzzy fuzzy_set);
void clearfuzzy(fuzzy* fuzzy_set);

#endif