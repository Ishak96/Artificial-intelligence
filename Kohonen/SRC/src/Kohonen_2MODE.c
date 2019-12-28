#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <GL/glut.h>
#include <ppm.h>
#include <kohonen.h>

#define DEFAULT_WIDTH  800
#define DEFAULT_HEIGHT 800

#define MODE 0

#if MODE
	#define LATICE_SIZE	   50

	#define MIN_R		   50
	#define MAX_R		   750
#else
	#define LATICE_SIZE	   20

	#define MIN_R		   -50
	#define MAX_R		   50
#endif

#define MAP_SIZE_X	   LATICE_SIZE
#define MAP_SIZE_Y	   1

#define DATA_SIZE	   20
#define DATA_NUM_IN	   2

#define NUM_WEIGHT	   2

#define MAX 		   200
#define MIN 		   0

#define SHIFT_X		   250
#define SHIFT_Y		   250

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

	for(int i = 0; i < size; i++){
			x = lattice[i].weights[0];
			y = lattice[i].weights[1];

			neighbour_i = (i != size-1) ? i+1 : 0;

			glBegin(GL_POINTS);
				float pot = lattice[i].pot;
				if(pot < 10)
					glColor3f(255,0,0);
				else if(pot < 20)
					glColor3f(255,128,0);
				else if(pot < 30)
					glColor3f(255,255,0);
				else if(pot < 40)
					glColor3f(128,255,0);
				else if(pot < 50)
					glColor3f(0,255,0);
				else if(pot < 60)
					glColor3f(0,255,128);
				else if(pot < 70)
					glColor3f(0,255,255);
				else if(pot < 80)
					glColor3f(0,128,255);
				else
					glColor3f(0,0,255);

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

/* fonction d'affichage appelée a chaque refresh*/
void affichage(){
	#if MODE
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
		
		draw_map();

		glColor3f(0.0,0.0,0.0);
		draw_text(60, 70, "nb iter: %d", cpt);
	#else
		glTranslatef(SHIFT_X, SHIFT_Y, 0);
			glBegin(GL_POINTS);
				glColor3f(1.0,1.0,1.0);
				glVertex2f(sDATA.input[0],sDATA.input[1]);
			glEnd();
			
			draw_map();
		glTranslatef(-SHIFT_X, -SHIFT_Y, 0);
		glColor3f(1.0,1.0,1.0);
		draw_text(230, 20, "nb iter: %d", cpt);
	#endif
}

void init(int w, int h){

	#if MODE
		int taille_point = 15;
	#else
		int taille_point = 10;
	#endif

	int line_width = 4;
	
	glViewport(0, 0, w, h);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	#if MODE
		glOrtho(0.0, w, h, 0.0, -1, 1);
	#else
		glOrtho(0.0, 200, 200, 0.0, -1, 1);
	#endif


	glPointSize(taille_point); 
	glLineWidth(line_width);
	glMatrixMode(GL_MODELVIEW);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void reshape(int h, int w){
	#if MODE
	#else
		width = w;
		height = h;
	#endif

	glViewport(0, 0,w ,h);
  	glMatrixMode(GL_PROJECTION);
  	glLoadIdentity();

  	#if MODE
  		glOrtho(0.0, width, height, 0.0, -1, 1);
  	#else
  		glOrtho(0.0, 500, 500, 0.0, -1, 1);
  	#endif

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

void idle(){
	if(calc){
		cpt++;
			sDATA = SortData(Dataset);
	
			MAP_activation(NeuronSet, sDATA);

			i_bum = find_bmu(NeuronSet);

			adjust_weights(NeuronSet, i_bum, sDATA);
			
			#if MODE
				sleep(0.5);
			#else
				sleep(1);
			#endif

			#if MODE
			#else
				if(DataSorted(Dataset)){
					printf("Tous les données ont été visité -> FIN !!\n");
					calc = !calc;
				}
			#endif
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

	srand(time(NULL));

	#if MODE
		unsigned char * data = NULL;
		if (argc != 2) 
			EXIT_ON_ERROR("You must specified a .ppm file");
		data = transform_img_to_vector(argv[1], &width, &height);
	
		load_cities();
	#else
		InitialiseSet(&Dataset, DATA_SIZE, DATA_NUM_IN, MIN, MAX);
		sDATA = SortData(Dataset);
	#endif

	NeuronSet = init_map(MAP_SIZE_X, MAP_SIZE_Y, NUM_WEIGHT, MIN_R, MAX_R);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(width, height);
	glutCreateWindow("Kohonen");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(clavier);

	init(width, height);

	#if MODE
		textureID = charger_texture(data);
		if(data != NULL){
			free(data);
			data = NULL;
		}
	#endif

	glutMainLoop();

	#if MODE
		glDeleteTextures(1, &textureID);
	#endif

  return 0;
}
