 /*   CREATION OF THE FUZZY ID3 DECISION TREE 
      THIS PROGRAM REMOVES THE REDUNDANT NODES  OF THE DECISION 
      TREE WITH NO PATTERNS  OR LESS THAN 90 PERCENT OF THE TOTAL IN 
      THAT NODE USE FUZZY ENTROPY                               
*/

#include "header.h"

PATTPTR  pattern;
NODEPTR rootnode;
int leaf_node=0;
int conf_cls_node=0;

FILE *fpout,*fpout1,*fpout2;
int image[ROW][COL];
int image1[ROW][COL];
char c=0;
int size_tree=0;
int no_lns;
float T_meas=0;
int fifth() {
     FILE *fpin1,*fpin2,*fpin3,*fpin4;
     int i,j;
     int no_lns;

     NODEPTR getnode(); 
     void maketree(NODEPTR); 
     void fno_lns(FILE *,int *);
     void loadfile_e(FILE *,FILE *,FILE *,PATTPTR);
     void traverse(int,int,int,NODEPTR,int,int,int,int,int);
     void travel(NODEPTR);
     void print_tree();

/* initialise stack to empty  */
     stack.top= 0;
    leaf_node=0;
    conf_cls_node=0;

 /* opening the files for input and output */
     fpin1 = fopen("mem.dat","r");
     fpin2 = fopen("bin.dat","r");
     fpin3 = fopen("inmem.dat","r");
     fpin4= fopen("tree.dat","a+");
     fpout1= fopen("travel.dat","w");
     fno_lns(fpin1,&no_lns);

/*  allocating memory for the pointers to the structures */
     pattern = (PATTPTR)malloc(no_lns*sizeof(struct pattern_pts *));
     for(i=0;i<no_lns;i++)
        pattern[i]=(struct pattern_pts *)malloc(sizeof(struct 
pattern_pts)); 

/*  loading the pattern no, class and the attributes  */
    loadfile_e(fpin1,fpin2,fpin3,pattern); 
    
/*  makes the decision tree  */
    rootnode =getnode(); 
    size_tree=0;
    rootnode->left = 0;
    rootnode->right = no_lns;
/*  for(i=0;i<no_lns;i++)
     printf("%d is ord no    %d\n",i,pattern[i]->bin[0]);
   printf("before the beginning of this\n");
  fflush(stdin);getchar();  
*/ 
    T_meas=0.;
    maketree(rootnode); 
    T_meas/=(float)no_lns;
    T_meas = (6.*(float)NO_ATRIB - T_meas)/(6.*(float)NO_ATRIB-1.);
   
    for(i=0;i<ROW;i++)
      for(j=0;j<72;j++) {
        image[i][j]=0;
        image1[i][j]=0;
      }
    traverse(0,ROW,5,rootnode,ROW,5,0,INIT,0); 
    print_tree();
    travel(rootnode);
    printf("TREE MEASURE = %f\n",T_meas); 
    fprintf(fpin4,"%f\n",T_meas); 

/* freeing the momory occupied by train set  */
     for(i=0;i<no_lns;i++) 
        free(pattern[i]);
     free(pattern); 

/* closing the files   */
     fclose(fpin1);
     fclose(fpin2);
     fclose(fpin3);
     fclose(fpin4);
     fclose(fpout1);


     return(0);
}



