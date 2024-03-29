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
#define MAXNAME 20          /* max number of characters in names*/
#define max(a,b)     (a<b ? b : a)
#define min(a,b)     (a>b ? b : a)
#define max_f(a,b,c) (c>max(a,b) ? c : max(a,b))
#define sign(a)      (a<0 ? -1 : 1)

struct point{
  float x;
  float y;
};

struct linear_function{
  struct point straight_line[2];
  float slopes;
  float intercept;
};

struct mf_type{
  char name[MAXNAME];     /* name of membership function (fuzzy set)    */
  float value;            /* degree of membership or output strength    */
  struct linear_function trapezoid[2];
};

struct io_type{
  char name[MAXNAME];                       /*  name of system input/output       */
  float value;                              /*  value of system input/output      */
  struct mf_type membership_functions[3];   /*  list of membership functions for this system input/outputs  */
  int nb_mf;
};

/*  Each rule has an if side and a then side. Elements making up if side are
pointers to antecedent values inside mf_type structure. Elements making up then
side of rule are pointers to output strength values, also inside mf_type
structure. Each rule structure contains a pointer to next rule in rule base. */
struct rule_element_type{
  float *value;                   /* pointer to antecedent/output strength value */
  struct rule_element_type *next;   /* next antecedent/output element in rule      */
};

struct rule_type{
  struct rule_element_type *if_side;     /* list of antecedents in rule */
  struct rule_element_type *then_side;   /* list of outputs in rule     */
  struct rule_type *next;                /* next rule in rule base      */
};

struct rule_type *Rule_Base;  /* list of all rules in rule base */
struct io_type System_Inputs[2];
struct io_type System_Outputs[2];
float UPPER_LIMIT = 1.f;    /* max number assigned as degree of membership */
int pas=0;

void init_point(p,x,y)
struct point* p;
float x,y;
{
  p->x = x;
  p->y = y;
}

void init_trapezoid(trapezoid,ax,ay,bx,by)
struct linear_function* trapezoid;
float ax,ay,bx,by;
{
  init_point(trapezoid->straight_line+0,ax,ay);
  init_point(trapezoid->straight_line+1,bx,by);
  trapezoid->slopes = ((bx-ax) != 0.f) ? (by-ay)/(bx-ax): 0.f;
  trapezoid->intercept = by-trapezoid->slopes*bx;
}

void create_mf(mf,name,ax,ay,bx,by,cx,cy,dx,dy)
struct mf_type* mf;
char name[MAXNAME];
float ax,ay,bx,by,cx,cy,dx,dy;
{
  sprintf(mf->name, "%s", name);
  init_trapezoid(mf->trapezoid+0,ax,ay,bx,by);
  init_trapezoid(mf->trapezoid+1,cx,cy,dx,dy);
}


/*clipping a trapezoid forme*/
void clipping_trapezoid(mf)
struct mf_type *mf;
{
  float value = mf->value;
  struct linear_function* trapezoid;
  struct point* p;
  float slopes, intercept;
  
  if(value != 0.f){
    for(int i = 0; i < 2; i++){
      trapezoid=mf->trapezoid+i;
      slopes = trapezoid->slopes;
      intercept = trapezoid->intercept;
      for(int j = 0; j < 2; j++){
        if(i==0 && j==1){
          p=trapezoid->straight_line+j;
          p->y = value;
          p->x = (slopes!=0.f) ? (value-intercept)/slopes : p->x;
        }
        else if(i==1 && j==0){
          p=trapezoid->straight_line+j;
          p->y = value;
          p->x = (slopes!=0.f) ? (value-intercept)/slopes : p->x;
        }
      }
    }
  }
}

