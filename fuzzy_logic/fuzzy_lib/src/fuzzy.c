#include <fuzzy.h>
#include <linguistic.h>

fuzzy insert_fuzzy_varaibles(linguistic_variable* input, linguistic_variable* output, int nb_input, int nb_output){
	fuzzy fuzzy_varaibles;
	int siez_linguistic_variable = sizeof(linguistic_variable);

	fuzzy_varaibles.input_linguistic_variable = initListe();
	fuzzy_varaibles.output_linguistic_variable = initListe();
	fuzzy_varaibles.fuzzy_result_liste = initListe();

	for(int i = 0; i < nb_input; i++)
		inserQueue(input + i, &(fuzzy_varaibles.input_linguistic_variable), siez_linguistic_variable);

	for(int i = 0; i < nb_output; i++)
		inserQueue(output + i, &(fuzzy_varaibles.output_linguistic_variable), siez_linguistic_variable);

	return fuzzy_varaibles;
}

liste fuzzyfication(linguistic_variable l_variable, float input){
	liste fuzzy_value_liste = initListe();
	int size_fuzzy_value = sizeof(crispy_fuzzy_value);

	if(input >= l_variable.univers_discourse[0]
		&& input <= l_variable.univers_discourse[1]){

		liste values_liste = l_variable.values_liste;

		if(!videListe(values_liste)){
			while(!videListe(values_liste)){

				crispy_fuzzy_value fuzzfied;
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

void fuzzy_all(fuzzy* fuzzy_set, crispy_fuzzy_value* input, int nb_input){
	int size = sizeof(crispy_fuzzy_value);
	for(int i = 0; i < nb_input; i++){
		crispy_fuzzy_value value = input[i];
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
			concatListe(&(fuzzy_set->fuzzy_result_liste),&fuzzy_value_liste,compare_crispy_fuzzy_value, size);
		}
	}
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