/*  this function makes the decision tree */
void maketree(NODEPTR root) {
    NODEPTR getnode(); 
    int min_entropy_atr(NODEPTR,int,int);
    int min_atr=0;
    void sort_e(int,int,int);
    int split(int,int) ;
    int class(int,int);
    
    if(stack.top>=NO_ATRIB*3) {
       root->class= class(root->left,root->right);
       if(root->class <10 && root->class >0) {
          leaf_node++;
          T_meas+= (float)(root->right-root->left)*(float)stack.top;
       }
       else if(root->class >10) {
          conf_cls_node++;
          T_meas+= (float)(root->right-root->left)*(float)stack.top;
       }
       return; 
    }
    if(root!=NULL) {
      min_atr=min_entropy_atr(root,root->left,root->right);
       root->atrib=min_atr; 
    }

    
    sort_e(min_atr,root->left,root->right);

    for(root->mid=root->left;root->mid<root->right;root->mid++) 
      if(pattern[root->mid]->bin[min_atr]==1) {
         break;
    }
   

    push(min_atr);
#ifdef PRUNING
    if(stack.top<= NO_ATRIB*3 && split(root->left,root->right)) {
#endif
#ifdef NO_PRUNING
    if(stack.top<=NO_ATRIB*3) {
#endif
       if(root->left != root->mid) {  
          root->leftptr=getnode();
          root->leftptr->left = root->left;
          root->leftptr->right = root->mid;
          maketree(root->leftptr);
       }
       if(root->mid != root->right) {  
          root->rightptr=getnode();
          root->rightptr->left = root->mid;
          root->rightptr->right = root->right;
          maketree(root->rightptr);
       }   
    }
    else {
       root->class= class(root->left,root->right);
       if(root->class <10 && root->class >0) {
          leaf_node++;
          T_meas+= (float)(root->right-root->left)*(float)stack.top;
       }
       else if(root->class >10) {
          conf_cls_node++;
          T_meas+= (float)(root->right-root->left)*(float)stack.top;
       }
    }
    pop(); 
    return;
        
}

/* returns the class of a leaf node  */
int class(int left,int right) {
    int class_size[NO_CLASS];
    int i,temp=0;
   
    for(i=0;i<NO_CLASS;i++) 
       class_size[i]=0; 

    for(i=left;i<right;i++) 
       class_size[pattern[i]->class]++; 

#ifdef PRUNING   
    for(i=0;i<NO_CLASS;i++)
       if(class_size[i] > (int)(THRES*(float)(right-left)))
           return(i+1);
#endif 

#ifdef NO_PRUNING   
    for(i=0;i<NO_CLASS;i++)
       if(class_size[i] == right-left)
           return(i+1);
#endif 
    
    for(i=0;i<NO_CLASS;i++)
       if(class_size[i] !=0)
          temp=temp*10+i+1;
    return(temp);
 } 
    

/*  returns 0 if the node should NOT be split otherwise 1 */
int split(int left,int right) {
    int class_size[NO_CLASS];
    int i;
   
    for(i=0;i<NO_CLASS;i++) 
       class_size[i]=0; 

    for(i=left;i<right;i++) 
       class_size[pattern[i]->class]++; 
   
    for(i=0;i<NO_CLASS;i++)
       if(class_size[i] > (int)(THRES*(float)(right-left)))
           return(0); 

    return(1);   
 }

/* this function return the attribute with which entropy is minimum */
int min_entropy_atr(NODEPTR root,int left,int right) {
    int i,j;
    int left_class,right_class;
    int left_freq[NO_CLASS],right_freq[NO_CLASS];
    int unex_attributes[NO_ATRIB*3];
    int min_class=9000;
    float min_entropy= FLT_MAX,new_entropy;
    
    float entropy(int,int,int,int *,int *,int,int);

/* determining the attributes to be explored to calculate entropy */
    for(i=0;i<NO_ATRIB*3;i++)
       unex_attributes[i]=i;

    for(i=0;i<stack.top;i++) 
       unex_attributes[stack.arr[i]] = -1; 

    for(i=0;i<NO_ATRIB*3;i++) {
       if(unex_attributes[i]!=-1)  {
          left_class=0;
          right_class=0;

          for(j=0;j<NO_CLASS;j++) {
             left_freq[j]=0;
             right_freq[j]=0.;
          }

          for(j=left;j<right;j++)  
             if( pattern[j]->bin[i]<1) {
                left_class++;
                left_freq[pattern[j]->class]++;
             }
             else {
                right_class++;
                right_freq[pattern[j]->class]++;
             }
          
          
new_entropy=entropy(i,left_class,right_class,left_freq,right_freq,left,right);
          if(min_entropy > new_entropy) {
             min_entropy=new_entropy;
             min_class= i;
          }
       }
    }
    return(min_class);
 }

#ifdef ONLY_FUZZY_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float left_fuzz=0.,right_fuzz=0.;
    float tot_entropy;
    float pi_left,pi_right;
    float cl_right_fuzz,cl_left_fuzz;
    int i,j;
    
    cl_right_fuzz=0.;
    cl_left_fuzz=0.;
    for(j=0;j<NO_CLASS;j++) {
      for(i=left;i<right;i++)
         if(pattern[i]->bin[atrib]<1) 
            cl_left_fuzz+= pattern[i]->mem[j];
         else 
            cl_right_fuzz+= pattern[i]->mem[j];
         
         if(cl_left_fuzz > .005)
            left_fuzz+= cl_left_fuzz*log(cl_left_fuzz);
         if(cl_right_fuzz > .005)
            right_fuzz+= cl_right_fuzz*log(cl_right_fuzz);
    }
    left_fuzz/= (float)(right-left);
    right_fuzz/= (float)(right-left);

/*
    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1) 
                   left_fuzz+= pattern[i]->mem[j];
            else 
                   right_fuzz+=pattern[i]->mem[j];
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;
*/
    left_entropy = left_fuzz; 
    right_entropy = right_fuzz;  
    
tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
}
#endif


