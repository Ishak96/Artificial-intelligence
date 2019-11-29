/*****************************************************************************/
/* File:        user.c (Khepera Simulator)                                   */
/* Author:      Olivier MICHEL <om@alto.unice.fr>                            */
/* Date:        Thu Sep 21 14:39:05 1995                                     */
/* Description: example of user.c file                                       */
/*                                                                           */
/* Copyright (c) 1995                                                        */
/* Olivier MICHEL                                                            */
/* MAGE team, i3S laboratory,                                                */
/* CNRS, University of Nice - Sophia Antipolis, FRANCE                       */
/*                                                                           */
/* Permission is hereby granted to copy this package for free distribution.  */
/* The author's name and this copyright notice must be included in any copy. */
/* Commercial use is forbidden.                                              */
/*****************************************************************************/

#include "../SRC/include.h"
#include "../CONTRIB/multirobots.h"
#include "user.h"
#include "user_info.h"

#define FORWARD_SPEED   5                    /* normal (slow) forward speed*/
#define TURN_SPEED      4                    /* normal (slow) turn speed */
#define COLLISION_TH    500                  /* value of IR sensors to be considered as collision */


#define MIN(a,b) (a<b ? a : b)
#define MAX(a,b) (a<b ? b : a)
#define max_f(a,b,c) (c>MAX(a,b) ? c : MAX(a,b))

#define NB_INPUT 2
#define NB_OUTPUT 2
#define NB_TRAPEZ 2

typedef struct{
  float x1,x2,x3,x4;
  float hauteur;

}trapez;

typedef struct{
  trapez fonction_linguistique[NB_TRAPEZ];
}variable_linguistique;

/*entre[0] = variable linguistique d'entrer n° 1 (capteur gauche)
  entre[1] = variable linguistique d'entrer n° 2 (capteur droit)

  pour les deux variables linguistique:
    fonction_linguistique[0] = trapez loin
    fonction_linguistique[1] = trapez proche
*/
variable_linguistique entree[NB_INPUT];

/*sortie[0] = variable linguistique de sortie n° 1 (moteur gauche)
  sortie[1] = variable linguistique de sortie n° 2 (moteur droit)

   pour les deux variables linguistique:
    fonction_linguistique[0] = dec gauche
    fonction_linguistique[1] = inc droite
*/
variable_linguistique sortie[NB_OUTPUT];

int pas = 0;

trapez creer_trapez(x1,x2,x3,x4,hauteur)
float x1,x2,x3,x4,hauteur;
{
  trapez trap;
  
  trap.x1 = x1;
  trap.x2 = x2;
  trap.x3 = x3;
  trap.x4 = x4;

  trap.hauteur = hauteur;

  return trap;
}

float aire_trapez(trap)
trapez trap;
{
  float B = trap.x1-trap.x4;//grande base
  float b = trap.x2-trap.x3;//petite base
  float h = trap.hauteur;//hauteur
    return (B+b) * h/2.f;
}

float defuzzyfication(variable_l)
variable_linguistique variable_l;
{
  trapez trap;
  float som_aire = 0.f;
  float som_produit = 0.f;
  float centre,aire;
  for(int i = 0; i < NB_TRAPEZ; i++){
    trap = variable_l.fonction_linguistique[i];

    centre = (trap.x2+trap.x3)/2.f;
    aire = aire_trapez(trap);
    som_aire += aire;
    som_produit += centre*aire;
  }

  return (som_aire != 0) ? som_produit/som_aire : 0.f;
}

void insert_trapez(trap,variable_l,index)
trapez trap;
variable_linguistique* variable_l;
int index;
{
  if(index < 2)
    variable_l->fonction_linguistique[index] = trap;
  else
    fprintf(stderr, "index out of bounds !\n");
}

