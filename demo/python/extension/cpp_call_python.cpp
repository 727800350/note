#include <Python.h>
#include <assert.h>

int great_function_from_python(int a){
	int res;
	PyObject *pModule = NULL ;
	PyObject *pFunc = NULL;
	PyObject *pArgs = NULL;
	PyObject *pValue = NULL;
	
	/* import */
// 	great_module.py must be in the current directory
	pModule = PyImport_Import(PyString_FromString("great_module"));
	assert(pModule != NULL);

	/* great_module.great_function */
	pFunc = PyObject_GetAttrString(pModule, "great_function"); 
	assert(pFunc != NULL);
	
	/* build args */
	pArgs = PyTuple_New(1);
	assert(pArgs != NULL);
	PyTuple_SetItem(pArgs,0, PyInt_FromLong(a));
	  
	/* call */
	pValue = PyObject_CallObject(pFunc, pArgs);
	assert(pValue != NULL);
	
	res = PyInt_AsLong(pValue);
	return res;
}

int main(int argc, char *argv[]){
	Py_Initialize();
	printf("%d\n",great_function_from_python(2));
	Py_Finalize();
	return 0;
}

