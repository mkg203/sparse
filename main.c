#include "handler.c"

int main(int argc, char* argv[]) {
  ArgList* args = arglist_create();
  ArgList* tail = args;
  ArgList* temp = args;
  
  int64_t value = 5;
  double fl = 3.141592;
  char* test = "HELLO WORLD";
  bool bluesky = true;

  tail = arglist_add_arg(tail, "int", 0, ARG_TYPE_INT);
  tail = arglist_add_arg(tail, "float test", 0, ARG_TYPE_FLOAT);
  tail = arglist_add_arg(tail, "test", 1, ARG_TYPE_STRING);
  tail = arglist_add_arg(tail, "bool test", 0, ARG_TYPE_BOOL);

  arglist_parse(args, argv, argc);

  printf("%d\n", *(int*)arglist_get(args, "int"));
  printf("%s\n", (char*)arglist_get(args, "test"));

  // __builtin_dump_struct(args, &printf);
  // tail = arglist_add_int(tail, "test", 1, 5);
  // tail = arglist_add_string(tail, "test", 1, "big test here");
  // tail = arglist_add_float(tail, "test", 1, 5.32);

  print_args(args);
  arglist_del(args);
}
