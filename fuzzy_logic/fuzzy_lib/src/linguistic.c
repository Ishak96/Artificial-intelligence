#include <linguistic.h>

linguistic_value initLinguistic_value(char* name){
	linguistic_value l_value;

	l_value.value_name = name;
	l_value.coordinates_liste = initListe();

	return l_value;
}

linguistic_variable initLinguistic_variable(char* name, float* univers_discourse){
	linguistic_variable l_variable;

	l_variable.variable_name = name;
	l_variable.values_liste = initListe();
	l_variable.univers_discourse = univers_discourse;

	return l_variable;
}

liste get_coordinates(coordinates A, coordinates B, function_eval function, int linear){
	float inc = 0.5f;
	float a, b;
	coordinates c;
	liste coordinates_liste = initListe();
	int size_coordinates = sizeof(coordinates);

	if(linear){
		a = (B.x - A.x == 0) ? 0.f : (B.y - A.y) / (B.x - A.x);
		b = B.y - (a * B.x);
	}
	
	for(float i = A.x; i <= B.x; i+=inc){
		c.x = i;
		c.y = (linear) ? (a * c.x) + b : function(c.x);
		if(!existVal(&c,coordinates_liste,compare_coordinates))
			inserTete(&c,&coordinates_liste,size_coordinates);
	}

	return coordinates_liste;
}

linguistic_value get_trapezoidal_function(char* name, coordinates trapez[4]){
	linguistic_value l_value = initLinguistic_value(name);
	int size_coordinates = sizeof(coordinates);

	liste coordinates_liste = initListe();
	for(int i = 0; i < 3; i++){
		coordinates A = trapez[i];
		coordinates B = trapez[i+1];

		if(A.x == B.x){
			coordinates C = (A.y > B.y) ? A : B;
			if(!existVal(&C,coordinates_liste,compare_coordinates))
				inserTete(&C,&coordinates_liste,size_coordinates);
		}
		else{
			coordinates_liste = get_coordinates(A, B, NULL, 1);
		}
		concatListe(&(l_value.coordinates_liste),&coordinates_liste,compare_coordinates);
	}

	return l_value;
}

linguistic_variable creatlinguistic_variable(char* name, linguistic_value* l_values, 
	float* univers_discourse , int nb_l_value){
	
	linguistic_variable l_variable = initLinguistic_variable(name, univers_discourse);
	int size_linguistic_value = sizeof(linguistic_value);
	
	for(int i = 0; i < nb_l_value; i++){
		linguistic_value l_value = l_values[i];
		if(!existVal(&l_value,l_variable.values_liste,compare_linguistic_value))
			inserQueue(&l_value,&(l_variable.values_liste),size_linguistic_value);		
	}

	return l_variable;
}

void afflinguistic_variable(linguistic_variable l_variable){
	printf("Linguistic value [%s]:univers of discourse[%f-%f]-->[\n"
		, l_variable.variable_name, l_variable.univers_discourse[0], l_variable.univers_discourse[1]);
	
	affListe(l_variable.values_liste, afflinguistic_value);
	
	printf("]\n");
}

void clearlinguistic_variable(linguistic_variable *l_variable){
	
	liste values_liste = l_variable->values_liste;
	if(!videListe(values_liste)){
		while(!videListe(values_liste)){
			linguistic_value l_value = *(linguistic_value *)valCellule(values_liste);
			liste coordinates_liste = l_value.coordinates_liste;
			
			suppListe(&coordinates_liste);
			values_liste = suivCellule(values_liste);
		}
	}

	suppListe(&(l_variable->values_liste));
	if(l_variable->univers_discourse)
		free(l_variable->univers_discourse);
	if(l_variable->variable_name)
		free(l_variable->variable_name);
}