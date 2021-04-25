Michael Ameteku
Algorithms - Project 3
---------------------------------------------------------------------------------------------------
Purpose:

This program uses dynammic programming algorithm to remove pixels from an image vertically and horizontally whilst taking into consideration the content of the image. 

Running the program will take out the total number of seams instructed and generate a new file using the new matrix of pixels
-------------------------------------------------------------------------------------------------------

To run the program, run the following commands:

First unzip

Then build using the following command on the linux commandline:

$g++ carving.cpp -o carve

To run, use :
$ ./carve <filename> <numberOfVerticalSeams> <NumberOfHorizontalSeams>

An example would be,
$ ./carve bug.pgm 10 20

for carving 10 vertical seams and 20 horizontal seams

The generated file is then stored under the name :
 <filename>_processed.pgm
For example  bug_processed.pgm