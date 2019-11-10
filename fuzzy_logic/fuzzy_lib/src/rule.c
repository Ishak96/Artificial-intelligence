#include<rule.h>

void init_rules(rules *rule){

	rule->conditions = initListe();
}

double MAX(double a, double b){
	return (a > b) ? a : b;
}

double MIN(double a, double b){
	return (a > b) ? b : a;
}

double apply_condition(char* operator, liste liste_values){
	if(videListe(suivCellule(liste_values))){
		return *( double*)valCellule(liste_values);
	}
	else{
		if(operator != NULL){
			if(!strcmp(operator, "OR")){
				return MAX(*( double*)valCellule(liste_values), 
							apply_condition(operator, suivCellule(liste_values)));
			}
			else if(!strcmp(operator, "AND")){
				return MIN(*( double*)valCellule(liste_values), 
							apply_condition(operator, suivCellule(liste_values)));
			}
			else{
				printf("ERROR in the operator value\n");
				return 0.f;
			}
		}
		else{
			printf("ERROR in the operator value NULL\n");
			return 0.f;
		}
	}
}

double get_value_in_fuzzy_liste(fuzzy_controler condition, liste fuzzy_result_liste){
	double value = 0.f;
	if(!videListe(fuzzy_result_liste)){
		int find = 0;
		while(!videListe(fuzzy_result_liste) && !find){
			fuzzy_controler fuzzy = *( fuzzy_controler*)valCellule(fuzzy_result_liste);
			if(!strcmp(fuzzy.value_name, condition.value_name)
				&& !strcmp(fuzzy.variable_name, condition.variable_name)){
				
				value = fuzzy.value;
				find = 1;
			}
			fuzzy_result_liste = suivCellule(fuzzy_result_liste);
		}
	}
	return value;
}

liste apply_rules(liste rules_liste, liste fuzzy_result_liste){
	liste result = initListe();
	fuzzy_controler condition;
	int size_fuzzy_cont = sizeof(fuzzy_controler);
	if(!videListe(rules_liste)){
		while(!videListe(rules_liste)){
			rules rule = *( rules*)valCellule(rules_liste);

			liste conditions = rule.conditions;
			fuzzy_controler deduction = rule.deduction;	
			double result_value = 0.f;
			if(!videListe(conditions)){
				condition = *( fuzzy_controler*)valCellule(conditions);
				char* operator = condition.logic_operator;
				liste liste_values = initListe();
				int size_double = sizeof(double);
				while(!videListe(conditions)){
					condition = *( fuzzy_controler*)valCellule(conditions);
					double value = get_value_in_fuzzy_liste(condition, fuzzy_result_liste);
					inserQueue(&value, &liste_values, size_double);
					conditions = suivCellule(conditions);
				}
				result_value = apply_condition(operator, liste_values);
			}
			deduction.value = result_value;
			inserQueue(&deduction, &result, size_fuzzy_cont);
			rules_liste = suivCellule(rules_liste);
		}
	}
	return result;
}

void clearRule(rules* r1){
	suppListe(&(r1->conditions));
}