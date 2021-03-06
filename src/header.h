#ifndef FILE_FOO_SEEN
#define FILE_FOO_SEEN

//switches    which algorithm  rand and konwledge encoded    num iterations 

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <limits.h>
#include <float.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

#define DATA_FILE "vowel.da" /*  INPUT FILE NAME */

#define SHIFT 0.0
#define SAMPLE_PERCENT 20
#define TIMES 5
#undef AUTO_SAMPLE_PERCENT 

#define NO_ATRIB 3  /* maxinum number of input attributes is 9 */
#define NO_CLASS 6

#define  INPUT  3*NO_ATRIB 
#define  HIDDEN  NO_CLASS 
#define  OUTPUT  NO_CLASS
#define  ITER   300
#define  THRESHOLD .01
#define  DECAY .00001  /* originally 0.00001 */
#define  ALPHA .3
#define  ETA .5


#define ON_TRAIN_SET

#undef PAWAN_MEMB
#define KISHOR_MEMB

/* MODELS OF ENTROPY */
#undef MY_FUZZY_ENTROPY
#undef CONVENTIONAL_ENTROPY
#undef PS_FUZZY_ENTROPY
#undef NEW_FUZZY_ENTROPY
#undef ONLY_FUZZY_ENTROPY
#undef TWO_FUZZY_ENTROPY
#define HEURISTIC_ENTROPY_1
#undef HEURISTIC_ENTROPY_2

#undef PRUNING
#define NO_PRUNING
#undef THRES_PRUNING 


#define MAX_CONFIDENCE 
#undef  PAW_CONFIDENCE 
#undef  NO_CONFIDENCE 

#define  THRES  .80

#define  Fd  15.0  /* standard is 15.0 */

#define  Fe  .6   /* standard is 0.6 */

/* Flags for stopping before  display */
#undef FUZZY_ID3_STOP
#undef KNOWLEDGE_STOP
#undef ORDINARY_MLP_STOP

/* Different models of weight encoding  */
#define MODEL_I
#undef MODEL_II
#undef MODEL_III

/*   INTO TEST SET AND TRAINING SET    */
int first();
void randomise(int *rand_array,int no_lns);
void read_dat(FILE *fpin);
int get_lines(FILE *fpin) ;
void swap(int *array,int i,int j);
void rsort(int *rand_array,int left,int right);


/*  THIS PROGRAM CALCULATES THE INPUT MEMBERSHIP KISHORI'S METHOD  */

void second2() ;
/* this function calculates the membership of the attributes */
 void in_attr_mem(FILE *fpout,int no_lns,int *class_arr, float 
**F_array, float *F_min,float *low,float *hig,float *F_max) ;

 void sort(float *temp_array,int no_lns);

/*  this function loads the pattern no , pattern class and the atributes */
void loadfile_b1(FILE *fpin,float **f_array,int *class_arr,int *patt_no);

void fno_lns_b1(FILE *fpin,int *no_lns);

void fmaxminval_b1(float *min,float *max,FILE *fin);
   
/*    CALCULATING THE BINARY MEMBERSHIP AND THE MEMBERSHIP OF
                   THE TRAINING SET                    */

void third();

/*  CALCULATING THE MEMBERSHIP OF THE TRAINING PATTERNS TO DIFFERENT
    CLASSES  AND THE MEANS ASSOCIATED WITH THEM */          
void fourth();

 /*     RULES ARE GENERATED IN THIS PROGRAM  */
typedef struct rules {
   int no;
   int rule_no[3*NO_ATRIB];
   int class;
   int freq;
 } RULE;

void eighth();
int get_rule(char *s);
void select_h(RULE **rule,int no_lns);
void sort_h1(RULE **rule,int no_lns) ;
void sort_h(RULE **rule,int no_lns) ;


/*    THIS PROGRAM USES A THREE LAYER RANDOMLY INITIALISED NEURAL NETWORK
     TO CLASSSIFY THE PATTERNS  */

typedef struct pattern_i {
   int class;
   double atrib[INPUT];
 } PATTERN_I;

void ninth(); 

void flagset_i(int **flag_wj,int **flag_wk);

/* THIS PROGRAM USES A THREE LAYER KNOWLEDGE ENCODED NEURAL NETWORK
     TO CLASSIFY THE PATTERN_IS */

typedef struct maprule { 
   int no_terms;
   int terms[3*NO_ATRIB];
   int class;
   float freq;
 } MAPRULE;

void tenth();

