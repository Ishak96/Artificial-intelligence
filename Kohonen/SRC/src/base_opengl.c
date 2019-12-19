/*
#########################
Installation des packages
sudo apt install libglu1-mesa-dev freeglut3-dev mesa-common-dev
#########################
Simple programme d'affichage de points et de segments en opengl
utilise GL et glut
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include <string.h>
#include <base_opengl.h>
#include <ppm.h>
#include <kohonen.h>

#define DEFAULT_WIDTH  500
#define DEFAULT_HEIGHT 500
#define NB_VILLE 21
#define MODE 1

int cpt = 0;
int calc = 0;
char presse;
Point ville[NB_VILLE];
int anglex=0;
int angley=0;
int x, y, xold, yold;
GLuint textureID;

int width  = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
/* affiche la chaine fmt a partir des coordonnées x,y*/
void draw_text(float x, float y, const char *fmt, ...)
{
  char    buf[1024];      //Holds Our String
  char    *text = buf;
  va_list   ap;       // Pointer To List Of Arguments

  if (fmt == NULL)        // If There's No Text
    return;         // Do Nothing

  va_start(ap, fmt);        // Parses The String For Variables
  vsprintf(text, fmt, ap);      // And Converts Symbols To Actual Numbers
  va_end(ap);         // Results Are Stored In Text

  glDisable(GL_TEXTURE_2D);
  glRasterPos2i( x, y );
  while (*text)
    glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *text++);

  glEnable(GL_TEXTURE_2D);
}

GLuint charger_texture(unsigned char * data)
{
  GLuint textureBidule;
  glGenTextures(1, &textureBidule); /* Texture name generation */
  glBindTexture(GL_TEXTURE_2D, textureBidule); /* Binding of texture name */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);;

  return textureBidule;
}

unsigned char * transform_img_to_vector(const char * filename, int *width , int *height)
{
  Image* image = NULL;
  image = readPPM(filename);
  if (image == NULL) EXIT_ON_ERROR("error loading img");
  int i, j;
  unsigned char* data = NULL;
  *width = image->x;
  *height = image->y;
  data = (unsigned char*)malloc(3 * image->x * image->y * sizeof(unsigned char));

  for (i = 0; i < image->x * image->y; i++)
  {
    j = i * 3;
    data[j] = image->data[i].r;
    data[j + 1] = image->data[i].g;
    data[j + 2] = image->data[i].b;
  }

  if (image != NULL)
  {
    free(image->data);
    image->data = NULL;
    free(image);
    image = NULL ;
  }
  return data;
}

void load_cities()
{
  int i, j = 0;
  FILE * file = NULL;
  char buffer[50];
  char *store[3];
  char * parsed_buffer;
  file = fopen("Villes_et_positions_dans_image.txt", "rb");
  if (file == NULL) EXIT_ON_ERROR("error while loading cities txt file");
  while (fgets(buffer, sizeof(buffer), file) != NULL)
  {
    buffer[strlen(buffer) - 1] = '\0';
    j = 0;
    parsed_buffer = strtok(buffer, " ");
    while (parsed_buffer != NULL)
    {
      store[j++] = parsed_buffer;
      parsed_buffer = strtok (NULL, " ");
    }
    strcpy(ville[i].name, store[0]);
    ville[i].x = atoi(store[1])-5; // shift du au resize de l'image
    ville[i].y = atoi(store[2])-5;
    i++;
  }
  fclose(file);
}

/* Initialize OpenGL Graphics */
void initGL(int w, int h)
{
	 
  #if MODE
  int taille_point = 15;
  #else
  int taille_point = 5;
  #endif

  glViewport(0, 0, w, h); // use a screen size of WIDTH x HEIGHT
  glEnable(GL_TEXTURE_2D);     // Enable 2D texturing

  glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
  glLoadIdentity();

  #if MODE
  glOrtho(0.0, w, h, 0.0, -1, 1);
  #else
  glOrtho(0.0, 200, 200, 0.0, -1, 1);
  #endif

  glPointSize(taille_point);  
  glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
}
/* *************************************************** */
/* main */
int main(int argc, char **argv)
{

  #if MODE
  unsigned char * data = NULL;
  if (argc != 2) EXIT_ON_ERROR("You must specified a .ppm file");
  data = transform_img_to_vector(argv[1], &width, &height);
  load_cities();
  #endif

  /* GLUT init */
  glutInit(&argc, argv);            // Initialize GLUT
  glutInitDisplayMode(GLUT_DOUBLE); // Enable double buffered mode
  glutInitWindowSize(width, height);   // Set the window's initial width & height
  glutCreateWindow("Kohonen");      // Create window with the name of the executable

  /* enregistrement des fonctions de rappel */
  glutDisplayFunc(affichage);
  glutKeyboardFunc(clavier);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutMouseFunc(mouse);
  glutMotionFunc(mousemotion);

  /* OpenGL 2D generic init */
  initGL(width, height);

  #if MODE
  textureID = charger_texture(data);
  if (data != NULL)
  {
    free(data);
    data = NULL;
  }
  #endif

  /* Main loop */
  glutMainLoop();

  #if MODE
  /* Delete used resources and quit */
  glDeleteTextures(1, &textureID);
  #endif

  return 0;
}

