#include<linguistic.h>
#include<fuzzy.h>
#include<rule.h>
#include <assert.h>

int main(int argc, char **argv){
	printf("test for a linguistic variable\n");
	printf("-----------------------\n");

	linguistic_variable l_variable1, l_variable2, l_variable3;
	int nb_l_values;
	float univers_discourse[2] = {0, 100};

	// Projetc staffing
	nb_l_values = 2;
	coordinates A = {0.f, 0.f};
	coordinates B = {0.f, 1.f};
	coordinates C = {20.f, 1.f};
	coordinates D = {70.f, 0.f};
	coordinates trapez1_1[4] = {A, B, C, D};

	coordinates A1 = {30.f, 0.f};
	coordinates B1 = {80.f, 1.f};
	coordinates C1 = {100.f, 1.f};
	coordinates D1 = {100.f, 0.f};
	coordinates trapez2_1[4] = {A1, B1, C1, D1};

	creatlinguistic_variable(nb_l_values, "Project staffing", univers_discourse, &l_variable1,
	 	get_trapezoidal_function("small", trapez1_1), get_trapezoidal_function("large", trapez2_1));

	// Projetc funding
	nb_l_values = 3;
	A.x = 0.f; A.y = 0.f;
	B.x = 0.f; B.y = 1.f;
	C.x = 30.f; C.y = 1.f;
	D.x = 50.f; D.y = 0.f;
	coordinates trapez1_2[4] = {A, B, C, D};

	A1.x = 30.f; A1.y = 0.f;
	B1.x = 50.f; B1.y = 1.f;
	C1.x = 50.f; C1.y = 1.f;
	D1.x = 70.f; D1.y = 0.f;
	coordinates trapez2_2[4] = {A1, B1, C1, D1};

	coordinates A2 = {50.f, 0.f};
	coordinates B2 = {70.f, 1.f};
	coordinates C2 = {100.f, 1.f};
	coordinates D2 = {100.f, 0.f};
	coordinates trapez3_2[4] = {A2, B2, C2, D2};
	
	creatlinguistic_variable(nb_l_values, "Project funding", univers_discourse, &l_variable2, 
		get_trapezoidal_function("inadequate", trapez1_2), get_trapezoidal_function("marjinal", trapez2_2),
		get_trapezoidal_function("adequate", trapez3_2));

	// Projetc risk
	nb_l_values = 3;
	A.x = 0.f; A1.y = 0.f;
	B.x = 0.f; B1.y = 1.f;
	C.x = 20.f; C.y = 1.f;
	D.x = 40.f; D.y =  0.f;
	coordinates trapez1_3[4] = {A, B, C, D};

	A1.x = 25.5f; A1.y = 0.f;
	B1.x = 40.f; B1.y = 1.f;
	C1.x = 52.f; C1.y = 1.f;
	D1.x = 72.f; D1.y = 0.f;
	coordinates trapez2_3[4] = {A1, B1, C1, D1};

	A2.x = 60.f; A2.y = 0.f;
	B2.x = 80.f; B2.y = 1.f;
	C2.x = 100.f; C2.y = 1.f;
	D2.x = 100.f; D2.y = 0.f;
	coordinates trapez3_3[4] = {A2, B2, C2, D2};
	
	creatlinguistic_variable(nb_l_values, "Project risk", univers_discourse, &l_variable3, 
		get_trapezoidal_function("low", trapez1_3), get_trapezoidal_function("normal", trapez2_3),
		get_trapezoidal_function("high", trapez3_3));

	//fuzzy

	fuzzy fuzzy_univers = init_fuzzy();
	insert_fuzzy_varaibles(2, 1, &fuzzy_univers, l_variable1, l_variable2);
	insert_fuzzy_varaibles(1, 0, &fuzzy_univers, l_variable3);

	int nb_input = 2;
	fuzzy_controler input1 = {"", "Project funding", 40};
	fuzzy_controler input2 = {"", "Project staffing", 65};

	fuzzy_all(nb_input ,&fuzzy_univers, input1, input2);
	printf("\n------------------------------------------\n");
	affListe(fuzzy_univers.fuzzy_result_liste, afffuzzy_result);

	rules r1;
	init_rules(&r1);
	
	fuzzy_controler cond1 = {"adequate", "Project funding", 0.f, "OR"};
	fuzzy_controler cond2 = {"small", "Project staffing", 0.f, ""};
	fuzzy_controler dedu1 = {"low", "Project risk", 0.f, ""};

	r1.deduction = dedu1;
	insert_values_into_liste(2, sizeof(fuzzy_controler), &(r1.conditions), &cond1, &cond2);

	rules r2;
	init_rules(&r2);
	
	fuzzy_controler cond1_2 = {"marjinal", "Project funding", 0.f, "AND"};
	fuzzy_controler cond2_2 = {"large", "Project staffing", 0.f, ""};
	fuzzy_controler dedu2 = {"normal", "Project risk", 0.f, ""};

	r2.deduction = dedu2;
	insert_values_into_liste(2, sizeof(fuzzy_controler), &(r2.conditions), &cond1_2, &cond2_2);

	rules r3;
	init_rules(&r3);
	
	fuzzy_controler cond1_3 = {"inadequate", "Project funding", 0.f, ""};
	fuzzy_controler dedu3 = {"high", "Project risk", 0.f, ""};

	r3.deduction = dedu3;
	insert_values_into_liste(1, sizeof(fuzzy_controler), &(r3.conditions), &cond1_3);

	liste rules_liste = initListe();
	insert_values_into_liste(3, sizeof(rules), &rules_liste, &r1, &r2, &r3);

	printf("\n------------------------------------------\n");
	liste result_apply_rules = apply_rules(rules_liste, fuzzy_univers.fuzzy_result_liste);
	affListe(result_apply_rules, afffuzzy_result);

	aggregation(fuzzy_univers, result_apply_rules);
	
	linguistic_variable l_variable_result = *( linguistic_variable*)
											valCellule(fuzzy_univers.output_linguistic_variable);
	afflinguistic_variable(l_variable_result, afflinguistic_value_trapez);

	suppListe(&rules_liste);
	suppListe(&result_apply_rules);
	clearRule(&r1);
	clearRule(&r2);
	clearRule(&r3);
	clearfuzzy(&fuzzy_univers);

	return 0;
}