#ifndef HANDLER_H
#define HANDLER_H

#include "arg.h"
#include "types.h"

typedef struct ArgList ArgList;

ArgList* arglist_create();

ArgList* arglist_add_arg(ArgList*, const char*, bool, ArgType);

ArgList* arglist_add_int(ArgList*, const char*, bool, int);

ArgList* arglist_add_string(ArgList*, const char*, bool, const char*);

ArgList* arglist_add_float(ArgList*, const char*, bool, float);

ArgList* arglist_get_next(ArgList*);

Arg* arglist_get_arg(ArgList*);

void arglist_del(ArgList*);

void print_args(ArgList*);

int arglist_count(ArgList*);

void arglist_parse(ArgList*, char*[], int);

/**
 * Casting:
 * `int x = *(int*) arglist_get(arglist, "foo");`
 * `char* s = (char*) arglist_get(arglist, "bar");`
 *
 * Returns:
 *   A `void*` pointer to the arg value or `NULL`.
 */
void* arglist_get(ArgList*, const char*);

#endif // !HANDLER_H
