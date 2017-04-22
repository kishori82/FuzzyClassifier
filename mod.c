#include <stdlib.h>
#include <stdio.h>

main() {
 int  i,arr[11];
 FILE *fp;
 char c,class;

 fp = fopen("balance.txt","r");

for(i=0;i< 625;i++)
{
fscanf(fp,"%d  %d  %d  %d  %d",  &arr[0], &arr[1], &arr[2], &arr[3], &arr[4]);
/* printf("%c  %c %d %c %d %c %d %c %d\n", class, c, arr[0], c ,arr[1], c,arr[2],c,arr[3]);
*/

printf("%d  %d  %d  %d  %d\n", arr[0],arr[1], arr[2], arr[3], arr[4]);

}

fclose(fp);

}
