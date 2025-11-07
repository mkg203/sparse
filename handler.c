#include "arg.c"

typedef struct ArgList {
  Arg* arg;
  bool required;
  struct ArgList* next;
} ArgList;

ArgList* arglist_create() {
  ArgList* arglist = calloc(1, sizeof(ArgList));

  return arglist;
}

ArgList* arglist_add_arg(ArgList* arglist, char* name, bool required, void* value,
                     ArgType type) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arglist->arg = arg;

  switch (type) {
  default:
    break;
  case ARG_TYPE_BOOL: {
    arg_init_bool(arg, *(bool*)value);
    break;
  }
  case ARG_TYPE_INT: {
    arg_init_int(arg, *(int*)value);
    break;
  };
  case ARG_TYPE_FLOAT: {
    arg_init_float(arg, *(double*)value);
    break;
  };
  case ARG_TYPE_STRING: {
    arg_init_string(arg, (char*)value);
    break;
  };
  }

  return arglist->next;
}

ArgList* arglist_add_int(ArgList* arglist, char* name, bool required,
                         int value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_int(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_add_string(ArgList* arglist, char* name, bool required,
                            char* value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_string(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_add_float(ArgList* arglist, char* name, bool required,
                           float value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_float(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

void arglist_del(ArgList* head) {
  ArgList* next;
  while (head != NULL) {
    next = head->next;
    arg_del(head->arg);
    free(head);
    head = next;
  }
}

void print_args(ArgList* arglist) {
  while (arglist->next != NULL) {
    print_arg(arglist->arg);

    arglist = arglist->next;
  }
}


int main(int argc, char* argv[]) {
  ArgList* args = arglist_create();
  ArgList* tail = args;
  int64_t value = 5;
  double fl = 3.141592;
  char* test = "HELLO WORLD";
  bool bluesky = true;

  tail = arglist_add_arg(tail, "int test", 1, &value, ARG_TYPE_INT);
  tail = arglist_add_arg(tail, "float test", 1, &fl, ARG_TYPE_FLOAT);
  tail = arglist_add_arg(tail, "string test", 1, test, ARG_TYPE_STRING);
  tail = arglist_add_arg(tail, "bool test", 1, &bluesky, ARG_TYPE_BOOL);
  // tail = arglist_add_int(tail, "test", 1, 5);
  // tail = arglist_add_string(tail, "test", 1, "big test here");
  // tail = arglist_add_float(tail, "test", 1, 5.32);

  print_args(args);
  arglist_del(args);
}
