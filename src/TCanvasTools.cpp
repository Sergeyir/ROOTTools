/** 
 *  @file   TCanvasTools.cpp 
 *  @brief  Contains useful set of functions that simplify work with TCanvas
 *
 *  In order to use these functions libTCanvasTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_TCANVAS_TOOLS_CPP
#define ROOT_TOOLS_TCANVAS_TOOLS_CPP

#include "../include/TCanvasTools.hpp"

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

#endif /* ROOT_TOOLS_TCANVAS_TOOLS_CPP */
