/** 
 *  @file   GUIFit.hpp 
 *  @brief  Contains class that can be used for providing GUI for improving the approximations of form "foreground + background" by tweaking the background
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_GUI_FIT_HPP
#define ROOT_TOOLS_GUI_FIT_HPP

#include <stdio.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "TPad.h"
#include "TFrame.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TF1.h"

/// @namespace ROOTTools ROOTTools namespace
namespace ROOTTools
{
   /*! @class GUIFit
    * @brief class GUIFit that can be used for providing GUI for improving the approximations of form "foreground + background" by tweaking the background
    *
    * Not finished yet
    */
   class GUIFit
   {
      public:

      GUIFit();

      void AddFitType(const std::string& outputFileName);
      void AddHist(TH1F *dataHist, const std::string& xVal);
      void AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber, const unsigned int histNumber);
      void AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), 
                  const int bgFitParStartNumber, const unsigned int histNumber);
      void AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber);
      void AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), 
                  const int bgFitParStartNumber);
      void Start();

      ~GUIFit();

      private:

      void SetBgPoints();
      void SetActiveFit();
      void PerformFreeFit();
      void ResetFit();
      bool IsActivePoint();
      void ActivatePoint(const int pointNumber, const double x, const double y);
      void DeactivatePoint(); 
      void PrintHelp();
      void Draw(bool doDrawHist = true, bool isFirstDraw = true);

      std::vector<TH1F *> hists;
      std::vector<std::string> histsXValues;

      std::vector<std::string> outputFilesNames;
      std::vector<std::ofstream> outputFiles;

      std::vector<std::vector<TF1 *>> fits, bgFits;
      std::vector<std::vector<int>> bgFitParNumbersStart;

      TLatex fitsNames = TLatex(0.2, 0.2, "");
      TLatex chi2NDF = TLatex(0.8, 0.9, "");

      //graph that contains points for background tweaking
      TGraph *bgPointsGr;

      long unsigned int currentHistId = -1;
      unsigned long currentFitModeId = 0;

      bool isFitPointActive = false;
      int currentActivePointId = -1;
      TGraph *currentActivePointGr;

      const Color_t activeColor = kRed + 1;
   };
}

#endif /* ROOT_TOOLS_GUI_FIT_HPP */
