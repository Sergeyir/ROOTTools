/** 
 *  @file   TCanvasTools.cpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TCanvas objects
 *
 *  In order to use these functions libTCanvasTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_TCANVAS_TOOLS_CPP
#define ROOT_TOOLS_TCANVAS_TOOLS_CPP

#include "TCanvasTools.hpp"

void ROOTTools::DrawFrame(const double xMin, const double yMin, 
                          const double xMax, const double yMax,
                          const std::string title, 
                          const std::string& xTitle, const std::string& yTitle,
                          const bool drawOppositeAxis,
                          const double xTitleOffset, const double yTitleOffset,
                          const double xTitleSize, const double yTitleSize)
{
   TH1 *frame = gPad->DrawFrame(xMin, yMin, xMax, yMax);

   frame->SetTitle(title.c_str());

   frame->GetXaxis()->SetTitle(xTitle.c_str());
   frame->GetYaxis()->SetTitle(yTitle.c_str());

   frame->GetXaxis()->SetTitleOffset(xTitleOffset);
   frame->GetYaxis()->SetTitleOffset(yTitleOffset);

   frame->GetXaxis()->SetLabelSize(xTitleSize);
   frame->GetYaxis()->SetLabelSize(yTitleSize);
   
   frame->SetTitleSize(xTitleSize, "X");
   frame->SetTitleSize(yTitleSize, "Y");

   if (drawOppositeAxis) frame->Draw("SAME AXIS X+ Y+");
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

#endif /* ROOT_TOOLS_TCANVAS_TOOLS_CPP */
