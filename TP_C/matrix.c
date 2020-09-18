#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

enum TYPE {INTEGER,FLOATANT};

typedef union {
	int int_val;
	float float_val;
} value;

typedef struct {
	int rows; int cols;
	enum TYPE type;
	value** mat;
} Matrix;

Matrix matrix(int rows, int cols, enum TYPE t) {
	Matrix m;
	m.rows=rows; m.cols=cols;
	m.type=t;

	m.mat=(value **) malloc(rows*sizeof(value *));
	for(int i=0; i<rows; i++)
		m.mat[i]=(value *) malloc(cols*sizeof(value));

	return m;
}

void clearMatrix(Matrix* m) {
	for(int i=0; i<m->rows; i++) {
		free(m->mat[i]);
		m->mat[i]=NULL;
	}

	free(m->mat);
	m->mat=NULL;
	
	m->rows=0; m->cols=0;
}

void dumpMatric(Matrix m) {
	for(int i=0; i<m.rows; i++) {
		for(int j=0; j<m.cols; j++) {
			if(m.type==INTEGER)
				printf("%d\t", m.mat[i][j].int_val);
			else
				printf("%f\t", m.mat[i][j].float_val);
		}
		printf("\n");
	}
}

Matrix addMatrix(Matrix m1, Matrix m2) {
	int min_rows=MIN(m1.rows,m2.rows), min_cols=MIN(m1.cols,m2.cols);
	int max_rows=MAX(m1.rows,m2.rows), max_cols=MAX(m1.cols,m2.cols);

	Matrix m3=matrix(max_rows, max_cols,m1.type);

	for(int i=0; i<min_rows; i++) {
		for(int j=0; j<min_cols; j++) {
			if(m3.type==INTEGER)
				m3.mat[i][j].int_val=m1.mat[i][j].int_val+m2.mat[i][j].int_val;
			else
				m3.mat[i][j].float_val=m1.mat[i][j].float_val+m2.mat[i][j].float_val;
		}
	}

	for(int i=0; i<m1.rows; i++) {
		if(i>=min_rows)
			min_cols=0;
		for(int j=min_cols; j<m1.cols; j++) {
				m3.mat[i][j]=m1.mat[i][j];
		}
	}

	for(int i=0; i<m2.rows; i++) {
		if(i>=min_rows)
			min_cols=0;		
		for(int j=min_cols; j<m2.cols; j++) {
				m3.mat[i][j]=m2.mat[i][j];
		}
	}

	return m3;
}

Matrix productMatrix(Matrix m1, Matrix m2) {
	int rows=m1.rows, cols=m2.cols;

	Matrix m3=matrix(rows, cols,m1.type);

	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			m3.mat[i][j].int_val=m3.mat[i][j].float_val=0;
			for(int k=0; k<m2.rows; k++) {
				if(m3.type==INTEGER)
					m3.mat[i][j].int_val+=(m1.mat[i][k].int_val*m2.mat[k][j].int_val);
				else
					m3.mat[i][j].float_val+=(m1.mat[i][k].float_val*m2.mat[k][j].float_val);
			}
		}
	}

	return m3;
}

Matrix readFileMatrix(const char* file_name) {
	if(file_name==NULL) {
		fprintf(stderr, "Argument error check the file name.\n");
		exit(1);
	}

	Matrix m;
	enum TYPE t; char type_file[5];
	int rows, cols;

	FILE *file;

	file=fopen(file_name,"r");
	if(file==NULL) {
		fprintf(stderr, "%s dose not exist !\n", file_name);
		exit(1);		
	}

	fscanf(file, "%s\n", type_file);
	t=(!strcmp(type_file,"int")) ? INTEGER:FLOATANT;
	
	fscanf(file, "%d %d\n", &rows, &cols);
	m=matrix(rows, cols, t);

	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			if(m.type==INTEGER)
				fscanf(file, "%d", &m.mat[i][j].int_val);
			else
				fscanf(file, "%f", &m.mat[i][j].float_val);
		}
	}
	
	fclose(file);
	return m;
}

int main() {

	const char* file_matrixA="intMatrixA.txt";
	const char* file_matrixB="intMatrixB.txt";

	Matrix A = readFileMatrix(file_matrixA);
	Matrix B = readFileMatrix(file_matrixB);
	Matrix C = addMatrix(A,B);
	Matrix D = productMatrix(A,B);

	printf("-------- A --------\n");
	dumpMatric(A);

	printf("-------- B --------\n");
	dumpMatric(B);

	printf("-------- Sum of A and B --------\n");
	dumpMatric(C);

	printf("-------- Product of A and B --------\n");
	dumpMatric(D);	

	clearMatrix(&A);
	clearMatrix(&B);
	clearMatrix(&C);
	clearMatrix(&D);

	return 0;
}