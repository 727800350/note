/**
 * 生成python 能够调用的库
 * g++ -fPIC -shared great_module.cpp -o great_module.so -I/usr/include/python2.7 -lpython2.7
 *
 * python 使用, 需要前面生成的great_module.so 在当前文件夹下
 * import great_module as gm
 * print gm.great_function(2)
 **/

#include <string.h>
#include <Python.h>

/** 
 * 实际的函数
 **/
int inc(int a){
	return a + 1;
}

/**
 * 包裹函数_great_function.
 * 它负责将Python的参数转化为C的参数(PyArg_ParseTuple),
 * 调用实际的great_function,并处理great_function的返回值,最终返回给Python环境.
 **/
static PyObject *_inc(PyObject *self, PyObject *args){
	int a = 0;
	if (!PyArg_ParseTuple(args, "i", &a)){
		return NULL;
	}
	int res = inc(a);
	return PyInt_FromLong(res);
}

int str_len(char *str){
	return strlen(str);
}

static PyObject *_str_len(PyObject *self, PyObject *args){
	char *str = NULL;
	if (!PyArg_ParseTuple(args, "s", &str)){
		return NULL;
	}
	int res = str_len(str);
	return PyInt_FromLong(res);
}

/**
 * 导出表GreateModuleMethods.
 * 它负责告诉Python这个模块里有哪些函数可以被Python调用.
 * 导出表的名字可以随便起,每一项有4个参数:Python环境的函数名称;_great_function,即包裹函数;参数变长;说明性的字符串.
 * 导出表总是以{NULL, NULL, 0, NULL}结束.
 **/
static PyMethodDef GreateModuleMethods[] = {
	{"inc", _inc, METH_VARARGS, "increment by one"},
	{"str_len", _str_len, METH_VARARGS, "strlen"},
	{NULL, NULL, 0, NULL}
};

/** 
 * 导出函数initgreat_module.
 * 这个的名字不是任取的,是你的module名称(文件名称要与之一致)添加前缀init.
 * 导出函数中将模块名称与导出表进行连接.
 **/
PyMODINIT_FUNC initgreat_module(void){
	(void) Py_InitModule("great_module", GreateModuleMethods);
}

