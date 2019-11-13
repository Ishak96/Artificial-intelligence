#include <linguistic.h>

linguistic_value initLinguistic_value(char* name, coordinates* trapez){
	linguistic_value l_value;
	
	l_value.value_name = name;
	l_value.trapez = trapez;

	return l_value;
}

linguistic_variable initLinguistic_variable(char* name, float* univers_discourse){
	linguistic_variable l_variable;

	l_variable.variable_name = name;
	l_variable.values_liste = initListe();
	l_variable.univers_discourse = univers_discourse;

	return l_variable;
}

void creatlinguistic_variable(int nb, char* name, float* univers_discourse, linguistic_variable* l_variable, ...){
	va_list ap;
	va_start(ap, l_variable);
	*l_variable = initLinguistic_variable(name, univers_discourse);
	int size_linguistic_value = sizeof(linguistic_value);
	
	while(nb > 0){
		linguistic_value l_value;
		l_value = va_arg(ap, linguistic_value);
		
		if(!existVal(&l_value,l_variable->values_liste,compare_linguistic_value))
			inserQueue(&l_value,&(l_variable->values_liste),size_linguistic_value);	

		nb--;	
	}
	va_end(ap);
}

void afflinguistic_variable(linguistic_variable l_variable, print_element aff_function){
	printf("Linguistic value [%s]:univers of discourse[%f-%f]-->[\n"
		, l_variable.variable_name, l_variable.univers_discourse[0], l_variable.univers_discourse[1]);
	
	affListe(l_variable.values_liste, aff_function);
	
	printf("]\n");
}

void clearlinguistic_variable(linguistic_variable* l_variable){

	suppListe(&(l_variable->values_liste));
}