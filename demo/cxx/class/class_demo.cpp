#include <stdio.h>

namespace wcg{
class Point{
public:
	Point(){};

	Point(int x, int y);

	~Point(){};

	void set_values(int x, int y);
	int area();
	Point operator+(Point b);

private:
	int _x;
	int _y;
};

Point::Point(int x, int y){
	_x = x;
	_y = y;
}

void Point::set_values(int x, int y){
	_x = x;
	_y = y;
}

int Point::area(){
	return _x * _y;
}

Point Point::operator+(Point b){
	Point res;
	res._x = this->_x + b._x;
	res._y = this->_y + b._y;
	return res;
}

}

int main(){
	wcg::Point point;
	point.set_values(3, 4);
	fprintf(stdout, "area:%d\n", point.area());

	return 0;
}

