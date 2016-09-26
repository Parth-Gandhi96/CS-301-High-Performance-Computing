#!/bin/bash

echo "write a File(.c File) which you want to Execute(do not write \".c\") - Filtering:-"
read filename

echo "Write a name of image(must be .ppm) which you want to process(do not write \".ppm\"):-"
read imageName;

outputFileName="Outputs_on_Rotating_$imageName.txt"
xlsFilename="stats_Of_Rotating_$imageName.xls"
imageName="$imageName.ppm"
filename="$filename.c"


NUMBERS="0.25 0.50 0.75 1.0 2.0"



for p in `seq 1 4`;							# p --> Number of Cores
do
	export OMP_NUM_THREADS=$p
    for number in `echo $NUMBERS`					# jnumber--> value of Initial Degree
	do
		gcc -fopenmp $filename -lm
		./a.out $imageName $number $p $outputFileName
	done
done  

gcc fileIO.c -lm
./a.out $outputFileName $xlsFilename


