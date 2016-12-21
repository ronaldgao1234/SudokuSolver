#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

int main() {
	//grayscale since we don't care about color
	Mat sudoku = imread("sudoku.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//CV_8UC1 means	
	//CV_[number of bits per item][signed or unsigned][type prefix][the channel number]
	Mat outerBox = Mat(sudoku.size(), CV_8UC1);

	//GaussianBlur(inputArraysrc, outputArray dst, Size ksize, double sigmaX, double sigmaY=0, 
	//int borderType=BORDER_DEFAULT)
	//http://docs.opencv.org/2.4/modules/imgproc/doc/filtering.html
	//
	GaussianBlur(sudoku, sudoku, Size(11, 11), 0);

	//with noise smoothed out, we now threshold the image
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

	return 0;
}