#ifdef NEW_FUZZY_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float left_fuzz=0.,right_fuzz=0.;
    float tot_entropy;
    int tot_pat;
    float pi_left,pi_right;
    float cl_right_fuzz,cl_left_fuzz;
    int i,j;
    
    tot_pat=left_class+right_class;
    for(i=0;i<NO_CLASS;i++) {
      pi_left=(double)left_freq[i]/(double)tot_pat;
      pi_right = 1.-pi_left;
      if(pi_left>.0005)
         left_entropy-=pi_left*log10((double)pi_left);
      if(pi_right>.0005)
         right_entropy-=pi_right*log10((double)pi_right);
    }

    
    cl_right_fuzz=0.;
    cl_left_fuzz=0.;
    for(j=0;j<NO_CLASS;j++) {
      for(i=left;i<right;i++)
         if(pattern[i]->bin[atrib]<1) 
            cl_left_fuzz+= pattern[i]->mem[j];
         else 
            cl_right_fuzz+= pattern[i]->mem[j];
         
         if(cl_left_fuzz > .005)
            left_fuzz+= cl_left_fuzz*log(cl_left_fuzz);
         if(cl_right_fuzz > .005)
            right_fuzz+= cl_right_fuzz*log(cl_right_fuzz);
    }
    left_fuzz/= (float)(right-left);
    right_fuzz/= (float)(right-left);
/*    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1) 
                   left_fuzz+= pattern[i]->mem[j];
            else 
                   right_fuzz+=pattern[i]->mem[j];
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;
*/
    left_entropy += left_fuzz; 
    right_entropy += right_fuzz;  
    
tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
}
#endif




#ifdef PS_FUZZY_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float left_fuzz=0.,right_fuzz=0.;
    float tot_entropy;
    int tot_pat;
    float pi_left,pi_right;
    int i,j;

    for(i=0;i<NO_CLASS;i++) {
      pi_left=(double)left_freq[i]/(double)tot_pat;
      pi_right = 1.-pi_left;
      if(pi_left>.0005)
         left_entropy-=pi_left*log10((double)pi_left);
      if(pi_right>.0005)
         right_entropy-=pi_right*log10((double)pi_right);
    }

    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1) 
                if(pattern[i]->mem[j]<(1.-pattern[i]->mem[j]))
                   left_fuzz+= pattern[i]->mem[j];
                else
                   left_fuzz+=1.- pattern[i]->mem[j];
            
            else 
                if(pattern[i]->mem[j]<(1.-pattern[i]->mem[j]))
                   right_fuzz+= pattern[i]->mem[j];
                else
                   right_fuzz+= 1.-pattern[i]->mem[j];
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;

    left_entropy += left_fuzz; 
    right_entropy += right_fuzz;  
    
