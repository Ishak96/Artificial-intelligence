#include<draw.h>
#include <camera.h>
#include <cloud.h>
#include <stdio.h>

extern float xrot, yrot;
extern float zoom_x, zoom_y;

//global variables
double a_t,b_t,c_t,e1_t,e2_t,r0_t,r1_t;
float** cloud;
int size = 0;
int xo = 0;
int yo = 0;
int zo = 0;

GLfloat angle = 90;

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

	glPushMatrix();

	if(size == 0)
		cloud = generate_cloud_point(70, 70, a_t, b_t, c_t, e1_t, e2_t, &size, xo, yo, zo, r0_t, r1_t);
	
	float color[3] = {1.0, 1.0, 1.0};
	draw_cloud_point(cloud, size, color);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv){
	int H, W;
	getScreenSize(&H, &W);

	if(argc < 8){
		fprintf(stderr, "main: invalid argument!\n");
		printf("usage: %s [a] [b] [c] [e1] [e2] [r0] [r1]..\n", argv[0]);
		printf("give r0 = 0 and r1 = 1 to gener a normal superquadrics\n");
		return -1;
	}

	a_t = atof(argv[1]);
	b_t = atof(argv[2]);
	c_t = atof(argv[3]);
	e1_t = atof(argv[4]);
	e2_t = atof(argv[5]);
	r0_t = atof(argv[6]);
	r1_t = atof(argv[7]);

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