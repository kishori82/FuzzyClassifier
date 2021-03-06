/* THIS PROGRAM USES A THREE LAYER KNOWLEDGE ENCODED NEURAL NETWORK
     TO CLASSIFY THE PATTERN_IS */

#include "header.h" 

void tenth()  {
   /*time_t t; */
   int i,j,k;
   long iter;
   int link_count;
   double conf[OUTPUT][OUTPUT];
   double oi[INPUT+1],oj[HIDDEN+1],ok[OUTPUT],tar[OUTPUT];
   double **wj,**wk;
   int **flag_wj,**flag_wk;
   double dwj[HIDDEN][INPUT+1],dwk[OUTPUT][HIDDEN+1];
   double odwj[HIDDEN][INPUT+1];
   double delta_j[HIDDEN+1],delta_k[OUTPUT];
   PATTERN_I **patt,**train,**test;
   MAPRULE *maprule;
   FILE *fp,*fq,*fr;
   int no_lns1,no_lns2;
   int train_size,test_size;
   int get_lines(FILE *);
   MAPRULE *weightset();
   double theta_0=1.;
   double eta=ETA;
   double alpha=ALPHA;
   double Error=0.;
   

/* function declarations   */
   //void flagset(MAPRULE *,int **,int **,double [][],double [][]);
   void flagset(MAPRULE *,int **,int **,double **,double **);

/* temporary variables   */
   double net;
   PATTERN_I *temp_pat;
   int ran1,ran2;
   double tot;
   double max_val;
   int max_class=-1;

/* allocate space for the weights */
   wj=(double **)malloc(HIDDEN*sizeof(double *));
   for(i=0;i<HIDDEN;i++)
     wj[i]=(double *)malloc(sizeof(double)*(INPUT+1));

   wk=(double **)malloc(OUTPUT*sizeof(double *));
   for(i=0;i<OUTPUT;i++)
     wk[i]=(double *)malloc(sizeof(double)*(HIDDEN+1));

/* allocate space for the flags */
   flag_wj=(int **)malloc(HIDDEN*sizeof(int *));
   for(i=0;i<HIDDEN;i++)
     flag_wj[i]=(int *)malloc(sizeof(int)*(INPUT+1));

   flag_wk=(int **)malloc(sizeof(int *)*OUTPUT);
   for(i=0;i<OUTPUT;i++)
     flag_wk[i]=(int *)malloc(sizeof(int)*(HIDDEN+1));

   maprule = weightset(); 
/* set the flags    */
   flagset(maprule,flag_wj,flag_wk,wj,wk);

/* open the files  */
   fp=fopen("inmem.dat","r"); 
   fq=fopen("inmemtest.dat","r"); 
   fr=fopen("spnres.dat","a"); 
   no_lns1=get_lines(fp);
   no_lns2=get_lines(fq);
   train_size = no_lns1;
   test_size = no_lns2;

/* allocate memory location for test and train patterns */
   patt=(PATTERN_I **)malloc((no_lns1+no_lns2)*sizeof(PATTERN_I *));
   for(i=0;i<no_lns1+no_lns2;i++)
      patt[i]=(PATTERN_I *)malloc(sizeof(PATTERN_I));

   train=(PATTERN_I **)malloc(train_size*sizeof(PATTERN_I *));

   test=(PATTERN_I **)malloc(test_size*sizeof(PATTERN_I *));

/* read the patterns */
   k=0;
   do {
      if(fscanf(fp,"%d",&patt[k]->class)<0)
        break;  
      for(i=0;i<INPUT;i++)
        fscanf(fp,"%lf",&patt[k]->atrib[i]);
      k++;
   }while(!feof(fp));
   fclose(fp);

   do {
      if(fscanf(fq,"%d",&patt[k]->class)<0)
        break;  
      for(i=0;i<INPUT;i++)
        fscanf(fq,"%lf",&patt[k]->atrib[i]);
      k++;
   }while(!feof(fq));
   fclose(fq);


/* seperate the trainig and test set */
   for(i=0;i<train_size;i++) 
      train[i]=patt[i];
   for(i=train_size;i<no_lns1+no_lns2;i++) 
      test[i-train_size]=patt[i];

/* initialize the weights initially */
   oi[INPUT] =1.;
   oj[HIDDEN]=1.;
   for(i=0;i<HIDDEN;i++)    /* for wj */
     for(j=0;j<=INPUT;j++) 
       if(flag_wj[i][j]==0) {
          wj[i][j] = (float)rand()/(float)RAND_MAX-.5; 
          wj[i][j]/=1000.; 
          flag_wj[i][j]=1;  
       }
     

   for(i=0;i<OUTPUT;i++)    /* for wk */
     for(j=0;j<=HIDDEN;j++)  
       if(flag_wk[i][j]==0) {
          wk[i][j] = (float)rand()/(float)RAND_MAX-.5;  
          wk[i][j]/=1000.;  
/*          flag_wk[i][j]=1;  */
       }
  
 /* calculates the delta weights changes   */
   for(k=0;k<OUTPUT;k++)
      for(j=0;j<=HIDDEN;j++)
         dwk[k][j]=0.;
   for(j=0;j<HIDDEN;j++)
      for(k=0;k<=INPUT;k++)
         dwj[j][k]=0.;

/*   this loops trains the network    */
 iter =0; 
 do {
   Error=0.;
   for(i=0;i<train_size;i++)  {
       /* updates the weights   */
       for(k=0;k<OUTPUT;k++)
         for(j=0;j<=HIDDEN;j++)
           wk[k][j]+=dwk[k][j];
       for(j=0;j<HIDDEN;j++)
         for(k=0;k<=INPUT;k++)
           wj[j][k]+=dwj[j][k];

       /*neutralising the weights  */
       for(j=0;j<HIDDEN;j++)
         for(k=0;k<INPUT+1;k++)
            if(flag_wj[j][k]==0)
               wj[j][k]=0.;

       for(j=0;j<OUTPUT;j++)
         for(k=0;k<HIDDEN+1;k++)
            if(flag_wk[j][k]==0)
               wk[j][k]=0.;  

       /* reads a pattern */
       for(j=0;j<OUTPUT;j++)
          tar[j]=.1;
       tar[train[i]->class]=.9;
       for(j=0;j<INPUT;j++)
          oi[j]=train[i]->atrib[j];

       /* calculates the net for hidden layer */
       for(j=0;j<HIDDEN;j++) {
          net =0.;
          for(k=0;k<=INPUT;k++)
             net+= wj[j][k]*oi[k];
          net = -net/theta_0;
          oj[j] = 1./(1.+exp(net));
       } 

       /* calculates the net for output layer */
       for(j=0;j<OUTPUT;j++) {
          net =0.;
          for(k=0;k<=HIDDEN;k++)
             net+= wk[j][k]*oj[k];
          net = -net/theta_0;
          ok[j] = 1./(1.+exp(net));
       } 

    /* calculates the deltas of output layer */
       for(j=0;j<OUTPUT;j++)   
          delta_k[j]=(tar[j]-ok[j])*ok[j]*(1.-ok[j]);

   /*   calculates the deltas of hidden layer */
       for(j=0;j<=HIDDEN;j++)  {
          delta_j[j]=0.;
          for(k=0;k<OUTPUT;k++)
             delta_j[j]+= delta_k[k]*wk[k][j];
          delta_j[j]*=oj[j]*(1.-oj[j]);
       }
       /* stores the old delta weights */ 
       for(j=0;j<HIDDEN;j++)
         for(k=0;k<=INPUT;k++)
           odwj[j][k]=dwj[j][k];

       /* calculates the delta ws   */
       for(k=0;k<OUTPUT;k++)
         for(j=0;j<=HIDDEN;j++) {
           dwk[k][j]=eta*delta_k[k]*oj[j];
/*            dwk[k][j]-=DECAY*wk[j][k];  */
         }
       for(j=0;j<HIDDEN;j++)
         for(k=0;k<=INPUT;k++) {
           dwj[j][k]=eta*delta_j[j]*oi[k]+alpha*odwj[j][k];
   /*        dwj[j][k]-=DECAY*wj[j][k]; */
         }
       
       /* Calculate the sytem error */
       for(k=0;k<OUTPUT;k++) 
         Error+= .5*pow((tar[k]-ok[k]),2);
    }  /* end of no_lns for loop */

  /* shuffle the train set */
   for(i=0;i<100*train_size;i++) {
      ran1=train_size*(int)((float)rand()/((float)RAND_MAX+1.));
      ran2=train_size*(int)((float)rand()/((float)RAND_MAX+1.));
      temp_pat = train[ran1];
      train[ran1]=train[ran2];
      train[ran2]=temp_pat;   
   } 
    
    Error=Error/(float)train_size;
/*    printf("System error is %lf\n",Error);  */
    iter++;
    if(iter%100 ==0)
       alpha+=.1;
 } while(fabs(Error)>THRESHOLD && iter < ITER);

/* initializing the confusion matrix   */
   for(i=0;i<OUTPUT;i++)
     for(j=0;j<OUTPUT;j++)
        conf[i][j]=0.;

/* classifying the test set and determining the confusion matrix  */
  for(i=0;i<test_size;i++) {
       /* reads a pattern from the test set */
       for(j=0;j<INPUT;j++)
          oi[j]=test[i]->atrib[j]; 

       /* calculates the net for hidden layer */
       for(j=0;j<HIDDEN;j++) {
          net =0.;
          for(k=0;k<=INPUT;k++)
             net+= wj[j][k]*oi[k];
          net = -net/theta_0;
          oj[j] = 1./(1.+exp(net));
       } 
       oj[HIDDEN] = 1.;   
       
       max_val = -1.;
       /* calculates the net for output layer */
       for(j=0;j<OUTPUT;j++) {
          net =0.;
          for(k=0;k<=HIDDEN;k++)
             net+= wk[j][k]*oj[k];
          net = -net/theta_0;
          ok[j] = 1./(1.+exp(net));
          if(max_val<ok[j]) {
             max_val=ok[j];
             max_class = j;
          }
       }
       conf[test[i]->class][max_class]++;
  } 
   printf("\nCONFUSION MATRIX:\n");
   for(i=0;i<OUTPUT;i++) {
     for(j=0;j<OUTPUT;j++)
        printf(" %d ",(int)conf[i][j]);
     printf("\n"); 
   }  

   printf("\n"); 
   printf("SUCCESS PERCENT BY CLASS:\n");
   for(i=0;i<OUTPUT;i++) {
     tot=0.;
     for(j=0;j<OUTPUT;j++)
       tot+=conf[i][j];
     tot = 100.*conf[i][i]/tot;
     printf("CLASS %d = %5.2lf\n",i+1,tot); 
     fprintf(fr,"%lf ",tot); 
   }

   printf("\n");   
   tot=0.;
   for(i=0;i<OUTPUT;i++) 
     tot+=conf[i][i];
   tot = 100.*tot/(double)test_size;

   printf("TOTAL SUCCESS %% = %5.2f  in %ld ITERATIONS\n\n",tot,iter);
   fprintf(fr,"%f  %ld ",tot,iter);
   link_count=0;


   printf("INITIALIZED WITH KNOWLEDGE-ENCODING:\n");
   printf("+++++++++++++++++++++++++++++++++++\n");
   printf("WEIGHTS: INPUT ---> HIDDEN LAYER\n");
   for(i=0;i<HIDDEN;i++) {
      for(j=0;j<INPUT+1;j++) {
         if(fabs(wj[i][j])>.5) link_count++;
         printf("%-8.4f ",wj[i][j]);  
      }
     printf("\n");   
   }
   printf("\n");   
   printf("WEIGHTS: HIDDEN  --> OUTPUT LAYER\n");
   for(i=0;i<OUTPUT;i++) {
      for(j=0;j<HIDDEN+1;j++) {
         if(fabs(wk[i][j])>.5) link_count++;
         printf("%-8.4f ",wk[i][j]); 
      }
      printf("\n");   
   } 
  
   printf("\n# SIGNIFICANT LINKS IN  NETWORK: %d\n",link_count);
   fprintf(fr,"%d\n",link_count);


/*  freeing memory  */
   for(i=0;i<HIDDEN;i++)
     free(flag_wj[i]);
   free(flag_wj);

   for(i=0;i<OUTPUT;i++)
     free(flag_wk[i]);
   free(flag_wk);

   for(i=0;i<no_lns1+no_lns2;i++)
      free(patt[i]);
   free(patt);
   free(train);
   free(test);
   fclose(fr); 
}

