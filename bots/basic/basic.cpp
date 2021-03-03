#include "controller.h"

RegisterBot tata("tata", [](int step, int id, int input) {
  static int a = 0;
  a += 1;
  return input + a;
});