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

#include "TH2.h"

/*! @namespace GUIDistrCutter2D GUIDistrCutter2D namespace
 * @brief Stores various useful data and functions for functionality of GUI cutter. The only useful funtions for user are AddHistogram and WriteCutAreas. Other functions and variables are employed automaticaly when needed.
 *
 * Not finished yet
 */
namespace GUIDistrCutter2D
{
   /*! @brief Add histogram
    *
    * @param[in] hist histogram to be added. All added histograms must have the same number of bins and ranges of X and Y axis.
    * @param[in] name name of a histogram to be displayed
    */
   void AddHistogram(T *hist, const std::string& name);
   /*! @brief Write cut areas in the output file
    *
    * Writes in output file for every bin 0 or 1, i.e. whether the bin was or was not cut. Additionaly, in the beginning of the file number of x bins, x range, number of y bins, y range are written.
    *
    * @param[in] outputFileName name of the output file in which the data will be written. If the file with the same name exists, the old file will be renamed to (outputFileName + ".tmp") and the data will be written in file (outputFileName).
    */
   void WriteCutAreas(const std::string& outputFileName);
   /// Draws cut lines. This function is called automaticaly when needed.
   void DrawCutLines(double xMax, double yMax);
   /// Sets style to the provided TLine. This function is called automaticaly when needed.
   void SetLineStyle(const TLine& line, const Color_t color=kRed);
   /// Draws histogram with cuts applied. This function is called automaticaly when needed.
   void Draw(const bool isRangeFixed);
   /// Applies cuts to the passed distribution. This function is called automaticaly when needed.
   void ApplyCuts(const TH2D& hist);

   /// stores added distributions
   std::vector<T> hists;
   /// stores added distributions with cuts applied
   std::vector<T> histsWithCuts;
   /// stores information about the currently displayed histogram
   unsigned short currentHist = 0;
   /// stores information about the current cut mode
   short currentCutMode = -1;
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
   std::vector<double>angledLine1Y1;
   /// stores y2 values of the 1st line of angled line cuts
   std::vector<double>angledLine1Y2;
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
};

/// @brief Executable to pass to TPad::AddExec(name, command) to start GUI session for 2D distributions
void GUIDistrCutter2DExec();

#endif /* ROOT_TOOLS_GUI_DISTR_CUTTER_HPP */
