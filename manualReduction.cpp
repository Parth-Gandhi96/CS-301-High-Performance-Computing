#include <bits/stdc++.h>
#include <omp.h>
#include <time.h>
using namespace std;

#define ll long long int

ll serialSum = 0;
ll parallelManualSum=0;
ll parallelSum=0;

void store(ll n,ll arr[]){
	for(ll i=0;i<n;i++){
		arr[i] = 1;
	}
}

double serial(ll n,ll arr[]){
	double t1 = omp_get_wtime();
	for(ll i=0;i<n;i++){
		serialSum += arr[i];
	}
	double t2 = omp_get_wtime();
	return (t2-t1);
}

void printArr(ll n,ll arr[]){
	for(ll i=0;i<n;i++){
		printf("%lld ",arr[i]);
	}
	printf("\n");
	return;
}
double parallel(ll n,ll arr[]){
	int p = omp_get_max_threads();
	double t1 = omp_get_wtime();
	#pragma omp parallel reduction(+:parallelSum)
	{
		ll mySum=0;
		#pragma omp for
		for(ll i=0;i<n;i++){
			mySum += arr[i];
		}
		parallelSum += mySum;
	}
	double t2 = omp_get_wtime();
	return (t2-t1);

}
double parallelManual(ll n,ll arr[]){
	int p = omp_get_max_threads();
	ll sum[p];
	double t1 = omp_get_wtime();
	#pragma omp parallel
	{
		ll mySum=0;
		#pragma omp for
		for(ll i=0;i<n;i++){
			mySum += arr[i];
		}
		int id = omp_get_thread_num();
		sum[id] = mySum;
		
		ll stride=1;
		#pragma omp barrier
		while(stride<p){
			if((2*stride*id)+stride < p){
				sum[2*stride*id] = sum[2*stride*id] + sum[(2*stride*id)+stride];
			}
			#pragma omp barrier
			stride *=2;
		}
	}
	parallelManualSum = sum[0];
	double t2 = omp_get_wtime();
	return (t2-t1);
}

int main(int argc, char* argv[]){
	if(argc<2)
    	printf("Enter filename\n");
	int p = atoi(argv[1]);                //  First Argument -> Number off Processor
  	int logN = atoi(argv[2]);             //  Second Argument -> Logarithmic Problem Size (base 10)
	char *outputFile = argv[3];           //  Third Argument -> .txt File (to Store Output)
  	
	ll n=pow(10,logN);
	ll arr[n];
	store(n,arr);

	FILE *fp = fopen(outputFile,"a");

	fprintf(fp,"%d\n",p);
	fprintf(fp,"%lld\n",n);

	double serialTime = serial(n,arr);
	double parallelTime = parallel(n,arr);
	double parallelManualTime = parallelManual(n,arr);

	fprintf(fp,"%f\n%lld\n",serialTime,serialSum);
	fprintf(fp,"%f\n%lld\n",parallelTime,parallelSum);
	fprintf(fp,"%f\n%lld\n",parallelManualTime,parallelManualSum);


	return 0;
}
/*


*/