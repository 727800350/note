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

class B1:virtual public B0{
public:
    int nV1;
	B1(){
		cout << "constructing B1" << endl;
	};
	~B1(){
		cout << "destructing B1" << endl;
	};
};

class B2:virtual public B0{
public:
    int nV2;
	B2(){
		cout << "constructing B2" << endl;
	};
	~B2(){
		cout << "destructing B2" << endl;
	};
};

class D1:public B1, public B2{
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
    d1.nV = 2;
    d1.fun();

    return 0; 
}

/**
 * constructing B0
 * constructing B1
 * constructing B2
 * constructing D1
 * member of B0 2
 * destructing D1
 * destructing B2
 * destructing B1
 * destructing B0
 **/
