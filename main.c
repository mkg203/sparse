#include "handler.c"

int main(int argc, char* argv[]) {
  ArgList* args = arglist_create();
  ArgList* tail = args;
  ArgList* temp = args;
  
  int64_t value = 5;
  double fl = 3.141592;
  char* test = "HELLO WORLD";
  bool bluesky = true;

  tail = arglist_add_arg(tail, "int test", 1, ARG_TYPE_INT);
  arg_set_value(temp->arg, &value);
  temp = temp->next;
  tail = arglist_add_arg(tail, "float test", 1, ARG_TYPE_FLOAT);
  arg_set_value(temp->arg, &fl);
  temp = temp->next;
  tail = arglist_add_arg(tail, "string test", 1, ARG_TYPE_STRING);
  arg_set_value(temp->arg, test);
  temp = temp->next;
  tail = arglist_add_arg(tail, "bool test", 1, ARG_TYPE_BOOL);
  arg_set_value(temp->arg, &bluesky);
  temp = temp->next;

  // tail = arglist_add_int(tail, "test", 1, 5);
  // tail = arglist_add_string(tail, "test", 1, "big test here");
  // tail = arglist_add_float(tail, "test", 1, 5.32);

  print_args(args);
  arglist_del(args);
}
