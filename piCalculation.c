#include<stdio.h>
#include<omp.h>
double piSerial(long long int n){
	long long int i=0;
	double ans=0;
	double t1 = omp_get_wtime();
	for(i=n;i>=0;i--){
		ans += ((i%2==0?1.0:-1.0)/((2*(double)i)+1.0));
	}
	ans = ans*4.0;
	double t2 = omp_get_wtime();
	printf("\tPI =  %.4f\n",ans);
	return (t2-t1);
}
double piParallel(long long int n){
	double ans=0;
	double t1 = omp_get_wtime();
	#pragma omp parallel num_threads(4)
	{
		double temp=0;
		long long int i=0;
		#pragma omp for
		for(i=n;i>=0;i--){
			temp += ((i%2==0?1.0:-1.0)/((2*(double)i)+1.0));
		}
		#pragma omp reduction(+:ans)
		ans += temp;
	}
	ans = ans*4.0;
	double t2 = omp_get_wtime();
	printf("\tPI = %.4f\n",ans);
	return (t2-t1);
}
int main(){
	long long int n = 10000000;
	printf("SERIAL:-\n");
	double serialTime = piSerial(n);
	printf("\tTime = %.8f\n",serialTime);
	printf("PARALLEL:-\n");
	double parallelTime = piParallel(n);
	printf("\tTime = %.8f\n",parallelTime);
	printf("Speed Up = %.8f\n",(serialTime/parallelTime));
	return 0;
}