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

template<typename T>
void ROOTTools::DrawFrame(T *hist, const std::string& title, 
                          const std::string& xTitle, const std::string& yTitle,
                          const double xTitleOffset, const double yTitleOffset,
                          const double xTitleSize, const double yTitleSize,
                          const bool drawOppositeAxis, const bool drawContents)
{
   if (drawContents) hist->Draw();
   else hist->Draw("AXIS");

   hist->SetTitle(title.c_str());

   hist->GetXaxis()->SetTitle(xTitle.c_str());
   hist->GetYaxis()->SetTitle(yTitle.c_str());

   hist->GetXaxis()->SetTitleOffset(xTitleOffset);
   hist->GetYaxis()->SetTitleOffset(yTitleOffset);

   hist->GetXaxis()->SetLabelSize(xTitleSize);
   hist->GetYaxis()->SetLabelSize(yTitleSize);
   
   hist->SetTitleSize(xTitleSize, "X");
   hist->SetTitleSize(yTitleSize, "Y");

   if (drawOppositeAxis) hist->Draw("SAME AXIS X+ Y+");
}

void ROOTTools::DrawFrame(const double xMin, const double yMin, 
                          const double xMax, const double yMax,
                          const std::string& title, 
                          const std::string& xTitle, const std::string& yTitle,
                          const double xTitleOffset, const double yTitleOffset,
                          const double xTitleSize, const double yTitleSize,
                          const bool drawOppositeAxis)
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
                            const bool printPng, const bool printPdf, 
                            const bool compressPdf, const bool parallelCompression)
{
   if (!printPng && !printPdf)
   {
      std::cout << "Error in ROOTTools::PrintTCanvas(TCanvas *canv, "\
                   "const std::string& outputFileNameNoExt, const bool printPng, "\
                   "const bool printPdf, const bool compressPdf, const bool parallelCompression) :"\
                   " either printPng or printPdf must be true" << std::endl;
      exit(1);
   }

   canv->SetFillStyle(4000);
   canv->SetFrameFillColor(0);
   canv->SetFrameFillStyle(0);
   canv->SetFrameBorderMode(0);

   if (printPng) canv->SaveAs((outputFileNameNoExt + ".png").c_str());

   if (printPdf)
   {
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
}

// explicit instantiations of ROOTTools::DrawFrame(T *, ...)
template void ROOTTools::DrawFrame(TH1 *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1C *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1D *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1F *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1I *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1L *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);
template void ROOTTools::DrawFrame(TH1S *, const std::string&, 
                                   const std::string&, const std::string&, 
                                   const double, const double, 
                                   const double, const double, 
                                   const bool, const bool);

#endif /* ROOT_TOOLS_TCANVAS_TOOLS_CPP */
