// -*- mode: c++; -*-
/* register_types.cpp */

#include "register_types.h"

#include "core/class_db.h"
#include "scheme.h"

void register_scheme_types () {
  ClassDB::register_class<Scheme>();
}

void unregister_scheme_types () {
  // nothing to do here
}
