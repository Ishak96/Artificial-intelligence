#ifndef STRUCT_H
#define STRUCT_H

typedef void* TElement;

typedef struct Cellule{
	TElement donnee;
	struct Cellule *suivant;
}*liste;

typedef struct {
	double x;
	double y;
}coordinates;

typedef struct{
	char* value_name;
	liste coordinates_liste;
	coordinates* trapez;
}linguistic_value;

typedef struct{
	char* variable_name;
	liste values_liste;
	double* univers_discourse;
}linguistic_variable;

typedef struct {
	char* value_name;
	char* variable_name;
	double value;
	char* logic_operator;
}fuzzy_controler;

typedef struct{
	liste fuzzy_result_liste;
	liste input_linguistic_variable;
	liste output_linguistic_variable;
}fuzzy;

typedef struct{
	liste conditions;
	fuzzy_controler deduction;	
}rules;

typedef void (*print_element)(void* element);
typedef int (*compare_element)(void* element1,void* element2);
typedef double (*function_eval)(double x);


#endif