#include <iostream>
#include <time.h>
using namespace std;

class B1{
public:
    B1(int i){
        cout<<"constructing B1 "<<i<<endl;
    }
};

class B2{
public:
    B2(int j){
        cout<<"constructing B2 "<<j<<endl;
    }
};

class B3{
public:
    B3(){
        cout<<"constructing B3"<<endl;
    }
};

class C: public B2, public B1, public B3{
public:
    C(int a, int b, int c, int d):B1(a), memberB2(d), memberB1(c),B2(b){
		cout << "constructing C" << endl;
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
 *
 * 1. 先是基类构造函数, 从左到右, 基类的声明顺序为: public B2, public B1, public B3, 所以先是 B2, 然后是B1, 最后是B3
 * 所以首先是B2(b = 2), 输出 constructing B2 2
 * 然后是B1(a = 1), 输出 constructing B1 1
 * 最后是B3, constructing B3
 *
 * 2. 然后是内嵌成员对象的构造函数, 三个内嵌成员对象的顺序为: B1 memberB1; B2 memberB2; B3 memberB3;
 * 所以依次构造 memberB1(c = 3), memberB2(d = 4), memberB3,
 * 输出
 * constructing B1 3
 * constructing B2 4
 * constructing B3
 *
 * 3. 构造函数体
 * constructing C
 **/
