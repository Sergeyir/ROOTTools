/** 
 *  @file   TF1Tools.cpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TF1 objects
 *
 *  In order to use these functions libROOTObjTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_TF1_TOOLS_CPP
#define ROOT_TOOLS_TF1_TOOLS_CPP

#include "ROOTTools/TF1Tools.hpp"

unsigned int ROOTTools::GetNumberOfParameters(const std::string& formula, const std::string& parName)
{
   unsigned int numberOfParameters = 0;
   
   unsigned long braceOpenPos = 0;
   // check for lambda expression
   if (formula.find("{") < formula.size()) braceOpenPos = formula.find("{") + 1;
   unsigned long braceClosePos = braceOpenPos;
   
   int i = 0;
   while (braceOpenPos < formula.size() - 1)
   {
      // searching for open brace position right after parameter name
      braceOpenPos = formula.find(parName + "[", braceClosePos);
      
      // check for the last parameter
      if (braceOpenPos > formula.size() - 1) break;
      // incrementing after check since it would have resulted in overflow for a final check
      braceOpenPos += parName.size() + 1;
         
      // searching for close brace position right after open brace
      braceClosePos = formula.find("]", braceOpenPos);
      
      // converting string number between square braces into int
      const unsigned int parameterNumber = 
         atoi(formula.substr(braceOpenPos, braceClosePos - braceOpenPos).c_str());
      if (parameterNumber > numberOfParameters) numberOfParameters = parameterNumber;

      i++;
   }
   // since parameters numeration in root starts from 0 increment is needed
   return numberOfParameters + 1;
}

#endif /* ROOT_TOOLS_TF1_TOOLS_CPP */
