#include<stdio.h>
#include<stdlib.h>
#include <math.h>

#include<superquadrics.h>

int main(int argc, char** argv) {

	double m = 10;
	double n = 10;
	double a = 2;
	double b = 2;
	double c = 2;
	double p = 0;
	double q = 0;
	double r0 = 1;
	double r1 = 0.5;
	
	double** values = discretization(-PI / 2, PI / 2, -PI, PI, m, n);

	for(int i = 0; i <= m; i++){
		printf("%f || ", values[0][i]);
	}
	
	printf("\n\t\t\t\t\t\t\t______________________________________________________________\n");

	for(int i = 0; i <= n; i++){
		printf("%f || ", values[1][i]);
	}

	printf("\n\n summit building :");
	summit** sum = summit_building(a, b, c, p, q, m, n, values, r0, r1);

	for(int i = 0; i <= m; i++){
		for(int j = 0; j <= n; j++){
			printf("for{phi = %f, theta = %f} => {x = %f, y = %f, z = % f}\n", values[0][i], values[1][j], sum[i][j].x, sum[i][j].y, sum[i][j].z);
		}
	}

	double phi = -1.570796;
	double theta = -2.513274;
	double e1 = 2;
	double e2 = 2;
	double x, ret;

	x = 60.0;
	ret = cos( x*VAL );

	printf("abs(phi) = %f\n", abs_(phi));
	printf("cos(phi) = %f\n", cos(phi*VAL));
	printf("(pow(cos(phi), e1)) = %f\n", pow(cos(phi*VAL), e1));
	printf("(pow(cos(theta), e2)) = %f\n", pow(cos(theta*VAL), e2));
	printf("a * (pow(cos(phi), e1)) * (pow(cos(theta), e2)) = %f\n", a * (pow(cos(phi*VAL), e1)) * (pow(cos(theta*VAL), e2)));

	printf("The cosine of %lf is %lf degrees\n", x, ret);

	return 0;
}
