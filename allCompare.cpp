#include <bits/stdc++.h>
#include <omp.h>
#include <time.h>
using namespace std;

#define ll long long int

ll serialSum = 0;
ll criticalSum = 0;
ll reductionSum = 0;
ll sumArray = 0;
ll atomicSum=0;
ll finalSum =0;

void store(ll n,ll arr[]){
	for(ll i=0;i<n;i++){
		arr[i] = rand()%100 -50;
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

double usingCritical(ll n,ll arr[]){
	double t1 = omp_get_wtime();
	#pragma omp parallel
	{
		ll mySum=0;
		#pragma omp for
		for(ll i=0;i<n;i++){
			mySum += arr[i];
		}
		#pragma omp critical
		criticalSum += mySum;
	}
	double t2 = omp_get_wtime();
	return (t2-t1);
}

double usingReduction(ll n,ll arr[]){
	double t1 = omp_get_wtime();
	#pragma omp parallelreduction(+:reductionSum)
	{
		ll mySum=0;
		#pragma omp for
		for(ll i=0;i<n;i++){
			mySum += arr[i];
		}
		reductionSum += mySum;
	}
	double t2 = omp_get_wtime();
	return (t2-t1);
}

double usingArray(ll n,ll arr[]){
	ll arrSum[omp_get_max_threads()];
	for(ll i=0;i<omp_get_max_threads();i++)
		arrSum[i]=0;
	double t1 = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp for
		for(ll i=0;i<n;i++){
			arrSum[omp_get_thread_num()] += arr[i];
		}
	}
	for(ll i=0;i<omp_get_max_threads();i++)
		sumArray += arrSum[i];
	double t2 = omp_get_wtime();
	return (t2-t1);
}
double Manually(ll n,ll arr[]){
	ll arrSum[omp_get_max_threads()];
	for(ll i=0;i<omp_get_max_threads();i++)
		arrSum[i]=0;
	double t1 = omp_get_wtime();
	#pragma omp parallel
	{
		#pragma omp for
		for(ll i=0;i<n;i++){
			arrSum[omp_get_thread_num()] += arr[i];
		}
	}
	for(ll i=0;i<omp_get_max_threads();i++)
		sumArray += arrSum[i];
	double t2 = omp_get_wtime();
	return (t2-t1);
}

double usingAtomic(ll n,ll arr[]){
	double t1 = omp_get_wtime();
    #pragma omp parallel
	{
		ll mySum=0;
		#pragma omp for nowait
		for(ll i=0;i<n;i++){
			mySum += arr[i];
		}
		#pragma omp atomic
		atomicSum += mySum;
	}
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
	double criticalTime = usingCritical(n,arr);
	double reductionTime = usingReduction(n,arr);
	double arrayTime = usingArray(n,arr);
	double atomicTime = usingAtomic(n,arr);

	fprintf(fp,"%f\n%lld\n",serialTime,serialSum);
	fprintf(fp,"%f\n%lld\n",criticalTime,criticalSum);
	fprintf(fp,"%f\n%lld\n",atomicTime,atomicSum);
	fprintf(fp,"%f\n%lld\n",reductionTime,reductionSum);
	fprintf(fp,"%f\n%lld\n",arrayTime,sumArray);

	return 0;
}
/*

10 lines:-

Number of Processor
Problem Size
Serial Time
Serial Sum
Critical Time
Critical Sum
Reduction Time
Reduction Sum
Using Array Time
Using Array TIme
	
*/