/*
	PI Calculation using Monte-Carlo
*/
#include <bits/stdc++.h>
#include <omp.h>
#include <time.h>
using namespace std;

#define ll long long int

double piSerial(long long int n,long long int r){
	long long int i=0,x,y,in=0;
	unsigned int myseed = time(NULL);
	for(i=0;i<n;i++){
		x = (rand_r(&myseed)%(2*r + 1))-r;
		y = (rand_r(&myseed)%(2*r + 1))-r;
		if((x*x)+(y*y)<=(r*r))
			in++;
	}
	return 4.0*(double)in/(double)n;
}

/*
Seed: 
	A integer used to set the starting point for generating a series of random numbers.
	The seed sets the generator to a random starting point.
	A unique seed returns a unique random number sequence.

Here,
	Therad ID is Diferant for each thread so using it we can get differant the set of random number.
*/

double piParallel(long long int n,long long int r){
	long long int totalIn=0;
	#pragma omp parallel reduction(+:totalIn)
	{
		long long int i=0,x,y,in=0;
    	unsigned int myseed = omp_get_thread_num();
    	#pragma omp for
		for(i=0;i<n;i++){
			x = (rand_r(&myseed)%(2*r + 1))-r;
			y = (rand_r(&myseed)%(2*r + 1))-r;
			if((x*x)+(y*y)<=(r*r))
				in++;
		}
		totalIn+=in;
	}
	return 4.0*(double)totalIn/(double)n;
}

int main(int argc, char* argv[]){
	if(argc<2)
    	printf("Enter filename\n");
	int p = atoi(argv[1]);                //  First Argument -> Number off Processor
  	int logN = atoi(argv[2]);             //  Second Argument -> Logarithmic Problem Size (base 10)
	int logR = atoi(argv[3]);             //  Third Argument -> Logarithmic Radius (base 10)
	char *outputFile = argv[4];           //  Forth Argument -> .txt File (to Store Output)
  	
	ll r = pow(10,logR);
	ll n = pow(10,logN);
	double t1,t2,serialPi,parallelPi,serialTime,parallelTime;

	FILE *fp = fopen(outputFile,"a");

	fprintf(fp,"%d\n",p);
	fprintf(fp,"%lld\n",n);
	fprintf(fp,"%lld\n",r);

	t1 = omp_get_wtime();
	serialPi = piSerial(n,r);
	t2 = omp_get_wtime();
	serialTime = (t2-t1);

	t1 = omp_get_wtime();
	parallelPi = piParallel(n,r);
	t2 = omp_get_wtime();
	parallelTime = (t2-t1);


	fprintf(fp,"%f\n%f\n",serialTime,serialPi);
	fprintf(fp,"%f\n%f\n",parallelTime,parallelPi);
	fprintf(fp,"%f\n",(serialTime)/(parallelTime));
	
	return 0;
}
/*

8 lines of Information:-

Number of Processor
Number of Iteration
Radius
serialTime
serail PI Value
Parallel Time
Parallel PI Value
Speedup

*/

