  /*   THIS PROGRAM ANALYSES THE TEST SET AND USES FUZZY TERMS */
#include "header.h"

extern NODEPTR rootnode;  
float Mean[NO_CLASS][NO_ATRIB],Stdev[NO_CLASS][NO_ATRIB];
extern int conf_cls_node,leaf_node;
extern int size_tree;
 
FILE *fpout;
void sixth() {
     int i,j,k;
     int no_lns;
     FILE *fpin1,*fpin2,*fpout1,*fpout2;
     FILE *fclass;
     int conf_matrix[NO_CLASS][NO_CLASS];
     PATTERN_PTR  pattern;
     int ac_class[NO_CLASS],cal_class[NO_CLASS];
     int tot_ac_class=0,tot_cal_class=0;
     void fno_lns(FILE *,int *);
     void loadfile_f(FILE *,PATTERN_PTR);
     void loadfile_f1(FILE *);
     void test_in_attr_mem(int,PATTERN_PTR); 
     void classifier(int,PATTERN_PTR,NODEPTR);
     int confidence(int,PATTERN_PTR);
     float temp_user_accuracy,user_accuracy,accuracy;
     float kappa,temp_kappa;
     void freenode(NODEPTR);
     float av_confusion,confusion;
     float temp;
      


/*   opening the test set file to count the no lines  */
     fpin1 = fopen("test.dat","r");
     fpin2 = fopen("mnstd.dat","r");
     fpout1 = fopen("inmemtest.dat","w");   
     fpout2 = fopen("confusion.dat","a+");   


/* counting the no lines  */
    fno_lns(fpin1,&no_lns);

/*  allocating memory for the pointers to the structures */
     pattern = (PATTERN_PTR)malloc(no_lns*sizeof(struct test_pts *));
     for(i=0;i<no_lns;i++)
        pattern[i]=(struct test_pts *)malloc(sizeof(struct test_pts)); 

/*   loading the test set to the pattern array  */
     loadfile_f(fpin1,pattern);

/*   loading the mean and standard deviation   */
     loadfile_f1(fpin2);

/*   calculating the input membership of the test patterns */
     test_in_attr_mem(no_lns,pattern) ;

       
/*   testing/classifying the test set one by one into the dec tree */
     for(i=0;i<no_lns;i++)  {
        classifier(i,pattern,rootnode);
     }

/*   this part calcualates the actual percent of correct classification */
     for(i=0;i< NO_CLASS;i++) {
        ac_class[i]=0;
        cal_class[i]=0;
     }
    
     for(i=0;i<no_lns;i++) {
        ac_class[pattern[i]->lab_class]++;
        if(pattern[i]->class == pattern[i]->lab_class)
           cal_class[pattern[i]->class]++;
/* *******
        else  {
           fprintf(fpout1,"%3d  %3d  
",pattern[i]->class,pattern[i]->lab_class);
           for(j=0;j<NO_CLASS;j++)
              fprintf(fpout1,"%7.5f ",pattern[i]->inmem[j]);
        }
   ************** */

        else if(pattern[i]->lab_class==confidence(i,pattern)) 
           cal_class[confidence(i,pattern)]++;
        else ;

      }
/*    calculates total actual and calculated class  */
     for(i=0;i<NO_CLASS;i++) { 
         tot_ac_class+=ac_class[i];
         tot_cal_class+=cal_class[i];
     }
     for(i=0;i<NO_CLASS;i++)  
       for(j=0;j<NO_CLASS;j++) 
          conf_matrix[i][j]=0; 

     for(k=0;k<no_lns;k++)
       if(pattern[k]->class< NO_CLASS)
           conf_matrix[pattern[k]->lab_class][pattern[k]->class]++; 

     accuracy=0.;
     for(i=0;i<NO_CLASS;i++)  
       accuracy+=100.*(float)conf_matrix[i][i];
     accuracy/=(float)tot_ac_class;
     printf("ACCURACY %%: %5.2f\n",accuracy); 
     fprintf(fpout2,"%f  ",accuracy); 
 
     user_accuracy=0.;
     for(i=0;i<NO_CLASS;i++) {
       temp_user_accuracy=0.; 
       for(j=0;j<NO_CLASS;j++) 
          temp_user_accuracy+=(float)conf_matrix[i][j]; 
       
       if(temp_user_accuracy == 0.0)
          user_accuracy+=100.0;
       else
          user_accuracy+=100.0*(float)(conf_matrix[i][i]*ac_class[i])/temp_user_accuracy;
     }
     if(tot_ac_class ==0)
       user_accuracy = 100.0;
     else
       user_accuracy/=(float)tot_ac_class;
     printf("USER ACCURACY %% : %5.2f\n",user_accuracy);  
     fprintf(fpout2,"%f  ",user_accuracy); 


     kappa =0.;
     for(i=0;i<NO_CLASS;i++) {
        temp_user_accuracy=0.; 
        for(j=0;j<NO_CLASS;j++) 
          temp_user_accuracy+=(float)conf_matrix[i][j]; 
        
        temp_kappa=(float)(no_lns*conf_matrix[i][i]-ac_class[i]*(int)temp_user_accuracy);
        
        if(((float)(no_lns*(int)temp_user_accuracy-ac_class[i]*(int)temp_user_accuracy)) == 0.0)
           temp_kappa = 1.0;
        else
           temp_kappa/=(float)(no_lns*(int)temp_user_accuracy-ac_class[i]*(int)temp_user_accuracy);
        temp_kappa*=(float)ac_class[i];
        kappa+=temp_kappa;
     }
     if(tot_ac_class ==0)
       kappa = 1.0;
     else
       kappa/=(float)tot_ac_class;

     printf("KAPPA : %f\n",kappa);  
     fprintf(fpout2, "%f  ",kappa);  
  

/* calculating the confusion  */
     av_confusion=0.;
     confusion=0.;
     for(i=0;i<NO_CLASS;i++)  
       for(j=0;j<NO_CLASS;j++) 
          if(i!=j)
             av_confusion+=conf_matrix[i][j]; 

     av_confusion/= (float)(NO_CLASS*NO_CLASS-NO_CLASS);
     for(i=0;i<NO_CLASS;i++)  
       for(j=0;j<NO_CLASS;j++) 
          if(conf_matrix[i][j]>=av_confusion)
             confusion++;
   
     confusion/=(float)NO_CLASS; 
     printf("CONFUSION :  %f\n",confusion);  
     fprintf(fpout2,"%f  ",confusion);  
  

/****************************************************************************/
  /*    printf("actual class   correctly  calculated class\n"); 
     for(i=0;i<NO_CLASS;i++) 
       printf("   %d              %d\n",ac_class[i],cal_class[i]);  
       for(i=0;i<NO_CLASS;i++)  {
          for(j=0;j<NO_CLASS;j++) 
             printf("%5d ",conf_matrix[i][j]);
          printf("\n");
       }    */

  for(i=0;i<NO_CLASS;i++) 
     printf("CLASS %d = %5.2f\n",i+1,(float)cal_class[i]*100./(float)ac_class[i]);

  fclass = fopen("class_data.dat","a+");
  for(i=0;i<NO_CLASS;i++)  {
    if(ac_class[i]==0)
      temp = 100.0 ;
    else
      temp = (float)cal_class[i]*100./(float)ac_class[i];
    fprintf(fclass,"%5.2f  ",temp);
  }
  fprintf(fclass,"  %5.2f \n",(float)tot_cal_class*100./(float)tot_ac_class); 
  fclose(fclass);




  printf("TOTAL %f \n",(float)tot_cal_class*100./(float)tot_ac_class); 
  printf("#TREE NODES ::  DEFINITE : %d   CONFUSED : %d\n",leaf_node, conf_cls_node);  
  printf("TREE SIZE = %d\n",size_tree); 
 
/* for(i=0;i<NO_CLASS;i++) 
     printf("%5.2f ",(float)cal_class[i]*100./(float)ac_class[i]);
  printf("%f \n",(float)tot_cal_class*100./(float)tot_ac_class); 
  printf("%f ",(float)tot_cal_class*100./(float)tot_ac_class);  */
/****************************************************************************/
/* writing the input membership of the test patterns */
  for(i=0;i<no_lns;i++) {
  /*   printf("%d  %d\n",pattern[i]->lab_class,pattern[i]->class);  */
     fprintf(fpout1,"%d ",pattern[i]->lab_class);
     for(j=0;j<NO_ATRIB*3;j++) 
        fprintf(fpout1,"%f ",pattern[i]->inmem[j]);
     fprintf(fpout1,"\n");
  }

     for(i=0;i<NO_CLASS;i++)
        free(pattern[i]);
     free(pattern);
  /*   freenode(rootnode); */

     fclose(fpin1); 
     fclose(fpin2); 
     fclose(fpout1); 
     fclose(fpout2); 
     
     return;
}


