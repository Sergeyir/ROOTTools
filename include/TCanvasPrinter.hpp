// $HEADER$
//------------------------------------------------------------------------------------------------
//                        TCanvas printer functions declarations
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

#ifndef TCANVAS_PRINTER_HPP
#define TCANVAS_PRINTER_HPP

#include <string>
#include "TCanvas.h"

//outputFileName is written without extention
void PrintCanvas (TCanvas *canv, const std::string outputFileName, bool printPdf = true);

#endif /*TCANVAS_PRINTER_HPP*/
