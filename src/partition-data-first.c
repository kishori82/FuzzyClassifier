/* THIS IS THE 1ST FILE IN THE PROJECT USED TO PARTITION THE DATA
   INTO TEST SET AND TRAINING SET    */
#include "header.h"
int *class_no,**F_class;
int no_lns;
extern float sample_percent;
int first() {
	FILE *fpin,*fpout1,*fpout2;
	int i,j;
        /*time_t t; */
	int train_size,real_size;
	int *rand_array;
        float sample_per;
	int get_lines(FILE *);
	void read_dat(FILE *);
        void rsort(int *,int,int);
        void randomise(int *,int);

	fpin=fopen(DATA_FILE,"r");
        fpout1=fopen("train.dat","w");
	fpout2=fopen("test.dat","w");


/*        printf("Enter sample percent:");
        fflush(stdin); 
        scanf("%f",&sample_per); */

        sample_per =SAMPLE_PERCENT;
#ifdef  AUTO_SAMPLE_PERCENT
        sample_per =sample_percent;
#endif

	no_lns=get_lines(fpin);
        train_size = (int)(sample_per*no_lns/100.);

        rand_array = (int *)malloc(no_lns*sizeof(int));
	class_no = (int *)malloc(no_lns*sizeof(int));
        F_class=(int **)malloc(no_lns*sizeof(int *));
	for(i=0;i<no_lns;i++)
	   F_class[i]=(int *)malloc(NO_ATRIB*sizeof(int));

/*  reads the file vowel in to the variables   */
        read_dat(fpin);

/*   determining randomly the patterns to be the selected  as test and 
train set */
	for(i=0;i<no_lns;i++)
   	   rand_array[i]=i;
        for(i=0;i<no_lns;i++)
           if(rand_array[i] >= no_lns || rand_array[i] < 0) 
               printf("%d \n",rand_array[i]);
        randomise(rand_array,no_lns);
        rsort(rand_array,0,train_size-1);   
    
	real_size = 0;
	for(i=0;i<no_lns;i++) 
	   if(F_class[rand_array[i]]!=NULL) {
	      fprintf(fpout1,"%d ",class_no[rand_array[i]]-1);
              for(j=0;j<NO_ATRIB;j++) 
 	         fprintf(fpout1,"%d ",F_class[rand_array[i]][j]);
	      fprintf(fpout1,"\n");
              F_class[rand_array[i]]=NULL;
	      real_size++;
              if(real_size==train_size) break;
           }

	for(i=0;i<no_lns;i++)  
           if(F_class[i]!=NULL) {
	      fprintf(fpout2,"%d ",class_no[i]-1);
              for(j=0;j<NO_ATRIB;j++) 
 	         fprintf(fpout2,"%d ",F_class[i][j]);
	      fprintf(fpout2,"\n");
           }
             

	for(i=0;i<no_lns;i++)
	   free(F_class[i]);
	free(F_class);
	free(rand_array);
	free(class_no);
	fclose(fpin);
	fclose(fpout1);
	fclose(fpout2);
	return(0);

}
void randomise(int *rand_array,int no_lns) {
       int i;
       int a,b;
       void swap(int *,int,int);
       srand(time(0));
       
       for(i=0;i<9*no_lns;i++)  {
   	  a=(int)((float)no_lns*(float)rand()/(float)RAND_MAX);
   	  b=(int)((float)no_lns*(float)rand()/(float)RAND_MAX);
          if(a >= no_lns || b >=no_lns)
            continue; 
          swap(rand_array,a,b); 
       } 
 }
       
void read_dat(FILE *fpin) {
	int i,lines;
        lines=0;
	do {
           fscanf(fpin,"%d ",&class_no[lines]);
	   for(i=0;i<NO_ATRIB;i++)
	      fscanf(fpin,"%d ",&F_class[lines][i]);
	   lines++;
	}while(!feof(fpin));
	rewind(fpin);
}

int get_lines(FILE *fpin) {
	int no_lns = 0;
	do {
	   if(fgetc(fpin)=='\n')
	     no_lns++;
	}while(!feof(fpin));
	rewind(fpin);
	return(no_lns);
}


/*
void rsort(int *rand_array,int left,int right) {
        int i,j;
        void swap(int *,int,int);
    
        for(i=right+1;i>left;i--)
          for(j=left;j<i-1;j++)
             if(rand_array[j]>rand_array[j+1])
                swap(rand_array,j,j+1);
  }   
*/


void swap(int *array,int i,int j) {
        int temp;
        
         temp=array[i];
         array[i]=array[j];
         array[j]=temp;
  }

void rsort(int *rand_array,int left,int right) {
        int i,last;
        void swap(int *,int,int);
        
        if(left>=right)
           return;
        swap(rand_array,left,(left+right)/2);
        last=left;
        for(i=left+1;i<=right;i++)
           if(rand_array[i]<rand_array[left])
              swap(rand_array,++last,i);
        swap(rand_array,left,last);
        rsort(rand_array,left,last-1);
        rsort(rand_array,last+1,right);
  }   
