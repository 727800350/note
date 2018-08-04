#include <stdio.h>

/*
 * https://blog.csdn.net/weiwangchao_/article/details/16984213
 * https://stackoverflow.com/questions/3982470/what-does-typedef-void-something-mean
 */

class Closure{
public:
	virtual ~Closure(){}
	virtual void Run() {}

protected:
	Closure(){}
};

template<class T>
class Callback0 : public Closure{
public:
	typedef void (T::*Done)(); // Done 是一个属于Class T 的成员函数指针, void T::fun()
	
public:
	Callback0(T *obj, Done run) : object_(obj), run_(run) {
	}

	virtual void Run(){
		(object_->*run_)();
	}

private:
	T *object_;
	Done run_;
};

template<class T, class T1>
class Callback1 : public Closure {
public:
	typedef void (T::*Done)(T1); // Done 是一个属于Class T 的成员函数指针, void T::fun(T1 arg)
	
public:
	Callback1(T *obj, Done run, T1 arg) : object_(obj), run_(run), arg0_(arg){
	}

	virtual void Run(){
		(object_->*run_)(arg0_);
	}

private:
	T *object_;
	Done run_;
	T1 arg0_;
};


template<class T>
Closure* NewCallback(T *obj, void (T::*member)()){
	return new Callback0<T>(obj, member);
}

template<class T, class T1>
Closure* NewCallback(T *obj, void (T::*member)(T1), T1 P){
	return new Callback1<T, T1>(obj, member, P);
}

class Test{
public:
	void Run0(){
		fprintf(stdout, "in Test::Run0\n");
	}

	void Run1(int i){
		fprintf(stdout, "in Test::Run1 %d\n", i);
	}
};

int main(){
	Test test;

	Closure *callback0 = NewCallback(&test, &Test::Run0);
	callback0->Run();
	delete callback0;

	Closure *callback1 = NewCallback(&test, &Test::Run1, 2);
	callback1->Run();
	delete callback1;

	return 0;
}

