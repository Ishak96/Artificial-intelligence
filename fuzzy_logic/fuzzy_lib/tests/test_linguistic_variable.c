#include<linguistic.h>
#include<fuzzy.h>
#include <assert.h>

int main(int argc, char **argv){
	printf("test for a linguistic variable\n");
	printf("-----------------------\n");

	linguistic_variable l_variable1, l_variable2, l_variable3;
	int nb_l_values;
	float univers_discourse[2] = {0, 100};

	// Projetc staffing
	nb_l_values = 2;
	linguistic_value l_values1[nb_l_values];

	coordinates A = {0.f, 0.f};
	coordinates B = {0.f, 1.f};
	coordinates C = {20.f, 1.f};
	coordinates D = {70.f, 0.f};
	coordinates trapez1_1[4] = {A, B, C, D};

	l_values1[0] = get_trapezoidal_function("small", trapez1_1);

	coordinates A1 = {30.f, 0.f};
	coordinates B1 = {80.f, 1.f};
	coordinates C1 = {100.f, 1.f};
	coordinates D1 = {100.f, 0.f};
	coordinates trapez2_1[4] = {A1, B1, C1, D1};
	
	l_values1[1] = get_trapezoidal_function("large", trapez2_1);

	l_variable1 = creatlinguistic_variable("Project staffing", l_values1, univers_discourse, nb_l_values);

	// Projetc funding
	nb_l_values = 3;
	linguistic_value l_values2[nb_l_values];
	A.x = 0.f; A.y = 0.f;
	B.x = 0.f; B.y = 1.f;
	C.x = 30.f; C.y = 1.f;
	D.x = 50.f; D.y = 0.f;
	coordinates trapez1_2[4] = {A, B, C, D};

	l_values2[0] = get_trapezoidal_function("inadequate", trapez1_2);

	A1.x = 30.f; A1.y = 0.f;
	B1.x = 50.f; B1.y = 1.f;
	C1.x = 50.f; C1.y = 1.f;
	D1.x = 70.f; D1.y = 0.f;
	coordinates trapez2_2[4] = {A1, B1, C1, D1};
	
	l_values2[1] = get_trapezoidal_function("marjinal", trapez2_2);

	coordinates A2 = {50.f, 0.f};
	coordinates B2 = {70.f, 1.f};
	coordinates C2 = {100.f, 1.f};
	coordinates D2 = {100.f, 0.f};
	coordinates trapez3_2[4] = {A2, B2, C2, D2};

	l_values2[2] = get_trapezoidal_function("adequate", trapez3_2);
	
	l_variable2 = creatlinguistic_variable("Project funding", l_values2, univers_discourse, nb_l_values);

	// Projetc risk
	nb_l_values = 3;
	linguistic_value l_values3[nb_l_values];
	A.x = 0.f; A1.y = 0.f;
	B.x = 0.f; B1.y = 1.f;
	C.x = 20.f; C.y = 1.f;
	D.x = 40.f; D.y =  0.f;
	coordinates trapez1_3[4] = {A, B, C, D};

	l_values3[0] = get_trapezoidal_function("low", trapez1_3);

	A1.x = 25.5f; A1.y = 0.f;
	B1.x = 40.f; B1.y = 1.f;
	C1.x = 52.f; C1.y = 1.f;
	D1.x = 72.f; D1.y = 0.f;
	coordinates trapez2_3[4] = {A1, B1, C1, D1};
	
	l_values3[1] = get_trapezoidal_function("normal", trapez2_3);

	A2.x = 60.f; A2.y = 0.f;
	B2.x = 80.f; B2.y = 1.f;
	C2.x = 100.f; C2.y = 1.f;
	D2.x = 100.f; D2.y = 0.f;
	coordinates trapez3_3[4] = {A2, B2, C2, D2};

	l_values3[2] = get_trapezoidal_function("high", trapez3_3);
	
	l_variable3 = creatlinguistic_variable("Project risk", l_values3, univers_discourse, nb_l_values);

	//fuzzy

	fuzzy fuzzy_univers = init_fuzzy();
	insert_fuzzy_varaibles(2, 1, &fuzzy_univers, l_variable1, l_variable2);
	insert_fuzzy_varaibles(1, 0, &fuzzy_univers, l_variable3);

	int nb_input = 2;
	fuzzy_controler input1 = {"", "Project funding", 40};
	fuzzy_controler input2 = {"", "Project staffing", 65};

	fuzzy_all(nb_input ,&fuzzy_univers, input1, input2);
	affListe(fuzzy_univers.fuzzy_result_liste, afffuzzy_result);

	/*fuzzy_controler cond1 = {"adequate", "Project funding", 0.f, "OR"};
	fuzzy_controler cond2 = {"small", "Project staffing", 0.f, ""};*/

	clearfuzzy(&fuzzy_univers);

	return 0;
}