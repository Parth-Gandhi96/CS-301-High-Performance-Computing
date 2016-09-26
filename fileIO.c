#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
	if(argc<3){
		printf("Please give the Name of Files First is Input And Second is for Output(.xls)\n");
		return 0;
	}
	char *fileName = argv[1];
	char *xlsFile = argv[2];
	FILE *fp = fopen(fileName,"r");
	FILE *fq = fopen(xlsFile,"w");
	if(!fp){
		printf("File named %s Doesnot exist\n",fileName);
		return 0;
	}
	
	// Heading to all the Columns
	fprintf(fq, "%s\t", "Number_of_Cores");
	fprintf(fq, "%s\t", "Number_of_Rows");
	fprintf(fq, "%s\t", "Number_of_Columns");
	fprintf(fq, "%s\t", "Initial_Degree");
	fprintf(fq, "%s\t", "SerialTime");
	fprintf(fq, "%s\t", "ParallelTime");
	fprintf(fq, "%s\t", "SpeedUp");
	fprintf(fq, "%s\n", "overHead");

	int i=0,count=0,max=12;							// Data can be atmost 12 Digits long
	char c,buff[6][max],line[max];
	char *temp;

	// Reading From input File And Puting information into xls File
	while (fgets(line,max, fp)>0) {			// Read Whole Line & returns the number of bytes read
        count=8;
        do{
        	count--;
        	i=0;
        	while(line[i]!='\n')		
        		i++;
        	line[i] = '\0';						// Change '\n' to '\0' because in xls it will go to down cell
        	fprintf(fq, "%s\t", line);			// '\t' will move current cell to right cell
    	}while(count>0 && fgets(line,max, fp)>0);
    	fprintf(fq, "\n");
    }

    //Closing the Files
	fclose(fp);
	fclose(fq);
}

/*
Total 8 Lines of Information:-

Number of Processor 1 Digit
Number of Rows      max 4 Digits
Number of Columns   max 4 Digits
initial Degree      2+1+6 = 9 Digits
Serial Time         2+1+6 = 9 Digits
Parallel Time       2+1+6 = 9 Digits
Speed Up            2+1+6 = 9 Digits
Overhead            2+1+6 = 9 Digits

*/