/* freeing the memory held by the decision tree 
void freenode(NODEPTR rootnode) {
    if(rootnode ==NULL)
        return;
    freenode(rootnode->leftptr);
    freenode(rootnode->rightptr);
    free(rootnode);
}   */


/*  classifier funcion to classify the test  */
void classifier(int i,PATTERN_PTR pattern,NODEPTR root) {
        if(root==NULL) return;
        if((root->leftptr==NULL)&&(root->rightptr==NULL))  
            pattern[i]->class = root->class-1;
        

        if(pattern[i]->bin[root->atrib]<1)
          if(root->leftptr == NULL)
   	    classifier(i,pattern,root->rightptr);
          else  
	    classifier(i,pattern,root->leftptr); 
        else
           if(root->rightptr == NULL)
	    classifier(i,pattern,root->leftptr); 
          else   
   	    classifier(i,pattern,root->rightptr);
}


/* this function calculates the membership of the attributes */
 void test_in_attr_mem(int no_lns,PATTERN_PTR pattern)  {
	
   int i,j,k;
   float F_min[NO_ATRIB],F_max[NO_ATRIB]; 
   float min_low[NO_ATRIB],mid[NO_ATRIB],max_hig[NO_ATRIB];
   float mem_ship[NO_ATRIB][3];
   FILE *favg;
   favg = fopen("avg.dat","r");

/* calculating the average of Min/max value and lower/ higher quartile 
*/
   for(i=0;i<NO_ATRIB;i++) {
       fscanf(favg,"%f",&F_min[i]);
       fscanf(favg,"%f",&min_low[i]);
       fscanf(favg,"%f",&mid[i]);
       fscanf(favg,"%f",&max_hig[i]);
       fscanf(favg,"%f",&F_max[i]);
   }

/* calculating the membership values of all the attributes of all 
     the patterns   */
  for(i=0;i<no_lns;i++) {
    for(j=0;j<NO_ATRIB;j++) {
 /*  calculating the membership for low */	   	      
       if((pattern[i]->atrib[j])<=min_low[j])
          mem_ship[j][0]=1.;
       else if(pattern[i]->atrib[j]>min_low[j] && pattern[i]->atrib[j]<=mid[j]) 
         if((mid[j]-min_low[j])==0.0) 
            mem_ship[j][0]=1.0;
         else
            mem_ship[j][0]=1.-(pattern[i]->atrib[j]-min_low[j])/(mid[j]-min_low[j]);
       else
          mem_ship[j][0]=0.;
		                           
   /*  calculating the membership for medium */	   	      
       if(pattern[i]->atrib[j]>=min_low[j] && pattern[i]->atrib[j]<=mid[j])
         if((mid[j]-min_low[j])==0.0)
            mem_ship[j][1]= 0.0;
         else
            mem_ship[j][1]=(pattern[i]->atrib[j]-min_low[j])/(mid[j]-min_low[j]);
       else if(pattern[i]->atrib[j]>mid[j] && pattern[i]->atrib[j] <=max_hig[j])
          if((max_hig[j]-mid[j])==0.0)
             mem_ship[j][1]=1.0;
          else
              mem_ship[j][1]=1.-(pattern[i]->atrib[j]-mid[j])/(max_hig[j]-mid[j]);
       else
            mem_ship[j][1]=0.;
         
   /*  calculating the membership for high */	   	      
         if(pattern[i]->atrib[j] < mid[j])
      	    mem_ship[j][2]=0.;
         else if(pattern[i]->atrib[j]>=mid[j] && pattern[i]->atrib[j]<max_hig[j])
           if((max_hig[j]-F_min[j])==0.0)
              mem_ship[j][2]=0.0;
           else
              mem_ship[j][2]=(pattern[i]->atrib[j]-F_min[j])/(max_hig[j]-F_min[j]);
	 else
	    mem_ship[j][2] = 1.;  
      }
      
       for(j=0;j<NO_ATRIB;j++) 
	 for(k=0;k<3;k++) {
	    pattern[i]->inmem[3*j+k]=mem_ship[j][k];
            if(pattern[i]->inmem[3*j+k]>.5)
               pattern[i]->bin[3*j+k]=1;
            else
               pattern[i]->bin[3*j+k]=0;
         }
         

   }
   fclose(favg);
 }




