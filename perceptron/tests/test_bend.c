#include<draw.h>
#include <camera.h>
#include <stdio.h>

extern float xrot, yrot;
extern float zoom_x, zoom_y;
extern int bend_control;
//global variables
double a_t,b_t,c_t,p_t,q_t,r0_t,r1_t;
extern float k, alpha;

superquadrics forme;

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
	
	if(bend_control == 1){
		forme.k = k;
		forme.alpha = alpha;
		if(bend_forme(forme) < 0){
			fprintf(stderr, "bed test_taper: invalid argument!\n");
		}
		bend_control = 0;
	}
	else if(bend_control == -1){
		if(reverse_bend_forme(forme) < 0){
			fprintf(stderr, "reverse_ben test_taper: invalid argument!\n");
		}
		bend_control = 0;
	}

	draw_debug(forme.summits, forme.m, forme.n);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

int main(int argc, char** argv){

	if(argc < 8){
		fprintf(stderr, "main: invalid argument!\n");
		printf("usage: %s [a] [b] [c] [e1] [e2] [r0] [r1]..\n", argv[0]);
		printf("give r0 = 0 and r1 = 1 to gener a normal superquadrics\n");
		return -1;
	}

	a_t = atof(argv[1]);
	b_t = atof(argv[2]);
	c_t = atof(argv[3]);
	p_t = atof(argv[4]);
	q_t = atof(argv[5]);
	r0_t = atof(argv[6]);
	r1_t = atof(argv[7]);

	forme.m = 35.f;
	forme.n = 35.f;
	double** values = discretization(-PI / 2, PI / 2, -PI, PI, forme.m, forme.n);
	forme.summits = summit_building(a_t, b_t, c_t, p_t, q_t, forme.m, forme.n, values, r0_t, r1_t);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(50, 50);
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