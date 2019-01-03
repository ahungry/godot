// -*- mode: c++; -*-
/* scheme.cpp */

// http://docs.godotengine.org/en/latest/development/cpp/custom_modules_in_cpp.html

#include <iostream>
#include "scheme.h"

using namespace std;

static void*
register_functions (void* data)
{
  return NULL;
}

int
Scheme::listen ()
{
  cout << "Listening for connections on port maybe..." << endl;
  scm_with_guile (&register_functions, NULL);
  scm_shell (0, NULL);
  cout << "Well, we did something at least." << endl;

  return 42;
}

void
Scheme::add (int value)
{
  count += value;
}

void
Scheme::reset ()
{
  count = 0;
}

int
Scheme::get_total () const
{
  return count;
}

void
Scheme::_bind_methods ()
{
  ClassDB::bind_method (D_METHOD ("add", "value"), &Scheme::add);
  ClassDB::bind_method (D_METHOD ("reset"), &Scheme::reset);
  ClassDB::bind_method (D_METHOD ("get_total"), &Scheme::get_total);
  ClassDB::bind_method (D_METHOD ("listen"), &Scheme::listen);
}

Scheme::Scheme ()
{
  count = 0;
}
