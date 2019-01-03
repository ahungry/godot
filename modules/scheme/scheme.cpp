// -*- mode: c++; -*-
/* scheme.cpp */

// http://docs.godotengine.org/en/latest/development/cpp/custom_modules_in_cpp.html

#include "scheme.h"

void Scheme::add (int value) {
  count += value;
}

void Scheme::reset () {
  count = 0;
}

int Scheme::get_total () const {
  return count;
}

void Scheme::_bind_methods () {
  ClassDB::bind_method (D_METHOD ("add", "value"), &Scheme::add);
  ClassDB::bind_method (D_METHOD ("reset"), &Scheme::reset);
  ClassDB::bind_method (D_METHOD ("get_total"), &Scheme::get_total);
}

Scheme::Scheme () {
  count = 0;
}
