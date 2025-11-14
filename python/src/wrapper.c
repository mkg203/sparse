#include "arg.h"
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <handler.h>

typedef struct {
  PyObject_HEAD ArgList* arglist;
  ArgList* tail;
  uint arr_len;
  const char** args_array;
} PyArgListObject;

static PyObject* SparseError = NULL;

PyObject* pyarglist_new(PyTypeObject* type, PyObject* args, PyObject* kwds) {
  PyArgListObject* self;

  self = (PyArgListObject*)type->tp_alloc(type, 0);

  if (self != NULL) {
    self->arglist = NULL;
  }
  return (PyObject*)self;
}

static int pyarglist_init(PyArgListObject* self, PyObject* args) {
  self->arglist = arglist_create();
  self->tail = self->arglist;
  return 0;
}

ArgType get_argtype(const char* pytype) {
  ArgType type = ARG_TYPE_DEFAULT;
  int len = strnlen(pytype, MAX_ARG_STRING_LEN);

  if (strncmp(pytype, "int", len) == 0) {
    type = ARG_TYPE_INT;
  } else if (strncmp(pytype, "string", len) == 0) {
    type = ARG_TYPE_STRING;
  } else if (strncmp(pytype, "float", len) == 0) {
    type = ARG_TYPE_FLOAT;
  } else if (strncmp(pytype, "bool", len) == 0) {
    type = ARG_TYPE_BOOL;
  }

  return type;
}

void cleanup(PyArgListObject* self) {
  if (self->arglist != NULL) {
    arglist_del(self->arglist);
  }

  if (self->args_array != NULL) {
    for (int i = 0; i < self->arr_len; i++) {
      if (self->args_array[i] != NULL) {
        free((void*)self->args_array[i]);
      }
    }
    free(self->args_array);
  }
}

void pyarglist_dealloc(PyArgListObject* self) {
  cleanup(self);
  Py_TYPE(self)->tp_free((PyObject*)self);
}

PyObject* pyarglist_add_arg(PyArgListObject* self, PyObject* args) {
  ArgType type = ARG_TYPE_DEFAULT;
  const char* pytype = NULL;
  bool pyrequired = false;

  const char* pyname;

  if (!PyArg_ParseTuple(args, "s|sb", &pyname, &pytype, &pyrequired)) {
    cleanup(self);
    PyErr_SetString(SparseError, "Error in parsing arguments");
    return NULL;
  }

  if (pytype != NULL) {
    type = get_argtype(pytype);
  }

  self->tail = arglist_add_arg(self->tail, pyname, pyrequired, type);

  return Py_None;
}

PyObject* pyarglist_parse(PyArgListObject* self, PyObject* args) {
  PyObject* string_list;

  if (!PyArg_ParseTuple(args, "O", &string_list)) {
    cleanup(self);
    PyErr_SetString(SparseError, "Error in parsing arguments");
    return NULL;
  }

  if (!PyList_Check(string_list)) {
    cleanup(self);
    PyErr_SetString(PyExc_TypeError, "Argument not of type `List`");
    return NULL;
  }

  int len = PyObject_Length(string_list);
  if (len < 0) {
    cleanup(self);
    PyErr_SetString(PyExc_Exception, "List length less than 0??");
    return NULL;
  }
  self->arr_len = len;

  self->args_array = (const char**)calloc(len, sizeof(char**));

  for (int i = 0; i < len; i++) {
    PyObject* item = PyList_GET_ITEM(string_list, i);
    if (!PyUnicode_Check(item)) {
      cleanup(self);
      PyErr_SetString(PyExc_TypeError, "Argument not of type `String`");
      return NULL;
    }
    const char* str = PyUnicode_AsUTF8(item);
    self->args_array[i] = strndup(str, MAX_ARG_STRING_LEN);

    // PyObject_Print(item, stdout, Py_PRINT_RAW);

    // printf("array element: %s\n", self->args_array[i]);
  }

  arglist_parse(self->arglist, self->args_array, len);

  return Py_None;
}

PyObject* pyarglist_arg_wrapper(Arg* arg) {
  void* value = arg_get_value(arg);
  // printf("value: %s\n", (char*) value);
  if (value == NULL) {
    return Py_None;
  }
  switch (arg_get_type(arg)) {
  case ARG_TYPE_BOOL:
    return PyBool_FromLong(*(bool*)value);

  case ARG_TYPE_INT:
    return PyLong_FromLong(*(int*)value);

  case ARG_TYPE_STRING:
    return PyUnicode_FromString((char*)value);

  case ARG_TYPE_FLOAT:
    return PyFloat_FromDouble(*(double*)value);

  default:
    Py_RETURN_NONE;
  }
}

PyObject* pyarglist_get_dict(PyArgListObject* self) {
  ArgList* arglist = self->arglist;
  PyObject* dict = PyDict_New();

  while (arglist_get_next(arglist) != NULL) {
    Arg* arg = arglist_get_arg(arglist);

    PyObject* key = PyUnicode_FromString(arg_get_name(arg));
    PyObject* value = pyarglist_arg_wrapper(arg);
    // PyObject_Print(key, stdout, Py_PRINT_RAW);
    // PyObject_Print(value, stdout, Py_PRINT_RAW);

    PyDict_SetItem(dict, key, value);
    // Py_DECREF(key);
    // Py_DECREF(value);

    arglist = arglist_get_next(arglist);
  }

  return dict;
}

static PyMethodDef PyArgList_methods[] = {
    {"add_argument", (PyCFunction)pyarglist_add_arg, METH_VARARGS,
     "Add an argument to the list."},

    {"to_dict", (PyCFunction)pyarglist_get_dict, METH_NOARGS,
     "Return all arguments as a Python dict."},

    {"parse", (PyCFunction)pyarglist_parse, METH_VARARGS,
     "Parse the arguments from a list"},

    {NULL}};

static PyTypeObject PyArgListType = {
    PyVarObject_HEAD_INIT(NULL, 0).tp_name = "sparse.Sparse",
    .tp_basicsize = sizeof(PyArgListObject),
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = pyarglist_new,
    .tp_init = (initproc)pyarglist_init,
    .tp_dealloc = (destructor)pyarglist_dealloc,
    .tp_methods = PyArgList_methods,
};

static PyModuleDef sparsemodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "sparse",
    .m_size = 0,
};

PyMODINIT_FUNC PyInit_sparse(void) {
  PyObject* m;

  if (PyType_Ready(&PyArgListType) < 0)
    return NULL;

  m = PyModule_Create(&sparsemodule);
  if (!m)
    return NULL;

  Py_INCREF(&PyArgListType);
  if (PyModule_AddObject(m, "Sparse", (PyObject*)&PyArgListType) < 0) {
    Py_DECREF(&PyArgListType);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}
