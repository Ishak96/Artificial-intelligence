#include <fuzzy.h>

liste fuzzyfication(linguistic_variable l_variable, float input){
	liste fuzzy_value_liste = initListe();
	int size_fuzzy_value = sizeof(fuzzy_value);

	if(input >= l_variable.univers_discourse[0]
		&& input <= l_variable.univers_discourse[1]){

		liste values_liste = l_variable.values_liste;

		if(!videListe(values_liste)){
			while(!videListe(values_liste)){

				fuzzy_value fuzzfied;
				linguistic_value l_value = *(linguistic_value *)valCellule(values_liste);
				fuzzfied.name_linguistic_value = l_value.value_name;

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