tot_entropy+=(double)left_class*left_entropy+(double)right_class*right_entropy;

    return(tot_entropy);
}
#endif

#ifdef TWO_FUZZY_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float left_fuzz=0.,right_fuzz=0.;
    float tot_entropy;
    int tot_pat;
    float pi_left,pi_right;
    int i,j;
    float cl_right_fuzz=0.;
    float cl_left_fuzz=0.;
    

    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1) 
                   left_fuzz+= pattern[i]->mem[j];
            else 
                   right_fuzz+=pattern[i]->mem[j];
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;
    left_entropy = left_fuzz; 
    right_entropy = right_fuzz;  

    left_fuzz=0.;
    right_fuzz=0.;
    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1)  {
              if(pattern[i]->mem[j] >=0.0005 && 
pattern[i]->mem[j]<=.9995) {
                left_fuzz-= 
pattern[i]->mem[j]*log10(pattern[i]->mem[j]);
                left_fuzz-= 
(1.-pattern[i]->mem[j])*log10((1.-pattern[i]->mem[j]));
               }
            }
            else {
              if(pattern[i]->mem[j] >=0.0005 && 
pattern[i]->mem[j]<=.9995) {
                right_fuzz-= 
pattern[i]->mem[j]*log10(pattern[i]->mem[j]);
                right_fuzz-= 
(1.-pattern[i]->mem[j])*log10((1.-pattern[i]->mem[j]));
               }
            }
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;

    left_entropy+= left_fuzz; 
    right_entropy+= right_fuzz;  
    
    cl_right_fuzz=0.;
    cl_left_fuzz=0.;
    right_fuzz=0.;
    left_fuzz=0.;
    for(j=0;j<NO_CLASS;j++) {
      for(i=left;i<right;i++)
         if(pattern[i]->bin[atrib]<1) 
            cl_left_fuzz+= pattern[i]->mem[j];
         else 
            cl_right_fuzz+= pattern[i]->mem[j];
         
         if(cl_left_fuzz > .005)
            left_fuzz+= cl_left_fuzz*log(cl_left_fuzz);
         if(cl_right_fuzz > .005)
            right_fuzz+= cl_right_fuzz*log(cl_right_fuzz);
    }
    left_fuzz/= (float)(right-left);
    right_fuzz/= (float)(right-left);
    left_entropy += left_fuzz; 
    right_entropy += right_fuzz;  
    
tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
}

#endif

#ifdef MY_FUZZY_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float left_fuzz=0.,right_fuzz=0.;
    float tot_entropy;
    int tot_pat;
    float pi_left,pi_right;
    int i,j;
    
    tot_pat=left_class+right_class;
    for(i=0;i<NO_CLASS;i++) {
      pi_left=(double)left_freq[i]/(double)tot_pat;
      pi_right = 1.-pi_left;
      if(pi_left>.0005)
         left_entropy-=pi_left*log10((double)pi_left);
      if(pi_right>.0005)
         right_entropy-=pi_right*log10((double)pi_right);
    }

    for(i=left;i<right;i++)  {
         for(j=0;j<NO_CLASS;j++)  {
            if(pattern[i]->bin[atrib]<1)  {
              if(pattern[i]->mem[j] >=0.0005 && 
pattern[i]->mem[j]<=.9995) {
                left_fuzz-= 
pattern[i]->mem[j]*log10(pattern[i]->mem[j]);
                left_fuzz-= 
(1.-pattern[i]->mem[j])*log10((1.-pattern[i]->mem[j]));
               }
            }
            else {
              if(pattern[i]->mem[j] >=0.0005 && 
pattern[i]->mem[j]<=.9995) {
                right_fuzz-= 
pattern[i]->mem[j]*log10(pattern[i]->mem[j]);
                right_fuzz-= 
(1.-pattern[i]->mem[j])*log10((1.-pattern[i]->mem[j]));
               }
            }
         }
    }
    if(left_class!=0)
        left_fuzz/=(float)left_class*NO_CLASS;
    if(right_class!=0)
        right_fuzz/=(float)right_class*NO_CLASS;

    left_entropy += left_fuzz; 
    right_entropy += right_fuzz;  
    
tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
}

#endif

#ifdef CONVENTIONAL_ENTROPY

/* calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {
    float left_entropy=0.,right_entropy=0.; 
    float tot_entropy;
    int tot_pat;
    float pi_left,pi_right;
    int i,j;
    
    tot_pat=left_class+right_class;
    for(i=0;i<NO_CLASS;i++) {
      pi_left=(double)left_freq[i]/(double)tot_pat;
      pi_right = 1.-pi_left;
      if(pi_left>.0005)
         left_entropy-=pi_left*log10((double)pi_left);
      if(pi_right>.0005)
         right_entropy-=pi_right*log10((double)pi_right);
    }

    
tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
}

#endif



/*   allocated the memory of a node and returns the address */
NODEPTR getnode() {
    NODEPTR  nodeptr;

    size_tree++; 
    nodeptr=(NODEPTR)malloc(sizeof(struct node));
    nodeptr->class = -1;
    nodeptr->atrib = -1;
    nodeptr->leftptr=NULL;
    nodeptr->rightptr=NULL;
    return(nodeptr);
 }


void sort_e(int min_atr,int left,int right) {
   int i,j;
   struct pattern_pts *temp;
   for(j=right-1;j>left;j--)
      for(i=left;i<j;i++)
        if(pattern[i]->bin[min_atr]>pattern[i+1]->bin[min_atr]) {
	  temp=pattern[i];
          pattern[i]=pattern[i+1];
          pattern[i+1]=temp;
        }
 }


/* counts the no lines */
/*  void fno_lns(FILE *fpin,int *no_lns) {
    *no_lns = 0;
    do {
       if(fgetc(fpin)=='\n')
          (*no_lns)++;
    } while(!feof(fpin));
    rewind(fpin);
}  */




/*  this function loads the pattern no , pattern class and the 
atributes */
void loadfile_e(FILE *fpin1,FILE *fpin2,FILE *fpin3,PATTPTR pattern) {
   int i = 0,j,temp; 
   while(!feof(fpin1)) {
     if(fscanf(fpin1,"%d ",&pattern[i]->class)<=0)
        break; 
     for(j=0;j<NO_CLASS;j++)
        fscanf(fpin1,"%f",&pattern[i]->mem[j]);
     
     fscanf(fpin2,"%d",&temp); 
     for(j=0;j<NO_ATRIB*3;j++)
        fscanf(fpin2,"%d",&pattern[i]->bin[j]); 

     fscanf(fpin3,"%d",&temp); 
     for(j=0;j<NO_ATRIB*3;j++)
        fscanf(fpin3,"%f",&pattern[i]->inmem[j]);
 
     i++;
   }
   rewind(fpin1);
   rewind(fpin2);
 }

/* stack operations   */     
void push(int a) {
   stack.arr[stack.top++]=a;
}

void pop() {
   stack.top--;
}

