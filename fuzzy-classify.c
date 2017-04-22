#include "header.h"
#include "k.c"  


float sample_percent;
enum {TRUE, FALSE};

//#define  INPUT  3*NO_ATRIB
//#define  HIDDEN  NO_CLASS
//#define  OUTPUT  NO_CLASS
//#define  ITER   300
//#define  THRESHOLD .01
//#define  DECAY .00001  /* originally 0.00001 */
//#define  ALPHA .3
//#define  ETA .5



int main(int argc, char *argv[]) {
  int i ;

  int ERR = FALSE;
  char mlp_init_method[100];

  for(i =1; i<argc; i++)
  {
        if(strcmp(argv[i],"-mlp_init_method")==0)
             strcpy(mlp_init_method, argv[++i]);
        else
         {
           ERR=TRUE;
         }
   }

   if( 
        strcmp(mlp_init_method, "RANDOM" ) &&
        strcmp(mlp_init_method, "KNOWLEDGE" ) 
     )
   {
       ERR = TRUE;
   }


   if(ERR==TRUE)
   {
       fprintf(stderr,"Usage : %s -mlp_init_method [ \"RANDOM\" or \"KNOWLEDGE\" ]\n", argv[0]);
       exit(0);
   }

 
   remove("spnres.dat");
   remove("tree.dat");
   remove("class_data.dat");
   remove("ornres.dat");
   remove("confusion.dat");

   for(i=1;i<=TIMES;i++) {
     sample_percent = (1. + floor((float)i/50.0))*(float)SAMPLE_PERCENT;
     printf("\n\n            EXPERIMENTAL RUN # : %d\n",i);
         printf("            ++++++++++++++++++\n\n");

/* THIS IS THE 1ST FILE IN THE PROJECT USED TO PARTITION THE DATE
   INTO TEST SET AND TRAINING SET   */
     first();   /* in a.c  */

#ifdef ON_TRAIN_SET     
     system("cp train.dat test.dat"); 
#endif

#ifdef PAWAN_MEMB
/*       THIS PROGRAM CALCULATES THE INPUT MEMBERSHIP OF THE TRAIN SET
         USING PAWAN'S METHOD OF CALCULATING THE INPUT MEMBERSHIP    */
    
     second1();  /* in k.c   */
#endif

#ifdef KISHOR_MEMB
 /*   THIS PROGRAM CALCULATES THE INPUT MEMBERSHIP KISHORI'S METHOD  */
     second2();   /* in i.c   */
#endif

     printf("FUZZY ID3 RESULTS:\n"); 
     printf("=================\n");  

#ifdef FUZZY_ID3_STOP
   getchar();
#endif

 /*  CALCULATING THE BINARY MEMBERSHIP AND THE MEMBERSHIP OF THE TRAINING SET   */
     third();    /* in j.c */
  
 /*  CALCULATING THE MEMBERSHIP OF THE TRAINING PATTERNS TO 
DIFFERENT CLASSES  AND THE MEANS ASSOCIATED WITH THEM */          
     fourth();   /*  in h.c   */


 /*   CREATION OF THE FUZZY ID3 DECISION TREE 
      THIS PROGRAM REMOVES THE REDUNDANT NODES  OF THE DECISION 
      TREE WITH NO PATTERNS  OR LESS THAN 90 PERCENT OF THE TOTAL IN 
      THAT NODE USE FUZZY ENTROPY   */
     fifth();  /*    in g.c   */


   /*   THIS PROGRAM ANALYSES THE TRAINING SET AND USES FUZZY TERMS  */
     sixth();  /*    in f.c   */


 /*   THIS PROGRAM ANALYSES THE TESTRAINING SET AND USES FUZZY TERMS  */
     seventh(); /*   in e.c   */

    /*     RULES ARE GENERATED IN THIS PROGRAM  */
     eighth();  /*   in d.c   */



#ifdef ORDINARY_MLP_STOP
     getchar();
#endif
    if( !strcmp(mlp_init_method, "RANDOM" ) ) 
    {
        printf("\nRANDOMLY INITIALIZED MLP RESULTS:\n"); 
        printf("================================\n");  
      /*    THIS PROGRAM USES A THREE LAYER RANDOMLY INITIALISED NEURAL 
                     NETWORK TO CLASSSIFY THE PATTERNS  */
        ninth();  /*    in c.c    */
    }

    if( !strcmp(mlp_init_method, "KNOWLEDGE" ) )
    {
        printf("\nKNOWLEDGE-ENCODED MLP RESULTS:\n"); 
        printf("=============================\n"); 
#ifdef KNOWLEDGE_STOP
  getchar();
#endif
/* THIS PROGRAM USES A THREE LAYER  KNOWLEDGE ENCODED NEURAL NETWORK
     TO CLASSIFY THE PATTERN_IS */
        tenth();  /*      in b.c     */
    }


   }
   
   return(0);
   

}