//void flagset(MAPRULE *maprule,int **flag_wj,int **flag_wk,double wj[HIDDEN][INPUT+1],double wk[OUTPUT][HIDDEN+1]) {
void flagset(MAPRULE *maprule,int **flag_wj,int **flag_wk,double **wj,double **wk) {
   int i,j;
   FILE *fp;
   float weight;
   int index;
   int no_lns;
   int get_lines(FILE *);

   fp=fopen("selrule.dat","r");
   no_lns = get_lines(fp);
   for(i=0;i<HIDDEN;i++)
      for(j=0;j<INPUT+1;j++)
         flag_wj[i][j]=0;

   for(i=0;i<OUTPUT;i++)
      for(j=0;j<HIDDEN+1;j++)
         flag_wk[i][j]=0;

/* setting weights in the hidden layer */
   for(i=0;i<no_lns;i++) {
#ifdef MODEL_I
        weight = 1./(float)maprule[i].no_terms; 
#endif

#ifdef MODEL_II
        weight = maprule[i].freq/(float)maprule[i].no_terms; 
#endif

#ifdef MODEL_III
        weight = maprule[i].freq/(float)maprule[i].no_terms; 
#endif

   for(j=0;j<maprule[i].no_terms;j++) {
         index = abs(maprule[i].terms[j])-1;
         flag_wj[maprule[i].class-1][index]=1;
         if(maprule[i].terms[j]<0) 
            wj[maprule[i].class-1][index]=-weight;
         else 
            wj[maprule[i].class-1][index]= weight;

#ifdef MODEL_III
         wj[maprule[i].class-1][index]*=maprule[i].no_terms-(float)j;
         wj[maprule[i].class-1][index]/=maprule[i].no_terms/2.;
         wj[maprule[i].class-1][index]/=(maprule[i].no_terms+1.);
#endif
      }
   }
 

/* setting weights in the output layer */
  for(i=0;i<no_lns;i++) {
         weight = maprule[i].freq;
         flag_wk[maprule[i].class-1][i]=1;
         wk[maprule[i].class-1][i]=weight;
         wk[maprule[i].class-1][HIDDEN]=weight;
         flag_wk[maprule[i].class-1][HIDDEN]=1;
   }

 }

MAPRULE *weightset() {
   int i;
   FILE *fp;
   void map_rule(FILE *,MAPRULE *);
   MAPRULE *maprule;
   int no_lns;
   int get_lines(FILE *);

   int sum;

   fp=fopen("selrule.dat","r");
   no_lns=get_lines(fp);
 
   maprule=(MAPRULE *)malloc(no_lns*sizeof(MAPRULE));
   map_rule(fp,maprule);
   fclose(fp);

   sum=0.;
   for(i=0;i<no_lns;i++)  
      sum+=maprule[i].freq;
  
   for(i=0;i<no_lns;i++) 
      maprule[i].freq/=sum;

  return(maprule);
  
}

void map_rule(FILE *fp,MAPRULE *maprule) {
   int i,j;

   i=0;
   do {
      if(fscanf(fp,"%d",&maprule[i].no_terms)<0) break;
      for(j=0;j<maprule[i].no_terms;j++) 
        fscanf(fp,"%d",&maprule[i].terms[j]);
      fscanf(fp,"%d",&maprule[i].class);
      fscanf(fp,"%f",&maprule[i].freq);
      i++;
   }while(!feof(fp));
}
        

