/*  CALCULATING THE MEMBERSHIP OF THE TRAINING PATTERNS TO DIFFERENT
    CLASSES  AND THE MEANS ASSOCIATED WITH THEM */          

#include "header.h"

struct pattern {
     int class;
     float attrib[NO_ATRIB];
 };

void fourth() {
     FILE *fpin,*fpout1,*fpout2;
     int i,j,k;
     int no_lns;
     struct pattern *F_patts;
     float Mean[NO_CLASS][NO_ATRIB],Stdev[NO_CLASS][NO_ATRIB];
     float Zmem;
     int class_size[NO_CLASS];
   
     void fno_lns(FILE *,int *);
     void loadfile_d(FILE *,struct pattern *);

 /* opening the files for input and output */
     fpin = fopen("train.dat","r");
     fpout1 = fopen("mnstd.dat","w");
     fpout2 = fopen("mem.dat","w");
     fno_lns(fpin,&no_lns);

/*  allocating memory for the pointers to the structures */
    F_patts=(struct pattern *)malloc(no_lns*sizeof(struct pattern)); 

/*  loading the pattern no, class and the attributes  */
    loadfile_d(fpin,F_patts); 


/* calculating the means for different classes */    
    for(i=0;i<NO_CLASS;i++) {
       for(j=0;j<NO_ATRIB;j++){
          Mean[i][j]=0.;
          Stdev[i][j]=0.;
       }
       class_size[i]=0;
    }
    for(i=0;i<no_lns;i++) {
       for(j=0;j<NO_ATRIB;j++) 
          Mean[F_patts[i].class][j]+=F_patts[i].attrib[j];
       class_size[F_patts[i].class]++;
    }
    for(i=0;i<NO_CLASS;i++) 
       for(j=0;j<NO_ATRIB;j++) 
          if(class_size[i] ==0)
            Mean[i][j] =0.0;
          else
            Mean[i][j]/=(float)class_size[i];


/* calculating the standard deviation for different classes */    
    for(i=0;i<no_lns;i++) 
      for(j=0;j<NO_ATRIB;j++) 
        Stdev[F_patts[i].class][j]+=pow((Mean[F_patts[i].class][j]
                                    -F_patts[i].attrib[j]),2);
    for(i=0;i<NO_CLASS;i++) 
       for(j=0;j<NO_ATRIB;j++) {
           if(class_size[i] ==0)  
              Stdev[i][j] =0.0;
           else
              Stdev[i][j]/=(float)class_size[i];
           Stdev[i][j]=sqrt(Stdev[i][j]);
       }

/* writing the mean and standard deviation */
    for(i=0;i<NO_CLASS;i++) {
       fprintf(fpout1,"\n%d ",i);
       for(j=0;j<NO_ATRIB;j++) 
          fprintf(fpout1,"%f ",Mean[i][j]);
       fprintf(fpout1,"\n%d ",i);
       for(j=0;j<NO_ATRIB;j++) 
          fprintf(fpout1,"%f ",Stdev[i][j]);
    }

/*  calculating the membership of different patterns to classes  */
    for(i=0;i<no_lns;i++) {
       fprintf(fpout2,"\n%d ",F_patts[i].class); 
       for(k=0;k<NO_CLASS;k++) {
          Zmem=0.;
          for(j=0;j<NO_ATRIB;j++){
            if(Stdev[k][j]==0.0)
               Stdev[k][j]=1.;
            Zmem+=pow((F_patts[i].attrib[j]-Mean[k][j])/Stdev[k][j],2);
          }
          Zmem=sqrt(Zmem);
          Zmem/=Fd;
          Zmem= pow(Zmem,Fe);
          Zmem= 1./(1+Zmem);
          fprintf(fpout2,"%f ",Zmem); 
       }
    }

     free(F_patts); 
     fclose(fpin);
     fclose(fpout1); 
     fclose(fpout2); 

     return;
}



/*  this function loads the  pattern class and the atributes */
void loadfile_d(FILE *fpin,struct pattern *F_patts) {
   int i = 0,j; 
   while(!feof(fpin)) {
     if(fscanf(fpin,"%d ",&F_patts[i].class) <=0) 
         break;
     for(j=0;j<NO_ATRIB;j++)
        fscanf(fpin,"%f",&F_patts[i].attrib[j]);
     i++;
   }
   rewind(fpin);
 }
     

/*   counts the no lines in the input file for training set */
/*void fno_lns(FILE *fpin,int *no_lns) {
    *no_lns = 0;
    do {
       if(fgetc(fpin)=='\n')
          (*no_lns)++;
    } while(!feof(fpin));
    rewind(fpin);
}  */
