#include <cv.h>
#include <highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;

int main(int argc, char** argv){
	if(argc != 3){
		fprintf(stderr, "usage: %s in out\n", argv[0]);
		return -1;
	}
	
	char* image_in = argv[1];
	char* image_out = argv[2];
	
	Mat image;
	image = imread(image_in, 1);
	if(!image.data){
		fprintf(stderr, "image read error\n");
		return -1;
	}
	
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	imwrite(image_out, gray_image);
	
	return 0;
}

