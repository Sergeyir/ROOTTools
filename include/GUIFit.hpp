// $HEADER$
//------------------------------------------------------------------------------------------------
//                                  GUIFit declaration
//------------------------------------------------------------------------------------------------
// GUIFit - graphical user interface for fits
//
// ** Free and open code for anyone to use **
//
// Author: Sergei Antsupov
// Email: antsupov0124@gmail.com
//
/**
 * Basic tool for tweaking TF1 approximations of the form: foreground + background
 **/
//------------------------------------------------------------------------------------------------

#ifndef GUI_FIT_HPP
#define GUI_FIT_HPP

#include <string>
#include <vector>

#include "TPad.h"
#include "TFrame.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TF1.h"

#include "ErrorHandler.hpp"
#include "IOTools.hpp"

class GUIFit
{
   public:
   
   GUIFit();
   
   void AddFitType(std::string outputFileName);
   void AddHist(TH1F *dataHist, std::string xVal);
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

   const Color_t activeColor = kRed+1;   
};

#endif /*GUI_FIT_HPP*/
