#include <highgui.h>
#include <cv.h>
#include <iostream>

/**
 * note:
 * OpenCV只支持avi的格式, 而且生成的视频文件不能大于2GB, 而且不能添加音频.
 * 如果你想突破这些限制, 建议最好还是看看ffMpeg, 而不是浪费时间在OpenCV上.
 * OpenCV是一个强大的计算机视觉库,而不是视频流编码器或者解码器.
 **/

std::string int2string(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main(int argc, char* argv[]){
	cv::VideoCapture cap(0); // open the video camera no. 0
	if(!cap.isOpened()){
		std::cerr << "error initializing video capture" << std::endl;
		return -1;
	}


	std::string filename = "./myvideo.avi";
	int fcc = CV_FOURCC('D', 'I', 'V', '3');
	int fps = 20;
	cv::Size framesize(cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cv::VideoWriter writer = cv::VideoWriter(filename, fcc, fps, framesize);
	if(!writer.isOpened()){
		std::cerr << "writer opened error" << std::endl;
		return -1;
	}

	std::string windowName = "Webcam Feed";
	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE); //create a window to display our webcam feed

	cv::Mat frame;
	while(1){
		bool bSuccess = cap.read(frame); // read a new frame from camera feed
		if(!bSuccess){
			std::cerr << "error reading frame from camera feed" << std::endl;
			break;
		}
		
		writer.write(frame);
		cv::putText(frame, "REC", cv::Point(0, 60), 2, 2, cv::Scalar(0,0,255), 2);
		cv::imshow(windowName, frame);

		//listen for 10ms for a key to be pressed
		switch(cv::waitKey(10)){
			case 27:
				//'esc' has been pressed (ASCII value for 'esc' is 27), exit program.
				return 0;
		}
	}
	writer.release();

	return 0;
}

