#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glut.h>
#include <ppm.h>
#include <kohonen.h>

#define height  800
#define width 	800

#define NUM_WEIGHT 	   3
#define MIN_R	   	   0
#define MAX_R	   	   256

#define NB_COLOR	   32

#define SIZE 		   NB_COLOR/2

#define LATICE_SIZE	   SIZE*SIZE

#define MAP_SIZE_X	   sqrt(LATICE_SIZE)
#define MAP_SIZE_Y	   MAP_SIZE_X

#define rows 		   sqrt(LATICE_SIZE)
#define cols  		   rows

int caseHeight = height/rows;
int caseWidth  = width/cols;

TRAINING_DATA Dataset;
Map* NeuronSet;
Image* imageOriginal;

int cpt = 0;
int calc = 0;
int comp = 0;

TRAINING_DATA convertImageToDataTrain(Image* image){
	int sizeX = image->x;
	int sizeY = image->y;
	TRAINING_DATA Dataset;

	init_TRAINING_DATA(&Dataset, sizeX*sizeY, NUM_WEIGHT);
	for(int i = 0; i < sizeY; i++){
		for(int j = 0; j < sizeX; j++){
			float scale = (i != 0 && j != 0) ? (j/i) : 0.f;
			Dataset.entries[i*sizeX+j].input[0] = image->data[i*sizeX+j].r + scale;
			Dataset.entries[i*sizeX+j].input[1] = image->data[i*sizeX+j].g + scale;
			Dataset.entries[i*sizeX+j].input[2] = image->data[i*sizeX+j].b + scale;
		}
	}

	return Dataset;
}

double distance(double* x, double* y){
	return sqrt(pow(x[0]-y[0], 2) + pow(x[1]-y[1], 2) + pow(x[2]-y[2], 2));
}

Pixel MIN_RGB(Map* n, double* data){
	int latice_size = n->latice_size;
	double min_dist = 100000;
	double* data_n;
	Pixel result;

	for(int i = 0; i < latice_size; i++){
		data_n = n->lattice[i].weights;
		if(distance(data, data_n) < min_dist){
			min_dist = distance(data, data_n);
			result.r = data_n[0];
			result.g = data_n[1];
			result.b = data_n[2];
		}
	}

	return result;
}

Image* IMAGECompressee(Map* n, Image* imageOriginal){

	Image* image = (Image *)malloc(sizeof(Image));
	
	image->x 	 = imageOriginal->x;
	image->y 	 = imageOriginal->y;

	image->data  = (Pixel *)malloc(image->x * image->y * sizeof(Pixel));

	for(int i = 0; i < image->y; i++){
		for(int j = 0; j < image->x; j++){
			double data[3];
			data[0] = imageOriginal->data[i*image->x+j].r;
			data[1] = imageOriginal->data[i*image->x+j].g;
			data[2] = imageOriginal->data[i*image->x+j].b;

			image->data[i*image->x+j] = MIN_RGB(n, data);
		}
	}

	return image;
}

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
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);

	glEnable(GL_TEXTURE_2D);
}

void affichage(){
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < cols; j++){
			glBegin(GL_QUADS);
				glColor3f(NeuronSet->lattice[i*(int)cols+j].weights[0]/256.0,
						  NeuronSet->lattice[i*(int)cols+j].weights[1]/256.0,
						  NeuronSet->lattice[i*(int)cols+j].weights[2]/256.0);

				glVertex2d(j*caseWidth,i*caseHeight);
				glVertex2d((j+1)*caseWidth,i*caseHeight);
				glVertex2d((j+1)*caseWidth,(i+1)*caseHeight);
				glVertex2d(j*caseWidth,(i+1)*caseHeight);
			glEnd();
		}
	}
	glColor3f(0.0,0.0,0.0);
	draw_text(230, 20, "nb iter: %d", cpt);
}

void init(void){
	glClearColor(0.0,0.0,0.0,0.0);
    glViewport(0, 0, 800, 800);
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    glOrtho(0, 800, 0, 800, 1, -1);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		affichage();
	glFlush();
}

void idle(){
	if(calc){
		cpt++;
		DATA sDATA = SortUnselectedData(&Dataset);

		MAP_activation(NeuronSet, sDATA);

		int i_bum = find_bmu(NeuronSet);

		adjust_weights(NeuronSet, i_bum, sDATA);
			
		if(DataSorted(Dataset)){
			printf("Tous les données ont été visité -> FIN !!\n");
			calc = !calc;
		}

   		glutPostRedisplay();
  	}
  	else if(comp){
		Image* imageCompressee = IMAGECompressee(NeuronSet, imageOriginal);

		writePPM("imageCompressee.ppm", imageCompressee);
		free(imageCompressee->data);
		free(imageCompressee);
		comp = !comp;
  	}
}

void clavier(unsigned char touche, int x, int y) {
  switch (touche){
	case 'p':
		calc = !calc;
		comp = 0;
	break;

	case 'c':
		calc = 0;
		comp = 1;
	break;

	case 'q':
		exit(0);
	}
}

int main(int argc, char **argv){
	
	srand(time(NULL));

	if (argc != 2) 
		EXIT_ON_ERROR("You must specified a .ppm file");
	imageOriginal = readPPM(argv[1]);

	NeuronSet = init_map((int)MAP_SIZE_X, (int)MAP_SIZE_Y, NUM_WEIGHT, MIN_R, MAX_R);
	Dataset = convertImageToDataTrain(imageOriginal);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width,height);
	glutInitWindowPosition(900, 100);	
	glutCreateWindow("Kohonen compression");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(clavier);

	glutMainLoop();

	free(imageOriginal->data);
	free(imageOriginal);
	return 0;
}
