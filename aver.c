/* THIS PROGRAM CALCULATES THE AVERAGE OF COLUMNS OF DATA */
#include <stdio.h>
#include <stdlib.h>

main() {
  int col, row;
  double *columns;
  float value;
  int i,j;
  FILE *fp;
  char strn[30];
  

  printf("Enter the number of columns:");
  scanf("%d",&col);

  printf("Enter the number of rows/data sets:");
  scanf("%d",&row);

  printf("Enter the name of data file:");
  scanf("%s",strn);
  strn[strlen(strn)] = '\0';

  fp = fopen(strn,"r");  
  columns = (double *)malloc(col*sizeof(double));

  for(j=0;j<col;j++) 
     columns[j] = 0.0;

  for(i=0;i < row; i++) 
     for(j=0;j<col;j++) {
        fscanf(fp,"%f", &value);
        columns[j] += value;
     }


  for(j=0;j<col;j++) 
     columns[j] /= (float)row;

 
  for(j=0;j<col;j++)
   if(col == 5 || col == 1)
    printf("%5.2f & ", columns[j]);
   else
    printf("%5.1f & ", columns[j]);

  printf("\n");
  free(columns);
  fclose(fp);

  return(0);
}
