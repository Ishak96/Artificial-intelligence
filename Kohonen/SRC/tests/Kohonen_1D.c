#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glut.h>
#include <kohonen.h>

#define DEFAULT_WIDTH  500
#define DEFAULT_HEIGHT 500

#define LATICE_SIZE	   20
#define MAP_SIZE_X	   20
#define MAP_SIZE_Y	   1

#define DATA_SIZE	   20
#define DATA_NUM_IN	   2

#define NUM_WEIGHT	   2

#define MAX 		   100
#define MIN 		   0

#define SHIFT_X		   100
#define SHIFT_Y		   100

#define MIN_R		   50
#define MAX_R		   750

/*Variables globales*/
TRAINING_DATA Dataset;
Map* NeuronSet;
DATA sDATA;
int i_bum = 0;

int cpt = 0;
int calc = 0;


int width  = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
/*OpenGL*/
void draw_text(float x, float y, const char *fmt, ...){
	char    buf[1024];
	char    *text = buf;
	va_list ap;

	if(fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	glDisable(GL_TEXTURE_2D);
	glRasterPos2i(x, y);
	while (*text)
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text++);

	glEnable(GL_TEXTURE_2D);
}

/* fonction d'affichage appelée a chaque refresh*/
void affichage(){
	double x,y,neighbour_x,neighbour_y;
	int neighbour_i;
	
	Neuron* lattice = NeuronSet->lattice;
	int size = NeuronSet->latice_size;

	glPointSize(5);
	glTranslatef(SHIFT_X, SHIFT_Y, 0);
	glBegin(GL_POINTS);
		glColor3f(1.0,1.0,1.0);
		glVertex2f(sDATA.input[0],sDATA.input[1]);
	glEnd();	
	for(int i = 0; i < size; i++){
			x = lattice[i].weights[0];
			y = lattice[i].weights[1];

			neighbour_i = (i != size-1) ? i+1 : 0;

			glBegin(GL_POINTS);
				if(i == i_bum)
					glColor3f(0.0,1.0,0.0);
				else
					glColor3f(1.0,0.0,0.0);
				glVertex2f(x,y);
			glEnd();

			neighbour_x = lattice[neighbour_i].weights[0];
			neighbour_y = lattice[neighbour_i].weights[1];

			glBegin(GL_LINE_LOOP);
				glColor3f(1.0,0.0,0.0);
				glVertex2f(x,y);
				glVertex2f(neighbour_x,neighbour_y);
			glEnd();
	}
	glTranslatef(-SHIFT_X, -SHIFT_Y, 0);
	glColor3f(1.0,1.0,1.0);
	draw_text(150, 20, "nb iter: %d", cpt);

	glFlush();
	glutSwapBuffers();
}

void init(int w, int h){
	int taille_point = 5;
	
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, 200, 200, 0.0, -1, 1);


  	glPointSize(taille_point);  
  	glMatrixMode(GL_MODELVIEW);

  	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  	glClearDepth(0.0f);
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void reshape(int h, int w){
	width = w;
	height = h;

	glViewport(0, 0,w ,h);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();


	glOrtho(0.0, 200, 200, 0.0, -1, 1);

  	glMatrixMode(GL_MODELVIEW);

  	glutPostRedisplay();
}

void display(void){
  
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	glMatrixMode(GL_MODELVIEW);
  	glLoadIdentity();	
		affichage();
	glFlush();
	glutSwapBuffers();
}

void idle() {
	if(calc){
		cpt++;
		if(!DataSorted(Dataset)){

			sDATA = SortData(Dataset);
	
			MAP_activation(NeuronSet, sDATA);

			i_bum = find_bmu(NeuronSet);

			adjust_weights(NeuronSet, i_bum, sDATA);
			sleep(0.5);
		}
		else{
			printf("Tous les données ont été visité -> FIN !!\n");
			calc = !calc;
		}	
    	glutPostRedisplay();
  	}
}

void clavier(unsigned char touche, int x, int y) {
  switch (touche){
	case 'p':
		calc = !calc;
	break;

	case 'q':
		exit(0);
	}
}

int main(int argc, char** argv){

	srand(time(NULL));


	NeuronSet = init_map(MAP_SIZE_X, MAP_SIZE_Y, NUM_WEIGHT, MIN_R, MAX_R);
	InitialiseSet(&Dataset, DATA_SIZE, DATA_NUM_IN, MIN, MAX);
	sDATA = SortData(Dataset);

  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_DOUBLE);
  	glutInitWindowSize(width, height);
  	glutCreateWindow("Kohonen 1D");  

	init(width, height);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(clavier);

	glutMainLoop();
	
	distroyTRAINING_DATA(&Dataset);

	distroyMap(NeuronSet);

	return 0;
}