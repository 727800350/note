// 作用域分辨
#include <iostream>
#include <time.h>
using namespace std;

class B1{
public:
    int nV;
    void fun(){
        cout<<"member of B1 "<<nV<<endl;
    }
};

class B2{
public:
    int nV;
    void fun(){
        cout<<"member of B2 "<<nV<<endl;
    }
};

// 派生类会覆盖基类的
class D1: public B1, public B2{
public:
    int nV;
    void fun(){
        cout<<"member of D1 "<<nV<<endl;
    }
};

int main(){ 
    D1 d1;
    d1.nV = 1;
    d1.fun();
    d1.B1::nV = 2;
    d1.B1::fun();
    d1.B2::nV = 3;
    d1.B2::fun();

    return 0; 
}

/**
 * member of D1 1
 * member of B1 2
 * member of B2 3
**/