/*  this function loads the pattern no , pattern class and the 
atributes */
void loadfile_f(FILE *fpin1,PATTERN_PTR pattern) {
   int i = 0,j; 
   while(!feof(fpin1)) {
     if(fscanf(fpin1,"%d ",&pattern[i]->lab_class) <=0) 
         break;
     for(j=0;j<NO_ATRIB;j++)
        fscanf(fpin1,"%f",&pattern[i]->atrib[j]);

     i++;
   }
   rewind(fpin1);
 }




#ifdef NO_CONFIDENCE
/* this is the calculatiion of a class by max confidence factor */
int  confidence(int i,PATTERN_PTR pattern)  {
     return(pattern[i]->class);
 }
#endif


#ifdef MAX_CONFIDENCE
/* this is the calculatiion of a class by max confidence factor */
int  confidence(int i,PATTERN_PTR pattern)  {
     float Zmem;
     int cal_class,ac_class = -1;
     int j,k;
     float Zmax=FLT_MIN;

     cal_class = pattern[i]->class;
/*  calculating the membership of different patterns to classes  */
     while(cal_class%10!=0) {
          k = cal_class%10 -1;
          Zmem=0.;
          for(j=0;j<NO_ATRIB;j++)
            Zmem+=pow((pattern[i]->atrib[j]-Mean[k][j])/Stdev[k][j],2);
          Zmem=sqrt(Zmem);
          Zmem/=Fd;
          Zmem= pow(Zmem,Fe);
          Zmem= 1./(1+Zmem);
          if(Zmax< Zmem) {
              Zmax=Zmem;
              ac_class=k;
          }
          cal_class = (int)(float)cal_class/10.;
      } 
      return(ac_class);
}
#endif

