// $SOURCE$
//------------------------------------------------------------------------------------------------
//                         TCanvas printer functions realisations
//------------------------------------------------------------------------------------------------
// TCanvasPrinter
//
// ** Free and open code for anyone to use **
//
// Author: Sergei Antsupov
// Email: antsupov0124@gmail.com
//
/**
 * Basic tool for easier ROOT6 TCanvas printing
 * ghostscript v.10 or newer is required for correct pdf compression
 **/
//------------------------------------------------------------------------------------------------

#ifndef TCANVAS_PRINTER_CPP
#define TCANVAS_PRINTER_CPP

#include "../include/TCanvasPrinter.hpp"

//outputFileName is written without extention
void PrintCanvas (TCanvas *canv, const std::string& outputFileName, const bool printPdf)
{
   canv->SaveAs((outputFileName + ".png").c_str());
   if (printPdf) canv->SaveAs((outputFileName + ".tmp.pdf").c_str());

   // ghostscript to shrink pdf size
   if (printPdf) system(
      ("gs -sDEVICE=pdfwrite -dCompatibilityLevel=1.5 \
      -dNOPAUSE -dQUIET -dBATCH -dPrinted=false -sOutputFile=" + 
      outputFileName + ".pdf " + outputFileName + ".tmp.pdf && rm " + 
      outputFileName + ".tmp.pdf &").c_str());
}

#endif /*TCANVAS_PRINTER_CPP*/
