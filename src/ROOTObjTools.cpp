/** 
 *  @file   ROOTObjTools.cpp 
 *  @brief  Contains useful set of functions that simplify work with various ROOT classes such as TCanvas, TF1, TFile, etc.
 *
 *  In order to use these functions libROOTObjTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_OBJ_TOOLS_CPP
#define ROOT_OBJ_TOOLS_CPP

#include "../include/ROOTObjTools.hpp"

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

void ROOTTools::PrintCanvas(TCanvas *canv, const std::string& outputFileNameNoExt, 
                            const bool printPng, const bool compressPdf, 
                            const bool parallelCompression)
{
   if (printPng) canv->SaveAs((outputFileNameNoExt + ".png").c_str());
   
   if (compressPdf) 
   {
      // temporary .pdf file; will be removed later
      canv->SaveAs((outputFileNameNoExt + ".tmp.pdf").c_str());
      
      // ghostscript command to reduce pdf size and remove temporary .pdf file
      std::string compressionCommand = 
         "ghostscript -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 \
          -dNOPAUSE -dQUIET -dBATCH -dPrinted=false \
          -sOutputFile=" + outputFileNameNoExt + ".pdf " + 
         outputFileNameNoExt + ".tmp.pdf && rm " + 
         outputFileNameNoExt + ".tmp.pdf";
      
      // option to detach the command call from the current process
      if (parallelCompression) compressionCommand += " &";
      
      system(compressionCommand.c_str());
   }
   else
   {
      canv->SaveAs((outputFileNameNoExt + ".pdf").c_str());
   }
}

#endif /* ROOT_OBJ_TOOLS_CPP */
