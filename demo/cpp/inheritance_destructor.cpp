#include <iostream>
#include <time.h>
using namespace std;

class B1{
public:
    B1(int i){
        cout<<"constructing B1 "<<i<<endl;
    }
    ~B1(){
        cout<<"destructing B1"<<endl;
    }
};

class B2{
public:
    B2(int j){
        cout<<"constructing B2 "<<j<<endl;
    }
    ~B2(){
        cout<<"destructing B2"<<endl;
    }
};

class B3{
public:
    B3(){
        cout<<"constructing B3"<<endl;
    }
    ~B3(){
        cout<<"destructing B3"<<endl;
    }
};

class C: public B2, public B1, public B3{
public:
    C(int a, int b, int c, int d):B1(a), memberB2(d), memberB1(c),B2(b){
		cout << "constructing C" << endl;
	}
	~C(){
		cout << "destructing C" << endl;
	}
private:
    B1 memberB1;
    B2 memberB2;
    B3 memberB3;
};

int main(){ 
    C obj(1,2,3,4);

    return 0; 
}

/**
 * output:
 *
 * constructing B2 2
 * constructing B1 1
 * constructing B3
 * constructing B1 3
 * constructing B2 4
 * constructing B3
 * constructing C
 * destructing C
 * destructing B3
 * destructing B2
 * destructing B1
 * destructing B3
 * destructing B1
 * destructing B2
 *
 * 和构造的顺序相反, 所以先是
 * 1. 析构函数体
 * destructing C
 *
 * 2. 内成员变量, 逆顺序, 所以依次为: memberB3, memberB2, memberB1
 * destructing B3
 * destructing B2
 * destructing B1
 *
 * 3. 基类的析构函数, 逆顺序, 所以依次为: B3, B1, B2
 * destructing B3
 * destructing B1
 * destructing B2
**/

