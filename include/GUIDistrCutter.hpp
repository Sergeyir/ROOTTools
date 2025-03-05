/** 
 *  @file   GUIDistrCutter.hpp 
 *  @brief  Contains class that can be used for providing GUI for removal of "bad"/"dead" areas from 1D and 2D distributions
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_GUI_DISTR_CUTTER_HPP
#define ROOT_TOOLS_GUI_DISTR_CUTTER_HPP

#include "TH1.h"
#include "TH2.h"

/*! @class GUIDistrCutter
 * @brief class GUIFDA that can be used for providing GUI for removal of "bad"/"dead" areas from TH2D, TH2F, TH2L distributions
 *
 * Multiple histograms can be viewed at the same time if they were added with a constructor or with AddHistogram method
 *
 * Not finished yet
 */
template<typename T>
class GUIDistrCutter2D
{
   public:
   /// @brief Default constructor
   GUIDistrCutter2D();
   /*! @brief Constructor for TH1F, TH1D, and TH1L types
    *
    * @param[in] hist histogram to be used
    * @param[in] name name of a histogram to be displayed
    */
   GUIDistrCutter2D(T *hist, const std::string& name);
   /*! @brief Add histogram
    *
    * @param[in] hist histogram to be used
    * @param[in] name name of a histogram to be displayed
    */
   void AddHistogram(T *hist, const std::string& name);

   /// @brief Destructor
   virtual ~GUIDistrCutter1D();

   protected:

   int currentCutMode = -1;

	std::vector<double> rectXMin, rectXMax, rectYMin, rectYMax;
	std::vector<double> lineXMin, lineXMax;
	std::vector<double> lineYMin, lineYMax;
	std::vector<double> invRectXMin, invRectXMax, invRectYMin, invRectYMax;
	std::vector<double> angledLine1X1, angledLine1X2, angledLine1Y1, angledLine1Y2;
	std::vector<double> angledLine2X1, angledLine2X2, angledLine2Y1, angledLine2Y2;
	std::vector<double> tanAlpha1, tanAlpha2, shiftY1, shiftY2;
}

#endif /* ROOT_TOOLS_GUI_DISTR_CUTTER_HPP */