/* Compute Degree of Membership--Degree to which input is a member of mf is
calculated as follows: 1. Compute delta terms to determine if input is inside
or outside membership function. 2. If outside, then degree of membership is 0.
Otherwise, smaller of delta_1 * slope1 and delta_2 * slope2 applies.
3. Enforce upper limit. */
void compute_degree_of_membership(mf,input)
struct mf_type *mf;
float input;
{
  float delta_1;
  float delta_2;
  delta_1 = input - mf->trapezoid[0].straight_line[0].x;
  delta_2 = mf->trapezoid[1].straight_line[1].x - input;

  if ((delta_1 < 0) || (delta_2 < 0))   /* input outside mem. function ? */
    mf->value = 0;                      /* then degree of membership is 0 */
  else{
    if(mf->trapezoid[0].slopes == 0){
      mf->value = min( (mf->trapezoid[0].slopes*delta_1),
        (mf->trapezoid[1].slopes*delta_2) );
    }
    else if(mf->trapezoid[1].slopes == 0){
      mf->value = max( (mf->trapezoid[0].slopes*delta_1),
        (mf->trapezoid[1].slopes*delta_2) );
    }
    
    mf->value = min(sign(mf->value)*mf->value,UPPER_LIMIT);  /* enforce upper limit */
  }
}

/* Compute Area of Trapezoid--Each inference pass produces a new set of output
strengths which affect the areas of trapezoidal membership functions used in
center-of-gravity defuzzification.*/
float compute_area_of_trapezoid(mf)
struct mf_type mf;
{
  float base;
  float top;
  float area;
  base = mf.trapezoid[0].straight_line[0].x - mf.trapezoid[1].straight_line[1].x;
  top = mf.trapezoid[0].straight_line[1].x - mf.trapezoid[1].straight_line[0].x;
  area = mf.value * ( base + top ) / 2;
  return(area);
}

/* Rule Evaluation--Each rule consists of a list of pointers to antecedents
(if side), list of pointers to outputs (then side), and pointer to next rule
in rule base. When a rule is evaluated, its antecedents are ANDed together,
using a minimum function, to form strength of rule. Then strength is applied
to each of listed rule outputs.*/
void rule_evaluation(){
  struct rule_type *rule;
  struct rule_element_type *ip;       /* pointer of antecedents  (if-parts)        */
  struct rule_element_type *tp;       /* pointer to consequences (then-parts)      */
  float strength;                   /* strength of  rule currently being evaluated */
 
  for(rule=Rule_Base; rule != NULL; rule=rule->next){
    strength = UPPER_LIMIT;                       /* max rule strength allowed */
        /* process if-side of rule to determine strength */
    for(ip=rule->if_side; ip != NULL; ip=ip->next){
      strength = min(strength,*(ip->value));
    }
    /* process then-side of rule to apply strength */
    for(tp=rule->then_side; tp != NULL; tp=tp->next)
      *(tp->value) = max(strength,*(tp->value));
  }
}

/* Fuzzification--Degree of membership value is calculated for each membership
function of each system input. Values correspond to antecedents in rules. */
void fuzzification(){
  struct io_type* si;    /* system input pointer        */
  struct mf_type* mf;    /* membership function pointer */
  
  for(int i = 0; i < 2; i++){
    si=System_Inputs+i;
    for(int j = 0; j < si->nb_mf; j++){
      mf=si->membership_functions+j;
      compute_degree_of_membership(mf,si->value);
    }
  }
}

/*Aggregation*/
void aggregation(){
  struct io_type* si;    /* system input pointer        */
  struct mf_type* mf;    /* membership function pointer */
  
  for(int i = 0; i < 2; i++){
    si=System_Outputs+i;
    for(int j = 0; j < si->nb_mf; j++){
      mf=si->membership_functions+j;
      clipping_trapezoid(mf);
    }
  }
}

/* Defuzzification */
void defuzzification(){
  struct io_type *so;      /* system output pointer */
  struct mf_type mf;     /* output membership function pointer */
  float sum_of_products;   /* sum of products of area & centroid */
  float sum_of_areas;    /* sum of shortend trapezoid area */
  float area;
  float centroid;
 
 /* compute a defuzzified value for each system output */
  for(int i = 0; i < 2; i++){
    so=System_Outputs+i;
    sum_of_products = 0;
    sum_of_areas = 0;
    
    for(int j = 0; j < so->nb_mf; j++){
      mf=so->membership_functions[j];
      area = compute_area_of_trapezoid(mf);
      centroid = (mf.trapezoid[0].straight_line[1].x + mf.trapezoid[1].straight_line[0].x) / 2.f;
      sum_of_products += area * centroid;
      sum_of_areas += area;
    }
    so->value = (sum_of_areas > 0.000001f || sum_of_areas < -0.000001f) 
    ? sum_of_products/sum_of_areas : sum_of_areas;   /* weighted average */
  }
}

