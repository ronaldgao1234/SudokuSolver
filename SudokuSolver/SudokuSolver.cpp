#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include "opencv\cv.h"

using namespace cv;

int main() {
	
	//grayscale since we don't care about color
	Mat sudoku = imread("sudoku.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//CV_8UC1 means	
	//CV_[number of bits per item][signed or unsigned][type prefix][the channel number]
	Mat outerBox = Mat(sudoku.size(), CV_8UC1);

	//GaussianBlur(inputArraysrc, outputArray dst, Size ksize, double sigmaX, double sigmaY=0, 
	//int borderType=BORDER_DEFAULT)
	GaussianBlur(sudoku, sudoku, Size(11, 11), 0);

	//with noise smoothed out, we now threshold the image
	//it calculates over 5x5 window and subtracts 2 from the mean. this is the threshold
	//level for every pixel
	adaptiveThreshold(sudoku, outerBox, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 5, 2);

	//invert black and white. borders come in black, now they are white
	bitwise_not(outerBox, outerBox);

	//thresholding operation can have small cracks appear so we should dilate to fill it in
	Mat kernel = (Mat_<uchar>(3, 3) << 0, 1, 0, 1, 1, 1, 0, 1, 0);
	dilate(outerBox, outerBox, kernel);

	//find biggest blob
	int count = 0; 
	int max = -1;
	Point maxPt;
	for (int y = 0; y < outerBox.size().height; y++) {
		uchar* row = outerBox.ptr(y);
		for (int x = 0; x < outerBox.size().width; x++) {
			if (row[x] >= 128) {
				int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 64));

				if (area > max) {
					maxPt = Point(x, y);
					max = area;
				}
			}
		}
	}

	//turn biggest blobs white to black
	for (int y = 0; y < outerBox.size().height; y++) {
		uchar* row = outerBox.ptr(y);
		for (int x = 0; x < outerBox.size().width; x++) {
			if (row[x] == 64 && x != maxPt.x && y != maxPt.y) {
				int area = floodFill(outerBox, Point(x, y), CV_RGB(0, 0, 0));
			}
		}
	}

	//because we dilated a bit earlier
	erode(outerBox, outerBox, kernel);
	namedWindow("thresholded");
	imshow("thresholded", outerBox);
	waitKey(0);
	return 0;
}