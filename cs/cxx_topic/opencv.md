# Mat
- mat.cols; 返回图像列数, 也就是图像的宽
- mat.rows; 返回图像行数, 也就是图像的高
- mat.size(): size是一个结构体,定义了Mat矩阵内数据的分布形式,数值上有关系式: `image.size().width==image.cols; image.size().height==image.rows`
- mat.channels(); 返回通道数. RGB 就是3, 而灰度图像只有一个灰度分量信息, 为1
- mat.depth(); 度量每一个像素中每一个通道的精度, depth数值越大,精度越高.
	mat.depth()得到的是一个0~6的数字,分别代表不同的位数,对应关系如下: `enum{CV_8U=0,CV_8S=1,CV_16U=2,CV_16S=3,CV_32S=4,CV_32F=5,CV_64F=6}`
- mat.dims: 矩阵的维度
- mat.elemSize():矩阵中每一个元素的数据大小(size_t类型), 以Byte 为单位, 例如:type是CV_16SC3,那么elemSize = 3 * 16 / 8 = 6 bytes
- mat.step: 可以理解为Mat矩阵中每一行的"步长", 以字节为基本单位,每一行中所有元素的字节总量,是累计了一行中所有元素,所有通道,所有通道的elemSize1之后的值
- mat.type: 矩阵的类型,包含有矩阵中元素的类型以及通道数信息
- mat.isContinuous(); 返回bool类型表示是否连续存储

- no longer need to manually allocate its memory and release it as soon as you do not need it. 
- basically a class with two data parts: 固定大小的matrix header and a pointer to the matrix containing the pixel values 
- using a reference counting system, each Mat object has its own header, however the matrix may be shared between two instance of them
	- can create headers which refer to only a subsection of the full data
	
		```C++
		Mat D (A, Rect(10, 10, 100, 100) ); // using a rectangle
		Mat E = A(Range::all(), Range(1,3)); // using row and column boundaries
		```

	- 由于默认的赋值操作, 数据部分是共享的, 通过clone() and copyTo() 可以把数据部分也复制
		```C++
		Mat B = A.clone();
		Mat C;
		A.copyTo(C);
		```

the OpenCV display system uses BGR colors.

`CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]`
`CV_8UC3`: unsigned char data type, three channels, so each pixel use 3 unsigned chars

# IO
图片在内存中的两种形式: Mat(C++ style), IplImage(C style).

- `Mat imread(const string& filename, int flags=1)`: loads an image from a file; If the image cannot be read, empty matrix (Mat::data==NULL) is returned.
	Flags specifying the color type of a loaded image:

	- CV_LOAD_IMAGE_COLOR(> 0)- If set, always convert image to the color one
	- CV_LOAD_IMAGE_GRAYSCALE(0) - If set, always convert image to the grayscale one
	- CV_LOAD_IMAGE_UNCHANGED(< 0) loads the image as is (including the alpha channel if present)
	- CV_LOAD_IMAGE_ANYDEPTH - If set, return 16-bit/32-bit image when the input has the corresponding depth, otherwise convert it to 8-bit.

- `IplImage *img = cvLoadImage("../test_imgs/Lena.jpg");Mat L(img);`: 通过C 的方式读入, 然后再转为Mat

- `bool imwrite(const string& filename, InputArray img, const vector<int>& params=vector<int>())`
	- img 可以是Mat 类型
	- params - This is a int vector to which you have to insert some int parameters specifying the format of the image
		- JPEG format: You have to puch_back CV_IMWRITE_JPEG_QUALITY first and then a number between 0 and 100 (higher is the better)
			
			```C++
			vector<int> params;  
		    params.push_back(CV_IMWRITE_JPEG_QUALITY);  
		    params.push_back(98); 
			```
		- PNG format: You have to puch_back CV_IMWRITE_PNG_COMPRESSION first and then a number between 0 and 9 (higher is the better compression, but slower)

			```C++
			vector<int> params;
		    params.push_back(CV_IMWRITE_PNG_COMPRESSION);
		    params.push_back(9);
			```

时间统计
```C++
double t = (double)getTickCount();
// do something ... 
t = ((double)getTickCount() - t) / getTickFrequency();
cout << "Times passed in seconds: " << t << endl;
```

Point: a 2D point, specified by its image coordinates x and y
```C++
Point pt;
pt.x = 10;
pt.y = 8;

Point pt =  Point(10, 8);
```

Scalar: a 4-element vector. The type Scalar is widely used in OpenCV for passing pixel values.
`Scalar(a, b, c)` could be defining a BGR color such as: Blue = a, Green = b and Red = c

# Transformation
- `image.convertTo(new_image, -1, alpha, beta);`: `new_image = alpha * image + beta`.
- `addWeighted(src1, alpha, src2, beta, r, dst);`: `dst = alpha * src1 + beta * src2 + r`,
	since we are adding src1 and src2, they both have to be of the same size (width and height) and type.

