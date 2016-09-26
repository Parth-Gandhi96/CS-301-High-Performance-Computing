#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
void printArr(double **a,int n){
	if(n>5)
		return;
	int i=0,j=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			printf("%.3f ",a[i][j]);
		}
		printf("\n");
	}
}
double matrixMultiplySerial(double **a,double **b,int n){
	int i=0,j=0,k=0;
	double **ans = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		ans[i] = (double *)malloc(sizeof(double)*n);
	double t1 = omp_get_wtime();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			for(k=0;k<n;k++){
				ans[i][j] = ans[i][j] + (a[i][k] * b[k][j]);
			}
		}
	}
	double t2 = omp_get_wtime();
	if(n<5){
		//printf("\nArray (axb):-\n");
		//printArr(ans,n);
	}
	return (t2-t1);
}
double matrixMultiplyParallelSimple(double **a,double **b,int n){
	int h=0;
	double **ans = (double **)malloc(sizeof(double)*n);
	for(h=0;h<n;h++)
		ans[h] = (double *)malloc(sizeof(double)*n);
	double t1 = omp_get_wtime();
	#pragma omp parallel
	{
		int i=0,j=0,k=0;
		#pragma omp for
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				for(k=0;k<n;k++){
					ans[i][j] = ans[i][j] + (a[i][k] * b[k][j]);
				}
			}
		}
	}
	double t2 = omp_get_wtime();
	return (t2-t1);
}
double matrixMultiplyParallelByDividing(double **a,double **b,int n){
	int h=0;
	double **ans = (double **)malloc(sizeof(double)*n);
	for(h=0;h<n;h++)
		ans[h] = (double *)malloc(sizeof(double)*n);
	double t1 = omp_get_wtime();
	#pragma omp parallel num_threads(2)
	{
		int i=0,j=0,k=0,l=0;
		int id = omp_get_thread_num();
		int indexA = 2*(id/2),indexB = id%2;
		int a2 = (indexA%2)*(n/2), a1 = (indexA/2)*(n/2);
		int b2 = (indexB%2)*(n/2), b1 = (indexB/2)*(n/2);
		/*
		printf("Thread ID Number :- %d\n",id);
		printf("Array A:- index=%d Raw-%d to %d  And column-%d to %d\n",indexA,a1,a1+(n/2),a2,a2+(n/2));
		printf("Array B:- index=%d Raw-%d to %d  And column-%d to %d\n",indexB,b1,b1+(n/2),b2,b2+(n/2));
		*/
		for(i=a1;i<(a1+(n/2));i++){
			for(j=b2;j<(b2+(n/2));j++){
				for(k=a2,l=b1;k<(a2+(n/2)) && l<(b1+(n/2));k++,l++){
					#pragma omp reduction(+:ans[i][j])
					ans[i][j] += (a[i][k]*b[l][j]);  
					//printf("ans[%d][%d](=%.3f) = previous(%.3f) + a[%d][%d](=%.3f) + b[%d][%d](=%.3f)\n",i,j,ans[i][j],pre,i,k,a[i][k],l,j,b[l][j]);
				}
			}
		}
		indexA = (2*(id/2))+1,indexB = (id%2)+2;
		a2 = (indexA%2)*(n/2), a1 = (indexA/2)*(n/2);
		b2 = (indexB%2)*(n/2), b1 = (indexB/2)*(n/2);
		/*
		printf("Thread ID Number :- %d\n",id);
		printf("Array A:- index=%d Raw-%d to %d  And column-%d to %d\n",indexA,a1,a1+(n/2),a2,a2+(n/2));
		printf("Array B:- index=%d Raw-%d to %d  And column-%d to %d\n",indexB,b1,b1+(n/2),b2,b2+(n/2));
		*/
		for(i=a1;i<(a1+(n/2));i++){
			for(j=b2;j<(b2+(n/2));j++){
				for(k=a2,l=b1;k<(a2+(n/2)) && l<(b1+(n/2));k++,l++){
					#pragma omp reduction(+:ans[i][j])
					ans[i][j] += (a[i][k]*b[l][j]);  
					//printf("ans[%d][%d](=%.3f) = previous(%.3f) + (a[%d][%d](=%.3f) * b[%d][%d](=%.3f))\n",i,j,ans[i][j],pre,i,k,a[i][k],l,j,b[l][j]);
				}
			}
		}
		
	}
	double t2 = omp_get_wtime();
	//printArr(ans,n);
	return (t2-t1);
}
void store(double **a,double **b,int n){
	int i, j;
    for (i=0;i<n;i++){
      for (j=0;j<n;j++){
         a[i][j] = ((i*n)+j)%7;							// C programming is Raw Major
         b[i][j] = ((i*n)+j+1)%7;
      }
  }
 
}
int main(){
	int n=1024,i=0,p=4;
	int temp[n][n];
	double **a = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		a[i] = (double *)malloc(sizeof(double)*n);
	double **b = (double **)malloc(sizeof(double)*n);
	for(i=0;i<n;i++)
		b[i] = (double *)malloc(sizeof(double)*n);
	store(a,b,n);
	if(n<5){
		printf("\nArray a:-\n");
		printArr(a,n);
		printf("\nArray b:-\n");
		printArr(b,n);
	}
	printf("SERIAL:-\n");
	double serialTime = matrixMultiplySerial(a,b,n);
	printf("\tTime = %.8f\n",serialTime);
	
	printf("PARALLEL:-\n");
	double parallelTime = matrixMultiplyParallelSimple(a,b,n);
	printf("\t1) Simple parallel:-\n\t   Time = %.8f\n",parallelTime);
	printf("\t   SpeedUp :- %.8f\n\n",(serialTime/parallelTime));
	
	printf("\t2) Divide Matrix into %d Parts:-\n",p);
	double timeByDividingParallel = matrixMultiplyParallelByDividing(a,b,n);
	printf("\t   Time = %.8f\n",timeByDividingParallel);
	printf("\t   SpeedUp = %.8f\n\n",(serialTime/timeByDividingParallel));
	return 0;
}