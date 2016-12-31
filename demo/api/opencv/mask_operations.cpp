#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

static void help(char* progName){
    cout<<  "This program shows how to filter images with mask: the write it yourself and the filter2d way." << endl
        <<  "Usage:" << progName << " [image_name -- default demo.jpg] [G -- grayscale]" << endl << endl;
}

void Sharpen(const Mat& myImage, Mat& Result);

int main( int argc, char* argv[]){
    help(argv[0]);
    const char* filename = argc >=2 ? argv[1] : "demo.jpg";

    Mat I, J, K;

    if (argc >= 3 && !strcmp("G", argv[2]))
        I = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
    else
        I = imread(filename, CV_LOAD_IMAGE_COLOR);

    double t = (double)getTickCount();
    Sharpen(I, J);
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Hand written function times passed in seconds: " << t << endl;
	imwrite("sharpen.jpg", J);

    Mat kern = (Mat_<char>(3, 3) <<  0, -1,  0,
                                   -1,  5, -1,
                                    0, -1,  0);
    t = (double)getTickCount();
    filter2D(I, K, I.depth(), kern );
    t = ((double)getTickCount() - t) / getTickFrequency();
    cout << "Built-in filter2D time passed in seconds:      " << t << endl;
	imwrite("filter2d.jpg", K);

    return 0;
}

void Sharpen(const Mat& myImage,Mat& Result){
    CV_Assert(myImage.depth() == CV_8U);  // accept only uchar images

    const int nChannels = myImage.channels();
    Result.create(myImage.size(), myImage.type());

    for(int j = 1 ; j < myImage.rows - 1; ++j){
		// 以行为单位进行操作
        const uchar* previous = myImage.ptr<uchar>(j - 1);
        const uchar* current = myImage.ptr<uchar>(j);
        const uchar* next = myImage.ptr<uchar>(j + 1);

        uchar* output = Result.ptr<uchar>(j);
		/**
		 * 左和右需要通过 current[i -+ nChannels]
		 * 因为紧挨着i 的是同一列的其他通过, 而我们需要的是左右两列的相同通道
		 **/
        for(int i= nChannels;i < nChannels*(myImage.cols - 1); ++i){
            *output++ = saturate_cast<uchar>(5 * current[i] - current[i - nChannels] - current[i + nChannels] - previous[i] - next[i]);
        }
    }

	// 边界的处理, 统一设为0
    Result.row(0).setTo(Scalar(0));
    Result.row(Result.rows-1).setTo(Scalar(0));
    Result.col(0).setTo(Scalar(0));
    Result.col(Result.cols-1).setTo(Scalar(0));
}

