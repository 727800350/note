#include <iostream>
#include <time.h>
using namespace std;

class B0{
public:
    void display(){
        cout<<"B0::display()"<<endl;
    }
};

class B1:public B0{
public:
    void display(){
        cout<<"B1::display()"<<endl;
    }
};

class B2:public B0{
public:
    void display(){
        cout<<"B2::display()"<<endl;
    }
};

int main(){ 
    B0 b0;
	b0.display();

    B1 b1;
	b1.display();

    B2 b2;
	b2.display();

// 	B0是B1 的基类, 也就是说B1中包含B0的成员, 所以可以把B1的实例赋值给B0的实例, 也就是说将B1 的实例当成B0使用
    b0 = b1;
	b0.display();

    b0 = b2;
	b0.display();
    return 0; 
}

/**
 * output:
 *
 * B0::display()
 * B1::display()
 * B2::display()
 * B0::display()
 * B0::display()
**/

