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

ArgList* arglist_add_arg(ArgList* arglist, char* name, bool required,
                         ArgType type) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_set_type(arg, type);
  arglist->arg = arg;

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
