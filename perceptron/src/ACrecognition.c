#include<stdio.h>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>
#include<perceptron.h>

#define height 500
#define width 500

#define cellI 5
#define cellJ 4

#define MAXGRID 20

#define OUTPUT_NB 2
#define INPUT_NB 20

#define sign(a)    (a<0 ? -1 : (a>0 ? 1 : 0))

int caseHeight = height / cellI;
int caseWidth = width / cellJ;

int* grid;
int* grid_test;

perceptron network;

void print_result(){
	if(network.output[0] == 1 && network.output[1] == 0){
		printf("The perceptron recognize [A]\n");
	}
	else{
		printf("The perceptron recognize [C]\n");
	}
}

int convertPoint(p,limit)
int p, limit;
{
	return sign(p-limit) * (p-limit);
}

void initSystem(){
	grid = (int *) calloc(MAXGRID, sizeof(int));
	grid_test = (int *) calloc(MAXGRID, sizeof(int));
	
	int output_layer_size = OUTPUT_NB;
	int input_layer_size = INPUT_NB;
	float weight_interval[2] = {0.0, 1.0};

	network = initialize_perceptron(weight_interval, output_layer_size, input_layer_size, heaviside);

	char* fileC = "data/dataC.dat";
	float desired_outputC[OUTPUT_NB] = {0.0, 1.0};

	learn_perceptron(fileC, &network, desired_outputC);

	char* fileA = "data/dataA.dat";
	float desired_outputA[OUTPUT_NB] = {1.0, 0.0};

	learn_perceptron(fileA, &network, desired_outputA);
}

void onMouseClick(button,state,x,y)
int button,state,x,y;
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		int i = convertPoint(y/caseHeight,cellI) - 1;
		int i_test = y/caseHeight;
		int j = x/caseWidth;
		
		grid[i*cellJ+j] = 1 - grid[i*cellJ+j];
		grid_test[i_test*cellJ+j] = 1 - grid_test[i_test*cellJ+j];

		glutPostRedisplay();
	}
}

void keyPressed(unsigned char key, int x, int y) {  
	if(key - '0' == -35){
		if(grid != NULL){
			test_phase(grid_test, &network);
			print_result();
		}
	}
}  

void drawPaper(void)
{
	int incY = caseHeight;
	int incX = caseWidth;
	int x = incX;
	int y = incY;

	while(x < width){
		glBegin(GL_LINES);
			glVertex2f(x, 0);
			glVertex2f(x, height);
		glEnd();
		x += incX;
	}

	while(y < height){
		glBegin(GL_LINES);
			glVertex2f(0, y);
			glVertex2f(width, y);
		glEnd();
		y += incY;
	}
}

void drawGrid(void)
{
	int color;
	
	if(grid){
		for(int i = 0; i < cellI; i++){
			for(int j = 0; j < cellJ; j++){
				color = grid[i*cellJ+j];
				if(color != 0){
					glBegin(GL_QUADS);
						glColor3d(color,color,color);
						glVertex2d(j*caseWidth,i*caseHeight);
						glVertex2d((j+1)*caseWidth,i*caseHeight);
						glVertex2d((j+1)*caseWidth,(i+1)*caseHeight);
						glVertex2d(j*caseWidth,(i+1)*caseHeight);
					glEnd();
				}
			}
		}
	}

	drawPaper();
}

void init(void) {
    glClearColor(0.0,0.0,0.0,0.0);
    glViewport(0, 0, 500, 500);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glOrtho(0, 500, 0, 500, 1, -1);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawGrid();
	
	glFlush();
}

int main(argc, argv)
int argc;
char** argv;
{

	srand((unsigned int)time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(900, 100);
	glutCreateWindow("ACrecognition");

	init();
	initSystem();

	glutDisplayFunc(display);
	glutMouseFunc(onMouseClick);
	glutKeyboardFunc(keyPressed);

	glutMainLoop();
	
	if(grid)
		free(grid);

	if(grid_test)
		free(grid_test);

	destroy_perceptron(&network);

	return 0;
}