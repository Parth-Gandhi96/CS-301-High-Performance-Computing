#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>
#include "imageProcessing.h"

void BilinearlyInterpolate(PPMImage *im,int width,int height,float x,float y,PPMImage *im2,int i,int j){
  int contrass[]={0,0,0},count=0,id;
  PPMPixel *temp;;
  int p=0,q=0;;
  for(p=0;p<2;p++){
      if(floor(x+p)<width && floor(x+p)>0){
        for(q=0;q<2;q++){
          if(floor(y+q)<height && floor(y+q)>0){
            id = width*floor(y+q) + floor(x+p);
            temp = im->data + id;
            contrass[0] += temp->red;
            contrass[1] += temp->green;
            contrass[2] += temp->blue;
            count++;
          }
        }
      }
  }
  id = width*i + j;
  temp = im2->data + id;
  temp->red = contrass[0]/4;
  temp->green =  contrass[1]/4;
  temp->blue = contrass[2]/4;
  
}

PPMImage *rotateImageParallel(PPMImage *im,float initial){
	float pi = 3.14;
	float theta = 0; 
	float DTR = initial*(3.14/180);

  	int height = im->y;
  	int width = im->x;
  	
  	float radius;
  	int i,j,id1,id2,x,y;
  	//printf("Width:-%d  height:-%d\n",width,height);
	int cx=((width+1)/2),cy=((height+1)/2);
  	
  	PPMImage *im2 = (PPMImage *) malloc(sizeof(PPMImage));
  	im2->y = height;
  	im2->x = width;
  	im2->data = (PPMPixel *) malloc(height*width*sizeof(PPMPixel));
  	
  	#pragma omp parallel
  	{
  		#pragma omp for private(radius,i,j,id1,id2,x,y,theta)
  		for(j=0;j<width;j++){										// X - Axis
      		for(i=0;i<height;i++){									// Y - Axis
	  			  radius = sqrt( (j-cx)*(j-cx) + (i-cy)*(i-cy) );
	  			  theta = (radius/2)*DTR;
	  			  x = (float)(j-cx)*(float)cos(theta) - (float)(i-cy)*(float)sin(theta) + (float)cx;    // X Co-Ordinate in Original image
            y = (float)(j-cx)*(float)sin(theta) + (float)(i-cy)*(float)cos(theta) + (float)cy;    // Y Co-Ordinate in Original image
        
            if(((int)x>=width || (int)y>=height) || (x<0.0 || y<0.0))
               continue;

            BilinearlyInterpolate(im,width,height,x,y,im2,i,j);
		    }	
   	  }
   	}
  	return im2;
	
}

PPMImage *rotateImageSerial(PPMImage *im,float initial){
	float pi = 3.14;
	float theta = 0; 
	float DTR = initial*(3.14/180);

  	int height = im->y;
  	int width = im->x;
  	//printf("Width:-%d  height:-%d\n",width,height);
  	float radius;
  	int i,j,id1,id2,x,y;
	int cx=((width+1)/2),cy=((height+1)/2);
  	
  	PPMImage *im2 = (PPMImage *) malloc(sizeof(PPMImage));
  	im2->y = height;
  	im2->x = width;
  	im2->data = (PPMPixel *) malloc(height*width*sizeof(PPMPixel));
  	
  	for(j=0;j<width;j++){										// X - Axis
      for(i=0;i<height;i++){									// Y - Axis
	  		radius = sqrt( (j-cx)*(j-cx) + (i-cy)*(i-cy) );
	  		theta = (radius/2)*DTR;
	  		x = (float)(j-cx)*(float)cos(theta) - (float)(i-cy)*(float)sin(theta) + (float)cx;   // X Co-Ordinate in Original image
        y = (float)(j-cx)*(float)sin(theta) + (float)(i-cy)*(float)cos(theta) + (float)cy;    // Y Co-Ordinate in Original image
        
        if(((int)x>=width || (int)y>=height) || (x<0.0 || y<0.0))
          continue;

        BilinearlyInterpolate(im,width,height,x,y,im2,i,j);
		  }	
   	}
  	return im2;
}

int main(int argc, char* argv[]){
  	if(argc<2)
    printf("Enter filename\n");
  char *filename = argv[1];             // First Argument -> File Name of Image
  float initialDegree = atof(argv[2]);        // Second Argument -> Initial degree
  int p = atoi(argv[3]);                //  Third Argument -> Number off Processor
  char *outputFile = argv[4];            // Forth Argument -> .txt File (to Store Output)

FILE *fp = fopen(outputFile,"a");

  fprintf(fp,"%d\n",p);                                       //Number of Processor

  	PPMImage *image;

  	// Reading The IMAGE
  	image = readPPM(filename);
  	fprintf(fp,"%d\n%d\n",image->x,image->y);                   //Raws & Columns

  	// Rotating Image - Serial
  	float startSerial = omp_get_wtime();
  	PPMImage *rSerial = rotateImageSerial(image,initialDegree);
  	float stopSerial = omp_get_wtime();

   	// Rotating Image - Parallel
  	float startParallel = omp_get_wtime();
  	PPMImage *rParallel = rotateImageParallel(image,initialDegree);
  	float stopParallel = omp_get_wtime();

  	// Printing The Results:-
  	fprintf(fp,"%0.6f\n", initialDegree);                                                            //Half Width
  fprintf(fp,"%0.6f\n",(stopSerial-startSerial));                                           //Serial Time
  fprintf(fp,"%0.6f\n",(stopParallel-startParallel));                                       //Parallel Time
  fprintf(fp,"%0.6f\n",((stopSerial-startSerial)/(stopParallel-startParallel)));            //Speed Up
  fprintf(fp,"%0.6f\n",((stopSerial-startSerial)-(stopParallel-startParallel)));            //Overhead

  char serial[50] = "Serial_";
  char width[10] ;
  sprintf(width, "%f", initialDegree);
  char extansion[] = ".ppm";
  strcat(serial,width);
  strcat(serial,extansion);
  writePPM(serial,rSerial);

  char parallel[50] = "Parallel_";
  strcat(parallel,width);
  strcat(parallel,extansion);
  writePPM(parallel,rParallel);


  return 0;
}

/*

File name:- imageRadialRotation_WidthParallel
Image name:- joker

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