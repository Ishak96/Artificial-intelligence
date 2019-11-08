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

void clipping(linguistic_value l_value, float clip, float* univers_discourse){
	int m = 100;
	float a, b;
	coordinates new;
	coordinates new_trapez[4];

	for(int i = 0; i < 3; i++){
		coordinates A = l_value.trapez[i];
		coordinates B = l_value.trapez[i+1];

		a = (B.x - A.x == 0) ? 0.f : (B.y - A.y) / (B.x - A.x);
		b = B.y - (a * B.x);

		int find = 0;
		for(int j = 0; j <= m && !find; j++){
			new.x = A.x + ((float)j * (B.x - A.x)) / m;
			new.y = (a * new.x) + b;

			if(new.y == clip)
				find = 1;
		}

		if(A.x == univers_discourse[0] || A.x == univers_discourse[1]){
			new_trapez[i].x = A.x;
			new_trapez[i].y = (A.y != 0.f) ? clip : 0.f;
			continue;
		}

		if(i == 0)
			new_trapez[i+1] = new;
		
		else if(i == 2)
			new_trapez[i] = new;
	}
	
	for (int i = 1; i < 3; i++){
		l_value.trapez[i] = new_trapez[i];
	}
}

void clipping_process(fuzzy fuzzy_set, fuzzy_controler result){
	
	liste output_liste = fuzzy_set.output_linguistic_variable;
	while(!videListe(output_liste)){
		linguistic_variable output_l_variable = *( linguistic_variable*)valCellule(output_liste);

		if(!strcmp(output_l_variable.variable_name, result.variable_name)){
			liste l_value_liste = output_l_variable.values_liste;
			int find = 0;
			while(!videListe(l_value_liste) && !find){
				linguistic_value l_value = *( linguistic_value*)valCellule(l_value_liste);
				if(!strcmp(l_value.value_name, result.value_name)){
					clipping(l_value, result.value, output_l_variable.univers_discourse);
					find = 1;
				}
				l_value_liste = suivCellule(l_value_liste);
			}	
		}
		output_liste = suivCellule(output_liste);
	}
}

void aggregation(fuzzy fuzzy_set, liste fuzzy_result_liste){
	
	while(!videListe(fuzzy_result_liste)){
		fuzzy_controler result = *( fuzzy_controler*)valCellule(fuzzy_result_liste);
		clipping_process(fuzzy_set, result);
		fuzzy_result_liste = suivCellule(fuzzy_result_liste);
	}
}

float trapez_area(coordinates* trapez){
	
	float area = 0.f;
	float h = 0.f;
	
	for(int i = 0; i < 4; i++){
		if(trapez[i].y > h)
			h = trapez[i].y;
	}

	if(h > 0.f){
		area = ((trapez[3].x - trapez[0].x)
				+ (trapez[2].x - trapez[1].x)) * 0.5 * h;
	}

	return area;
}

float center_of_trapez_area(coordinates* trapez){
	
	float center_area = 0.f;
	float h = 0.f;
	
	for(int i = 0; i < 4; i++){
		if(trapez[i].y > h)
			h = trapez[i].y;
	}

	if(h > 0.f){
		center_area = (trapez[1].x + trapez[2].x) / 2;
	}

	return center_area;
}

liste defuzzification(fuzzy fuzzy_set){
	
	liste output_l_variable = fuzzy_set.output_linguistic_variable;
	fuzzy_controler fuzzy_c;
	int size_fuzzy_controler = sizeof(fuzzy_controler);
	liste fuzzy_result_liste = initListe();

	while(!videListe(output_l_variable)){
		linguistic_variable l_variable = *( linguistic_variable*)valCellule(output_l_variable);
		fuzzy_c.variable_name = l_variable.variable_name;

		liste values_liste = l_variable.values_liste;
		float sum_area = 0.f;
		float sum_center_area = 0.f;

		while(!videListe(values_liste)){
			linguistic_value l_value = *( linguistic_value*)valCellule(values_liste);
			float area = trapez_area(l_value.trapez);
			sum_area += area;
			sum_center_area += (area * center_of_trapez_area(l_value.trapez));

			values_liste = suivCellule(values_liste);
		}
		
		fuzzy_c.value = (sum_area == 0) ? 0.f : sum_center_area / sum_area;
		inserQueue(&fuzzy_c, &fuzzy_result_liste, size_fuzzy_controler);

		output_l_variable = suivCellule(output_l_variable);
	}

	return fuzzy_result_liste;
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