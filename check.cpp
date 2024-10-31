#include "ThrObj.hpp"
#include "TCanvasPrinter.hpp"
#include "GUIFit.hpp"

// Calls a macros to include all headers and load all libraries
// so that root CLING can check the code
// In order to use call: root check
void check()
{
   std::cout << "Libraries loaded: " << std::endl;
   gSystem->ListLibraries();
}
