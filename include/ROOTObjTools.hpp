/** 
 *  @file   ROOTObjTools.hpp 
 *  @brief  Contains useful set of functions that simplify work with various ROOT classes such as TCanvas, TF1, TFile, etc.
 *
 *  In order to use these functions libROOTObjTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_OBJ_TOOLS_HPP
#define ROOT_OBJ_TOOLS_HPP

#include <string>

#include "TF1.h"
#include "TCanvas.h"

/// @namespace ROOTTools ROOTTools namespace
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

   /*! @brief Saves TCanvas in .pdf format and additionaly in .png format if specified.
    * @param[in] canv TCanvas object that will be written
    * @param[in] outputFileNameNoExt name of the output file without extention (such as ".pdf" or ".png"). Extentions of the files will be added automaticaly
    * @param[in] printPng if true .png file is also printed additionaly to .pdf file; else only .pdf file will be printed. Note that saving complex canvases in .png takes much more time than saving them in .pdf
    * @param[in] compressPdf if true .pdf file will be compressed with ghostscript. It is recommended to leave this parameter true since it doesn't take a lot of resources to compress the file and the size of the compressed file will usually be reduced by ~0.5-0.7 of the uncompressed file size (depends on the contents of canvas and with more complex canvases more reduction in size can be achieved)
    * @param[in] parallelCompression if true the compression will be ran on the separate thread detached from the current program. Parallel compression speeds up the function completion time since the program does not need to wait until compression is done. It is recommended to leave this parameter true unless the program you're running takes all resources of the machine.
    */
   void PrintCanvas(TCanvas *canv, const std::string& outputFileNameNoExt, 
                    const bool printPng = true, const bool compressPdf = true, 
                    const bool parallelCompression = true);
}

#endif /* ROOT_OBJ_TOOLS_HPP */
