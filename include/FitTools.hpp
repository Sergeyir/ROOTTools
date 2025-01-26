#ifndef FIT_TOOLS_HPP
#define FIT_TOOLS_HPP

#include <string>

#include "TF1.h"

/// acquire number of parameters from string function used in ROOT
unsigned int GetNumberOfParameters(const std::string& fitFunc, const std::string parName = "par");

#endif /* FIT_TOOLS_HPP */
