/** 
 *  @file   TF1Tools.hpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TF1 objects
 *
 *  In order to use these functions libTF1Tools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_TF1_TOOLS_HPP
#define ROOT_TOOLS_TF1_TOOLS_HPP

#include <string>

#include "TF1.h"

/// @namespace ROOTTools
namespace ROOTTools
{
   /*! @brief Return the biggest parameter index + 1 in formula of labmda function that was used in TF1 constructor
    * @param[in] formula formula that was used to define fit function in TF1. The formula must be a labmda expression (see examples below).
    * @param[in] parName parameter name in formula
    *
    * Example:
      @code
      printf("%u\n", ROOTTools::GetNumberOfParameters("[](double *x, double *p) {return p[0] + p[1]*x[0] + p[2]*x[0]*x[0];}"));
      printf("%u\n", ROOTTools::GetNumberOfParameters("[](double *x, double *p) {return p[0] + p[3]*x[0]*x[0];}"));
      printf("%u\n", ROOTTools::GetNumberOfParameters("[](double *x, double *p) {return p[0] + p[1]*pow(x, p[2]);}"));
      printf("%u\n", ROOTTools::GetNumberOfParameters("[](double *x, double *par) {return par[0]*x + par[1]*x[0]*x[0];}", "par"));
      @endcode
      Result:
      \verbatim
      3
      4
      2
      3
      \endverbatim
    */
   unsigned int GetNumberOfParameters(const std::string& formula, 
                                      const std::string& parName = "p");
}

#endif /* ROOT_TOOLS_TF1_TOOLS_HPP */
