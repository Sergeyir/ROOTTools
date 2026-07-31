/** 
 *  @file   HistTools.cpp 
 *  @brief  Contains useful set of functions that simplify work with ROOT's TH1, TH2, TH3 objects
 *
 *  In order to use these functions libHistTools.so must be loaded
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_HIST_TOOLS_CPP
#define ROOT_TOOLS_HIST_TOOLS_CPP

#include "HistTools.hpp"
#include <iostream>

template<typename T>
void ROOTTools::SwapAxis(T* hist)
{
   T *oldHist = static_cast<T *>(hist->Clone());

   const unsigned int xNBins = hist->GetXaxis()->GetNbins();
   const unsigned int yNBins = hist->GetYaxis()->GetNbins();

   std::vector<double> xBins;
   std::vector<double> yBins;

   xBins.resize(xNBins + 1);
   yBins.resize(yNBins + 1);

   for (unsigned int i = 1; i <= xNBins; i++)
   {
      xBins[i - 1] = hist->GetXaxis()->GetBinLowEdge(i);
   }
   xBins[xNBins] = hist->GetXaxis()->GetBinUpEdge(xNBins);

   for (unsigned int i = 1; i <= yNBins; i++)
   {
      yBins[i - 1] = hist->GetYaxis()->GetBinLowEdge(i);
   }
   yBins[yNBins] = hist->GetYaxis()->GetBinUpEdge(yNBins);

   hist->SetBins(yNBins, &yBins[0], xNBins, &xBins[0]);

   for (const double &x : xBins)
   {
      std::cout << x << std::endl;
   }
   std::cout << std::endl;
   for (const double &y : yBins) 
   {
      std::cout << y << std::endl;
   }

   for (unsigned int i = 1; i <= xNBins; i++)
   {
      for (unsigned int j = 1; j <= yNBins; j++)
      {
         hist->SetBinContent(j, i, oldHist->GetBinContent(i, j));
         hist->SetBinError(j, i, oldHist->GetBinError(i, j));
      }
   }

   hist->GetXaxis()->SetTitle(oldHist->GetYaxis()->GetTitle());
   hist->GetYaxis()->SetTitle(oldHist->GetXaxis()->GetTitle());
}

// explicit instantiatios of ROOTTools::SpawAxis
template void ROOTTools::SwapAxis(TH2F *);
template void ROOTTools::SwapAxis(TH2D *);

#endif /* ROOT_TOOLS_HIST_TOOLS_CPP */
