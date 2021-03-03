#include "register.h"


RegisterBot tata("tata", [](int input) {
  static int a = 0;
  a += 1;
  return input + a; });