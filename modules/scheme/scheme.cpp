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

static void*
guile_number_doubler (void* data)
{
  // This mess just to get to the values in data.
  // Cast to double pointer, dereference the new cast to a double.
  // I guess in a "real" case, I would be reading the memory for string data.
  double d = *((double*) data);

  return scm_from_double(d + d);
}

int
Scheme::listen (double x)
{
  cout << "Listening for connections on port maybe..." << endl;
  // https://www.gnu.org/software/guile/manual/html_node/Initialization.html
  // https://stackoverflow.com/questions/34725194/filtering-c-structures-with-gnu-guile#34728386
  scm_with_guile (&register_functions, NULL);
  // scm_shell (0, NULL);
  cout << "Well, we did something at least." << endl;

  SCM g = (SCM) scm_with_guile (&guile_number_doubler, &x);
  int d = scm_to_double (g);

  return d;
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
  ClassDB::bind_method (D_METHOD ("listen", "x"), &Scheme::listen);
}

Scheme::Scheme ()
{
  count = 0;
}
