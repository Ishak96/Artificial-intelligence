#ifndef BASE_H
#define BASE_H

/* structure de coordonnée 2D pour affichage*/
typedef struct {
  int x,y;
  char name[50];
}Point;

void draw_text(float x, float y, const char *fmt, ...);

/* Prototypes */
void affichage();
void clavier(unsigned char touche,int x,int y);
void reshape(int x,int y);
void mouse(int bouton,int etat,int x,int y);
void mousemotion(int x,int y);
void idle();

#endif