#include <stdio.h>
#include <stdlib.h>
#include <math.h>
void printMatrix(double **a,int n){
	int i=0,j=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%f ",a[i][j]);
		}
		printf("\n");
	}
}
void store(double **a,double **b,int n){
	int i=0, j=0;
    for (i=0;i<n;i++){
      	for (j=0;j<n;j++){
         	a[i][j] = ((i*n)+j)%7;							// C programming is Raw Major
         	b[i][j] = ((i*n)+j+1)%7;
      	}
  	}
 
}
double **transpose(double **a,int n){
	int i=0,j=0;
	double **ans = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		ans[i] = (double *)malloc(sizeof(double)*n);
	for (i=0;i<n;i++){
      	for (j=0;j<n;j++){
      		ans[i][j] = a[j][i];
      	}
  	}
  	return ans;
}
double **blockMatrixMultiplication(double **a, double **b, int n){
	int i=0,j=0,k=0;
	double **ans = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		ans[i] = (double *)malloc(sizeof(double)*n);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			ans[i][j] = 0;
			for(k=0;k<n;k++){
				ans[i][j] += a[i][k]*b[j][k];
			}
		}
	}
	return ans;
}
int main(int argc,char *argv[]){
	/*
	if(argc<5){
		printf("Please write Number of Processor, Logarithmic value, base And Output File Name\n");
		return 0;
	}
	int p = atoi(argv[1]),i=0;
	int base = atoi(argv[3]);
	int n = atoi(argv[2]);
	n = pow(base,n);
	char *fileName = argv[4];
	printf("\n in C:-Base:-%d  Problem Size:- %d  Number of Processor:-%d  Output File:-%s\n",base,n,p,fileName);
	*/
	int n=4,p=1,i=0;
	int temp[n][n];
	double **a = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		a[i] = (double *)malloc(sizeof(double)*n);
	double **b = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		b[i] = (double *)malloc(sizeof(double)*n);
	store(a,b,n);
	double **transB = transpose(b,n);
	double **answer = blockMatrixMultiplication(a,transB,n);
	printf("A:-\n");
	printMatrix(a,n);
	printf("B:-\n");
	printMatrix(b,n);
	printf("Transpose of B:-\n");
	printMatrix(transB,n);
	printf("AXB:-\n");
	printMatrix(answer,n);
	
	return 0;
}

/*
Total 6 Type of Information:-

Number_of_Cores(1 digit) , 
Logarithmic_Problem_Size(1 Digit) , 
SerialTime(1+1+6 Digits) , 
ParallelTime(1+1+6 Digits) , 
SpeedUp(1+1+6 Digits) , 
OverHea(1+1+6 Digits).

*/