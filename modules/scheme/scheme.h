// -*- mode: c++; -*-
/* scheme.h */
#ifndef SCHEME_H
#define SCHEME_H

// #include <core/Godot.hpp>
// #include <core/String.hpp>
// #include <core/Array.hpp>
#include <string>
#include <iostream>
#include <libguile.h>
#include "core/reference.h"

class Scheme : public Reference {
  GDCLASS (Scheme, Reference);

  int count;

protected:
  static void _bind_methods ();

public:
  double listen (double x);
  String eval (String scm);
  void add (int value);
  void reset ();
  int get_total () const;

  Scheme ();
};

#endif
