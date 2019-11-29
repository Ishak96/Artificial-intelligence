#include<draw.h>
#include <camera.h>
#include <cloud.h>
#include <X11/Xlib.h>
#include <stdio.h>

extern float xrot, yrot;
extern float zoom_x, zoom_y;
extern int aff;

//variables globales
float color[3];
char* file_name;
float** cloud;
int size;
float fact;
int done = 0;

float a_i, b_i, c_i, e1_i, e2_i, tx, ty, tz, angle1_i, angle2_i, angle3_i;
double m_t = 15.f;
double n_t = 15.f;
summit** sum_t;
int initial = 0;

void init(void){
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h){
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-50.0, 50.0, -25.0, 75.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void display(void){

	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glColor3d(1,0,0);

	glLoadIdentity();	
	glScalef(2.0, 2.0, 2.0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(0.0, 0.0, 0.75, 0.5);
	glLineWidth(2.0);
	
	glTranslatef(zoom_x, zoom_y, 0.0);
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);

	if(!done){
		cloud = get_cloud_point(file_name, &size, fact);
		done = 1;
	}

	if(size < 0)
		fprintf(stderr, "display: couldn't get cloud!\n");
	else
		if(aff == 1)
			draw_cloud_point(cloud, size, color);

	if(!initial){
		float* initial_values = initial_parameters(cloud, size);
		
		if(initial_values != NULL){
			e1_i = initial_values[0];
			e2_i = initial_values[1];
			tx = initial_values[2];
			ty = initial_values[3];
			tz = initial_values[4];
			angle1_i = initial_values[5];
			angle2_i = initial_values[6];
			angle3_i = initial_values[7];
			a_i = initial_values[8];
			b_i = initial_values[9];
			c_i = initial_values[10];

			double** values = discretization(-PI, PI, -PI, PI, m_t, n_t);
			sum_t = summit_building(a_i, b_i, c_i, e1_i, e2_i, m_t, n_t, values, 0, 1);

		
			glTranslatef(tx, ty, tz);
			glRotatef(angle1_i, 1.0f, 0.0f, 0.0f);
			glRotatef(angle2_i, 0.0f, 1.0f, 0.0f);
			glRotatef(angle3_i, 0.0f, 0.0f, 1.0f);

			draw_superquadrics(sum_t, m_t, n_t);

			initial = 1;
		}
	}
	else{
		glTranslatef(tx, ty, tz);
		draw_superquadrics(sum_t, m_t, n_t);
	}

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv){
	int H, W;
	getScreenSize(&H, &W);

	if(argc < 6){
		fprintf(stderr, "main: invalid argument!\n");
		printf("usage: %s [file] [R] [G] [B] [fact]..\n", argv[0]);
		return -1;
	}

	file_name = argv[1];
	color[0] = atof(argv[2]);
	color[1] = atof(argv[3]);
	color[2] = atof(argv[4]);
	fact = atof(argv[5]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(H, W);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("cloud points");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(camera_specialKeyboard);
	glutMouseFunc(camera_mouse);
	glutMotionFunc(camera_mouseMotion);
	glutKeyboardFunc(camera_keyboard);
	//glutIdleFunc(camera_idle);

	glutMainLoop();
	return 0;
}