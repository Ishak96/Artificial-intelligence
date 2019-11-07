#include <fuzzy.h>
#include <linguistic.h>

fuzzy init_fuzzy(){
	fuzzy fuzzy_varaibles;

	fuzzy_varaibles.input_linguistic_variable = initListe();
	fuzzy_varaibles.output_linguistic_variable = initListe();
	fuzzy_varaibles.fuzzy_result_liste = initListe();

	return fuzzy_varaibles;
}

void insert_fuzzy_varaibles(int nb, int output_intput, fuzzy* fuzzy_varaibles, ...){
	va_list ap;
	va_start(ap, fuzzy_varaibles);
	int siez_linguistic_variable = sizeof(linguistic_variable);
	
	while(nb > 0){
		linguistic_variable l_variable;
		l_variable = va_arg(ap, linguistic_variable);

		if(output_intput)
			inserQueue(&l_variable, &(fuzzy_varaibles->input_linguistic_variable), siez_linguistic_variable);
		else
			inserQueue(&l_variable, &(fuzzy_varaibles->output_linguistic_variable), siez_linguistic_variable);	
		
		nb--;
	}

	va_end(ap);
}

liste fuzzyfication(linguistic_variable l_variable, float input){
	liste fuzzy_value_liste = initListe();
	int size_fuzzy_value = sizeof(fuzzy_controler);

	if(input >= l_variable.univers_discourse[0]
		&& input <= l_variable.univers_discourse[1]){

		liste values_liste = l_variable.values_liste;

		if(!videListe(values_liste)){
			while(!videListe(values_liste)){

				fuzzy_controler fuzzfied;
				linguistic_value l_value = *(linguistic_value *)valCellule(values_liste);
				fuzzfied.value_name = l_value.value_name;
				fuzzfied.variable_name = l_variable.variable_name;

				liste coordinates_liste = l_value.coordinates_liste;
				int find = 0;
				if(!videListe(coordinates_liste)){
					while(!videListe(coordinates_liste) && !find){
						coordinates coordinate = *( coordinates*)valCellule(coordinates_liste);
						if(coordinate.x == input){
							fuzzfied.value = coordinate.y;
							find = 1;
						}

						coordinates_liste = suivCellule(coordinates_liste);
					}
				}
				if(!find){
					fuzzfied.value = 0.f;
				}

				inserQueue(&fuzzfied,&fuzzy_value_liste,size_fuzzy_value);
				values_liste = suivCellule(values_liste);
			}
		}
	}
	else{
		printf("error: entering %s %d\n", __FUNCTION__, __LINE__);
	}

	return fuzzy_value_liste;
}

void fuzzy_all(int nb, fuzzy* fuzzy_set, ...){
	int size = sizeof(fuzzy_controler);
	va_list ap;
	va_start(ap, fuzzy_set);

	while(nb > 0){
		fuzzy_controler value;
		value = va_arg(ap, fuzzy_controler);

		liste linguistic_variables = fuzzy_set->input_linguistic_variable;
		if(!videListe(linguistic_variables)){
			int find = 0;
			linguistic_variable l_variable;
			while(!videListe(linguistic_variables) && !find){
				l_variable = *( linguistic_variable*)valCellule(linguistic_variables);
				if(!strcmp(l_variable.variable_name, value.variable_name)){
					find = 1;
					continue;
				}
				linguistic_variables = suivCellule(linguistic_variables);
			}
			liste fuzzy_value_liste = fuzzyfication(l_variable, value.value);
			concatListe(&(fuzzy_set->fuzzy_result_liste),&fuzzy_value_liste,compare_fuzzy_controler, size);
		}
		nb--;
	}
}

float MAX(float a, float b){
	return (a > b) ? a : b;
}

float MIN(float a, float b){
	return (a > b) ? b : a;
}

float apply_condition(char* operator, liste liste_values){
	if(videListe(suivCellule(liste_values))){
		return *( float*)valCellule(liste_values);
	}
	else{
		if(operator != NULL){
			if(!strcmp(operator, "OR")){
				return MAX(*( float*)valCellule(liste_values), 
							apply_condition(operator, suivCellule(liste_values)));
			}
			else if(!strcmp(operator, "ET")){
				return MIN(*( float*)valCellule(liste_values), 
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

float get_value_in_fuzzy_liste(fuzzy_controler condition, liste fuzzy_result_liste){
	float value = 0.f;
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
			float result_value = 0.f;
			if(!videListe(conditions)){
				if(longListe(conditions) > 1){
					condition = *( fuzzy_controler*)valCellule(conditions);
					char* operator = condition.logic_operator;
					liste liste_values = initListe();
					int size_float = sizeof(float);
					while(!videListe(conditions)){
						condition = *( fuzzy_controler*)valCellule(conditions);
						float value = get_value_in_fuzzy_liste(condition, fuzzy_result_liste);
						inserQueue(&value, &liste_values, size_float);
						conditions = suivCellule(conditions);
					}
					result_value = apply_condition(operator, liste_values);
				}
				else{
					condition = *( fuzzy_controler*)valCellule(conditions);
					result_value = condition.value;
				}
			}
			deduction.value = result_value;
			inserQueue(&deduction, &result, size_fuzzy_cont);
			rules_liste = suivCellule(rules_liste);
		}
	}
	return result;
}

liste creat_condition(fuzzy_controler* rules_tab, int nb_rules){
	return NULL;
}

void clearfuzzy(fuzzy* fuzzy_set){
	linguistic_variable l_v;
	while(!videListe(fuzzy_set->input_linguistic_variable)){
		l_v = *(linguistic_variable *)valCellule(fuzzy_set->input_linguistic_variable);
		clearlinguistic_variable(&l_v);
		fuzzy_set->input_linguistic_variable = suivCellule(fuzzy_set->input_linguistic_variable);
	}
	suppListe(&(fuzzy_set->input_linguistic_variable));

	while(!videListe(fuzzy_set->output_linguistic_variable)){
		l_v = *(linguistic_variable *)valCellule(fuzzy_set->output_linguistic_variable);
		clearlinguistic_variable(&l_v);
		fuzzy_set->output_linguistic_variable = suivCellule(fuzzy_set->output_linguistic_variable);
	}
	suppListe(&(fuzzy_set->output_linguistic_variable));

	suppListe(&(fuzzy_set->fuzzy_result_liste));
}