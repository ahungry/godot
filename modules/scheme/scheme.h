// -*- mode: c++; -*-
/* scheme.h */
#ifndef SCHEME_H
#define SCHEME_H

#include "core/reference.h"

class Scheme : public Reference {
  GDCLASS (Scheme, Reference);

  int count;

protected:
  static void _bind_methods ();

public:
  void add (int value);
  void reset ();
  int get_total () const;

  Scheme ();
};

#endif
