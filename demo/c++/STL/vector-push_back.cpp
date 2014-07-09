#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

class Point{
public:
        Point(){
            cout << "construction" << endl;
        }
        Point(const Point& p){
            cout << "copy construction" << endl;
        }
        ~Point(){
            cout << "destruction" << endl;
        }
};

int main(){
    vector<Point> pointVec;
    Point a;
    Point b;

// 	pointVec.reserve(4);

    pointVec.push_back(a);
    cout<<pointVec.size()<<std::endl;
    cout<<pointVec.capacity()<<std::endl;

    pointVec.push_back(b);
    cout<<pointVec.size()<<std::endl;
    cout<<pointVec.capacity()<<std::endl;

    return 0;
}

/*************************************
[eric@iis STL]$ ./a.out
construction
construction
copy construction
1
1
copy construction
copy construction
destruction
2
2
destruction
destruction
destruction
destruction

+++++++++++++++++++++++++++++++++++++
分析
Point a;
Point b;
两个construction

pointVec.push_back(a);
将a插入到vector中, copy一次

pointVec.push_back(b);
将b插入到vector中, 但是发现vector 空间不够, 自动增长, 并把原来的a copy到新的vector中
在把b copy到新的vector 中
所以共三次copy

一共新建了5个point 对象, 所以一共有5 个 destruction

========================================
加上 pointVec.reserve(4);之后的运行结果

construction
construction
copy construction
1
4
copy construction
2
4
destruction
destruction
destruction
destruction
*************************************/
