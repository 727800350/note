#include <iostream>
#include <typeinfo>
#include <vector>
#include <stdio.h>
using namespace std;

int main(){
	cout<<"For integer:"<<endl;
	vector<int> v;
	int *a = new int(10);
	int &ra = *a;
	v.push_back(ra);
	cout <<*a <<endl;
	cout <<"before a deleted: "<<ra <<endl;
	cout <<"size: "<<v.size() <<"\tvalue: "<<v.back()<<endl;
	delete a;
	cout <<"after a deleted: "<<ra <<endl;
	cout <<"size: "<<v.size() <<"\tvalue: "<<v.back()<<endl;
	cout <<endl;

	cout<<"For string:"<<endl;
	vector<string> vs;
	string *s = new string("string value");
	string &rs = *s;
	vs.push_back(rs);
	cout <<*s <<endl;
	cout <<"before s deleted: "<<rs <<endl;
	cout <<"size: "<<vs.size() <<"\tvalue: "<<vs.back()<<endl;
	delete s;
	cout <<"after a deleted: "<<endl;
	cout <<"size: "<<vs.size() <<"\tvalue: "<<vs.back()<<endl;
// 	由于s的内存被释放, 所以下面的尝试打印出字符串出错
// 	cout <<*s <<endl;
	cout <<endl;

	cout<<"For void *:"<<endl;
	vector<void *> vp;
	int *p1 = new int(10);
	string *p2 = new string("string value");
	vp.push_back(p1);
	vp.push_back(p2);
	cout <<"before memory deleted: "<<endl;
	cout <<"size: "<<vp.size() <<"\tvalue: "<<*(int *)vp.front()<<endl;
	cout <<"size: "<<vp.size() <<"\tvalue: "<<*(string *)vp.back()<<endl;
	printf("pointed address of p1: %p\n",p1);
	printf("pointed address of p2: %p\n",p2);
	cout <<"*p1 = "<<*p1<<endl;
	delete p1;
	delete p2;
	cout <<"after memo deleted: "<<endl;
	cout <<"size: "<<vp.size() <<"\tvalue: "<<*(int *)vp.front()<<endl;
// 	由于p2的内存被释放, 所以下面的尝试打印出字符串出错
// 	cout <<"size: "<<vp.size() <<"\tvalue: "<<*(string *)vp.back()<<endl;
	printf("pointed address of p1: %p\n",p1);
	printf("pointed address of p2: %p\n",p2);
	cout <<"*p1 = "<<*p1<<endl;
	return 0;
}