#ifdef PAW_CONFIDENCE
/* this is the calculation of the confidence by Mandel's method */
int confidence(int i,PATTERN_PTR pattern) {
     int j,k,l;
     struct confiden {
        int class;
        float mem;
     } ;
     struct confiden **conf,*temp;
     int cal_class ;
     float Zmem;
     float CF1=0.,CF2=0.;
     
     
    conf= (struct confiden **)malloc(NO_CLASS*sizeof(struct confiden 
*));
    for(j=0;j< NO_CLASS;j++) {
       conf[j]=(struct confiden *)malloc(sizeof(struct confiden));
       conf[j]->mem =0.;
       conf[j]->class=-1;
    }


    cal_class = pattern[i]->class;
/*  calculating the membership of different patterns to classes  */
     l=0;
     while(cal_class%10!=0) {
          k = cal_class%10 -1;
          Zmem=0.;
          for(j=0;j<NO_ATRIB;j++)
            Zmem+=pow((pattern[i]->atrib[j]-Mean[k][j])/Stdev[k][j],2);
          Zmem=sqrt(Zmem);
          Zmem/=Fd;
          Zmem= pow(Zmem,Fe);
          conf[l]->mem = 1./(1+Zmem);
          conf[l]->class = k;
          cal_class = (int)(float)cal_class/10.;
          l++;
     }

    /* sorting the membership values of a pattern */ 
     for(j=NO_CLASS-1;j>0;j--)
        for(l=0;l<j;l++)
           if(conf[l]->mem<conf[l+1]->mem) {
  	       temp=conf[l];
               conf[l]=conf[l+1];
               conf[l+1]=temp;
            }

   for(j=0;j<NO_CLASS;j++) {
        CF1+= (conf[0]->mem - pattern[i]->mem[j]);
        CF2+= (conf[1]->mem - pattern[i]->mem[j]);
    }
    CF1= conf[0]->mem + CF1/(float)(NO_CLASS -1);
    CF2= conf[1]->mem + CF2/(float)(NO_CLASS -1);
    CF1/=2.;
    CF2/=2.;   

   for(i=0;i<NO_CLASS;i++)
      free(conf[i]);
   free(conf);
 

   if(CF1 >= .8 && CF1 <= 1.0)
       return(conf[0]->class);
    else
       return(-1);

 }
#endif


/*  this function loads the  mean and standard deviation */
void loadfile_f1(FILE *fpin) {
   int i = 0,j,temp; 
   while(!feof(fpin)) {
     if(fscanf(fpin,"%d ",&temp) <=0) 
         break;
     for(j=0;j<NO_ATRIB;j++)
        fscanf(fpin,"%f",&Mean[i][j]);

     if(fscanf(fpin,"%d ",&temp) <=0) 
         break;
     for(j=0;j<NO_ATRIB;j++)
        fscanf(fpin,"%f",&Stdev[i][j]);
     i++;
   }
   rewind(fpin);
 }
     
