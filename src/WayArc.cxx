#include "sys.h"
#include "debug.h"
#ifdef CWDEBUG
#include "debug_ostream_operators.h"
#endif

int main(int argc, char* argv[])
{
  Debug(debug::init());

  Dout(dc::notice, "The type of main is: " << print_type<decltype(&main)> << ".");
}
