/*
 * g++ -fPIC -shared util.cpp -o util.so -I/usr/include/python2.7 -lpython2.7
 * g++ -fPIC -shared util.cpp -o util.so -I/home/img/.jumbo/include/python2.7 -L/home/img/.jumbo/lib -lpython2.7
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
	sscanf(str, "%llu", &node.base64);
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "%u,%u", node.sign[0], node.sign[1]);
	return PyString_FromString(buffer);
}

static PyObject *sign32to64(PyObject *self, PyObject *args){
	char *str = NULL;
	if (!PyArg_ParseTuple(args, "s", &str)){
		return NULL;
	}

	node_t node;
	sscanf(str, "%u,%u", &node.sign[0], &node.sign[1]);
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "%llu", node.base64);
	return PyString_FromString(buffer);
}

/*
 * Methods exposed to Python env
 * fun in Python, fun in C++, arg, comment
 * this list always ends with {NULL, NULL, 0, NULL}
 */
static PyMethodDef Methods[] = {
	{"sign64to32", sign64to32, METH_VARARGS, "sign 64 to 32"},
	{"sign32to64", sign32to64, METH_VARARGS, "sign 32 to 64"},
	{NULL, NULL, 0, NULL}
};

/*
 * export function: initgreat_module.
 * this function name: 'init' + module name(which is also same as the file)
 */
PyMODINIT_FUNC initutil(void){
	(void) Py_InitModule("util", Methods);
}

