/*       THIS PROGRAM CALCULATES THE INPUT MEMBERSHIP OF THE TRAIN SET 
         USING PAWAN'S METHOD OF CALCULATING THE INPUT MEMBERSHIP    */

#include "header.h"

int second1() {
   int i,j,k,no_lns,low,hig;
   float **F_array;
   int *class_arr,*patt_no;
   float F_min[NO_ATRIB],F_max[NO_ATRIB];
   float Mean[NO_ATRIB][3];
   float Lambda[NO_ATRIB][3];
   float mem_ship[NO_ATRIB][3];

  /* function definitions */ 
   void fno_lns(FILE *,int *);
   void loadfile(FILE *, float **,int *,int *);
   void fmaxminval(float *,float *, FILE *);

   FILE *fpin, *fpout;    /* fpin & fpout are the i/p and o/p files */

/* opening the files for input and output */
   fpin = fopen("train.dat","r");
   fpout = fopen("inmem.dat","w+");
   fno_lns(fpin,&no_lns);

/* calculating max , min and means  */
   fmaxminval(F_min,F_max,fpin);

/* calculating allocating memory */
/*   holds the pattern no */
   patt_no=(int *)malloc(no_lns*sizeof(int));
/* stores the class no */
   class_arr=(int *)malloc(no_lns*sizeof(int));
/*  stores the pointers to each pattern attributes */
   F_array = (float **)malloc(no_lns*sizeof(float *));
   for(i=0;i<no_lns;i++)  
      F_array[i]=(float *)malloc(NO_ATRIB*sizeof(float));


/*  loading the pattern no, class and the attributes */
   loadfile(fpin, F_array,class_arr,patt_no);
 
/*  calculating the mean of the attributes   */ 
   for(j=0;j<NO_ATRIB;j++) 
      for(i=0;i<3;i++)
        Mean[j][i]=0.;
   for(j=0;j<NO_ATRIB;j++) 
      for(i=0;i<no_lns;i++)
        Mean[j][1]+=F_array[i][j]/(float)no_lns;

/*  calculating the lower and upper means of the attributes   */ 
   for(j=0;j<NO_ATRIB;j++)  { 
   low =0;
   hig =0;
      for(i=0;i<no_lns;i++) 
         if(F_array[i][j]<Mean[j][1]) {
            Mean[j][0]+=F_array[i][j];
            low++;
         }
         else {
            Mean[j][2]+=F_array[i][j];
            hig++;
         }
       
      Mean[j][0]/=(float)low;
      Mean[j][2]/=(float)hig;
   }

/*   calculating the lambda radii   */
   for(j=0;j<NO_ATRIB;j++) {
      Lambda[j][0] = 2.*(Mean[j][1] - Mean[j][0]);
      Lambda[j][2] = 2.*(Mean[j][2] - Mean[j][1]);
      Lambda[j][1] = Lambda[j][0]*(F_max[j]-Mean[j][1]);
      Lambda[j][1]+= Lambda[j][2]*(Mean[j][1]-F_min[j]);
      Lambda[j][1]/= (F_max[j]-F_min[j]);
   }

/* calculation of the PI function for all the patterns  */
   for(i=0;i<no_lns;i++)  {
      fprintf(fpout,"%d ",class_arr[i]);
      for(j=0;j<NO_ATRIB;j++) 
         for(k=0;k<3;k++) {
            if(Lambda[j][k]/2.<=fabs(F_array[i][j]-Mean[j][k])  &&
            fabs(F_array[i][j]-Mean[j][k]) < Lambda[j][k]) {
               
mem_ship[j][k]=1.-fabs(F_array[i][j]-Mean[j][k])/Lambda[j][k];
               mem_ship[j][k]*=2.*mem_ship[j][k];  /* squaring */
            }
            else if(0.<=fabs(F_array[i][j]-Mean[j][k])  &&
            fabs(F_array[i][j]-Mean[j][k])<Lambda[j][k]/2.) {
               
mem_ship[j][k]=pow(fabs(F_array[i][j]-Mean[j][k])/Lambda[j][k],2);
               mem_ship[j][k]=1.-2.*mem_ship[j][k];  
            }
            else
               mem_ship[j][k]=0.;
            fprintf(fpout,"%6.4f ",mem_ship[j][k]);
         }
      fprintf(fpout,"\n");
   } 


                
                    
/* this function calculates the membership of the attributes 
  in_attr_mem(fpout,no_lns,class_arr,F_array,F_min,low,hig,F_max);  */

   fclose(fpin);
   fclose(fpout);
   free(class_arr);
   for(i=0;i<no_lns;i++)  
     free(F_array[i]);
   free(F_array);
   free(patt_no);

   return(0);
}


/*  this function loads the pattern no , pattern class and the 
atributes */
void loadfile(FILE *fpin,float **f_array,int *class_arr,int *patt_no) {
   int i = 0; 
   while(!feof(fpin)) {
     if( fscanf(fpin,"%d ",&class_arr[i]) <=0) 
         break;;
     fscanf(fpin,"%f %f %f",&f_array[i][0],&f_array[i][1],&f_array[i][2]);
     i++;
   }
   rewind(fpin);
 }
     


void fno_lns(FILE *fpin,int *no_lns) {
    *no_lns = 0;
    do {
       if(fgetc(fpin)=='\n')
          (*no_lns)++;
    } while(!feof(fpin));
    rewind(fpin);
}

void fmaxminval(float *min,float *max,FILE *fin) {
   int i,tmp;
   float F[3];
   fscanf(fin,"%f",&min[0]);
   for(i=0;i<NO_ATRIB;i++)
      fscanf(fin,"%f ",&min[i]);
   for(i=0;i<NO_ATRIB;i++)
      max[i] = min[i];
   do  {
     fscanf(fin,"%d %f %f %f\n",&tmp,&F[0],&F[1],&F[2]);
     for(i=0;i<3;i++) {
        if(F[i]< min[i]) 
          min[i] = F[i];
          
        if(F[i]> max[i]) 
          max[i] = F[i];
     } 
   } while(!feof(fin));
   rewind(fin);
  }
 
   
