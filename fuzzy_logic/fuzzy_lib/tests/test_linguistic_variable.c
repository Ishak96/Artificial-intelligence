#include<linguistic.h>
#include<fuzzy.h>
#include <assert.h>

int main(int argc, char **argv){
	printf("test for a linguistic variable\n");
	printf("-----------------------\n");

	linguistic_variable l_variable;
	int nb_l_values = 2;
	linguistic_value l_values[nb_l_values];
	float univers_discourse[2] = {0, 100};

	coordinates A = {0.f, 0.f};
	coordinates B = {0.f, 1.f};
	coordinates C = {20.f, 1.f};
	coordinates D = {70.f, 0.f};
	coordinates trapez1[4] = {A, B, C, D};

	l_values[0] = get_trapezoidal_function("small", trapez1);

	coordinates A_ = {30.f, 0.f};
	coordinates B_ = {80.f, 1.f};
	coordinates C_ = {100.f, 1.f};
	coordinates D_ = {100.f, 0.f};
	coordinates trapez2[4] = {A_, B_, C_, D_};
	
	l_values[1] = get_trapezoidal_function("large", trapez2);
	
	l_variable = creatlinguistic_variable("Project staffing", l_values, univers_discourse, nb_l_values);

	//afflinguistic_variable(l_variable);

	float input = 0;
	liste fuzzied = fuzzyfication(l_variable, input);

	affListe(fuzzied, afffuzzy_value);

	return 0;
}