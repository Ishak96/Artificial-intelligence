#ifndef STRUCT_H
#define STRUCT_H

typedef void* TElement;

typedef struct Cellule{
	TElement donnee;
	struct Cellule *suivant;
}*liste;

typedef struct {
	float x;
	float y;
}coordinates;

typedef struct{
	char* value_name;
	liste coordinates_liste;
}linguistic_value;

typedef struct{
	char* variable_name;
	liste values_liste;
	float* univers_discourse;
}linguistic_variable;

typedef struct {
	char* name_linguistic_value;
	float value;
}fuzzy_value;

typedef void (*print_element)(void* element);
typedef int (*compare_element)(void* element1,void* element2);
typedef float (*function_eval)(float x);


#endif