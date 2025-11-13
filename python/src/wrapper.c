#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <handler.h>

typedef struct {
  PyObject_HEAD;
  ArgList* arglist;
  ArgList* tail;
} PyArgListObject;

PyObject* pyarglist_new(PyTypeObject* type, PyObject* args) {
  PyArgListObject* self;

  self = (PyArgListObject*)type->tp_alloc(type, 0);

  if (self != NULL) {
    self->arglist = NULL;
  }
  return (PyObject*)self;
}

static int pyarglist_init(PyArgListObject* self, PyObject* args) {
  self->arglist = arglist_create();
  self->tail = arglist_create();
  return 0;
}

ArgType get_argtype(const char* pytype) {
  ArgType type = ARG_TYPE_DEFAULT;
  int len = strnlen(pytype, MAX_ARG_STRING_LEN);

  if (strncmp(pytype, "int", MAX_ARG_STRING_LEN) == 0) {
    type = ARG_TYPE_INT;
  }
  else if (strncmp(pytype, "string", MAX_ARG_STRING_LEN) == 0) {
    type = ARG_TYPE_STRING;
  }
  else if (strncmp(pytype, "float", MAX_ARG_STRING_LEN) == 0) {
    type = ARG_TYPE_FLOAT;
  }
  else if (strncmp(pytype, "bool", MAX_ARG_STRING_LEN) == 0) {
    type = ARG_TYPE_BOOL;
  }

  return type;
}

PyObject* pyarglist_add_arg(PyArgListObject* self, PyObject args) {
  ArgType type = ARG_TYPE_DEFAULT;
  const char* pytype = NULL;
  bool pyrequired = false;

  const char* pyname;
  
  if (!PyArg_ParseTuple(&args, "s|sb", &pyname, &pytype, &pyrequired)) {
    return NULL;
  }

  if (pytype != NULL) {
    type = get_argtype(pytype);
  }

  self->tail = arglist_add_arg(self->tail, pyname, pyrequired, type);
  
  return Py_None;
}
