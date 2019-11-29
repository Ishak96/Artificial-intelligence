#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 10          /* max number of characters in names*/
#define max(a,b)   (a<b ? b : a)
#define min(a,b)   (a>b ? b : a)
#define sign(a)    (a<0 ? -1 : 1)

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
  char name[MAXNAME];                     	/*  name of system input/output       */
  float value;                              /*  value of system input/output      */
  struct mf_type membership_functions[2]; 	/*  list of membership functions for this system input/outputs  */
};

/*  Each rule has an if side and a then side. Elements making up if side are
pointers to antecedent values inside mf_type structure. Elements making up then
side of rule are pointers to output strength values, also inside mf_type
structure. Each rule structure contains a pointer to next rule in rule base. */
struct rule_element_type{
	float *value;                		/* pointer to antecedent/output strength value */
	struct rule_element_type *next; 	/* next antecedent/output element in rule 	   */
};

struct rule_type{
	struct rule_element_type *if_side;     /* list of antecedents in rule */
	struct rule_element_type *then_side;   /* list of outputs in rule     */
	struct rule_type *next;                /* next rule in rule base  	  */
};

struct rule_type *Rule_Base;	/* list of all rules in rule base */
struct io_type System_Inputs[2];
struct io_type System_Outputs[2];
float UPPER_LIMIT = 1.f;   	/* max number assigned as degree of membership */

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
					printf("name = %s, p.x = %f, p.y = %f\n", mf->name, p->x, p->y);
				}
				else if(i==1 && j==0){
					p=trapezoid->straight_line+j;
					p->y = value;
					p->x = (slopes!=0.f) ? (value-intercept)/slopes : p->x;
					printf("name = %s, p.x = %f, p.y = %f\n", mf->name, p->x, p->y);
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
center-of-gravity defuzzification. Area values must be recalculated with each
pass. Area of trapezoid is h*(a+b)/2 where h=height=output_strength=mf->value
b=base=mf->point2-mf->point1 a=top= must be derived from h,b, and slopes1&2 */
float compute_area_of_trapezoid(mf)
struct mf_type mf;
{
	float base;
	float top;
	float area;
	float run_1,run2;

	run_1 = mf->value/mf.straight_line[0].slopes;
	run_2 = mf->value/mf->straight_line[1].slopes;
 	base = mf.trapezoid[0].straight_line[0].x - mf.trapezoid[1].straight_line[1].x;
	top = base-run_1-run_2;
	area = mf.value * ( base + top ) / 2;
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
	struct rule_element_type *ip;       /* pointer of antecedents  (if-parts)   	   */
	struct rule_element_type *tp;       /* pointer to consequences (then-parts) 	   */
	float strength;                		/* strength of  rule currently being evaluated */
 
 	strength = UPPER_LIMIT;                       /* max rule strength allowed */
	for(rule=Rule_Base; rule != NULL; rule=rule->next){
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

/*Aggregation*/
void aggregation(){
	struct io_type* si;    /* system input pointer        */
	struct mf_type* mf;    /* membership function pointer */
	
	for(int i = 0; i < 2; i++){
		si=System_Outputs+i;
		for(int j = 0; j < 2; j++){
			mf=si->membership_functions+j;
			clipping_trapezoid(mf);
		}
	}
}

/* Defuzzification */
void defuzzification(){
	struct io_type *so;    	 /* system output pointer */
	struct mf_type mf;   	 /* output membership function pointer */
	float sum_of_products;   /* sum of products of area & centroid */
	float sum_of_areas;  	 /* sum of shortend trapezoid area */
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
			centroid = (mf.trapezoid[0].straight_line[1].x + mf.trapezoid[1].straight_line[0].x) / 2.f;
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
	create_mf(input_mf+0,"CL_FAR",0.f,0.f,0.f,1.f,408.f,1.f,511.5f,0.f);

	create_mf(input_mf+1,"CL_NEAR",511.5f,0.f,810.f,1.f,1023.f,1.f,1023.f,0.f);

	create_mf(output_mf+0,"MR--",-5.f,0.f,-5.f,1.f,-2.f,1.f,0.f,0.f);

	create_mf(output_mf+1,"MR++",0.f,0.f,2.f,1.f,5.f,1.f,5.f,0.f);


	/*initilize input/output system*/
	sprintf(System_Inputs[0].name, "%s", "CL");
	memcpy(System_Inputs[0].membership_functions, input_mf, sizeof(input_mf));

	sprintf(input_mf[0].name, "%s", "CR_FAR");
	sprintf(input_mf[1].name, "%s", "CR_NEAR");
	sprintf(System_Inputs[1].name, "%s", "CR");
	memcpy(System_Inputs[1].membership_functions, input_mf, sizeof(input_mf));

	sprintf(System_Outputs[0].name, "%s", "MR");
	memcpy(System_Outputs[0].membership_functions, output_mf, sizeof(output_mf));

	sprintf(output_mf[0].name, "%s", "ML--");
	sprintf(output_mf[1].name, "%s", "ML++");
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

void main(argc,argv)
int argc;
char *argv[];
{  
	float input1, input2;
	if(argc!=3){
		printf("Error - Must supply 2 numeric inputs.\n");
		printf("        Inputs scaled to range 0-1023.\n");
		printf("Usage: %s angle velocity\n",argv[0]);
		exit(0);
	}

	input1=atof(argv[1]);
	input2=atof(argv[2]);

	initialize_system();
	get_system_inputs(input1,input2);
	fuzzification();
	rule_evaluation();
	aggregation();
	defuzzification();
	put_system_outputs();
}