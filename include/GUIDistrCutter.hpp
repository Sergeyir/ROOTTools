/** 
 *  @file   GUIDistrCutter.hpp 
 *  @brief  Contains structs and functions that can be used for providing GUI for removal of "bad"/"dead" areas from 1D and 2D distributions
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_GUI_DISTR_CUTTER_HPP
#define ROOT_TOOLS_GUI_DISTR_CUTTER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

#include "TH2.h"
#include "TLine.h"
#include "Buttons.h"

/*! @namespace GUIDistrCutter2D GUIDistrCutter2D namespace
 * @brief Stores various useful data and functions for functionality of GUI cutter. The only useful funtions for user are AddHistogram, ReadCutAreas, WriteCutAreas, and Exec. Other functions and variables are employed automaticaly when needed.
 *
 * Not finished yet
 */
namespace GUIDistrCutter2D
{
   /*! @brief Add histogram
    *
    * @param[in] hist histogram to be added. All added histograms must have the same number of bins and ranges of X and Y axis.
    */
   void AddHistogram(TH2D *hist);
   /*! @brief Reads cut areas from the file. 
    *
    * The cuts from the file will be applied to all added histograms. If no histograms were added prior error will be written and exit(1) will be called.
    *
    * @param[in] inputFileName name of the file to read the info from
    */
   void ReadCutAreas(const std::string& inputFileName);
   /*! @brief Set the file in which cut areas will be written
    *
    * Data will be written for output file for every bin 0 or 1, i.e. whether the bin was or was not cut. Additionaly, in the beginning of the file number of x bins, x range, number of y bins, y range are written.
    *
    * @param[in] outputFileName name of the output file in which the data will be written. If the file with the same name exists, the old file will be renamed to (outputFileName + ".tmp") and the data will be written in file (outputFileName).
    */
   void SetOutputFile(const std::string& outputFileName);
   /// @brief Executable to pass to TPad::AddExec(name, command) to start GUI session
   void Exec();
   /// Sets style to the provided TLine. This function is called automaticaly when needed.
   void SetLine(TLine& line, const double x1, const double y1, 
                const double x2, const double y2, const Color_t color=kRed);
   /// Draws histogram with cuts applied. This function is called automaticaly when needed.
   void Draw(const bool isRangeFixed);
   /// Applies cuts to the passed distribution. This function is called automaticaly when needed.
   void ApplyCuts(TH2D* hist);
   /// Applies actions to kMouseMotion event. This function is called automaticaly when needed.
   void MouseMotionAction(const double x, const double y);
   /// Applies actions to kButton1Down event. This function is called automaticaly when needed.
   void Button1DownAction(const double x, const double y)
   /// Applies actions to kKeyPress event. This function is called automaticaly when needed.
   void KeyPressAction(const int button)
   /// Needed for angular cut mode. This function is called automaticaly when needed.
   double Pol1(const double x, const double par0, const double par1);
   /// stores added distributions
   std::vector<T> hists;
   /// stores added distributions with cuts applied
   std::vector<T> histsWithCuts;
   /// stores information about the index of the currently displayed histogram
   unsigned short currentHist = 0;
   /// stores information about cut areas that were read from the input file
   std::vector<std::vector<bool>> inputFileCutAreas;
   /// stores information about the current cut mode
   short currentCutMode = -1;
   /// stores x values of the single pixel cut mode
   std::vector<double> singleXCut;
   /// stores y values of the single pixel cut mode
   std::vector<double> singleYCut;
   /// stores minimum x values of the rectangular cut mode
	std::vector<double> rectXMin;
   /// stores maximum x values of the rectangular cut mode
   std::vector<double> rectXMax; 
   /// stores minimum y values of the rectangular cut mode
   std::vector<double> rectYMin;
   /// stores maximum y values of the rectangular cut mode
   std::vector<double> rectYMax;
   /// stores minimum x values of x linear cut mode
	std::vector<double> lineXMin;
   /// stores maximum x values of x linear cut mode
   std::vector<double> lineXMax;
   /// stores minimum y values of y linear cut mode
	std::vector<double> lineYMin;
   /// stores maximum y values of y linear cut mode
   std::vector<double> lineYMax;
   /// stores minimum x values of inverse rectangular cut mode
	std::vector<double> invRectXMin;
   /// stores maximum x values of inverse rectangular cut mode
   std::vector<double> invRectXMax;
   /// stores minimum y values of inverse rectangular cut mode
   std::vector<double> invRectYMin;
   /// stores maximum y values of inverse rectangular cut mode
   std::vector<double> invRectYMax;
   /// stores x1 values of the 1st line of angled line cuts
	std::vector<double> angledLine1X1;
   /// stores x2 values of the 1st line of angled line cuts
	std::vector<double> angledLine1X2;
   /// stores y1 values of the 1st line of angled line cuts
   std::vector<double> angledLine1Y1;
   /// stores y2 values of the 1st line of angled line cuts
   std::vector<double> angledLine1Y2;
   /// stores x1 values of the 2nd line of angled line cuts
	std::vector<double> angledLine2X1;
   /// stores x2 values of the 2nd line of angled line cuts
   std::vector<double> angledLine2X2;
   /// stores y1 values of the 2nd line of angled line cuts
   std::vector<double> angledLine2Y1;
   /// stores y2 values of the 2nd line of angled line cuts
   std::vector<double> angledLine2Y2;
   /// stores tangent of alpha of the 1st line of angled line cuts
	std::vector<double> tanAlpha1;
   /// stores tangent of alpha of the 2nd line of angled line cuts
   std::vector<double> tanAlpha2;
   /// stores y shifts of the 1st line of angled line cuts
   std::vector<double> shiftY1;
   /// stores y shifts of the 2nd line of angled line cuts
   std::vector<double> shiftY2;
   /// stores information that shows whether the first point was chosen for the given cut mode
	std::array<bool, 5> isMin = {true, true, true, true, true};
   /// file in which cut areas will be written
   std::ofstream outputFile;
};

#endif /* ROOT_TOOLS_GUI_DISTR_CUTTER_HPP */
