#include <iostream>
#include <time.h>
using namespace std;

class B0{
public:
    int nV;
	B0(){
		cout << "constructing B0" << endl;
	};
	~B0(){
		cout << "destructing B0" << endl;
	};
    void fun(){
        cout<<"member of B0 "<<nV<<endl;
    }
};

class B1:public B0{
public:
    int nV1;
	B1(){
		cout << "constructing B1" << endl;
	};
	~B1(){
		cout << "destructing B1" << endl;
	};
};

class B2:public B0{
public:
    int nV2;
	B2(){
		cout << "constructing B2" << endl;
	};
	~B2(){
		cout << "destructing B2" << endl;
	};
};

class D1:public B1, public B2 {
public:
    int nVd;
	D1(){
		cout << "constructing D1" << endl;
	};
	~D1(){
		cout << "destructing D1" << endl;
	};
    void fund(){
        cout<<"member of D1"<<endl;
    }
};

int main(){ 
    D1 d1;
    d1.B1::nV = 2;
    d1.B1::fun();
    d1.B2::nV = 3;
    d1.B2::fun();

    return 0; 
}

/**
 * constructing B0
 * constructing B1
 * constructing B0
 * constructing B2
 * constructing D1
 * member of B0 2
 * member of B0 3
 * destructing D1
 * destructing B2
 * destructing B0
 * destructing B1
 * destructing B0
 *
 * 调用B1的构造函数, 但是B1 又要调用B0的构造函数, 所以输出
 * constructing B0
 * constructing B1
 * 调用B2的构造函数, 但是B2 也要调用B0的构造函数, 所以输出
 * constructing B0
 * constructing B2
 * 最后调用D1 的构造函数,  输出
 * constructing D1
 *
 * 在这种情况下,派生类对象在内存中就同时拥有成员nV及fun的两份拷贝.
 * 但是很多情况下,我们只需要这样一个这样的数据拷贝,同一成员的多份拷贝增加了内存的开销.
 * 可以通过虚函数来解决这个问题
 **/
