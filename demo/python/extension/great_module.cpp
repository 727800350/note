/*
 * g++ -fPIC -shared great_module.cpp -o great_module.so -I/usr/include/python2.7 -lpython2.7
 *
 * make sure great_module.so in the same directory
 * import great_module as gm
 * print gm.inc(2)
 *
 * ref:
 * https://docs.python.org/2/c-api/concrete.html
 * https://docs.python.org/2/c-api/arg.html
 */

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

/*
 * Methods exposed to Python env
 * fun in Python, fun in C++, arg, comment
 * this list always ends with {NULL, NULL, 0, NULL}
 */
static PyMethodDef Methods[] = {
	{"inc", inc, METH_VARARGS, "increment by one"},
	{"str_len", str_len, METH_VARARGS, "strlen"},
	{"offset", offset, METH_VARARGS, "offset a str"},
	{"sign64to32", sign64to32, METH_VARARGS, "sign 64 to 32"},
	{NULL, NULL, 0, NULL}
};

/*
 * export function: initgreat_module.
 * this function name: 'init' + module name(which is also same as the file)
 */
PyMODINIT_FUNC initgreat_module(void){
	(void) Py_InitModule("great_module", Methods);
}

