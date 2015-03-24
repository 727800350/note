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

void fun(B0 *ptr){
    ptr->display();
}

int main(){ 
    B0 b0;
    B1 b1;
    B2 b2;
    fun(&b0);
    b0 = b1;
    fun(&b0);
    b0 = b2;
    fun(&b0);

    return 0; 
}

