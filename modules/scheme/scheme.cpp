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

int registeredFnIdx = 0;
Ref<Reference> selfPtr;
// https://www.sisik.eu/blog/godot/gd-native-calling-gdscript-method-from-cpp-code
// godot_variant self;
char registeredFnName[100];

// If we ever go further than arity/5, something is probably wrong.
// Also, we keep the call in/out simple and just use string in both directions.
// If someone needs to parse it into a "thing" they do so, vs many calls.
// TODO: Someday we should figure out array / list, and objects (hash/fn ptr)
static SCM
my_fn (SCM name, SCM arg1, SCM arg2, SCM arg3, SCM arg4, SCM arg5)
{
  char* result = scm_to_stringn (name, NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);
  int a1 = scm_to_int (arg1);
  int a2 = scm_to_int (arg2);
  int a3 = scm_to_int (arg3);
  int a4 = scm_to_int (arg4);
  int a5 = scm_to_int (arg5);

  cout << "Call my-fn as: " << result << a1 << a2 << a3 << a4 << a5 << endl;

  Variant ret = (selfPtr)->call(result, a1, a2, a3, a4, a5);
  // Variant ret = selfPtr->call("getNumber");
  // Variant ret = (*registeredFnInstance)->call(result);
  // See: https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
  // Variant ret = (selfPtr->*fnPtr)(result, 0, 0, 0, 0, 0);
  cout << "I dun called it!" << endl;
  String res = ret.get_construct_string ();

  cout << "Result of my-fn call was: " << res.c_str () << endl;

  std::wstring ws = res.c_str ();
  std::string s (ws.begin (), ws.end ());
  // int i = atoi (s.c_str ());

  // return scm_from_int (i);
  return scm_from_stringn (s.c_str (), NULL, "ascii", SCM_FAILED_CONVERSION_ESCAPE_SEQUENCE);
}

int
Scheme::getNumber ()
{
  Variant ret = (selfPtr)->call("getInputs");
  int n = (int) ret;

  cout << "It may not be right, but n was: " << n << endl;

  String res = ret.get_construct_string ();
  std::wstring ws = res.c_str ();
  std::string s (ws.begin (), ws.end ());

  cout << "Got some variant return from a class using getNumber call." << endl;
  cout << s << endl;

  return n;
}

// This needs to be an array or something, not just an override at last bind point.
void
Scheme::registerFn(int idx, Ref<Reference> customScriptInstance, String wname)
{
  std::wstring ws = wname.c_str ();
  std::string s (ws.begin (), ws.end ());

  registeredFnIdx = idx;
  selfPtr = customScriptInstance;

  strcpy (registeredFnName, s.c_str ());

  cout << "Registered Fn Name: " << s.c_str () << endl;
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
guile_repl (void* _data)
{
  scm_c_define_gsubr ("process-input", 1, 0, 0, (void*) &Scheme::processInput);
  scm_c_define_gsubr ("my-add", 1, 0, 0, (void*) &my_add);

  return scm_c_eval_string ("(use-modules (system repl server)) (spawn-server (make-tcp-server-socket #:port 12345)) (number->string 555)");
}

static void*
guile_eval (void* data)
{
  // https://www.gnu.org/software/guile/manual/html_node/Fly-Evaluation.html
  // const char* eval = ((char*) data);
  const char* eval = *((char**) data);

  cout << "Evaluation target: " << eval << endl;

    // https://godotengine.org/qa/21208/pass-a-gdscript-class-as-reference-in-a-cpp-module
  // we would need to set the script first in gdscript, then call things in it over here
  scm_c_define_gsubr ("process-input", 1, 0, 0, (void*) &Scheme::processInput);

  // lets try to just bind any old cpp call in here we can reach via scheme
  scm_c_define_gsubr ("my-add", 1, 0, 0, (void*) &my_add);
  scm_c_define_gsubr ("my-fn", 6, 0, 0, (void*) &my_fn);

  return scm_c_eval_string (eval);
}

void
Scheme::repl ()
{
  scm_with_guile (&guile_repl, NULL);

  return;
}

String
Scheme::eval (String scm)
{
  cout << "Time to eval" << endl;

  std::wstring ws = scm.c_str ();
  std::string s (ws.begin (), ws.end ());
  // std::string s = "(+ 1 2 3)";

  // This wrapper could still be useful for simple syntax not from file
  // But it will not work well from file
  // std::string preamble = "(begin (define (all-strings s) (if (string? s) s (number->string s))) (all-strings ";
  // std::string postamble = "))";
  // std::string lisp = preamble + s + postamble;
  std::string lisp = s;

  cout << lisp << endl;

  int len = s.length ();

  cout << "The length is: ";
  cout << len << endl;

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
  ClassDB::bind_method (D_METHOD ("repl"), &Scheme::repl);
  ClassDB::bind_method (D_METHOD ("processInput", "customScriptInstance"), &Scheme::processInput);
  ClassDB::bind_method (D_METHOD ("registerFn", "idx", "customScriptInstance", "name"), &Scheme::registerFn);
  ClassDB::bind_method (D_METHOD ("getNumber"), &Scheme::getNumber);
}

Scheme::Scheme ()
{
  count = 0;
}
