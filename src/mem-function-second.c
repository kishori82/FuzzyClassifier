/*  THIS PROGRAM CALCULATES THE INPUT MEMBERSHIP KISHORI'S METHOD  */

#include "header.h"
void second2() {
   int i,j,no_lns;
   float **F_array;
   int *class_arr,*patt_no;
   float *temp_array;
   float F_min[NO_ATRIB],F_max[NO_ATRIB];
   float low[NO_ATRIB],hig[NO_ATRIB];

  /* function definitions */ 
   void sort(float *,int);
   void fno_lns_b1(FILE *,int *);
   void loadfile_b1(FILE *, float **,int *,int *);
   void fmaxminval_b1(float *,float *, FILE *);
   void in_attr_mem(FILE *,int,int *, float **,float *,float *,float *, 
float *);

   FILE *fpin, *fpout;    /* fpin & fpout are the i/p and o/p files */

/* opening the files for input and output */
   fpin = fopen("train.dat","r");
   fpout = fopen("inmem.dat","w+");
   fno_lns_b1(fpin,&no_lns);
/* calculating max , min and means  */
   fmaxminval_b1(F_min,F_max,fpin);

/* calculating allocating memory */
/*   holds the pattern no */
   patt_no=(int *)malloc(no_lns*sizeof(int));
/* stores the class no */
   class_arr=(int *)malloc(no_lns*sizeof(int));
/*  stores the pointers to each pattern attributes */
   F_array = (float **)malloc(no_lns*sizeof(float *));
   for(i=0;i<no_lns;i++)  
      F_array[i]=(float *)malloc(NO_ATRIB*sizeof(float));

/*  temporary memory storage  */
   temp_array = (float *)malloc(no_lns*sizeof(float));

/*  loading the pattern no, class and the attributes */
   loadfile_b1(fpin, F_array,class_arr,patt_no);
 
/*  calculating the 1/3 rd quartile of the attributes   */ 
   for(j=0;j<NO_ATRIB;j++) {
      for(i=0;i<no_lns;i++)
        temp_array[i]=F_array[i][j];
      sort(temp_array,no_lns);     
      low[j]=(float)((temp_array[no_lns/3]+temp_array[no_lns/3+1])/2.);
      
hig[j]=(float)((temp_array[2*no_lns/3]+temp_array[2*no_lns/3+1])/2.);
      low[j]+=SHIFT;
      hig[j]+=SHIFT;
   }

/* this function calculates the membership of the attributes */
   in_attr_mem(fpout,no_lns,class_arr,F_array,F_min,low,hig,F_max);

   fclose(fpin);
   fclose(fpout);
   free(temp_array);
   free(class_arr);
   for(i=0;i<no_lns;i++)  
     free(F_array[i]);
   free(F_array);
   free(patt_no);
}



/* this function calculates the membership of the attributes */
 void in_attr_mem(FILE *fpout,int no_lns,int *class_arr, float 
**F_array,
	 float *F_min,float *low,float *hig,float *F_max)  {
   int i,j,k;
   float min_low[NO_ATRIB],mid[NO_ATRIB],max_hig[NO_ATRIB];
   float mem_ship[NO_ATRIB][3];
   FILE *favg;
   favg = fopen("avg.dat","w");

/* calculating the average of Min/max value and lower/ higher quartile 
*/
   for(i=0;i<NO_ATRIB;i++) {
       min_low[i] = (float)((F_min[i]+low[i])/2.);
       mid[i] = (float)((low[i]+hig[i])/2.);
       max_hig[i] = (float)((hig[i]+F_max[i])/2.);
   }

/* calculating the membership values of all the attributes of all 
     the patterns   */
   for(i=0;i<no_lns;i++) {
      for(j=0;j<NO_ATRIB;j++) {
   /*  calculating the membership for low */	   	      
         if(F_array[i][j]<=min_low[j])
            mem_ship[j][0]=1.;
         else if(F_array[i][j]> min_low[j] && F_array[i][j]<=mid[j]) 
           if((mid[j]-min_low[j]) == 0.)
              mem_ship[j][0]=1.0;
           else
              mem_ship[j][0]=1.-(F_array[i][j]-min_low[j])/(mid[j]-min_low[j]);
         else
           mem_ship[j][0]=0.;
		                           
   /*  calculating the membership for medium */	   	      
         if(F_array[i][j]>=min_low[j] && F_array[i][j]<=mid[j])
           if((mid[j]-min_low[j]) == 0.)
              mem_ship[j][1]= 0.0;
           else 
              mem_ship[j][1]=(F_array[i][j]-min_low[j])/(mid[j]-min_low[j]);
         else if(F_array[i][j]>mid[j] && F_array[i][j] <=max_hig[j])
           if((max_hig[j]-mid[j]) == 0.)
              mem_ship[j][1]=1.0;
           else
              mem_ship[j][1]=1.-(F_array[i][j]-mid[j])/(max_hig[j]-mid[j]);
	 else
            mem_ship[j][1]=0.;
         
   /*  calculating the membership for high */	   	      
         if(F_array[i][j] < mid[j])
      	    mem_ship[j][2]=0.;
         else if(F_array[i][j] >=mid[j] && F_array[i][j]< max_hig[j])
           if((max_hig[j]-F_min[j]) == 0.)
      	     mem_ship[j][2]=0.;
           else
             mem_ship[j][2]=(F_array[i][j]-F_min[j])/(max_hig[j]-F_min[j]);
	 else
	    mem_ship[j][2] = 1.;  


      }
      fprintf(fpout,"%d ",class_arr[i]);
      for(j=0;j<NO_ATRIB;j++) {
	 for(k=0;k<3;k++) 
	   fprintf(fpout,"%6.4f ",mem_ship[j][k]);
      }
	 fprintf(fpout,"\n"); 
         
   }
   for(i=0;i<NO_ATRIB;i++)
      fprintf(favg,"%f %f %f %f %f\n",
      F_min[i],min_low[i],mid[i],max_hig[i],F_max[i]);
   rewind(fpout);
   fclose(favg);
 }


 void sort(float *temp_array,int no_lns) {
   int i,j;
   float temp;
   for(j=no_lns-1;j>0;j--)
      for(i=0;i<j;i++)
        if(temp_array[i]>temp_array[i+1]) {
	  temp=temp_array[i];
          temp_array[i]=temp_array[i+1];
          temp_array[i+1]=temp;
        }
 }


/*  this function loads the pattern no , pattern class and the 
atributes */
void loadfile_b1(FILE *fpin,float **f_array,int *class_arr,int 
*patt_no) {
   int i = 0,j; 
   while(!feof(fpin)) {
     if( fscanf(fpin,"%d ",&class_arr[i]) <=0) 
         break;;
     for(j=0;j< NO_ATRIB;j++)
        fscanf(fpin,"%f",&f_array[i][j]);
     i++;
   }
   rewind(fpin);
 }
     


void fno_lns_b1(FILE *fpin,int *no_lns) {
    *no_lns = 0;
    do {
       if(fgetc(fpin)=='\n')
          (*no_lns)++;
    } while(!feof(fpin));
    rewind(fpin);
}

void fmaxminval_b1(float *min,float *max,FILE *fin) {
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

   for(i=0;i<3;i++) {
     min[i]+=SHIFT;
     max[i]+=SHIFT;
   }
   rewind(fin);
  } 
   
