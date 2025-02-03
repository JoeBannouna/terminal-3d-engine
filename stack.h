#pragma once

// generic stack type
// Wanring: no overflow saftey implementation
#define STACK(type, capacity, arr_name)\
type arr_name[capacity];\
int arr_name##_len = 0;\
void push##type(type item) {\
  arr_name[arr_name##_len] = item;\
  arr_name##_len++;\
}\
type pop##type() {\
  arr_name##_len--;\
  return arr_name[arr_name##_len];\
}

