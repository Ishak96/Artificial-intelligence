#ifndef RULE_H
#define RULE_H

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<malloc.h>
#include<struct.h>
#include<list.h>
#include<unistd.h>
#include<stdarg.h>

void init_rules(rules *rule);
float MAX(float a, float b);
float MIN(float a, float b);
float apply_condition(char* operator, liste liste_values);
float get_value_in_fuzzy_liste(fuzzy_controler condition, liste fuzzy_result_liste);
liste apply_rules(liste rules_liste, liste fuzzy_result_liste);
liste creat_rules(fuzzy_controler* rules_tab, int nb_rules);
void clearRule(rules* r1);

#endif