void init_systeme(){
  trapez trapez_loin = creer_trapez(0.f,0.f,400.f,500.f,1.f);
  trapez trapez_proche = creer_trapez(500.f,600.f,1023.f,1023.f,1.f);

  trapez trapez_moteur_dec = creer_trapez(-5.f,-5.f,-2.f,0.f,1.f);
  trapez trapez_moteur_inc = creer_trapez(0.f,2.f,5.f,5.f,1.f);

  insert_trapez(trapez_loin,entree+0,0);
  insert_trapez(trapez_proche,entree+0,1);

  insert_trapez(trapez_loin,entree+1,0);
  insert_trapez(trapez_proche,entree+1,1);

  insert_trapez(trapez_moteur_dec,sortie+0,0);
  insert_trapez(trapez_moteur_inc,sortie+0,1);

  insert_trapez(trapez_moteur_dec,sortie+1,0);
  insert_trapez(trapez_moteur_inc,sortie+1,1);
}

void agregation(variable_l)
variable_linguistique* variable_l;
{
  float a,b;
  trapez* trap = variable_l->fonction_linguistique+0;
  if(trap->hauteur != 0){
    a = (trap->x2-trap->x1 != 0) ? 1/(trap->x2-trap->x1) : 0.f;
    b = 1 - a*trap->x2;

    if(a != 0){
      trap->x2 = (trap->hauteur-b)/a;
    }

    a = (trap->x4-trap->x3 != 0) ? -1/(trap->x4-trap->x3) : 0.f;
    b = 1 - a*trap->x3;

    if(a != 0){
      trap->x3 = (trap->hauteur-b)/a;
    }
  }

  trap = variable_l->fonction_linguistique+1;
  if(trap->hauteur != 0){
    a = (trap->x2-trap->x1 != 0) ? 1/(trap->x2-trap->x1) : 0.f;
    b = 1 - a*trap->x2;

    if(a != 0){
      trap->x2 = (trap->hauteur-b)/a;
    }

    a = (trap->x4-trap->x3 != 0) ? -1/(trap->x4-trap->x3) : 0.f;
    b = 1 - a*trap->x3;

    if(a != 0){
      trap->x3 = (trap->hauteur-b)/a;
    }
  }
}

void fuzzyfication(valeur,variable_l)
float valeur;
variable_linguistique* variable_l;
{
  trapez* trap;
  for(int i = 0; i < NB_TRAPEZ; i++){
    trap = variable_l->fonction_linguistique+i;
    float delta1 = valeur-trap->x1;
    float delta2 = trap->x4-valeur;
    float a,b;

    if((delta1 < 0)||(delta2 < 0)){
      trap->hauteur = 0.f;
    }
    else{
      if(valeur < trap->x2){
        a = (trap->x2-trap->x1 != 0) ? 1/(trap->x2-trap->x1) : 0.f;
        if(a == 0){
          trap->hauteur = 1.f;
        }
        else{
          b = 1 - a*trap->x2;
          trap->hauteur = a*valeur+b;
        }
      }
      else if(valeur > trap->x3){
        a = (trap->x4-trap->x3 != 0) ? -1/(trap->x4-trap->x3) : 0.f;
        if(a == 0){
          trap->hauteur = 1.f;
        }
        else{
          b = 1 - a*trap->x3;
          trap->hauteur = a*valeur+b;
        }
      }
      else{
        trap->hauteur = 1.f;
      }
    }
  }
}

/*  1) si proche gauche et loin droite alors inc droite 
  2) si proche droite et loin gauche alors dec gauche 
  3) si proche gauche et proche droit alors dec gauche
  4) si loin gauche et loin droite alors inc gauche
  5) si loin gauche et loin droite alors inc droite*/
void moteur_inferance()
{
  float proche_gauche = entree[0].fonction_linguistique[1].hauteur;
  float loin_gauche = entree[0].fonction_linguistique[0].hauteur;
  
  float proche_droite = entree[1].fonction_linguistique[1].hauteur;
  float loin_droite = entree[1].fonction_linguistique[0].hauteur;

  float moteur_inc_d1 = MIN(proche_gauche,loin_droite);
  float moteur_dec_g1 = MIN(proche_droite,loin_gauche);
  float moteur_dec_g2 = MIN(proche_gauche,proche_droite);
  float moteur_inc_g1 = MIN(loin_gauche,loin_droite);
  float moteur_inc_d2 = MIN(loin_gauche,loin_droite);

  float moteur_inc_d = MAX(moteur_inc_d1,moteur_inc_d2);
  float moteur_dec_d = 0.f;
  float moteur_inc_g = moteur_inc_g1;
  float moteur_dec_g = MAX(moteur_dec_g1,moteur_dec_g2);

  sortie[0].fonction_linguistique[0].hauteur = moteur_dec_g;
  sortie[0].fonction_linguistique[1].hauteur = moteur_inc_g;

  sortie[1].fonction_linguistique[0].hauteur = moteur_dec_d;
  sortie[1].fonction_linguistique[1].hauteur = moteur_inc_d;
}

