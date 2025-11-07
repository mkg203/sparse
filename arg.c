#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARG_STRING_LEN 1024

typedef enum {
  ARG_TYPE_STRING,
  ARG_TYPE_INT,
  ARG_TYPE_BOOL,
  ARG_TYPE_FLOAT,
} ArgType;

typedef struct {
  char* name;
  ArgType type;
  union {
    char* s;
    int64_t i;
    bool b;
    double f;
    void* p;
  } value;
} Arg;

Arg* arg_create(char* name) {
  Arg* arg = calloc(1, sizeof(Arg));

  size_t len = strnlen(name, MAX_ARG_STRING_LEN);
  char* copy = malloc(len + 1);
  memcpy(copy, name, len);
  copy[len] = '\0';

  arg->name = copy;

  return arg;
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
  
  printf("name: %s\n", arg->name);
  printf("type: %u\n", arg->type);
  switch (arg->type) {
  case ARG_TYPE_STRING: {
    printf("value: %s\n", arg->value.s);
    break;
  }
  case ARG_TYPE_INT: {
    printf("value: %ld\n", arg->value.i);
    break;
  }
  case ARG_TYPE_FLOAT: {
    printf("value: %lf\n", arg->value.f);
    break;
  }
  case ARG_TYPE_BOOL: {
    printf("value: %b\n", arg->value.b);
    break;
  }
  default: {
  }
  }
}
