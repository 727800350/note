#include <vector>
#include <glog/logging.h>

class Point{
public:
	Point(){
		LOG(INFO) << "default constructor";
	}
	Point(const Point& p){
		LOG(INFO) << "copy constructor";
	}
	~Point(){
		LOG(INFO) << "default destructor";
	}
};

int main(int argc, char* argv[]){
	google::InitGoogleLogging(argv[0]);
	FLAGS_alsologtostderr = true;
	FLAGS_colorlogtostderr = true;

	std::vector<Point> pointVec;
	Point a;
	Point b;

	pointVec.reserve(4);
	/* 即使是引用类型的参数, 还是会把a 整个对象copy 一份, 再push 到vector 里面去
	 * 因此如果这里就return, 一共会有3次析构
	 */
	pointVec.push_back(a);
	pointVec.push_back(a);
	LOG(INFO) << pointVec.size() << " " << pointVec.capacity();

	/* erase the first element
	 * 因为vector 中的元素在push进去的时候是copy 的, 所以在erase 的时候, 会自动调用对象的析构
	 */
	pointVec.erase(pointVec.begin());

	/* call swap to truely clear the vector
	 * std::vector<Point>() 会产生一个std::vector<Point> 对象, 因为没有名字, 所以是一个临时对象, 所以到了下一个语句, 就出了作用域, 会被自动销毁.
	 * 在将pointVec 与临时对象交换后, 借助临时对象的销毁来销毁pointVec 对象
	 */
	std::vector<Point>().swap(pointVec);
	LOG(INFO) << pointVec.size() << " " << pointVec.capacity();

	google::ShutdownGoogleLogging();
	return 0;
}

