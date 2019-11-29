#include <stdio.h>
#include <cloud.h>
#include <jacobi_eigenvalue.h>

int main(int argc, char** argv) {

	int it_max = 500;
	float M[9], R[9];
	float EV[3];
	int IT_NUM, ROT_NUM;

	printf("entrez la matrice :\n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf("M[%d][%d] = ", i, j);
			scanf("%f", &M[3*i+j]);
		}
	}

	printf("la matrice est:\n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf("%f\t", M[3*i+j]);
		}
		printf("\n");
	}

	jacobi_eigenvalue(3, M, it_max, R, EV, &IT_NUM, &ROT_NUM);

	printf("la matrice de rotation est :\n");
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			printf("%f\t", R[3*i+j]);
		}
		printf("\n");
	}

	printf("les valeurs propres sont:\n");
	for(int i = 0; i < 3; i++){
			printf("%f\t", EV[i]);
	}

	printf("\nle nombre d'iterations est : %d\n", IT_NUM);
	printf("le nombre de rotations est : %d\n", ROT_NUM);

	return 0;
}
