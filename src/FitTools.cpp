#ifndef FIT_TOOLS_CPP
#define FIT_TOOLS_CPP

#include "../include/FitTools.hpp"

unsigned int GetNumberOfParameters(const std::string& fitFunc, const std::string parName)
{
   unsigned int numberOfParameters = 0;
   
   unsigned long braceOpenPos = 0;
   // check for lambda expression
   if (fitFunc.find("{") < fitFunc.size()) braceOpenPos = fitFunc.find("{") + 1;
   unsigned long braceClosePos = braceOpenPos;
   
   int i = 0;
   while (braceOpenPos < fitFunc.size() - 1)
   {
      // searching for open brace position right after parameter name
      braceOpenPos = fitFunc.find(parName + "[", braceClosePos);
      
      // check for the last parameter
      if (braceOpenPos > fitFunc.size() - 1) break;
      // incrementing after check since it would have resulted in overflow for a final check
      braceOpenPos += parName.size() + 1;
         
      // searching for close brace position right after open brace
      braceClosePos = fitFunc.find("]", braceOpenPos);
      
      // converting string number between square braces into int
      const unsigned int parameterNumber = 
         atoi(fitFunc.substr(braceOpenPos, braceClosePos - braceOpenPos).c_str());
      if (parameterNumber > numberOfParameters) numberOfParameters = parameterNumber;

      i++;
   }
   // since parameters numeration in root starts from 0 increment is needed
   return numberOfParameters + 1;
}

#endif /* FIT_TOOLS_CPP */
