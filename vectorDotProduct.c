#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
double vectorMultiSerial(double *a,double *b,int n){
	int i=0;
	double ans=0;
	double t1 = omp_get_wtime();
	for(i=0;i<n;i++){
		ans += (a[i]*b[i]);
	}
	double t2 = omp_get_wtime();
	printf("\tDot Product = %.8f\n",ans);
	return (t2-t1);
}
double vectorMultiParallel(double *a,double *b,int n){
	int chunk = 16;
  	double ans = 0.0;
	double t1 = omp_get_wtime();
	#pragma omp parallel num_threads(3)
	{ 
		double temp=0.0;
		int i=0;
		#pragma omp for
    	for (i=0; i < n; i++)
        	temp += (a[i] * b[i]);
        #pragma omp redunction(+:ans)
        ans += temp;
	}
	double t2 = omp_get_wtime();
	printf("\tDot Product = %.8f\n",ans);
	return (t2-t1);
}
void store(double *a,double *b,int n){
	int i=0;
	for(i=0;i<n;i++){
		a[i] = i%7;
		b[i] = i%7;
	}
}
int main(){
	int n=100000000;
	double *a = (double *)malloc(sizeof(double)*n);
	double *b = (double *)malloc(sizeof(double)*n);
	store(a,b,n);
	printf("SERIAL:-\n");
	double serialTime = vectorMultiSerial(a,b,n);
	printf("\tTime:- %.8f\n",serialTime);
	printf("PARALLEL:-\n");
	double parallelTime = vectorMultiParallel(a,b,n);
	printf("\tTime:- %.8f\n",parallelTime);
	printf("Speed Up:- %.8f\n",(serialTime/parallelTime));
	return 0;
}