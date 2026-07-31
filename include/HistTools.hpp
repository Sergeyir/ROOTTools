/** 
 *  @file   HistTools.hpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TH1, TH2, TH3 objects
 *
 *  In order to use these functions libHistTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_HIST_TOOLS_HPP
#define ROOT_TOOLS_HIST_TOOLS_HPP

#include <vector>

#include "TH1.h"
#include "TH2.h"

/// @namespace ROOTTools
namespace ROOTTools
{
   /// @brief Swaps X and Y axis of passed histogram
   template<typename T>
   void SwapAxis(T* hist);
}

#endif /* ROOT_TOOLS_HIST_TOOLS_HPP */
