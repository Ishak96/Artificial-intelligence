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
#define MAXNAME 10          /* max number of characters in names*/
#define max(a,b)   (a<b ? b : a)
#define min(a,b)   (a>b ? b : a)
#define sign(a)    (a<0 ? -1 : 1)

struct mf_type{
  char name[MAXNAME];     /* name of membership function (fuzzy set)    */
  float value;            /* degree of membership or output strength    */
  float point1;           /* leftmost x-axis point of mem. function     */
  float point2;           /* rightmost x-axis point of mem. function    */
  float slope1;           /* slope of left side of membership function  */
  float slope2;           /* slope of right side of membership function */
};

struct io_type{
  char name[MAXNAME];                       /*  name of system input/output       */
  float value;                              /*  value of system input/output      */
  struct mf_type membership_functions[2];   /*  list of membership functions for this system input/outputs  */
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
  delta_1 = input - mf->point1;
  delta_2 = mf->point2 - input;

  if ((delta_1 < 0) || (delta_2 < 0))   /* input outside mem. function ? */
    mf->value = 0;                      /* then degree of membership is 0 */
  else{
    if(mf->slope1 == 0)
      mf->value = min( (mf->slope1*delta_1),(mf->slope2*delta_2) );
    else if(mf->slope2 == 0)
      mf->value = max( (mf->slope1*delta_1),(mf->slope2*delta_2) );
    
    mf->value = min(sign(mf->value)*mf->value,UPPER_LIMIT);  /* enforce upper limit */
  }
}

/* Compute Area of Trapezoid--Each inference pass produces a new set of output
strengths which affect the areas of trapezoidal membership functions used in
center-of-gravity defuzzification. Area values must be recalculated with each
pass. Area of trapezoid is h*(a+b)/2 where h=height=output_strength=mf->value
b=base=mf->point2-mf->point1 a=top= must be derived from h,b, and slopes1&2 */
float compute_area_of_trapezoid(mf)
struct mf_type mf;
{
  float run_1;
  float run_2;
  float base;
  float top;
  float area;
  base = mf.point2 - mf.point1;
  run_1 = (mf.slope1 != 0) ? mf.value/mf.slope1 : mf.slope1;
  run_2 = (mf.slope2 != 0) ? mf.value/mf.slope2 : mf.slope2;
  top = base - run_1 - run_2;
  area = mf.value * ( base + top )/2;
  return(area);
}

/* Rule Evaluation--Each rule consists of a list of pointers to antecedents
(if side), list of pointers to outputs (then side), and pointer to next rule
in rule base. When a rule is evaluated, its antecedents are ANDed together,
using a minimum function, to form strength of rule. Then strength is applied
to each of listed rule outputs. If an output has already been assigned a rule
strength, during current inference pass, a maximum function is used to
determine which strength should apply. */
void rule_evaluation(){
  struct rule_type *rule;
  struct rule_element_type *ip;       /* pointer of antecedents  (if-parts)        */
  struct rule_element_type *tp;       /* pointer to consequences (then-parts)      */
  float strength;                   /* strength of  rule currently being evaluated */
 
  for(rule=Rule_Base; rule != NULL; rule=rule->next){
    strength = UPPER_LIMIT;                       /* max rule strength allowed */
        /* process if-side of rule to determine strength */
    for(ip=rule->if_side; ip != NULL; ip=ip->next)
      strength = min(strength,*(ip->value));
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
    for(int j = 0; j < 2; j++){
      mf=si->membership_functions+j;
      compute_degree_of_membership(mf,si->value);
    }
  }
}

/* Defuzzification */
void defuzzification(){
  struct io_type *so;    /* system output pointer */
  struct mf_type mf;    /* output membership function pointer */
  float sum_of_products;   /* sum of products of area & centroid */
  float sum_of_areas;   /* sum of shortend trapezoid area */
  float area;
  float centroid;
 
 /* compute a defuzzified value for each system output */
  for(int i = 0; i < 2; i++){
    so=System_Outputs+i;
    sum_of_products = 0;
    sum_of_areas = 0;
    
    for(int j = 0; j < 2; j++){
      mf=so->membership_functions[j];
      area = compute_area_of_trapezoid(mf);
      centroid = mf.point1 + (mf.point2 + mf.point1) / 2.f;
      sum_of_products += area * centroid;
      sum_of_areas += area;
    }
    
    so->value = (sum_of_areas != 0.f) ? sum_of_products/sum_of_areas : sum_of_areas;   /* weighted average */
  }
}

