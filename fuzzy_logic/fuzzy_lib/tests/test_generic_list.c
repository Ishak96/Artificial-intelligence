#include<list.h>
#include <assert.h>

int main(int argc, char **argv){
	printf("test for a generic list\n");
	printf("-----------------------\n");

	int size_int=sizeof(int);
	int size_coord=sizeof(coordinates);

	coordinates c1 = {1.23, 2.45};
	coordinates c2 = {3.13, 5.35};
	coordinates c3 = {7.33, 4.15};
	coordinates c4 = {8.23, 9.45};
	coordinates c5 = {5.23, 2.45};

	coordinates c_ex = {5.23, 2.45};

	int int1=14;
	int int2=15;
	int int3=1;
	int int4=8;
	int int5=11;

	int int_ex=20;

	liste int_liste = initListe();
	liste coordinates_liste = initListe();

	inserTete(&c1,&coordinates_liste,size_coord);
	inserTete(&c2,&coordinates_liste,size_coord);
	inserTete(&c3,&coordinates_liste,size_coord);
	inserTete(&c4,&coordinates_liste,size_coord);
	inserTete(&c5,&coordinates_liste,size_coord);

	inserTete(&int1,&int_liste,size_int);
	inserTete(&int2,&int_liste,size_int);
	inserTete(&int3,&int_liste,size_int);
	inserTete(&int4,&int_liste,size_int);
	inserTete(&int5,&int_liste,size_int);

	printf("\nlist of intiger :\n");
	affListe(int_liste, affInt);

	printf("\nlist of coordinates :\n");
	affListe(coordinates_liste, affcoordinates);

	printf("\n-----------------------\n");
	printf("the value %d %s int the intiger list\n", int_ex, 
		existVal(&int_ex,int_liste,compare_int) ? "exist" : "do not exist");

	printf("the value {%f, %f} %s int the coordinates list\n", c_ex.x, c_ex.y, 
		existVal(&c_ex,coordinates_liste,compare_coordinates) ? "exist" : "do not exist");

	printf("\n-----------------------\n");
	suppListe(&int_liste);
	assert(longListe(int_liste) == 0);
	(void)puts("list of integer is vide all is good!");
	
	suppListe(&coordinates_liste);
	assert(longListe(coordinates_liste) == 0);
	(void)puts("list of coordinates is vide all is good!");

	return 0;
}