/** 
 *  @file   GUIFit.hpp 
 *  @brief  Contains class that can be used for providing GUI for improving approximations of form "foreground + background" by tweaking the badly approximated background
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
#include "TFile.h"

/*! @namespace GUIFit
 * @brief GUIFit can be used for providing GUI for improving approximations of form "foreground + background" by tweaking badly approximated background
 */
namespace GUIFit
{
   /*! @brief Adds a histogram fits of which will be tweaked
    *
    * @param[in] hist histogram to be added
    * @param[in] val value corresponding to this histogram. This value will be written for the currenlty added histogram in output file containing approximation parameters. This way the value can help map approximation parameters to corresponding histograms. Values must be unique for each histogram
    */
   void AddHistogram(TH1D *hist, const std::string& histVal, const std::string& histName = "");
   /*! @brief Adds a new fit type. Fits across once fit type have the same function (GUIFit class will not check this, user needs to do this themselves)
    *
    * @param[in] outputFileName name of the file in which background approximation parameters will be written
    * @param[in] fitTypeName name of a fit type which will be displayed when this specific fit type is active
    *
    * @param[out] fit type index. This value is needed to tag the fit type of an approximation and is used in GUIFit::AddFit function call.
    */
   unsigned int AddFitType(const std::string& outputFileName, const std::string& fitTypeName);
   /*! @brief Adds a fit to a list of fits for the last added histogram. At least one histogram must be added via GUIFit::AddHist function call before you can add any fits.
    *
    * @param[in] fit fit of a signal+background
    * @param[in] fitBG fit of a background
    * @param[in] fitTypeIndex index of a fit type (returned by GUIFit::AddFitType)
    * @param[in] histIndex index of a histogram (number of the specific added histogram - 1)
    * @param[in] fitBGParIndexBegin index of a parameters at which indices for background function begin
    * @param[in] fitBGParIndexEnd index of a parameters at which indices for background function end. If negative number is specified (by default) this index will point to the end of fit parameter indices (useful for fits like "signal[0] + background[fitBGParBeginIndex]")
    */
   void AddFit(TF1 *fit, TF1 *fitBG, const unsigned int fitTypeIndex, const unsigned int histIndex,
               const int fitBGParIndexBegin, const int fitBGParIndexEnd);
   /*! @brief Adds a fit to a list of fits for the last added histogram. At least one histogram must be added via GUIFit::AddHist function call before you can add any fits.
    *
    * @param[in] fit fit of a signal+background
    * @param[in] fitBG fit of a background
    * @param[in] fitTypeIndex index of a fit type (returned by GUIFit::AddFitType)
    * @param[in] fitBGParIndexBegin index of a parameters at which indices for background function begin
    * @param[in] fitBGParIndexEnd index of a parameters at which indices for background function end. If negative number is specified (by default) this index will point to the end of fit parameter indices (useful for fits like "signal[0] + background[fitBGParBeginIndex]")
    */
   void AddFit(TF1 *fit, TF1 *fitBG, const unsigned int fitTypeIndex, 
               const int fitBGParIndexBegin, const int fitBGParIndexEnd = -1);
   /// @brief Executable to pass to TPad::AddExec(name, command) to start GUI session
   void Exec();
   /// Sets points for background. This function is called automaticaly when needed
   void SetBGPoints();
   /// Sets active fit. This function is called automaticaly when needed
   void SetActiveFit();
   /// Performs approximation of a histogram without any limitations on background. 
   /// This function is called automaticaly when needed
   void PerformFreeFit();
   /// Resets the background approximation (sets background points on top of a histogram). 
   /// This function is called automaticaly when needed
   void ResetFit();
   /// Sets the active point. This function is called automaticaly when needed
   void ActivatePoint(const int pointNumber, const double x, const double y);
   /// Sets current active point to be inactive. This function is called automaticaly when needed
   void DeactivateCurrentActivePoint(); 
   /// Prints help banner with info. This function is called automaticaly when needed
   void PrintHelp();
   /// Draws the current chosen histogram with its approximations. 
   /// This function is called automaticaly when needed
   void Draw(bool doDrawHist = true);
   /// Writes background approximation parameters in a specified file
   /// This function is called automaticaly when needed
   void Write();
   /// Sets up everything when GUI::Exec is called. This function is called automaticaly when needed
   void Start();
   /// Contains all added histograms. This variable is handled automaticaly
   std::vector<TH1D *> hists;
   /// Contains corresponding values of histograms. This variable is handled automaticaly
   std::vector<std::string> histValues;
   /// Contains corresponding names of histograms. Can be used to display the name on the canvas
   std::vector<std::string> histNames;
   /// Contains names of files in which parameters will be written. 
   /// This variable is handled automaticaly
   std::vector<std::string> outputFileNames;
   /// Contains fit type names. This variable is handled automaticaly
   std::vector<std::string> fitTypeNames;
   /// Contains fits of different types for different histograms. 
   /// This variable is handled automaticaly
   std::vector<std::vector<TF1 *>> fits;
   /// Contains background fits of different types for different histograms. 
   /// This variable is handled automaticaly
   std::vector<std::vector<TF1 *>> fitsBG;
   /// Contains begin indices of background function of fits of
   /// different types for different histograms.This variable is handled automaticaly
   std::vector<std::vector<int>> fitBGParIndicesBegin;
   /// Contains end indices of background function of fits of 
   /// different types for different histograms. This variable is handled automaticaly
   std::vector<std::vector<int>> fitBGParIndicesEnd;
   /// Used to display the name of a current fit type. This variable is handled automaticaly
   TLatex fitNameTex = TLatex(0.2, 0.2, "");
   /// Used to display chi^2/NDF of a fit. This variable is handled automaticaly
   TLatex chi2NDFTex = TLatex(0.8, 0.9, "");
   /// Used to display histogram name. This variable is handled automaticaly
   TLatex histNameTex = TLatex(0.1, 0.9, "");
   /// Graph that contains movable points for background tweaking. 
   /// This variable is handled automaticaly
   TGraph *grBGPoints = new TGraph;
   /// Points to the index of a currently displayed histogram. This variable is handled automaticaly
   int currentHistId = -1;
   /// Points to the index of a currently active fit. This variable is handled automaticaly
   int currentFitTypeIndex = -1;
   /// Shows whether there is an active (i.e. movable) point. This variable is handled automaticaly
   bool isFitPointActive = false;
   /// Points to the index of a currently active (i.e. movable) point. 
   /// This variable is handled automaticaly
   int currentActivePointIndex = -1;
   /// Graph that contains currently active (i.e. movable) point. 
   /// This variable is handled automaticaly
   TGraph *currentActivePointGr = new TGraph;
   /// Color of an active point and fit. This variable is handled automaticaly
   const Color_t activeColor = kRed + 1;
   /// Shows whether GUIFit::Start was succesfully called
   bool isStartSuccessfull = false;
};

#endif /* ROOT_TOOLS_GUI_FIT_HPP */