void flagset(MAPRULE *maprule,int **flag_wj,int **flag_wk,double **wj,double **wk) ; 

MAPRULE *weightset();

void map_rule(FILE *fp,MAPRULE *maprule) ;
        

 /*   CREATION OF THE FUZZY ID3 DECISION TREE 
      THIS PROGRAM REMOVES THE REDUNDANT NODES  OF THE DECISION 
      TREE WITH NO PATTERNS  OR LESS THAN 90 PERCENT OF THE TOTAL IN 
      THAT NODE USE FUZZY ENTROPY                               
*/

#define ROW  300
#define COL  150
#define INIT 70

struct STACK {
   int arr[3*NO_ATRIB];
   int top;
 } stack;
void push(int);
void pop();

typedef  struct pattern_pts {
   int class;
   float mem[NO_CLASS];
/*   float mem[NO_ATRIB*3]; */
   float inmem[NO_ATRIB*3];
   int  bin[NO_ATRIB*3];
 } **PATTPTR ;

typedef  struct node {
     int class,atrib;
     struct node *leftptr;
     struct node *rightptr;
     int left,mid,right;
 } *NODEPTR;

int fifth();

/*  this function makes the decision tree */
void maketree(NODEPTR root);

/* returns the class of a leaf node  */
int class(int left,int right);

/*  returns 0 if the node should NOT be split otherwise 1 */
int split(int left,int right);

/* this function return the attribute with which entropy is minimum */
int min_entropy_atr(NODEPTR root,int left,int right) ;

#ifdef ONLY_FUZZY_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif


#ifdef NEW_FUZZY_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif



#ifdef PS_FUZZY_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif

#ifdef TWO_FUZZY_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif

#ifdef MY_FUZZY_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif

#ifdef CONVENTIONAL_ENTROPY
/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right);
#endif



/*   allocated the memory of a node and returns the address */
NODEPTR getnode();

void sort_e(int min_atr,int left,int right);

/*  this function loads the pattern no , pattern class and the 
atributes */
void push(int a);

void pop();

/*  traverses the decision tree */
void traverse(int min,int max,int col,NODEPTR root,int x,int y,int k,int spc,int ud);

void print_tree();

/*  traverses the decision tree */
void travel(NODEPTR root);


#ifdef HEURISTIC_ENTROPY_1 
/* Using Entr calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int *left_freq,int *right_freq,int left,int right);
#endif

#ifdef HEURISTIC_ENTROPY_2
/* Using Ambiguity calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int *left_freq,int *right_freq,int left,int right);
#endif



struct RULESTACK{
     int top;
     int array[NO_ATRIB*3];
}  rulestack;

int seventh();

/*  function to extract the rules  */
void rules(NODEPTR root) ;
/* writes a rule into the rule file  */
void write_rule(NODEPTR root) ;

/* pops the stack by one */
void popg() ;

/* pushes a value into the stack */
void pushg(int val) ;

/* freeing the memory held by the decision tree */
void freenode(NODEPTR rootnode);

  /*   THIS PROGRAM ANALYSES THE TEST SET AND USES FUZZY TERMS */


typedef struct test_pts {
   int class,lab_class;
   float mem[NO_ATRIB*3];
   float inmem[NO_ATRIB*3];
   float atrib[NO_ATRIB];
   int  bin[NO_ATRIB*3];
 } **PATTERN_PTR ;


void sixth() ;

/*  classifier funcion to classify the test  */
void classifier(int i,PATTERN_PTR pattern,NODEPTR root) ;

/* this function calculates the membership of the attributes */
 void test_in_attr_mem(int no_lns,PATTERN_PTR pattern)  ;

/*  this function loads the pattern no , pattern class and the 
atributes */
void loadfile_f(FILE *fpin1,PATTERN_PTR pattern) ;

#ifdef NO_CONFIDENCE
/* this is the calculatiion of a class by max confidence factor */
int  confidence(int i,PATTERN_PTR pattern) ;
#endif

#ifdef MAX_CONFIDENCE
/* this is the calculatiion of a class by max confidence factor */
int  confidence(int i,PATTERN_PTR pattern);
#endif

#ifdef PAW_CONFIDENCE
/* this is the calculation of the confidence by Mandel's method */
int confidence(int i,PATTERN_PTR pattern);
#endif

/*  this function loads the  mean and standard deviation */
void loadfile_f1(FILE *fpin);
     
#endif /* !FILE_FOO_SEEN */
