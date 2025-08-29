/** 
 *  @file   TCanvasTools.hpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TCanvas objects
 *
 *  In order to use these functions libTCanvasTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_TCANVAS_TOOLS_HPP
#define ROOT_TOOLS_TCANVAS_TOOLS_HPP

#include <string>

#include "TH1.h"
#include "TCanvas.h"

/// @namespace ROOTTools
namespace ROOTTools
{
   /* @brief Draws frame for TCanvas
    *
    * @param[in] xMin minimum x value
    * @param[in] yMin minimum y value
    * @param[in] xMax maximum x value
    * @param[in] yMax maximum y value
    * @param[in] title title of a frame
    * @param[in] xTitle title of X axis
    * @param[in] yTitle title of Y axis
    * @param[in] xTitleOffset offset of X axis title
    * @param[in] yTitleOffset offset of Y axis title
    * @param[in] xTitleSize sizes of X axis title and label
    * @param[in] yTitleSize sizes of Y axis title and label
    * @param[in] drawOppositeAxis if true frame will be drawn additionally with options "SAME X+ Y+" for drawing X and Y axis on the top and on the right respectively
    */
   void DrawFrame(const double xMin, const double yMin, 
                  const double xMax, const double yMax,
                  const std::string title, 
                  const std::string& xTitle, const std::string& yTitle,  
                  const double xTitleOffset = 1., const double yTitleOffset = 1.5,
                  const double xTitleSize = 0.05, const double yTitleSize = 0.05,
                  bool drawOppositeAxis = true);
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

#endif /* ROOT_TOOLS_TCANVAS_TOOLS_HPP */