void initialize_system(){
  float seuil = 1.f;
  int a, b, c, d, x;
  char buff[10],buff1[10],buff2[10];
  FILE *fp;
  struct io_type* outptr;
  struct mf_type* mfptr;
  struct io_type* ioptr;
  struct rule_type *ruleptr;
  struct rule_element_type *ifptr;
  struct rule_element_type *thenptr;
  
  ruleptr=NULL;
  ifptr=NULL;
  thenptr=NULL;

  struct mf_type input_mf[3], output_mf[3];
  
  /*create membership function for input/output system*/
  create_mf(input_mf+0,"CL_FAR",0.f,0.f,0.f,1.f,100.f,1.f,250.f,0.f);

  create_mf(input_mf+1,"CL_MEDIUM",150.f,0.f,250.f,1.f,200.f,1.f,300.f,0.f);

  create_mf(input_mf+2,"CL_NEAR",200.f,0.f,300.f,1.f,1023.f,1.f,1023.f,0.f);

  create_mf(output_mf+0,"MR--",-5.f-seuil,0.f,-5.f-seuil,1.f,-2.f,1.f,0.f,0.f);

  create_mf(output_mf+1,"MR00",-2.f,0.f,0.f,1.f,0.f,1.f,2.f,0.f);  

  create_mf(output_mf+2,"MR++",0.f,0.f,2.f,1.f,5.f+seuil,1.f,5.f+seuil,0.f);


  /*initilize input/output system*/
  sprintf(System_Inputs[0].name, "%s", "CL");
  System_Inputs[0].nb_mf = 3;
  memcpy(System_Inputs[0].membership_functions, input_mf, sizeof(input_mf));

  sprintf(input_mf[0].name, "%s", "CR_FAR");
  sprintf(input_mf[1].name, "%s", "CR_MEDIUM");
  sprintf(input_mf[2].name, "%s", "CR_NEAR");
  sprintf(System_Inputs[1].name, "%s", "CR");
  System_Inputs[1].nb_mf = 3;
  memcpy(System_Inputs[1].membership_functions, input_mf, sizeof(input_mf));

  sprintf(System_Outputs[0].name, "%s", "MR");
  System_Outputs[0].nb_mf = 3;
  memcpy(System_Outputs[0].membership_functions, output_mf, sizeof(output_mf));

  sprintf(output_mf[0].name, "%s", "ML--");
  sprintf(output_mf[1].name, "%s", "ML00");
  sprintf(output_mf[2].name, "%s", "ML++");
  sprintf(System_Outputs[1].name, "ML");
  System_Outputs[1].nb_mf = 3;
  memcpy(System_Outputs[1].membership_functions, output_mf, sizeof(output_mf));

  if((fp=fopen("USER/rules","r"))==NULL){  
    printf("ERROR- Unable to open data file named %s.\n","rules");
    exit(0);
  }
  
  ruleptr=(struct rule_type *)calloc(1,sizeof(struct rule_type));
  
  Rule_Base=ruleptr;     /* first time thru, anchor */
  
  while((x=fscanf(fp,"%s %s %s",buff,buff1,buff2))!=EOF){  
    int i = 0;
    ioptr=System_Inputs+i;               /* points to angle */

    for(int j = 0; j < System_Inputs[i].nb_mf; j++){ 
      mfptr=ioptr->membership_functions+j;
      if((strcmp(mfptr->name,buff))==0){  
        
        ifptr=(struct rule_element_type *)
        calloc(1,sizeof(struct rule_element_type));
        ruleptr->if_side=ifptr;      /* points to angle */
        ifptr->value=&mfptr->value;  /* needs address here */
        ifptr->next=(struct rule_element_type *)
        calloc(1,sizeof(struct rule_element_type));
        ifptr=ifptr->next;
        break;                       /* match found */
      }
    }

    i++;
    ioptr=System_Inputs+i;  
    for(int j = 0; j < System_Inputs[i].nb_mf; j++){
      mfptr=ioptr->membership_functions+j;
      
      if((strcmp(mfptr->name,buff1))==0){  
        ifptr->value=&mfptr->value;  /* needs address here */
        break;                       /* match found */
      }
    }
    
    int find = 0;
    for(int k = 0; k < 2 && !find; k++){
      outptr=System_Outputs+k;/* point then stuff to output */
      
      for(int j = 0; j < outptr->nb_mf && !find; j++){
        mfptr=outptr->membership_functions+j;
        if((strcmp(mfptr->name,buff2))==0){  
          
          thenptr=(struct rule_element_type *)
          calloc(1,sizeof(struct rule_element_type));
          ruleptr->then_side=thenptr;
          thenptr->value=&mfptr->value; /* needs address here */
          find = 1;
        }
      }
    }
    
    ruleptr->next=(struct rule_type *)calloc(1,sizeof(struct rule_type));
    ruleptr=ruleptr->next;
  }
  
  fclose(fp);
}

