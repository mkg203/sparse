#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arg.h"

#define MAX_ARG_STRING_LEN 1024

struct Arg {
  char* name;
  ArgType type;
  union {
    char* s;
    int64_t i;
    bool b;
    double f;
    void* p;
  } value;
};

Arg* arg_create(char* name) {
  Arg* arg = calloc(1, sizeof(Arg));

  size_t len = strnlen(name, MAX_ARG_STRING_LEN);
  char* copy = malloc(len + 1);
  memcpy(copy, name, len);
  copy[len] = '\0';

  arg->name = copy;

  return arg;
}

char* arg_get_name(Arg* arg) {
  if (arg == NULL) {
    return NULL;
  }
  return arg->name;
}

void* arg_get_value(Arg* arg) {
  switch (arg->type) {
  case ARG_TYPE_BOOL:
    return &arg->value.b;
  case ARG_TYPE_INT:
    return &arg->value.i;
  case ARG_TYPE_STRING:
    return arg->value.s;
  case ARG_TYPE_FLOAT:
    return &arg->value.f;
  default:
    return NULL;
  }
}

void arg_set_type(Arg* arg, ArgType type) {
  if (arg == NULL) {
    return;
  }
  arg->type = type;
}

void arg_init_int(Arg* arg, int64_t value) {
  arg->type = ARG_TYPE_INT;
  arg->value.i = value;
}

void arg_init_bool(Arg* arg, bool value) {
  arg->type = ARG_TYPE_BOOL;
  arg->value.b = value;
}

void arg_init_float(Arg* arg, double value) {
  arg->type = ARG_TYPE_FLOAT;
  arg->value.f = value;
}

void arg_init_string(Arg* arg, char* value) {
  arg->type = ARG_TYPE_STRING;

  size_t len = strnlen(value, MAX_ARG_STRING_LEN);
  char* copy = malloc(len + 1);
  memcpy(copy, value, len);
  copy[len] = '\0';

  arg->value.s = copy;
}

int arg_set_value(Arg* arg, char* value) {
  if (arg == NULL || value == NULL) {
    return 1;
  }

  switch (arg->type) {
  default: {
    return 1;
  }
  case ARG_TYPE_STRING: {
    arg_init_string(arg, (char*)value);
    break;
  }
  case ARG_TYPE_INT: {
    arg_init_int(arg, atoi(value));
    break;
  }
  case ARG_TYPE_FLOAT: {
    arg_init_float(arg, atof(value));
    break;
  }
  case ARG_TYPE_BOOL: {
    arg_init_bool(arg, atoi(value));
    break;
  }
  }
  return 0;
}

void arg_del(Arg* arg) {
  if (arg == NULL) {
    return;
  }
  if (arg->type == ARG_TYPE_STRING && arg->value.s != NULL) {
    free(arg->value.s);
  }
  if (arg->name != NULL) {
    free(arg->name);
  }
  // printf("test: %s\n", arg->value.s);
  // printf("test: %s\n", arg->name);
  free(arg);
}

// printing function useful for debug
void print_arg(Arg* arg) {
  if (arg == NULL) {
    return;
  }

  printf("Arg {\n");
  printf("  name: %s\n", arg->name);
  printf("  type: %u\n", arg->type);
  switch (arg->type) {
  case ARG_TYPE_STRING: {
    printf("  value: %s\n", arg->value.s);
    break;
  }
  case ARG_TYPE_INT: {
    printf("  value: %ld\n", arg->value.i);
    break;
  }
  case ARG_TYPE_FLOAT: {
    printf("  value: %lf\n", arg->value.f);
    break;
  }
  case ARG_TYPE_BOOL: {
    printf("  value: %b\n", arg->value.b);
    break;
  }
  default: {
  }
  }
  printf("}\n");
}
