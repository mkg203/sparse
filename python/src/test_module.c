#include <Python.h>

static PyObject* test(PyObject* self, PyObject* args) {
  const char* string;

  if (!PyArg_ParseTuple(args, "s", &string)) {
    return NULL;
  }

  printf("%s", string);

  Py_IncRef(Py_None);
  return Py_None;
}

static PyMethodDef test_methods[] = {
    {"test", test, METH_VARARGS, "Print a string"}, {NULL, NULL, 0, NULL}};

static struct PyModuleDef test_module = {PyModuleDef_HEAD_INIT, "test", NULL, 0,
                                         test_methods};

PyMODINIT_FUNC PyInit_test(void) { return PyModuleDef_Init(&test_module); }