/* *************************************************** */

/* fonction d'affichage appelée a chaque refresh*/
void affichage()
{
  // Clear color and depth buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);     // Operate on model-view matrix
  glLoadIdentity();

  #if MODE
  int i ;
  /* Draw a quad */
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_QUADS);
  glTexCoord2i(0, 0); glVertex2i(0,   0);
  glTexCoord2i(0, 1); glVertex2i(0,   height);
  glTexCoord2i(1, 1); glVertex2i(width, height);
  glTexCoord2i(1, 0); glVertex2i(width, 0);
  glEnd();

  for (i = 0; i < NB_VILLE; i++)
  {
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(ville[i].x, ville[i].y);
    glEnd();
    glColor3f(0, 0, 0);
    draw_text(ville[i].x - 20, ville[i].y + 20, "%s", ville[i].name);
  }

 // glColor3f(1.0, 1.0, 1.0);

draw_text(60, 70, "nb iter: %d", cpt);
#else

// VOTRE CODE D'AFFICHAGE ICI, voir l'exemple ci-dessous

// ceci est un exemple pour tracer des points et des lignes
 glBegin(GL_POINTS);
 glColor3f(1.0,0.0,0.0);
 glVertex2f(50,25);
 glEnd();

 glBegin(GL_POINTS);
 glColor3f(1.0,0.0,0.0);
 glVertex2f(75,10);
 glEnd();

 glBegin(GL_LINE_LOOP);
 glColor3f(1.0,0.0,0.0);
 glVertex2f(50,25);
 glVertex2f(75,10);
 glEnd();

 glColor3f(1.0,1.0,1.0);
 draw_text(150, 20, "nb iter: %d", cpt);

#endif

 glFlush();
 glutSwapBuffers();
}

// VOTRE CODE DE KOHONEN ICI
void idle() {
  if (calc) { // calc est modifié si on presse "p" (voir la fonction "clavier" ci dessous)
    cpt++; // un simple compteur

    printf("mettre votre code ICI\n");

    glutPostRedisplay();
  }
}

void clavier(unsigned char touche, int x, int y) {
  switch (touche) {
  case 'p':
    calc = !calc;
    break;

  case 'q': /* la touche 'q' permet de quitter le programme */
    exit(0);
  } /* switch */

} /* clavier */

void reshape(GLsizei newwidth, GLsizei newheight)
{
  // On ecrase pas width et height dans le cas image car il s'agira de la taille de l'image
  #if MODE
  #else
  width = newwidth;
  height = newheight;
  #endif
  // Set the viewport to cover the new window
  glViewport(0, 0,newwidth ,newheight );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  #if MODE
  glOrtho(0.0, width, height, 0.0, -1, 1);
  #else
  glOrtho(0.0, 200, 200, 0.0, -1, 1);
  #endif

  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();
}

/* getion des boutons de  la souris*/
void mouse(int bouton, int etat, int x, int y) {
  /* si on appuie sur la bouton de gauche */
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_DOWN) {
    presse = 1; // vrai
    xold = x; // sauvegarde de la position de la souris
    yold = y;
  }

  /* si on relache la souris */
  if (bouton == GLUT_LEFT_BUTTON && etat == GLUT_UP) {
    presse = 0; // faux
  }
} /* mouse */

/*gestion des mouvements de la souris */
void mousemotion(int x, int y) {
  if (presse) { /* si le bouton gauche est presse */
    /* on mofifie les angles de rotation de l'objet en fonction de la position actuelle de la souris et de la derniere position sauvegard?e */
    anglex = anglex + (x - xold);
    angley = angley + (y - yold);
    glutPostRedisplay();
  }

  xold = x; /* sauvegarde des valeurs courante des positions de la souris */
  yold = y;
} /* mousemotion */