/*  traverses the decision tree */
  void traverse(int min,int max,int col,NODEPTR root,int x,int y,int 
k,int spc,int ud) {
	int mid,t1,t2;
        static int j=0;
        int i;
        float t;

	if(root==NULL)
	  return;
        mid = (int)(float)(max+min)/2.;
	image[mid][col] = root->class ; 
	image1[mid][col] = root->atrib+1 ; 
        t=0.0;
        if(j!=0)
        for(i=0;i<20;i++) {
           t+=0.05;
           t1 = (float)x+t*(float)(mid-x); 
           t2 = (float)y+t*(float)(col-y);
           if(!(t1 == x && t2 == y) && !(t1==mid && t2==col)) {
             if(k%2==0){
              image[t1][t2]= -10;
              image1[t1][t2]= -10;
             }
             else {
              image[t1][t2]= 10;
              image1[t1][t2]= 10;
             }
           }
        }
        j++; 
        x=mid;
        y=col;
        if(ud==0)
          image[mid][col-1]=-20;
        else
          image[mid][col-1]=20;
        if(ud==0)
          image1[mid][col-1]=-20;
        else
          image1[mid][col-1]=20;

        k++;
        spc = (float)spc/1.5;
	
traverse(mid,max-(int)(.2*(max-min)),col+spc,root->rightptr,x,y,k,spc,1);
	
traverse(min+(int)(.2*(max-min)),mid,col+spc,root->leftptr,x,y,k,spc,0); 
        return;   
}  

void print_tree() {
     int i,j;
     int left,right;
     fpout=fopen("trav.dat","w");   
     fpout2=fopen("travc.dat","w");   
    
     left=(int)(float)ROW/4.-5;
     right=ROW-(int)(float)ROW/4.+5; 
     for(i=left;i<right;i++){ 
        for(j=0;j<COL;j++)  {

       /* printf the classess   */
            if(image1[i][j]!=0)
               if(image1[i][j]== -10) {
                   c='+';
                   fprintf(fpout2,"%c",c);
               }
               else if(image1[i][j]== +10) {
                   c='-';
                   fprintf(fpout2,"%c",c);
               }
               else if(image1[i][j]== 20) {
                   c='d';
                   fprintf(fpout2,"%c",c);
               }
               else if(image1[i][j]==-20) {
                   c='u';
                   fprintf(fpout2,"%c",c);
               }
               else 
                   fprintf(fpout2,"%d",image1[i][j]);
            else
               fprintf(fpout2," ");
         
       /* printf the attributes   */
            if(image[i][j]!=0)
               if(image1[i][j]== -10) {
                   c='+';
                   fprintf(fpout,"%c",c);
               }
               else if(image[i][j]== +10) {
                   c='-';
                   fprintf(fpout,"%c",c);
               }
               else if(image[i][j]== 20) {
                   c='d';
                   fprintf(fpout,"%c",c);
               }
               else if(image[i][j]==-20) {
                   c='u';
                   fprintf(fpout,"%c",c);
               }
               else 
                   fprintf(fpout,"%d",(int)abs((double)image[i][j]));
            else
               fprintf(fpout," ");
        }
        fprintf(fpout,"\n");
        fprintf(fpout2,"\n");
     }
     
     fclose(fpout2);
     fclose(fpout);
}


/*  traverses the decision tree */
  void travel(NODEPTR root) {
	static int i=0;
	int j;
	if(root==NULL)
	  return;
	i++;
	travel(root->rightptr);
        i--;
	for(j=0;j<i;j++)
	   fprintf(fpout1,"     ");
	fprintf(fpout1,"%d \n",root->atrib+1);  
	i++;
	travel(root->leftptr); 
	i--;  
        return;   
}  


