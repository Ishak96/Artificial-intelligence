#include<stdio.h>
#include<stdlib.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<GL/glu.h>

#define height 500
#define width 500

#define cellI 5
#define cellJ 4

#define MAXGRID 20

#define sign(a)    (a<0 ? -1 : (a>0 ? 1 : 0))

int caseHeight = height / cellI;
int caseWidth = width / cellJ;

int* grid;

int convertPoint(p,limit)
int p, limit;
{
	return sign(p-limit) * (p-limit);
}

void initGrid(int* grid_init){
	grid = (int *) calloc(MAXGRID, sizeof(int));
}

void onMouseClick(button,state,x,y)
int button,state,x,y;
{
	if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){
		int i = convertPoint(y/caseHeight,cellI) - 1;
		int j = x/caseWidth;
		grid[i*cellJ+j] = 1 - grid[i*cellJ+j];

		glutPostRedisplay();
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
	/*if(argc < 2){
		fprintf(stderr, "ERROR missing parameter\n");
		printf("Usage -> %s [learning process file]\n", argv[0]);
		return -1;
	}*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(900, 100);
	glutCreateWindow("ACrecognition");

	init();
	initGrid(grid);

	glutDisplayFunc(display);
	glutMouseFunc(onMouseClick);

	glutMainLoop();
	
	if(grid)
		free(grid);

	return 0;
}