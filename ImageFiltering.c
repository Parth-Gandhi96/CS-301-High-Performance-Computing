#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "imageProcessing.h"

PPMImage *filterSerial(PPMImage *im,int halfDepth){
	int rows = im->x;
  	int cols = im->y;
  	int i=0,j=0,k=0,l=0,m=0;
  	int r[rows][cols],g[rows][cols],b[rows][cols];

	int id=0;
  	for(i=0;i<rows;i++){										// X - Axis < Number of Columns j-> x co-ordinate
      	for( j=0;j<cols;j++){									// Y - Axis < Number of raws	i-> y co-ordinate
			
			id = (rows*i + j);
			PPMPixel *temp = im->data + id;						// temp --> points to pixel at image
	  		
	  		// Storing the color contrass of each pixel in array
	  		r[i][j] = temp->red;																										
	  		g[i][j] = temp->green;
	  		b[i][j] = temp->blue;
		}	
   	}

	   PPMImage *im2 = (PPMImage *) malloc(sizeof(PPMImage));
  	im2->x = rows;
  	im2->y = cols;
  	im2->data = (PPMPixel *) malloc(rows*cols*sizeof(PPMPixel));

  	int count=0,avg[3],sum[3];
   	for( i=0;i<rows;i++){
   		for(j=0;j<cols;j++){
   			count=0;
   			sum[0]=0; sum[1]=0; sum[2]=0;
   			for(k=(i-halfDepth);k<rows && k<=(i+halfDepth);k++){
   				for(l=(j-halfDepth);l<cols && l<=(j+halfDepth);l++){
   					if(k<0 || l<0)
   						continue;
   					sum[0] += r[k][l];
   					sum[1] += g[k][l];
   					sum[2] += b[k][l];
   					count++;
   				}
   			}

   			PPMPixel *temp = im2->data + (rows*i + j);
   			temp->red = (sum[0]/count);
   			temp->green = (sum[1]/count);
   			temp->blue = (sum[2]/count);
   		}
   	}

  	return im2;
}

PPMImage *filterParallel(PPMImage *im,int halfDepth,int numberOfThreads){
  int rows = im->x;
  int cols = im->y;
  int i=0;
  int r[rows][cols],g[rows][cols],b[rows][cols];
  #pragma omp parallel
  {
    int j=0,id;
    #pragma omp for private(j,id)
    for(i=0;i<rows;i++){                     // X - Axis < Number of Columns j-> x co-ordinate
      for(j=0;j<cols;j++){                    // Y - Axis < Number of raws  i-> y co-ordinate
        id = (rows*i + j);
        PPMPixel *temp = im->data + id;       // temp --> points to pixel at image
        
        // Storing the color contrass of each pixel in array
        r[i][j] = temp->red;                                                    
        g[i][j] = temp->green;
        b[i][j] = temp->blue;
      } 
    }
  }

  PPMImage *im2 = (PPMImage *)malloc(sizeof(PPMImage));
  im2->x = rows;
  im2->y = cols;
  im2->data = (PPMPixel *) malloc(rows*cols*sizeof(PPMPixel));

  #pragma omp parallel
  {
    int j=0,count=0,sum[3],k=0,l=0;
    #pragma omp for private(j,k,l,count,sum)
    for( i=0;i<rows;i++){
      for(j=0;j<cols;j++){
        count=0;
        sum[0]=0; sum[1]=0; sum[2]=0;
        for(k=(i-halfDepth);k<rows && k<=(i+halfDepth);k++){
          for(l=(j-halfDepth);l<cols && l<=(j+halfDepth);l++){
            if(k<0 || l<0)
              continue;
            sum[0] += r[k][l];
            sum[1] += g[k][l];
            sum[2] += b[k][l];
            count++;
          }
        }
        PPMPixel *temp = im2->data + (rows*i + j);
        temp->red = (sum[0]/count);
        temp->green = (sum[1]/count);
        temp->blue = (sum[2]/count);
      }
    }
  }
  return im2;
}

int main(int argc, char* argv[]){
  if(argc<2)
    printf("Enter filename\n");
  char *filename = argv[1];             // First Argument -> File Name of Image
  int halfWidth = atoi(argv[2]);        // Second Argument -> Half Width for Filter
  int p = atoi(argv[3]);                //  Third Argument -> Number off Processor
  char *outputFile = argv[4];            // Forth Argument -> .txt File (to Store Output)

  FILE *fp = fopen(outputFile,"a");

  fprintf(fp,"%d\n",p);                                       //Number of Processor

  PPMImage *image;

  // Reading The IMAGE
  image = readPPM(filename);
  fprintf(fp,"%d\n%d\n",image->x,image->y);                   //Raws & Columns


  // Filtering Image Serial
  float startSerial = omp_get_wtime();
  PPMImage * fserial = filterSerial(image,halfWidth);
  float stopSerial = omp_get_wtime();
  
  // Filtering Image Serial
  float startParallel = omp_get_wtime();
  PPMImage * fparallel = filterParallel(image,halfWidth,p);
  float stopParallel = omp_get_wtime();
   

  fprintf(fp,"%d\n", halfWidth);                                                            //Half Width
  fprintf(fp,"%0.6f\n",(stopSerial-startSerial));                                           //Serial Time
  fprintf(fp,"%0.6f\n",(stopParallel-startParallel));                                       //Parallel Time
  fprintf(fp,"%0.6f\n",((stopSerial-startSerial)/(stopParallel-startParallel)));            //Speed Up
  fprintf(fp,"%0.6f\n",((stopSerial-startSerial)-(stopParallel-startParallel)));            //Overhead

  char serial[50] = "Serial_";
  char width[10] ;
  sprintf(width, "%d", halfWidth);
  char extansion[] = ".ppm";
  strcat(serial,width);
  strcat(serial,extansion);
  writePPM(serial,fserial);

  char parallel[50] = "Parallel_";
  strcat(parallel,width);
  strcat(parallel,extansion);
  writePPM(parallel,fparallel);

  return 0;
}
/*

Number of Processor 1 Digit
Number of Rows      max 4 Digits
Number of Columns   max 4 Digits
Half Width          max 4 Digits
Serial Time         2+1+6 = 9 Digits
Parallel Time       2+1+6 = 9 Digits
Speed Up            2+1+6 = 9 Digits
Overhead            2+1+6 = 9 Digits

*/