#ifdef HEURISTIC_ENTROPY_1 
/* Using Entr calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {

    float left_entropy=0.,right_entropy=0.; 
    float p[2][NO_CLASS];
    float MA[2];
    float tot_entropy;
    int i,j;

    MA[0]= 0.0; MA[1]= 0.0; 


    for(i=left;i<right;i++)
      if(pattern[i]->bin[atrib]==0) 
         MA[0]  += pattern[i]->inmem[atrib];
      else
         MA[1] += pattern[i]->inmem[atrib];


/* clearing the spaces */ 
    for(j=0;j<2;j++)
      for(i=0;i<NO_CLASS;i++)
         p[j][i] = 0.0;   

 
    for(j=0;j<NO_CLASS;j++) 
      for(i=left;i<right;i++)
         if(pattern[i]->bin[atrib]==0)
         {
          p[0][j] += (pattern[i]->mem[j] >= pattern[i]->inmem[atrib]) ? pattern[i]->inmem[atrib]:pattern[i]->mem[j];
         }
         else
         {
          p[1][j] += (pattern[i]->mem[j] >= pattern[i]->inmem[atrib]) ? pattern[i]->inmem[atrib]:pattern[i]->mem[j];
         }



/* calculationg the p_i's properly */  
    for(j=0;j<2;j++)
      for(i=0;i<NO_CLASS;i++) {
         if(MA[j] == 0.0)
            p[j][i] = 1.0;   
         else
            p[j][i] = p[j][i]/MA[j];   
      }


/*  calculationg the left and right entropy */  
    for(i=0;i<NO_CLASS;i++)
    {
      left_entropy += (p[0][i] != 0.0) ? p[0][i]*log10(p[0][i]) : 0.0;  
      right_entropy += (p[1][i] != 0.0) ? p[1][i]*log10(p[1][i]) : 0.0;
    }



   tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
    return(tot_entropy);
  
}  
#endif

#ifdef HEURISTIC_ENTROPY_2
/* Using Ambiguity calculates the entropy of a given attribute division  */ 
float entropy(int atrib,int left_class,int right_class,int 
*left_freq,int *right_freq,int left,int right) {

    float left_entropy=0.,right_entropy=0.; 
    float p[2][NO_CLASS];
    float MA[2];
    float tot_entropy;
    int i,j;
    int max_p[2], min_p[2];

    MA[0]= 0.0; MA[1]= 0.0; 


    for(i=left;i<right;i++)
      if(pattern[i]->bin[atrib]==0) 
         MA[0]  += pattern[i]->inmem[atrib];
      else
         MA[1] += pattern[i]->inmem[atrib];


/* clearing the spaces */ 
    for(j=0;j<2;j++)
      for(i=0;i<NO_CLASS;i++)
         p[j][i] = 0.0;   

 
    for(j=0;j<NO_CLASS;j++) 
      for(i=left;i<right;i++)
         if(pattern[i]->bin[atrib]==0)
         {
          p[0][j] += (pattern[i]->mem[j] >= pattern[i]->inmem[atrib]) ? pattern[i]->inmem[atrib]:pattern[i]->mem[j];
         }
         else
         {
          p[1][j] += (pattern[i]->mem[j] >= pattern[i]->inmem[atrib]) ? pattern[i]->inmem[atrib]:pattern[i]->mem[j];
         }



/* calculationg the p_i's properly */  
    for(j=0;j<2;j++)
      for(i=0;i<NO_CLASS;i++) {
         if(MA[j] == 0.0)
            p[j][i] = 1.0;   
         else
            p[j][i] = p[j][i]/MA[j];   
      }


/*  calculationg the left and right entropy */  
    max_p[0] =  p[0][0]; 
    max_p[1] =  p[0][0]; 
    min_p[0] =  p[0][0];
    min_p[1] =  p[0][0];
    for(j=0;j<2;j++)
       for(i=0;i<NO_CLASS;i++)
        {
          if(p[j][i] > max_p[j]) 
              max_p[j] = p[j][i];

          if(p[j][i] < min_p[j]) 
              min_p[j] = p[j][i];

        }

   left_entropy = (max_p[0] == 0) ? 1 : min_p[0]/max_p[0];
   right_entropy = (max_p[1] == 0) ? 1 : min_p[1]/max_p[1];

   tot_entropy=(double)left_class*left_entropy+(double)right_class*right_entropy;
   return(tot_entropy);
  
}  
#endif
