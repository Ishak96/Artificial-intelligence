#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glut.h>
#include <ppm.h>
#include <kohonen.h>

#define DEFAULT_WIDTH  500
#define DEFAULT_HEIGHT 500

#define LATICE_SIZE	   50
#define MAP_SIZE_X	   50
#define MAP_SIZE_Y	   1

#define DATA_SIZE	   1
#define DATA_NUM_IN	   2

#define NUM_WEIGHT	   2

/*Variables globales*/
TRAINING_DATA Dataset;
Map* NeuronSet;
DATA sDATA;
int i_bum = 0;

int cpt = 0;
int calc = 0;

int NB_VILLE = 0;

int width  = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;

CITY* citys;

GLuint textureID;

void load_cities(){
	FILE* fp;
	int line;
	int colum = 2;
	double x, y;
	CITY city;

	fp = fopen("Villes_et_positions_dans_image.txt", "r+");

	if(fp == NULL){
		printf("load_cities:invalide file entering %s %d\n", __FUNCTION__, __LINE__);
		exit(0);
	}

	fscanf(fp, "nombre de villes = %d\n", &line);
	init_TRAINING_DATA(&Dataset, line, colum);
	citys = (CITY *)malloc(line * sizeof(CITY));
	NB_VILLE = line;

	for(int k = 0; k < Dataset.size; k++){
			fscanf(fp, "%s %lf %lf\n", city.name, &x, &y);
			city.x = x;
			city.y = y;

			citys[k] = city;
			Dataset.entries[k].input[0] = x;
			Dataset.entries[k].input[1] = y;
	}

	fclose(fp);
}

/*OpenGL*/
GLuint charger_texture(unsigned char * data){
	GLuint textureBidule;
	glGenTextures(1, &textureBidule);
	glBindTexture(GL_TEXTURE_2D, textureBidule);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	return textureBidule;
}

unsigned char * transform_img_to_vector(const char * filename, int *width , int *height){
	Image* image = NULL;
	image = readPPM(filename);

	if (image == NULL) 
		EXIT_ON_ERROR("error loading img");
	
	int i, j;
	unsigned char* data = NULL;
	*width = image->x;
	*height = image->y;
	data = (unsigned char*)malloc(3 * image->x * image->y * sizeof(unsigned char));

	for(i = 0; i < image->x * image->y; i++){
		j = i * 3;
		data[j] = image->data[i].r;
		data[j + 1] = image->data[i].g;
		data[j + 2] = image->data[i].b;
	}

	if (image != NULL){
		free(image->data);
		image->data = NULL;
		free(image);
		image = NULL ;
	}
	return data;
}

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
void draw_map(){
	double x,y,neighbour_x,neighbour_y;
	int neighbour_i;
	
	Neuron* lattice = NeuronSet->lattice;
	int size = NeuronSet->latice_size;

	glPointSize(10); 
	glLineWidth(2);
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

			if(i != size-1){
				glBegin(GL_LINE_LOOP);
					glColor3f(0.0,1.0,0.0);
					glVertex2f(x,y);
					glVertex2f(neighbour_x,neighbour_y);
				glEnd();
			}
	}
}

void affichage(){
	glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(0,   0);
		glTexCoord2i(0, 1); glVertex2i(0,   height);
		glTexCoord2i(1, 1); glVertex2i(width, height);
		glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();
	
	glPointSize(15); 
	for (int i = 0; i < NB_VILLE; i++){
		glBegin(GL_POINTS);
			glColor3f(0.0, 0.0, 0.0);
			glVertex2f(citys[i].x, citys[i].y);
		glEnd();
		
		glColor3f(0, 0, 0);
			draw_text(citys[i].x - 20, citys[i].y + 20, "%s", citys[i].name);
	}

	glColor3f(0.0,0.0,0.0);
	draw_text(60, 70, "nb iter: %d", cpt);
}

/* Initialize OpenGL Graphics */
void init(int w, int h){

	glViewport(0, 0, w, h);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

  	glOrtho(0.0, w, h, 0.0, -1, 1);
 
	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void reshape(int h, int w){
	glViewport(0, 0,h ,w);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, width, height, 0.0, -1, 1);

	glMatrixMode(GL_MODELVIEW);

	glutPostRedisplay();
}

void display(void){

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	
		affichage();
		draw_map();
	glFlush();
	glutSwapBuffers();
}

void idle() {
	if(calc){
		cpt++;

			sDATA = SortData(Dataset);
			
			MAP_activation(NeuronSet, sDATA);

			i_bum = find_bmu(NeuronSet);

			adjust_weights(NeuronSet, i_bum, sDATA);
    	
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

int main(int argc, char **argv){

	unsigned char * data = NULL;
	if (argc != 2) 
		EXIT_ON_ERROR("You must specified a .ppm file");
	data = transform_img_to_vector(argv[1], &width, &height);
	
	load_cities();
	NeuronSet = init_map(MAP_SIZE_X, MAP_SIZE_Y, NUM_WEIGHT);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Kohonen plus court chemin");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(clavier);

	init(width, height);

	textureID = charger_texture(data);
	if(data != NULL){
		free(data);
		data = NULL;
	}

	glutMainLoop();

	glDeleteTextures(1, &textureID);

  return 0;
}