void initialize_system(){
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

  struct mf_type input_mf[2], output_mf[2];
  
  /*create membership function for input/output system*/
  sprintf(input_mf[0].name, "%s", "CL_FAR");
  input_mf[0].point1 = 0.f;
  input_mf[0].point2 = 511.5f;
  input_mf[0].slope1 = 0.f;
  input_mf[0].slope2 = -0.0097f;

  sprintf(input_mf[1].name, "%s", "CL_NEAR");
  input_mf[1].point1 = 511.5f;
  input_mf[1].point2 = 1023.f;
  input_mf[1].slope1 = 0.0034f;
  input_mf[1].slope2 = 0.f;

  sprintf(output_mf[0].name, "%s", "MR--");
  output_mf[0].point1 = -5.f;
  output_mf[0].point2 = 0.f;
  output_mf[0].slope1 = 0.f;
  output_mf[0].slope2 = -0.5f;

  sprintf(output_mf[1].name, "%s", "MR++");
  output_mf[1].point1 = 0.f;
  output_mf[1].point2 = 5.f;
  output_mf[1].slope1 = 0.5f;
  output_mf[1].slope2 = 0.f;


  /*initilize input/output system*/
  sprintf(System_Inputs[0].name, "%s", "CL");
  memcpy(System_Inputs[0].membership_functions, input_mf, sizeof(input_mf));

  sprintf(input_mf[0].name, "%s", "CR_FAR");
  sprintf(input_mf[1].name, "%s", "CR_NEAR");
  sprintf(System_Inputs[1].name, "%s", "CR");
  memcpy(System_Inputs[1].membership_functions, input_mf, sizeof(input_mf));

  sprintf(System_Outputs[0].name, "%s", "MR");
  memcpy(System_Outputs[0].membership_functions, output_mf, sizeof(output_mf));

  sprintf(output_mf[0].name, "%s", "ML++");
  sprintf(output_mf[1].name, "%s", "ML--");
  sprintf(System_Outputs[1].name, "ML");
  memcpy(System_Outputs[1].membership_functions, output_mf, sizeof(output_mf));

  if((fp=fopen("rules","r"))==NULL){  
    printf("ERROR- Unable to open data file named %s.\n","rules");
    exit(0);
  }
  
  ruleptr=(struct rule_type *)calloc(1,sizeof(struct rule_type));
  
  Rule_Base=ruleptr;     /* first time thru, anchor */
  

  while((x=fscanf(fp,"%s %s %s",buff,buff1,buff2))!=EOF){  
    int i = 0;
    ioptr=System_Inputs+i;               /* points to angle */
    
    for(int j = 0; j < 2; j++){ 
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
    for(int j = 0; j < 2; j++){ 
      mfptr=ioptr->membership_functions+j; 
      if((strcmp(mfptr->name,buff1))==0){  
        ifptr->value=&mfptr->value;  /* needs address here */
        break;                       /* match found */
      }
    }
    
    int find = 0;
    for(int k = 0; k < 2 && !find; k++){
      outptr=System_Outputs+k;/* point then stuff to output */
      
      for(int j = 0; j < 2 && !find; j++){
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
  
  for(int i = 0; i < 2; i++){
    ioptr=System_Inputs[i];
    printf("%s: Value= %f\n",ioptr.name,ioptr.value);

    for(int j = 0; j < 2; j++){
      mfptr=ioptr.membership_functions[j];
      printf("  %s: Value %f Left %f Right %f\n",
        mfptr.name,mfptr.value,mfptr.point1,mfptr.point2);
    }

    printf("\n");
  }
  
  for(int i = 0; i < 2; i++){
    ioptr=System_Outputs[i];
    printf("%s: Value= %f\n",ioptr.name,ioptr.value);
    
    for(int j = 0; j < 2; j++){
      mfptr=ioptr.membership_functions[j];
          printf("  %s: Value %f Left %f Right %f\n",
            mfptr.name,mfptr.value,mfptr.point1,mfptr.point2);
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
  if(pas == 0){
    initialize_system();
  }
  pas++;
  DrawPas();
  
  float left = (float)(robot->IRSensor[0].DistanceValue 
               + robot->IRSensor[1].DistanceValue 
               + robot->IRSensor[2].DistanceValue) / 3;

  float right = (float)(robot->IRSensor[4].DistanceValue 
               + robot->IRSensor[5].DistanceValue 
               + robot->IRSensor[6].DistanceValue) / 3;

  get_system_inputs(left,right);
  fuzzification();
  rule_evaluation();
  defuzzification();
  put_system_outputs();
  robot->Motor[LEFT].Value  = System_Outputs[0].value;
  robot->Motor[RIGHT].Value = System_Outputs[1].value;

  if ((robot->IRSensor[6].DistanceValue > COLLISION_TH)||  /* collision in */
      (robot->IRSensor[7].DistanceValue > COLLISION_TH))   /*   the back   */
    return(FALSE);
  else
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