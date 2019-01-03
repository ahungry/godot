// -*- mode: c++; -*-
/* scheme.cpp */

// https://godotengine.org/article/dlscript-here
// http://docs.godotengine.org/en/latest/development/cpp/custom_modules_in_cpp.html

// https://gamedevadventures.posthaven.com/using-c-plus-plus-and-gdnative-in-godot-part-1
#include "scheme.h"

using namespace std;

double sum = 0;

static SCM
my_add (SCM num)
{
  const double val = scm_to_double (num);
  sum += val;

  return scm_from_double (sum);
}

String
Scheme::processInput(Ref<Reference> customScriptInstance)
{
  Variant ret = customScriptInstance->call("getInputs");

  String res = ret.get_construct_string ();
  std::wstring ws = res.c_str ();
  std::string s (ws.begin (), ws.end ());

  cout << "Got some variant return from a class using processInput call." << endl;
  cout << s << endl;

  std::string sj = "Hello from your own class!" + s;

  String godotstr (sj.c_str ());

  return godotstr;
}

static void*
register_functions (void* data)
{
  // req, opt, &rest
  scm_c_define_gsubr ("process-input", 1, 0, 0, (void*) &Scheme::processInput);

  return NULL;
}

static void*
guile_number_doubler (void* data)
{
  // This mess just to get to the values in data.
  // Cast to double pointer, dereference the new cast to a double.
  // I guess in a "real" case, I would be reading the memory for string data.
  double d = *((double*) data);

  return scm_from_double (d + d);
}

static void*
guile_eval (void* data)
{
  // https://www.gnu.org/software/guile/manual/html_node/Fly-Evaluation.html
  const char* eval = ((char*) data);

  cout << "Evaluation target: " << eval << endl;

  // return scm_c_eval_string (eval);

  // Hey, what the heck, lets fire off a server thread and see what happens.

  // works - actually, it seems like string literal is ok, problem may be with the eval pointer.
  // Well, this is interesting.  We could run any scheme our repl receives - could it call
  // native godot calls from this vantage point?
  // If so, nice interactive workflow for game development (how would this bundle at runtime though?)

  // https://godotengine.org/qa/21208/pass-a-gdscript-class-as-reference-in-a-cpp-module
  // we would need to set the script first in gdscript, then call things in it over here
  scm_c_define_gsubr ("process-input", 1, 0, 0, (void*) &Scheme::processInput);

  // lets try to just bind any old cpp call in here we can reach via scheme
  scm_c_define_gsubr ("my-add", 1, 0, 0, (void*) &my_add);

  return scm_c_eval_string ("(use-modules (system repl server)) (spawn-server (make-tcp-server-socket #:port 12345)) (number->string 555)");

  // Will not work - wrong-type-arg throw
  // (use-modules (ice-9 format))
  // return scm_c_eval_string ("(use-modules (ice-9 format)) (format #t \"~a\" (+ 1 2 3 4))");
}

String
Scheme::eval (String scm)
{
  cout << "Time to eval" << endl;

  std::wstring ws = scm.c_str ();
  // std::string s (ws.begin (), ws.end ());
  std::string s = "(+ 1 2 3)";
  std::string preamble = "(begin (define (all-strings s) (if (string? s) s (number->string s))) (all-strings ";
  std::string postamble = "))";
  std::string lisp = preamble + s + postamble;

  cout << lisp << endl;
  // cout << scm << endl;
  // scm_with_guile (&register_functions, NULL);
  // scm_shell (0, NULL);

  // String godotstr ("something");
  // std::wstring ws = godotstr.c_str ();
  // std::string s (ws.begin (), ws.end ());
  // char* c = s.c_str ();
  const char* cscm = lisp.c_str ();

  SCM g = (SCM) scm_with_guile (&guile_eval, &cscm);
  char* result = scm_to_stringn (g, NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);

  String godotstr (result);

  return godotstr;
}

double
Scheme::listen (double x)
{
  // cout << "Listening for connections on port maybe..." << endl;
  // https://www.gnu.org/software/guile/manual/html_node/Initialization.html
  // https://stackoverflow.com/questions/34725194/filtering-c-structures-with-gnu-guile#34728386
  // scm_with_guile (&register_functions, NULL);
  // scm_shell (0, NULL);
  // cout << "Well, we did something at least." << endl;

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
  // return count;
  return (int) sum;
}

void
Scheme::_bind_methods ()
{
  ClassDB::bind_method (D_METHOD ("add", "value"), &Scheme::add);
  ClassDB::bind_method (D_METHOD ("reset"), &Scheme::reset);
  ClassDB::bind_method (D_METHOD ("get_total"), &Scheme::get_total);
  ClassDB::bind_method (D_METHOD ("listen", "x"), &Scheme::listen);
  ClassDB::bind_method (D_METHOD ("eval", "scm"), &Scheme::eval);
  ClassDB::bind_method (D_METHOD ("processInput", "customScriptInstance"), &Scheme::processInput);
}

Scheme::Scheme ()
{
  count = 0;
}
