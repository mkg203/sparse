#include "handler.h"
#include "arg.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ArgList {
  uint id;
  Arg* arg;
  bool required;
  struct ArgList* next;
};

ArgList* arglist_create() {
  ArgList* arglist = calloc(1, sizeof(ArgList));

  return arglist;
}

ArgList* arglist_add_arg(ArgList* arglist, const char* name, bool required,
                         ArgType type) {
  arglist->next = arglist_create();
  arglist->required = required;
  arglist->next->id = arglist->id + 1;

  Arg* arg = arg_create(name);
  arg_set_type(arg, type);
  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_add_int(ArgList* arglist, const char* name, bool required,
                         int value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_int(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_add_string(ArgList* arglist, const char* name, bool required,
                            const char* value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_string(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_add_float(ArgList* arglist, const char* name, bool required,
                           float value) {
  arglist->next = arglist_create();
  arglist->required = required;

  Arg* arg = arg_create(name);

  arg_init_float(arg, value);

  arglist->arg = arg;

  return arglist->next;
}

ArgList* arglist_get_next(ArgList* arglist) {
  if (arglist == NULL) {
    return NULL;
  }
  return arglist->next;
}

Arg* arglist_get_arg(ArgList* arglist) {
  if (arglist == NULL) {
    return NULL;
  }
  return arglist->arg;
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
    printf("Arg ID: %u\n", arglist->id);
    print_arg(arglist->arg);
    // __builtin_dump_struct(arglist->arg, &printf);

    arglist = arglist->next;
  }
}

int arglist_count(ArgList* arglist) {
  int count = 0;
  while (arglist->next != NULL) {
    count++;
    arglist = arglist->next;
  }

  return count;
}

int arglist_update_arg(Arg* arg, char* argv[], int argc) {
  const char* name = NULL;
  if (!(name = arg_get_name(arg))) {
    fprintf(stderr, "Error getting argument name from ArgList");
    return 1;
  }
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] != '-') {
      continue;
    }

    char* temp = argv[i];
    int len = strnlen(name, MAX_ARG_STRING_LEN);

    if (len <= 1) {
      continue;
    }

    char argname[len + 1];
    memcpy(argname, &temp[1], len);
    argname[len] = '\0';

    if (strncasecmp(name, argname, MAX_ARG_STRING_LEN) == 0) {
      const char* value = (i + 1 < argc) ? argv[i + 1] : NULL;
      return arg_set_value(arg, value);
    }
  }
  return 1;
}

void arglist_parse(ArgList* arglist, char* argv[], int argc) {
  while (arglist->next != NULL) {
    if (arglist_update_arg(arglist->arg, argv, argc) && arglist->required) {
      fprintf(stderr, "Error in setting required Argument: %s\n",
              arg_get_name(arglist->arg));
      exit(1);
    }
    arglist = arglist->next;
  }
}

/**
 * Casting:
 * `int x = *(int*) arglist_get(arglist, "foo");`
 * `const char* s = (char*) arglist_get(arglist, "bar");`
 *
 * Returns:
 *   A `void*` pointer to the arg value or `NULL`.
 */
void* arglist_get(ArgList* arglist, const char* name) {
  while (arglist->next != NULL) {
    const char* argname = NULL;
    if (!(argname = arg_get_name(arglist->arg))) {
      fprintf(stderr, "Error getting argument name from ArgList");
      continue;
    }

    if (strncasecmp(name, argname, MAX_ARG_STRING_LEN) == 0) {
      return arg_get_value(arglist->arg);
    }

    arglist = arglist->next;
  }

  return NULL;
}
