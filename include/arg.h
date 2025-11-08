#ifndef ARG_H
#define ARG_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "types.h"

#define MAX_ARG_STRING_LEN 1024

typedef struct Arg Arg;

Arg* arg_create(char* name);

char* arg_get_name(Arg* arg); 

void* arg_get_value(Arg* arg);

void arg_set_type(Arg* arg, ArgType type);

void arg_init_int(Arg* arg, int64_t value);

void arg_init_bool(Arg* arg, bool value);

void arg_init_float(Arg* arg, double value);

void arg_init_string(Arg* arg, char* value);

int arg_set_value(Arg* arg, char* value);

void arg_del(Arg* arg);

// printing function useful for debug
void print_arg(Arg* arg);

#endif // !ARG_H