void get_system_inputs(input1,input2)
float input1, input2;
{  
  struct io_type *ioptr;
  ioptr=System_Inputs+0;
  ioptr->value=input1;
  ioptr=System_Inputs+1;
  ioptr->value=input2;
} 
void put_system_outputs(){  
  struct io_type ioptr;
  struct mf_type mfptr;
  struct rule_type *ruleptr;
  struct rule_element_type *ifptr;
  struct rule_element_type *thenptr;
  int cnt=1;
  
  for(int i = 0; i < 3; i++){
    ioptr=System_Inputs[i];
    printf("%s: Value= %f\n",ioptr.name,ioptr.value);

    for(int j = 0; j < 2; j++){
      mfptr=ioptr.membership_functions[j];
      printf("  %s: Value %f Left %f Right %f\n",
        mfptr.name,mfptr.value,mfptr.trapezoid[0].straight_line[0].x,
        mfptr.trapezoid[1].straight_line[1].x);
    }

    printf("\n");
  }
  
  for(int i = 0; i < 2; i++){
    ioptr=System_Outputs[i];
    printf("%s: Value= %f\n",ioptr.name,ioptr.value);
    
    for(int j = 0; j < 2; j++){
      mfptr=ioptr.membership_functions[j];
          printf("  %s: Value %f Left %f Right %f\n",
            mfptr.name,mfptr.value,mfptr.trapezoid[0].straight_line[0].x,
            mfptr.trapezoid[1].straight_line[1].x);
    }
  }
  /* print values pointed to by rule_type (if & then) */
  printf("\n");
  for(ruleptr=Rule_Base;ruleptr->next!=NULL;ruleptr=ruleptr->next){
    printf("Rule #%d:",cnt++);
    
    for(ifptr=ruleptr->if_side;ifptr!=NULL;ifptr=ifptr->next)
      printf("  %f",*(ifptr->value));
    for(thenptr=ruleptr->then_side;thenptr!=NULL;thenptr=thenptr->next)
      printf("  %f\n",*(thenptr->value));
  }
  printf("\n");
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
  
  float left = (float)max_f(robot->IRSensor[0].DistanceValue 
               , robot->IRSensor[1].DistanceValue 
               , robot->IRSensor[2].DistanceValue);

  float right = (float)max_f(robot->IRSensor[3].DistanceValue 
               , robot->IRSensor[4].DistanceValue 
               , robot->IRSensor[5].DistanceValue);

  initialize_system();
  get_system_inputs(left,right);
  fuzzification();
  rule_evaluation();
  aggregation();
  defuzzification();
  robot->Motor[LEFT].Value  = System_Outputs[0].value;
  robot->Motor[RIGHT].Value = System_Outputs[1].value;

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