void DrawPas()
{
  char text[256];

  sprintf(text,"pas = %d",pas);
  Color(GREY);
  UndrawText(200,100,"pas = 0000");
  Color(BLUE);
  DrawText(200,100,text);
}

void UserInit(struct Robot *robot){}
void UserClose(struct Robot *robot){}
void NewRobot(struct Robot *robot){  pas = 0;}
boolean FastStepRobot(struct Robot *robot){return FALSE;}
boolean FastStepMultiRobots(struct MultiRobots *multi){return FALSE;}
void LoadRobot(struct Robot *robot,FILE *file){}
void SaveRobot(struct Robot *robotrobot,FILE *file){}

void RunRobotStart(struct Robot *robot){ShowUserInfo(2,1);}
void RunRobotStop(struct Robot *robot){ShowUserInfo(1,2);}

boolean StepRobot(struct Robot *robot)
{
  
  pas++;
  DrawPas();
  
  float gauche = (float)max_f(robot->IRSensor[0].DistanceValue 
               , robot->IRSensor[1].DistanceValue 
               , robot->IRSensor[2].DistanceValue);

  float droite = (float)max_f(robot->IRSensor[3].DistanceValue 
               , robot->IRSensor[4].DistanceValue 
               , robot->IRSensor[5].DistanceValue);

  init_systeme();
  
  fuzzyfication(gauche,entree+0);
  fuzzyfication(droite,entree+1);

  moteur_inferance();

  agregation(sortie+0);
  agregation(sortie+1); 

  float moteur_g = defuzzyfication(sortie[0]);
  float moteur_d = defuzzyfication(sortie[1]);

  robot->Motor[LEFT].Value  = moteur_g;
  robot->Motor[RIGHT].Value = moteur_d;

  return(TRUE);
}

boolean StepMultiRobots(struct MultiRobots *multi)
{
  struct Robot *robot;
  boolean      result;

  robot  = multi->robots[multi->current];
  result = StepRobot(robot);

return result;
}

void ResetRobot(struct Robot *robot)
{
  pas = 0;
}

void UserCommand(struct Robot *robot,char         *text)
{
  int i;
  struct MultiRobots  *multi;
  boolean stop = FALSE;

  if (strcmp(text,"test")==0)
  {
    WriteComment("Demonstration");
    ShowUserInfo(1,1);
    multi = CreateMultiRobots(5);
    PlaceRobots(context,multi);
    DrawWorldSquare(context->World);
    for(i=0;i<2000;i++)
    {
      MultiRobotRun(context,multi);
      if (StopCommand())
      {
        stop = TRUE;
        break;
      }
    }
    if (stop) WriteComment("break");
    else WriteComment("Done");
    ShowUserInfo(1,2);
  }
  else WriteComment("unknown command");
  FreeMultiRobots(multi);
}

void DrawUserInfo(struct Robot *robot,u_char info,u_char page)
{
  char text[256];

  switch(info)
  {
    case 1:
      switch(page)
      {
        case 1: Color(MAGENTA);
                FillRectangle(0,0,40,40);
                Color(BLUE);
                DrawLine(100,100,160,180);
                Color(WHITE);
                DrawPoint(200,200);
                Color(YELLOW);
                DrawRectangle(240,100,80,40);
                Color(GREEN);
                DrawText(240,230,"bonjour");
                break;
        case 2: Color(RED);
                DrawArc(200,50,100,100,0,360*64);
                Color(YELLOW);
                FillArc(225,75,50,50,0,360*64);
                Color(BLACK);
                DrawText(140,170,"This is the brain of the robot");
      }
      break;
    case 2:     DrawPas();
  }
}