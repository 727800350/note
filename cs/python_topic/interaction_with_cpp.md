它常被昵称为胶水语言,能够把用其他语言制作的各种模块(尤其是C/C++)很轻松地联结在一起.

[如何实现 C/C++ 与 Python 的通信?](http://www.zhihu.com/question/23003213/answer/56121859)

```C++
// wcg.cpp
#include <string.h>
#include <Python.h>

// int return int
static PyObject *inc(PyObject *self, PyObject *args){
	int a = 0;
	if (!PyArg_ParseTuple(args, "i", &a)){
		return NULL;
	}
	int res = a + 1;
	return PyInt_FromLong(res);
}

// str return int
static PyObject *str_len(PyObject *self, PyObject *args){
	char *str = NULL;
	if (!PyArg_ParseTuple(args, "s", &str)){
		return NULL;
	}
	int res = strlen(str);
	return PyInt_FromLong(res);
}

// str return str
static PyObject *offset(PyObject *self, PyObject *args){
	char *str = NULL;
	if (!PyArg_ParseTuple(args, "s", &str)){
		return NULL;
	}
	char *res = NULL;
	if(str != NULL){
		res = str + 1;
	}
	return PyString_FromString(res);
}

/* tuple example */
typedef union _node_t{
	uint32_t sign[2];
	uint64_t base64;
}node_t;

static PyObject *sign64to32(PyObject *self, PyObject *args){
	char *str = NULL;
	if (!PyArg_ParseTuple(args, "s", &str)){
		return NULL;
	}

	node_t node;
	sscanf(str, "%lu", &node.base64);
	return Py_BuildValue("(II)", node.sign[0], node.sign[1]);

	/* another way to build a tuple
	PyObject *pack = PyTuple_New(2);
	PyObject *cs1 = PyInt_FromLong(node.sign[0]);
	PyObject *cs2 = PyInt_FromLong(node.sign[1]);
	PyTuple_SetItem(pack, 0, cs1);
	PyTuple_SetItem(pack, 1, cs2);
	*/
}

/* Methods exposed to Python env
 * {fun in Python, fun in C++, arg, comment}
 * this list always ends with {NULL, NULL, 0, NULL}
 */
static PyMethodDef Methods[] = {
	{"inc", inc, METH_VARARGS, "increment by one"},
	{"str_len", str_len, METH_VARARGS, "strlen"},
	{"offset", offset, METH_VARARGS, "offset a str"},
	{"sign64to32", sign64to32, METH_VARARGS, "sign 64 to 32"},
	{NULL, NULL, 0, NULL}
};

/* export function: initwcg.
 * this function name: 'init' + module name(which is also same as the file)
 */
PyMODINIT_FUNC initwcg(void){
	(void) Py_InitModule("wcg", Methods);
}
```
编译成一个动态库: `g++ -fPIC -shared wcg.cpp -o wcg.so -I/usr/include/python2.7 -lpython2.7`

Python 使用
```Python
import wcg

print wcg.inc(2) ## 3
print wcg.str_len('abcd') ## 4
print wcg.offset('abcd') ## bcd
print wcg.sign64to32('1234567890123456') ## (1015724736, 287445)
```

- `PyArg_ParseTuple`: 解析参数
- `Py_BuildValue`: 构建return value, 具体的参数说明参见[Parsing arguments and building values](https://docs.python.org/2/c-api/arg.html)

## C call Python
```C++
// c_call_python_simple.c
#include <Python.h>
int main(int argc, char *argv[]){
	Py_SetProgramName(argv[0]);
	Py_Initialize();
	PyRun_SimpleString("print 'Hello Python!'\n");
	Py_Finalize();
	return 0;
}
```
编译生成可执行文件: `gcc c_call_python_simple.c -I/usr/include/python2.7/ -lpython2.7`

虽然非常简单,但这段代码除了能用C语言动态生成一些Python代码之外,并没有什么用处.我们需要的是C语言的数据结构能够和Python交互.

下面举个例子,比如说,有一天我们用Python写了一个功能特别强大的函数:
```python
## great_module.py
def great_function(a):
	return a + 1
```
接下来要把它包装成C语言的函数.我们期待的C语言的对应函数应该是这样的:
```C++
#include <Python.h>
#include <assert.h>

int great_function_from_python(int a){
	int res;
	PyObject *pModule = NULL ;
	PyObject *pFunc = NULL;
	PyObject *pArgs = NULL;
	PyObject *pValue = NULL;

	/* import
	 * great_module.py must be in the current directory
	 */
	pModule = PyImport_Import(PyString_FromString("great_module"));
	assert(pModule != NULL);

	/* great_module.great_function */
	pFunc = PyObject_GetAttrString(pModule, "great_function");
	assert(pFunc != NULL);

	/* build args */
	pArgs = PyTuple_New(1); // 创建一个Python 对象
	assert(pArgs != NULL);
	PyTuple_SetItem(pArgs, 0, PyInt_FromLong(a)); // 若a 是Tuple, 则a[i] = b对应于 PyTuple_SetItem(a, i, b), 有理由相信还有一个函数PyTuple_GetItem完成取得某一项的值.

	/* call */
	pValue = PyObject_CallObject(pFunc, pArgs);
	assert(pValue != NULL);

	res = PyInt_AsLong(pValue);
	return res;
}

int main(int argc, char *argv[]){
	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('./')");
	fprintf(stdout, "%d\n", great_function_from_python(2));
	Py_Finalize();
	return 0;
}
```
编译生成可执行文件: `g++ test.cpp -I/usr/include/python2.7/ -lpython2.7`

