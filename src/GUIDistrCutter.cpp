/** 
 *  @file   GUIDistrCutter.cpp 
 *  @brief  Contains structs and functions that can be used for providing GUI for removal of "bad"/"dead" areas from 1D and 2D distributions
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_GUI_DISTR_CUTTER_CPP
#define ROOT_TOOLS_GUI_DISTR_CUTTER_CPP

#include "../include/GUIDistrCutter.h"

void GUIDistrCutter2D::AddHistogram(TH2D *hist)
{
   if (hists.size() != 0)
   {
      const int currentHistXNBins = hist->GetXaxis()->GetNbins();
      const int previousHistXNBins = hists.front()->GetXaxis()->GetNbins();
      const int currentHistYNBins = hist->GetYaxis()->GetNbins();
      const int previousHistYNBins = hists.front()->GetYaxis()->GetNbins();

      if (currentHistXNBins != previousHistXNBins)
      {
         std::cout << "\033[1m\033[31mError:\033[0m Number of bins of X axis is inconsistent for the histogram \"" << hist->GetName() << "\" compared to previous added one(s)" << std::endl;
         exit(1);
      }
      if (currentHistYNBins != previousHistYNBins)
      {
         std::cout << "\033[1m\033[31mError:\033[0m Number of bins of Y axis is inconsistent for the histogram \"" << hist->GetName() << "\" compared to previous added one(s)" << std::endl;
         exit(1);
      }
      if (fabs(hists.front()->GetXaxis()->GetBinLowEdge(1) - 
               hist->GetXaxis()->GetBinLowEdge(1)) > 1e-15 ||
          fabs(hists.front()->GetXaxis()->GetBinUpEdge(previousHistXNBins) - 
               hist->GetXaxis()->GetBinUpEdge(currentHistXNBins)) > 1e-15)
      {
         std::cout << "\033[1m\033[31mError:\033[0m X axis range is inconsistent for the histogram \"" << hist->GetName() << "\" compared to previous added one(s)" << std::endl;
         exit(1);
      }
      if (fabs(hists.front()->GetYaxis()->GetBinLowEdge(1) - 
               hist->GetYaxis()->GetBinLowEdge(1)) > 1e-15 ||
          fabs(hists.front()->GetYaxis()->GetBinUpEdge(previousHistYNBins) - 
               hist->GetYaxis()->GetBinUpEdge(currentHistYNBins)) > 1e-15)
      {
         std::cout << "\033[1m\033[31mError:\033[0m X axis range is inconsistent for the histogram \"" << hist->GetName() << "\" compared to previous added one(s)" << std::endl;
         exit(1);
      }
   }
   hists.push_back(static_cast<TH1D *>(hist.Clone()));
   histsWithCuts.push_back(static_cast<TH1D *>(hist.Clone()));
}

void GUIDistrCutter2D::ReadCutAreas(const std::string& inputFileName)
{
   if (hists.size() == 0)
   {
      std::cout << "\033[1m\033[31mError:\033[0m No histograms were added before the cut areas are read. Add at least one histogram first before reading the cuts from the file" << std::endl;
      exit(1);
   }
   if (inputFileCutAreas.size() != 0)
   {
      std::cout << "\033[1m\033[35mWarning:\033[0m Input file was already read; replacing current cuts from the previously read input file to the cuts written in file \"" << inputFileName << "\"" << std::endl;
   }

   inputFileCutAreas.clear();
   std::ifstream inputFile(inputFileName);
   if (!inputFile.is_open())
   {
      std::cout << "\033[1m\033[31mError\033[0m while trying to read cuts from file: File \"" inputFileName << "\" does not exist" << std::endl;
      exit(1);
   }

   const int addedHistXNBins = hists.front().GetXaxis()->GetNBins();
   const int addedHistYNBins = hists.front().GetYaxis()->GetNBins();

   int inputXNBins, inputYNBins;
   double inputXAxisMin, inputXAxisMax, inputYAxisMin, inputYAxisMax;

   if (!(inputFile >> inputXNBins >> inputXAxisMin >> inputXAxisMax >> 
                      inputYNBins >> inputYAxisMin >> inputYAxisMax));
   {
      std::cout << "\033[1m\033[31mError\033[0m while trying to read axis information from file \"" inputFileName << "\": Unexpected end of file" << std::endl;
      exit(1);
   }

   if (inputXNBins != addedHistXNBins)
   {
      std::cout << "\033[1m\033[31mError:\033[0m Number of bins of X axis in file \"" << inputFileName << "\" is inconsistent with added histograms" << std::endl;
      exit(1);
   }
   if (inpuYNBins != addedHistYNBins)
   {
      std::cout << "\033[1m\033[31mError:\033[0m Number of bins of Y axis in file \"" << inputFileName << "\" is inconsistent with added histogram(s)" << std::endl;
      exit(1);
   }
   if (fabs(hists.front()->GetXaxis()->GetBinLowEdge(1) - inputXAxisMin) > 1e-15 ||
       fabs(hists.front()->GetXaxis()->GetBinUpEdge(addedHistXNBins) - inputXAxisMax) > 1e-15)
   {
      std::cout << "\033[1m\033[31mError:\033[0m X axis range in file \"" << inputFileName << "\" is inconsistent with added histogram(s)" << std::endl;
      exit(1);
   }
   if (fabs(hists.front()->GetYaxis()->GetBinLowEdge(1) - inputYAxisMin) > 1e-15 ||
       fabs(hists.front()->GetYaxis()->GetBinUpEdge(addedHistYNBins) - inputYAxisMax) > 1e-15)
   {
      std::cout << "\033[1m\033[31mError:\033[0m Y axis range in file \"" << inputFileName << "\" is inconsistent with added histogram(s)" << std::endl;
      exit(1);
   }

   inputFileCutAreas.resize(hists.back()->GetYaxis()->GetNbins());

   bool tmp;

   for (int i = 0; i < hists.back()->GetYaxis()->GetNbins(); i++)
   {
      if (!(inputFile >> tmp))
      {
      std::cout << "\033[1m\033[31mError\033[0m while trying to read cuts from file \"" << inputFileName << "\": Unexpected end of file" << std::endl;
         exit(1);
      }
      inputFileCutAreas[i].push_back(tmp);
   }

   if (inputFile >> tmp)
   {
      std::cout << "\033[1m\033[31mError\033[0m while trying to read cuts from file \"" << inputFileName << "\": leftover data detected" << std::endl;
      exit(1);
   }
}

void GUIDistrCutter2D::SetOutputFile(const std::string outputFileName)
{
   std::ifstream inputFile(outputFileName);
   if (outputFileName.is_open())
   {
      std::cout << "\033[1m\033[35mWarning:\033[0m Specified output file \"" << inputFileName << "\" already exists" << std::endl;
      std::cout << "Old file \"" << inputFileName << " will be renamed to \"" << inputFileName << ".tmp\"" << std::endl;
      system(("mv " + inputFileName + " " + inputFileName + ".tmp").c_str());
   }

   outputFile.open(outputFileName);
}

void GUIDistrCutter2D::SetLine(TLine &line, const double x1, const double y1, 
                               const double x2, const double y2, const color_t)
{
   line.SetX1(x1);
   line.SetY1(y1);
   line.SetX2(x2);
   line.SetY2(y2);
	line.SetLineColor(color);
	line.SetLineStyle(2);
	line.SetLineWidth(2);
}

void GUIDistrCutter2D::MouseMotionAction(const double x, const double y)
{
   if (currentCutMode < 0 || currentCutMode > 4) return;

   TLine line1, line2, line3, line4;

	switch (currentCutMode)

	{
		case 0:
      {
         if (isMin[currentCutMode]) return;

         SetLine(line1, rectXMin.back(), rectYMin.back(), x, rectYMin.back());
         SetLine(line2, rectXMin.back(), rectYMin.back(), rectXMin.back(), y);
         SetLine(line3, x, rectYMin.back(), x, y);
         SetLine(line4, rectXMin.back(), y, x, y);

         line1.Draw(); line2.Draw(); line3.Draw(); line4.Draw();

			break;
      }
		case 1:
      {
         if (isMin[currentCutMode]) return;

         const double yMin = hists.front()->GetYaxis()->GetBinLowEdge(1);
         const double yMax = 
            hists.front()->GetYaxis()->GetBinUpEdge(hists.front()->GetYaxis()->GetNbins());

         SetLine(line1, lineXMin.back(), yMin, lineXMin.back(), yMax);
         SetLine(line2, x, yMin, x, yMax);

         line1.Draw(); line2.Draw();

			break;
      }
		case 2:
      {
         if (isMin[currentCutMode]) return;

         const double xMin = hists.front()->GetXaxis()->GetBinLowEdge(1);
         const double xMax = 
            hists.front()->GetXaxis()->GetBinUpEdge(hists.front()->GetXaxis()->GetNbins());

         SetLine(line1, xMin, lineYMin.back(), xMax, lineYMin.back());
         SetLine(line2, xMin, y, xMax, y);

         line1.Draw(); line2.Draw();

			break;
      }
		case 3:
      {
         if (isMin[currentCutMode]) return;

         SetLine(line1, invRectXMin.back(), invRectYMin.back(), x, invRectYMin.back());
         SetLine(line2, invRectXMin.back(), invRectYMin.back(), invRectXMin.back(), y);
         SetLine(line3, x, invRectYMin.back(), x, y);
         SetLine(line4, invRectXMin.back(), y, x, y);

         line1.Draw(); line2.Draw(); line3.Draw(); line4.Draw();

			break;
      }
      case 4:
      {
         const double lineXMin = hists.front()->GetXaxis()->GetBinLowEdge(1);
         const double lineXMax = 
            hists.front()->GetXaxis()->GetBinUpEdge(hists.front()->GetXaxis()->GetNbins());

         if (angledLine1X1.size() > angledLine2X1.size())
         {
            if (angledLine1X1.size() > angledLine1X2.size())
            {
               if (isMin[currentCutMode]) return;

               const double tanAlpha = (angledLine1Y1.back() - y)/(angledLine1X1.back() - x);

               SetLine(line1, lineXMin, Pol1(lineXMin, y - x*tanAlpha, tanAlpha), 
                       lineXMax, Pol1(lineXMax, y - x*tanAlpha, tanAlpha));
            }
            else
            {
               SetLine(line1, lineXMin, Pol1(lineXMin, shiftY1.back(), tanAlpha1.back()), 
                       lineXMax, Pol1(lineXMax, shiftY1.back(), tanAlpha1.back()), kGray);
            }

            line1.Draw();
         }
         else if (angledLine2X1.size() > 0 && !isMin[currentCutMode])
         {
            const double tanAlpha = (angledLine2Y1.back() - y)/(angledLine2X1.back() - x);

            SetLine(line1, lineXMin, Pol1(lineXMin, y - x*tanAlpha, tanAlpha), 
                    lineXMax, Pol1(lineXMax, y - x*tanAlpha, tanAlpha));

            SetLine(lineXMin, Pol1(lineXMin, shiftY1.back(), tanAlpha1.back()), 
                    lineXMax, Pol1(lineXMax, shiftY1.back(), tanAlpha1.back()), kGray);

            line1.Draw(); line2.Draw();
         }
         break;
      }
	}

   gPad->Modified();
   gPad->Update();
   gPad->GetFrame()->SetBit(TBox::kCannotMove);
}

void GUIDistrCutter2D::Button1DownAction(const double x, const double y)
{
   switch (currentCutMode)
   {
      case 0:
      {
         if (isMin[0])
         {
            rectXMin.push_back(x);
            rectYMin.push_back(y);

            isMin[0] = false;
            std::cout << "Setting the first point" << std::endl;	
         }
         else
         {
            if (x >= rectXMin.back()) 
            {
               rectXMax.push_back(hists.front()->GetXaxis()->
                                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(x)));
               rectXMin.back() = hists.front()->GetXaxis()->
                  GetBinLowEdge(hists.front()->GetXaxis()->FindBin(rectXMin.back()));
            }
            else 
            {
               rectXMax.push_back(hists.front()->GetXaxis()->
                                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(rectXMin.back())));
               rectXMin.back() = hists.front()->GetXaxis()->
                  GetBinLowEdge(hists.front()->GetXaxis()->FindBin(x));
            }

            if (y >= rectYMin.back()) 
            {
               rectYMax.push_back(hists.front()->GetYaxis()->
                                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(y)));
               rectYMin.back() = hists.front()->GetYaxis()->
                  GetBinLowEdge(hists.front()->GetYaxis()->FindBin(rectYMin.back()));
            }
            else 
            {
               rectYMax.push_back(hists.front()->GetYaxis()->
                                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(rectYMin.back())));
               rectYMin.back() = hists.front()->GetYaxis()->
                  GetBinLowEdge(hists.front()->GetYaxis()->FindBin(y));
            }

            isMin[0] = true;
            std::cout << "Setting the second point" << std::endl;
            Draw(true);
         }
         break;
      }
      case 1:
      {
         if (isMin[1])
         {
            lineXMin.push_back(x);

            isMin[1] = false;
            std::cout << "Setting the first line" << std::endl;	
         }
         else
         {
            if (x >= lineXMin.back()) 
            {
               lineXMax.push_back(hists.front()->GetXaxis()->
                                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(x)));
               lineXMin.back() = hists.front()->GetXaxis()->
                  GetBinLowEdge(hists.front()->GetXaxis()->FindBin(lineXMin.back()));
            }
            else 
            {
               lineXMax.push_back(hists.front()->GetXaxis()->
                                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(lineXMin.back())));
               lineXMin.back() = hists.front()->GetXaxis()->
                  GetBinLowEdge(Par.realHist->GetXaxis()->FindBin(x));
            }

            isMin[1] = true;
            std::cout << "Setting the second point" << std::endl;
            Draw(true);
         }
         break;
      }
      case 2:
      {
         if (isMin[2])
         {
            lineYMin.push_back(y);

            isMin[2] = false;
            std::cout << "Setting the first line" << std::endl;	
         }
         else
         {
            if (y >= lineYMin.back()) 
            {
               lineYMax.push_back(hists.front()->GetYaxis()->
                                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(y)));
               lineYMin.back() = hists.front()->GetYaxis()->
                  GetBinLowEdge(hists.front()->GetYaxis()->FindBin(lineYMin.back()));
            }
            else 
            {
               lineYMax.push_back(hists.front()->GetYaxis()->
                                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(lineYMin.back())));
               lineYMin.back() = hists.front()->GetYaxis()->
                  GetBinLowEdge(hists.front()->GetYaxis()->FindBin(y));
            }

            isMin[2] = true;
            std::cout << "Setting the second point" << std::endl;
            Draw(true);
         }
         break;
      }
      case 3:
      {
         if (isMin[3])
         {
            invRectXMin.push_back(x);
            invRectYMin.push_back(y);

            isMin[3] = false;
            std::cout << "Setting the first point" << std::endl;	
         }
         else
         {
            if (x >= invRectXMin.back()) 
            {
               invRectXMax.push_back(hists.front()->GetXaxis()->
                                     GetBinLowEdge(hists.front()->GetXaxis()->FindBin(x)));
               invRectXMin.back() = hists.front()->GetXaxis()->
                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(invRectXMin.back()));
            }
            else 
            {
               invRectXMax.push_back(hists.front()->GetXaxis()->GetBinLowEdge(hists.front()->
                                     GetXaxis()->FindBin(invRectXMin.back())));
               invRectXMin.back() = hists.front()->GetXaxis()->
                  GetBinUpEdge(hists.front()->GetXaxis()->FindBin(x));
            }

            if (y >= invRectYMin.back()) 
            {
               invRectYMax.push_back(hists.front()->GetYaxis()->
                                     GetBinLowEdge(hists.front()->GetYaxis()->FindBin(y)));
               invRectYMin.back() = hists.front()->GetYaxis()->
                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(invRectYMin.back()));
            }
            else 
            {
               invRectYMax.push_back(hists.front()->GetYaxis()->GetBinLowEdge(hists.front()->
                                     GetYaxis()->FindBin(invRectYMin.back())));
               invRectYMin.back() = hists.front()->GetYaxis()->
                  GetBinUpEdge(hists.front()->GetYaxis()->FindBin(y));
            }

            isMin[3] = true;
            std::cout << "Setting the second point" << std::endl;
            Draw(true);
         }
         break;
      }
      case 4:
      {
         if (isMin[4])
         {
            if (angledLine1X1.size() == angledLine2X2.size())
            {
               angledLine1X1.push_back(hists.front()->GetXaxis()->
                                       GetBinLowEdge(hists.front()->GetXaxis()->FindBin(x)));
               angledLine1Y1.push_back(hists.front()->GetYaxis()->
                                       GetBinLowEdge(hists.front()->GetYaxis()->FindBin(y)));

               isMin[4] = false;
               std::cout << "Setting the first point of the first line" << std::endl;
            }
            else
            {
               angledLine2X1.push_back(hists.front()->GetXaxis()->
                                       GetBinLowEdge(hists.front()->GetXaxis()->FindBin(x)));
               angledLine2Y1.push_back(hists.front()->GetYaxis()->
                                       GetBinLowEdge(hists.front()->GetYaxis()->FindBin(y)));

               isMin[4] = false;
               std::cout << "Setting the second point of the first line" << std::endl;	
            }
         }
         else
         {
            if (angledLine1X2.size() == angledLine2X2.size())
            {
               angledLine1X2.push_back(hists.front()->GetXaxis()->GetBinLowEdge(
                                       hists.front()->GetXaxis()->FindBin(x)));
               angledLine1Y2.push_back(hists.front()->GetYaxis()->GetBinLowEdge(
                                       hists.front()->GetYaxis()->FindBin(y)));

               tanAlpha1.push_back((angledLine1Y1.back() - angledLine1Y2.back())/
                                   (angledLine1X1.back() - angledLine1X2.back()));
               shiftY1.push_back(angledLine1Y2.back() - angledLine1X2.back()*tanAlpha1.back());

               isMin[4] = true;
               std::cout << "Setting the second point of the first line" << std::endl;
               Draw(true);
            }
            else
            {
               angledLine2X2.push_back(hists.back()->GetXaxis()->
                                       GetBinLowEdge(hists.back()->GetXaxis()->FindBin(x)));
               angledLine2Y2.push_back(hists.back()->GetYaxis()->
                                       GetBinLowEdge(hists.back()->GetYaxis()->FindBin(y)));

               tanAlpha2.push_back((angledLine2Y1.back() - angledLine2Y2.back())/
                                   (angledLine2X1.back() - angledLine2X2.back()));
               shiftY2.push_back(angledLine2Y2.back() - angledLine2X2.back()*tanAlpha2.back());
 
               isMin[4] = true;
               std::cout << "Setting the second point of the second line" << std::endl;
               Draw(true);
            }
         }
         break;
      }
   }
}

void GUIDistrCutter2D::KeyPressAction(const int button)
{
   switch(button)
   {
      case 'u':
         switch (currentCutMode)
         {
            case 0:
            {
               if (rectXMin.size() != 0)
               {
                  if (isMin[0])
                  {
                     rectXMin.pop_back();
                     rectYMin.pop_back();
                     rectXMax.pop_back();
                     rectYMax.pop_back();

                     std::cout << "Deleting last minimum and maximum points" << std::endl;
                  }
                  else
                  {
                     rectXMin.pop_back();
                     rectYMin.pop_back();

                     std::cout << "Deleting last minimum point" << std::endl;
                  }
                  if (isMin[0]) Draw(true);
                  isMin[0] = true;
                  gPad->Update();
               }
               else std::cout << "Cannot delete last point since the current number of points is 0" << std::endl;
               break;
            }

            case 1:
               if (lineXMin.size() != 0)
               {
                  if (isMin[1])
                  {
                     lineXMin.pop_back();
                     lineXMax.pop_back();

                     std::cout << "Deleting last pair of lines" << std::endl;
                  }
                  else
                  {
                     lineXMin.pop_back();
                     std::cout << "Deleting last line" << std::endl;
                  }
                  if (isMin[1]) Draw(true);
                  isMin[1] = true;
                  gPad->Update();
               }
               else std::cout << "Cannot delete last line/lines since the current number of lines is 0" << std::endl;
               break;
            case 2:
               if (lineYMin.size() != 0)
               {
                  if (isMin[2])
                  {
                     lineYMin.pop_back();
                     lineYMax.pop_back();

                     std::cout << "Deleting last pair of lines" << std::endl;
                  }
                  else
                  {
                     lineYMin.pop_back();
                     std::cout << "Deleting last line" << std::endl;
                  }
                  if (isMin[2]) Draw(true);
                  isMin[2] = true;
                  gPad->Update();
               }
               else std::cout << "Cannot delete last line since the current number of lines is 0" << std::endl;
               break;
            case 3:
               if (invRectXMin.size() != 0)
               {
                  if (isMin[3])
                  {
                     invRectXMin.pop_back();
                     invRectYMin.pop_back();
                     invRectXMax.pop_back();
                     invRectYMax.pop_back();

                     std::cout << "Deleting last minimum and maximum points" << std::endl;
                  }
                  else
                  {
                     invRectXMin.pop_back();
                     invRectYMin.pop_back();

                     std::cout << "Deleting last minimum point" << std::endl;
                  }
                  if (isMin[3]) Draw(true);
                  isMin[3] = true;
                  gPad->Update();
               }
               else std::cout << "Cannot delete last point since the current number of points is 0" << std::endl;
               break;
            case 4:
               if (angledLine1X1.size() != 0)
               {
                  if (angledLine1X2.size() > angledLine2X1.size())
                  {
                     if (isMin[4])
                     {
                        angledLine1X1.pop_back();
                        angledLine1Y1.pop_back();
                        angledLine1X2.pop_back();
                        angledLine1Y2.pop_back();
                        tanAlpha1.pop_back();
                        shiftY1.pop_back();

                        std::cout << "Deleting last minimum and maximum points" << std::endl;
                     }
                     else
                     {
                        angledLine1X1.pop_back();
                        angledLine1Y1.pop_back();

                        std::cout << "Deleting last minimum point" << std::endl;
                     }
                     if (isMin[4]) Draw(true);
                     isMin[4] = true;
                     gPad->Update();
                  }
                  else
                  {
                     if (isMin[4])
                     {
                        angledLine2X1.pop_back();
                        angledLine2Y1.pop_back();
                        angledLine2X2.pop_back();
                        angledLine2Y2.pop_back();
                        tanAlpha2.pop_back();
                        shiftY2.pop_back();

                        std::cout << "Deleting last minimum and maximum points" << std::endl;
                     }
                     else
                     {
                        angledLine2X1.pop_back();
                        angledLine2Y1.pop_back();

                        std::cout << "Deleting last minimum point" << std::endl;
                     }
                  }
                  if (isMin[4]) Draw(true);
                  isMin[4] = true;
                  gPad->Update();
               }
               else std::cout << "Cannot delete last point since the current number of points is 0" << std::endl;
               break;
         }
         break;

      case 'r':
         Draw();
         std::cout << "Resetting the range of the selected area" << std::endl;
         break;

      case 'p':
      {
         // Printing inverse box cut first
         for (int i = 0; i < invRectXMax.size(); i++)
         {
            std::cout << Par.tab <<
               "if (" + Par.xValName + " < " << invRectXMin[i] <<
               " || " + Par.yValName + " < " << invRectYMin[i] <<
               " ||" << std::endl << Par.tab << "    " + Par.xValName + " > " << 
               invRectXMax[i] << " || " + Par.yValName + 
               " > " << invRectYMax[i] << ") return true;" << std::endl;
         }

         // Determining bin ranges along X and Y axis
         int minXBin = 1;
         int minYBin = 1;
         int maxXBin = Par.realHistDM->GetXaxis()->GetNbins();
         int maxYBin = Par.realHistDM->GetYaxis()->GetNbins();

         if (invRectXMax.size() != 0)
         {
            for (unsigned long i = 0; i < invRectXMax.size(); i++)
            {
               minXBin = CppTools::Maximum(Par.realHist->GetXaxis()->FindBin(invRectXMin[i]), 
                                 minXBin);
               minYBin = CppTools::Maximum(Par.realHist->GetYaxis()->FindBin(invRectYMin[i]), 
                                 minYBin);
               maxXBin = CppTools::Minimum(Par.realHist->GetXaxis()->FindBin(invRectXMax[i]), 
                                 maxXBin);
               maxYBin = CppTools::Minimum(Par.realHist->GetYaxis()->FindBin(invRectYMax[i]), 
                                 maxYBin);
            }
            minXBin++;
            minYBin++;
            maxXBin--;
            maxYBin--;
         }
         else
         {
            for (int i = 1; i <= Par.realHistDM->GetXaxis()->GetNbins(); i++)
            {
               bool isSelected = false;
               for (int j = 1; j <= Par.realHistDM->GetYaxis()->GetNbins(); j++)
               {
                  if (Par.realHist->GetBinContent(i, j) > 1e-7)
                  {
                     minXBin = i;
                     isSelected = true;
                     break;
                  }
               }
               if (isSelected) break;
            }
            for (int i = 1; i <= Par.realHistDM->GetYaxis()->GetNbins(); i++)
            {
               bool isSelected = false;
               for (int j = 1; j <= Par.realHistDM->GetXaxis()->GetNbins(); j++)
               {
                  if (Par.realHist->GetBinContent(j, i) > 1e-7)
                  {
                     minYBin = i;
                     isSelected = true;
                     break;
                  }
               }
               if (isSelected) break;
            }
            for (int i = Par.realHistDM->GetXaxis()->GetNbins(); i >= minXBin; i--)
            {
               bool isSelected = false;
               for (int j = maxYBin; j >= minYBin; j--)
               {
                  if (Par.realHist->GetBinContent(i, j) > 1e-7)
                  {
                     maxXBin = i;
                     isSelected = true;
                     break;
                  }
               }
               if (isSelected) break;
            }
            for (int i = Par.realHistDM->GetYaxis()->GetNbins(); i >= minYBin; i--)
            {
               bool isSelected = false;
               for (int j = maxXBin; j >= minXBin; j--)
               {
                  if (Par.realHist->GetBinContent(j, i) > 1e-7)
                  {
                     maxYBin = i;
                     isSelected = true;
                     break;
                  }
               }
               if (isSelected) break;
            }
         }

         // Printing all other cuts
         std::cout << "switch(" << Par.yValName << "Bin)" << std::endl;
         std::cout << "{" << std::endl;

         for (int i = minYBin; i <= maxYBin; i++)
         {
            unsigned int numberOfEmptyBins = 0;
            unsigned int numberOfCuts = 0;

            for (int j = minXBin; j <= maxXBin; j++)
            {
               if (Par.realHistDM->GetBinContent(j, i) < 1e-7) numberOfCuts++;
               if (Par.realHist->GetBinContent(j, i) < 1e-7) numberOfEmptyBins++;
            }

            //if (numberOfEmptyBins == numberOfCuts) continue;

            if (numberOfCuts != maxXBin - minXBin + 1)
            {
               std::cout << Par.tab << "case " << i << ": switch(" << Par.xValName << "Bin) {";

               for (int j = minXBin; j <= maxXBin; j++)
               {
                  if (Par.realHistDM->GetBinContent(j, i) < 1e-7)
                  {
                     std::cout << "case " << j << ": ";
                  }
               }

               std::cout << "return true; break;} ";
            }
            else 
            {
               std::cout << Par.tab << "case " << i << ": return true; ";
            }
            std::cout << "break;" << std::endl;
         }

         std::cout << "}" << std::endl;
         break;
      }

      case 's':
      {
         if (Par.useSimHist)
         {
            Par.isCurrentSim = !Par.isCurrentSim;
            if (Par.isCurrentSim) std::cout << "Showing sim histogram" << std::endl;
            else std::cout << "Showing real data histogram" << std::endl;
         }
         else
         {
            std::cout << "Cannot show sim histogram since the option for it was not specified" << std::endl;
         }
         Draw();
         break;
      }

      case '0':
         std::cout << "Deactivating cutting mode" << std::endl;
         currentCutMode = -1;
         break;

      case '1':
         std::cout << "Activating rectangular cutting mode" << std::endl;
         currentCutMode = 0;
         break;

      case '2':
         std::cout << "Activating linear cutting mode along x axis" << std::endl;
         currentCutMode = 1;
         break;

      case '3':
         std::cout << "Activating linear cutting mode along y axis" << std::endl;
         currentCutMode = 2;
         break;

      case '4':
         std::cout << "Activating inverse rectangular cutting mode" << std::endl;
         currentCutMode = 3;
         break;

      case '5':
         std::cout << "Activating angled linear cutting mode" << std::endl;
         currentCutMode = 4;
         break;
      
      case '6':
         std::cout << "Activating single bin cutting mode" << std::endl;
         currentCutMode = 5;
         break;
   }
}

double Pol1(const double x, const double par0, const double par1)
{
   return par0 + x*par1;
}

void GUIDistrCutter2D::Exec()
{
	const int event = gPad->GetEvent();
	const int px = gPad->GetEventX();
	const int py = gPad->GetEventY();

	const double x = gPad->PadtoX(gPad->AbsPixeltoX(px));
	const double y = gPad->PadtoY(gPad->AbsPixeltoY(py));

   switch (event)
   {
      case kMouseMotion:
         MouseMotionAction(x, y);
         break;
      case kButton1Down:
         Button1DownAction(x, y);
         break;
      case kKeyPress:
         KeyPressAction(px);
         break;
   }
}

#endif /* ROOT_TOOLS_GUI_DISTR_CUTTER_CPP */
