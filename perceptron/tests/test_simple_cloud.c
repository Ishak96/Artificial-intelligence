#include<draw.h>
#include <camera.h>
#include <cloud.h>
#include <stdio.h>

extern float xrot, yrot;
extern float zoom_x, zoom_y;
extern int aff;

//global variables
double a_t,b_t,c_t,e1_t,e2_t,r0_t,r1_t;
float** cloud;
int size = 0;
int initial = 0;
float a_i, b_i, c_i, e1_i, e2_i, tx, ty, tz, angle1_i, angle2_i, angle3_i;
int xo, yo, zo;

GLfloat angle = 90;

double m_t = 15.f;
double n_t = 15.f;
summit** sum_t;

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

	if(size == 0)
		cloud = generate_cloud_point(70, 70, a_t, b_t, c_t, e1_t, e2_t, &size, xo, yo, zo, r0_t, r1_t);
	
	float color[3] = {1.0, 0.0, 0.0};
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
	else
		if(aff == 1){
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

	if(argc < 11){
		fprintf(stderr, "main: invalid argument!\n");
		printf("usage: %s [a] [b] [c] [e1] [e2] [origin x] [origin y] [origin z] [r0] [r1]..\n", argv[0]);
		printf("give r0 = 0 and r1 = 1 to gener a normal superquadrics\n");
		return -1;
	}

	a_t = atof(argv[1]);
	b_t = atof(argv[2]);
	c_t = atof(argv[3]);
	e1_t = atof(argv[4]);
	e2_t = atof(argv[5]);
	xo = atoi(argv[6]);
	yo = atoi(argv[7]);
	zo = atoi(argv[8]);
	r0_t = atof(argv[9]);
	r1_t = atof(argv[10]);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(H, W);
	glutInitWindowPosition(400, 35);
	glutCreateWindow("superquadrics modelization");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(camera_specialKeyboard);
	glutMouseFunc(camera_mouse);
	glutMotionFunc(camera_mouseMotion);
	glutKeyboardFunc(camera_keyboard);

	glutMainLoop();
	return 0;
}