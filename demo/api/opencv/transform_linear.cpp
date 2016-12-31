#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace cv;

double alpha; /**< Simple contrast control */
int beta;  /**< Simple brightness control */

int main(int argc, char** argv){
	Mat image = imread(argv[1]);
	Mat new_image = Mat::zeros(image.size(), image.type());

	/// Initialize values
	std::cout<<" Basic Linear Transforms "<<std::endl;
	std::cout<<"-------------------------"<<std::endl;
	std::cout<<"* Enter the alpha value [1.0-3.0]: ";std::cin>>alpha;
	std::cout<<"* Enter the beta value [0-100]: "; std::cin>>beta;

	/// Do the operation new_image(i,j) = alpha*image(i,j) + beta
	for(int y = 0; y < image.rows; y++){
		for(int x = 0; x < image.cols; x++){
			for(int c = 0; c < image.channels(); c++){
	    		new_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(alpha*(image.at<Vec3b>(y,x)[c]) + beta);
			}
		}
	}

	/// Show stuff
	imwrite("new.jpg", new_image);

	return 0;
}

/**
 * Instead of using the for loops to access each pixel, we could have simply used this command:
 * image.convertTo(new_image, -1, alpha, beta);
 **/

