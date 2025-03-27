#include "sys.h"
#include "debug.h"

int main()
{
  Debug(debug::init());

  Dout(dc::notice, "This is WayArc!");
}
