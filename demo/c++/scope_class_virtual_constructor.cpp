#include <iostream>
#include <time.h>
using namespace std;

class B0{
public:
    B0(int n){
        nV = n;
    }
    int nV;
    void fun(){
        cout<<"member of B0 "<<nV<<endl;
    }
};

class B1:virtual public B0{
public:
    B1(int a):B0(a){ }
    int nV1;
};

class B2:virtual public B0{
public:
    B2(int a):B0(a){ }
    int nV2;
};

class D1:public B1, public B2{
public:
    D1(int a):B0(a), B1(a), B2(a) { }
    int nVd;
    void fund(){
        cout<<"member of D1"<<endl;
    }
};

int main(){ 
    D1 d1(1);
    d1.nV = 2;
    d1.fun();

    return 0; 
}
