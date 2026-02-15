/** 
 *  @file   GUIFit.cpp 
 *  @brief  Contains class that can be used for providing GUI for improving the approximations of form "foreground + background" by tweaking the background
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_GUI_FIT_CPP
#define ROOT_TOOLS_GUI_FIT_CPP

#include "GUIFit.hpp"

unsigned int GUIFit::AddFitType(const std::string& outputFileName, const std::string& fitTypeName)
{
   std::ifstream inputFile(outputFileName);
   if (inputFile.is_open())
   {
      std::cout << "\033[1m\033[35mWarning:\033[0m Specified output file \"" << 
                   outputFileName << "\" already exists" << std::endl;
      std::cout << "Old file \"" << outputFileName << " will be renamed to \"" << 
                   outputFileName << ".backup\" when new file will be written" << std::endl;
      system(("cp '" + outputFileName + "' '" + outputFileName + ".backup'").c_str());
   }
   outputFileNames.emplace_back(outputFileName);
   fitTypeNames.emplace_back(fitTypeName);

   return fitTypeNames.size() - 1;
}

void GUIFit::AddHistogram(TH1D *hist, const std::string& histVal)
{
   fits.resize(fits.size() + 1);
   fitsBG.resize(fitsBG.size() + 1);
   fitBGParIndicesBegin.resize(fitBGParIndicesBegin.size() + 1);
   fitBGParIndicesEnd.resize(fitBGParIndicesEnd.size() + 1);

   hists.emplace_back((TH1D *) hist->Clone());
   
   if (hists.size() == 0) currentHistId = 0;

   for (const std::string& val : histValues)
   {
      if (val == histVal)
      {
         std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddHistogram: non-unique histogram "\
                      "value was specified: " << histVal << std::endl;
         exit(1);
      }
   }
   histValues.push_back(histVal);
}

void GUIFit::AddFit(TF1 *fit, TF1 *fitBG, const unsigned int fitTypeIndex, 
                    const unsigned int histIndex,
                    const int fitBGParIndexBegin, const int fitBGParIndexEnd)
{
   if (!fit) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddFit: null was passed for "\
                   "fit for histogram index " << histIndex << 
                   " for fit type index " << fitTypeIndex << std::endl;
      exit(1);
   }

   if (!fitBG) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddFit: null was passed for "\
                   "background fit for histogram index " << histIndex << 
                   " for fit type index " << fitTypeIndex << std::endl;
      exit(1);
   }

   if (fitTypeIndex < 0 || fitTypeIndex >= fitTypeNames.size())
   {
      std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddFit: index of a "\
                   "fit type is out of range: " << fitTypeIndex << std::endl;
      exit(1);
   }

   if (histIndex < 0 || histIndex >= hists.size())
   {
      std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddFit: index of a "\
                   "histogram is out of range: " << histIndex << std::endl;
      exit(1);
   }

   if (fits[histIndex].size() != fitTypeNames.size())
   {
      for (unsigned int i = 0; i < hists.size(); i++)
      {
         fits[i].resize(fitTypeNames.size());
         fitsBG[i].resize(fitTypeNames.size());
         fitBGParIndicesBegin[i].resize(fitTypeNames.size());
         fitBGParIndicesEnd[i].resize(fitTypeNames.size());
      }
   }

   if (fits[histIndex][fitTypeIndex])
   {
      std::cout << "\033[1m\033[35mWarning:\033[0m Trying to add the fit that was already added "\
                   "for fit type index " << fitTypeIndex << " for histogram index " << 
                   histIndex << "; rewriting the old fit" << std::endl;
   }

   fits[histIndex][fitTypeIndex] = static_cast<TF1 *>(fit->Clone());
   fitsBG[histIndex][fitTypeIndex] = static_cast<TF1 *>(fitBG->Clone());
   fitBGParIndicesBegin[histIndex][fitTypeIndex] = fitBGParIndexBegin;

   if (fitBGParIndexEnd < 0) fitBGParIndicesEnd[histIndex][fitTypeIndex] = fit->GetNpar() - 1;
   else fitBGParIndicesEnd[histIndex][fitTypeIndex] = fitBGParIndexEnd;

   if ((fitBGParIndicesEnd[histIndex][fitTypeIndex] - 
        fitBGParIndicesBegin[histIndex][fitTypeIndex]) != fitBG->GetNpar())
   {
      std::cout << "\033[1m\033[31mError:\033[0m GUIFit::AddFit: number of inidices for "\
                   "background fit deduced from provided begin and end indices mismatch "\
                   "with the number of parameters of an actual background fit: " << 
                   fitBGParIndicesEnd[histIndex][fitTypeIndex] - 
                   fitBGParIndicesBegin[histIndex][fitTypeIndex] << " vs " <<
                   fitBG->GetNpar() << std::endl;
      exit(1);
   }

   for (int i = 0; i < fitBG->GetNpar(); i++)
   {
      fitsBG[histIndex][fitTypeIndex]->ReleaseParameter(i);
   }

   for (int i = fitBGParIndexBegin; i <= fitBGParIndexEnd; i++)
   {
      fits[histIndex][fitTypeIndex]->ReleaseParameter(i);
   }
}

void GUIFit::AddFit(TF1 *fit, TF1 *fitBG, const unsigned int fitTypeIndex, 
                    const int fitBGParIndexBegin, const int fitBGParIndexEnd)
{
   AddFit(fit, fitBG, fitTypeIndex, hists.size() - 1, fitBGParIndexBegin, fitBGParIndexEnd);
}

void GUIFit::Exec()
{
   const int event = gPad->GetEvent();
   const int px = gPad->GetEventX();
   const int py = gPad->GetEventY();
   const double y = gPad->PadtoX(gPad->AbsPixeltoY(py));

   if (!isStartSuccessfull) 
   {
      Start();
      Draw();
   }

   switch (event)
   {
      case kKeyPress:
      {
         if (px > 47 && px < 58) 
         {
            currentFitTypeIndex = px - 48;
            DeactivateCurrentActivePoint();
            SetActiveFit();
         }
         switch (px)
         {
            case 'j':
            {
               if (hists.size() == 1)
               {
                  std::cout << "\033[1m\033[35mWarning:\033[0m Cannot switch "\
                               "between histograms since only one was added" << std::endl;
               }
               else if (currentHistId < hists.size() - 1) currentHistId++;
               else currentHistId = 0;

               Draw(true);

               break;
            }
            case 'k':
            {
               if (hists.size() == 1)
               {
                  std::cout << "\033[1m\033[35mWarning:\033[0m Cannot switch "\
                               "between histograms since only one was added" << std::endl;
               }
               else if (currentHistId > 0) currentHistId--;
               else currentHistId = hists.size() - 1;

               Draw(true);

               break;
            }
            case 'p':
            {
               for (unsigned int j = 0; j < outputFileNames.size(); j++)
               {
                  std::ofstream outputFile(outputFileNames[j]);

                  for (unsigned int i = 0; i < hists.size(); i++)
                  {
                     outputFile << histValues[i] << " ";

                     double xMin, xMax;
                     fits[i][j]->GetRange(xMin, xMax);

                     outputFile << fitsBG[i][j]->GetParameter(0) << " ";

                     for (int k = 1; k < fitsBG[i][j]->GetNpar() - 1; k++)
                     {
                        outputFile << fitsBG[i][j]->GetParameter(k) << " ";
                     }

                     outputFile << fitsBG[i][j]->GetParameter(fitsBG[i][j]->GetNpar() - 1) << 
                                   std::endl;
                  }
                  outputFile.close();
               }
               std::cout << "\033[32mInfo:\033[0m Background approximation "\
                            "parameters were written" << std::endl;
               break;
            }
            case 'f':
            {
               PerformFreeFit();
               break;
            }
            case 'n':
            {
               ResetFit();
               break;
            }
            case 'h':
            {
               PrintHelp();
               break;
            }
         }
         break;
      }
      case kButton1Down:
      {
         if (!isFitPointActive) 
         {
            for (int i = 0; i < bgPointsGr->GetN(); i++)
            {
               const double markerPositionX = gPad->XtoPixel(bgPointsGr->GetPointX(i));
               const double markerPositionY = gPad->YtoPixel(bgPointsGr->GetPointY(i));

               if (pow(markerPositionX - px, 2) + pow(markerPositionY - py, 2) < 
                   pow(4.*bgPointsGr->GetMarkerSize(), 2)) 
               {
                     ActivatePoint(i, bgPointsGr->GetPointX(i), bgPointsGr->GetPointY(i));
               }
            }
         }
         break;
      }
      case kButton1Motion:
      {
         if (isFitPointActive)
         {
            currentActivePointGr->SetPointY(0, y);
            bgPointsGr->SetPointY(currentActivePointIndex, y);

            bgPointsGr->Fit(fitsBG[currentHistId][currentFitTypeIndex - 1], "RQNM");

            for (int i = 0; i < fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar(); i++)
            {
               fits[currentHistId][currentFitTypeIndex - 1]->
                  FixParameter(fitBGParIndicesBegin[currentHistId][currentFitTypeIndex - 1] + i, 
                               fitsBG[currentHistId][currentFitTypeIndex - 1]->GetParameter(i));
            }

            fits[currentHistId][currentFitTypeIndex - 1]->Update();

            hists[currentHistId]->Fit(fits[currentHistId][currentFitTypeIndex - 1], "RQMN");

            fits[currentHistId][currentFitTypeIndex - 1]->Update();

            /*
            std::cout << "Fit parameters: ";

            for (int i = 0; i < fitBGParIndicesBegin[currentHistId][currentFitTypeIndex - 1]; i++)
            {
               std::cout << fits[currentHistId][currentFitTypeIndex - 1]->GetParameter(i) << " ";
            }
            std::cout << std::endl;
            */
         }
         else return;
         break;
      }
      case kButton1Up:
      {
         DeactivateCurrentActivePoint();
         break;
      }
   }

   Draw(false);
};

void GUIFit::SetBGPoints()
{
   double xMin, xMax;
   fits[currentHistId][currentFitTypeIndex - 1]->GetRange(xMin, xMax);

   //resetting bg points graph
   delete bgPointsGr;
   bgPointsGr = new TGraph();

   bgPointsGr->SetEditable(kFALSE);

   bgPointsGr->SetMarkerStyle(89);
   bgPointsGr->SetMarkerSize(3.);
   bgPointsGr->SetMarkerColor(activeColor);

   const double xShift = (xMax - xMin)/
      (static_cast<double>(fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar()) - 1.);

   bgPointsGr->AddPoint(xMin + 1e-15, 
                        fitsBG[currentHistId][currentFitTypeIndex - 1]->Eval(xMin));

   for (int j = 1; j < fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar() - 1; j++)
   {
      const double xPos = xMin+xShift*static_cast<double>(j);
      bgPointsGr->AddPoint(xPos, fitsBG[currentHistId][currentFitTypeIndex - 1]->Eval(xPos));
   }

   bgPointsGr->AddPoint(xMax - 1e-15, 
                        fitsBG[currentHistId][currentFitTypeIndex - 1]->Eval(xMax));
}

void GUIFit::SetActiveFit()
{
   bool isActiveSetCheck = false;

   for (unsigned long i = 0; i < fits[currentHistId].size(); i++)
   {
      if (i == currentFitTypeIndex - 1)
      {
         isActiveSetCheck = true;

         fits[currentHistId][i]->SetLineColor(activeColor);
         fitsBG[currentHistId][i]->SetLineColor(activeColor);
         fits[currentHistId][i]->SetLineWidth(4);
         fitsBG[currentHistId][i]->SetLineWidth(4);

         SetBGPoints();
      }
      else
      {
         fits[currentHistId][i]->SetLineColor(kGray+1);
         fitsBG[currentHistId][i]->SetLineColor(kGray+1);
         fits[currentHistId][i]->SetLineWidth(1);
         fitsBG[currentHistId][i]->SetLineWidth(1);
      }
   }

   if (!isActiveSetCheck)
   {
      delete bgPointsGr;
      bgPointsGr = new TGraph();
   }
}

void GUIFit::PerformFreeFit()
{
   if (currentFitTypeIndex == 0 || currentFitTypeIndex > fits[currentHistId].size()) return;

   for (int i = 0; i < fitsBG[currentHistId].back()->GetNpar(); i++)
   {
      fits[currentHistId][currentFitTypeIndex - 1]->
         ReleaseParameter(fitBGParIndicesBegin[currentHistId][currentFitTypeIndex - 1] + i);
   }

   hists[currentHistId]->Fit(fits[currentHistId][currentFitTypeIndex - 1], "RQMNB");

   for (int i = 0; i < fitsBG[currentHistId].back()->GetNpar(); i++)
   {
      const double value= fits[currentHistId][currentFitTypeIndex - 1]->
         GetParameter(fitBGParIndicesBegin[currentHistId][currentFitTypeIndex - 1] + i);

      fitsBG[currentHistId][currentFitTypeIndex - 1]->SetParameter(i, value);
   }

   SetBGPoints();
}

void GUIFit::ResetFit()
{
   if (currentFitTypeIndex == 0 || currentFitTypeIndex > fits[currentHistId].size()) return;

   double xMin, xMax;
   fits[currentHistId][currentFitTypeIndex - 1]->GetRange(xMin, xMax);

   //resetting bg points graph
   delete bgPointsGr;
   bgPointsGr = new TGraph();

   bgPointsGr->SetEditable(kFALSE);

   bgPointsGr->SetMarkerStyle(89);
   bgPointsGr->SetMarkerSize(3.);
   bgPointsGr->SetMarkerColor(activeColor);

   const double xShift = (xMax - xMin)/
      (static_cast<double>(fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar()) - 1.);

   bgPointsGr->AddPoint(xMin + 1e-15, 
               hists[currentHistId]->GetBinContent(hists[currentHistId]->GetXaxis()->
               FindBin(xMin)));

   for (int j = 1; j < fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar() - 1; j++)
   {
      const double xPos = xMin+xShift*static_cast<double>(j);
      bgPointsGr->AddPoint(xPos, hists[currentHistId]->GetBinContent(hists[currentHistId]->
                           GetXaxis()->FindBin(xPos)));
   }

   bgPointsGr->AddPoint(xMax - 1e-15, hists[currentHistId]->GetBinContent(hists[currentHistId]->
                        GetXaxis()->FindBin(xMax)));

   bgPointsGr->Fit(fitsBG[currentHistId][currentFitTypeIndex - 1], "RQMN");

   for (int i = 0; i < fitsBG[currentHistId][currentFitTypeIndex - 1]->GetNpar(); i++)
   {
      fits[currentHistId][currentFitTypeIndex - 1]->
         FixParameter(fitBGParIndicesBegin[currentHistId][currentFitTypeIndex - 1] + i, 
                      fitsBG[currentHistId][currentFitTypeIndex - 1]->GetParameter(i));
   }
   fits[currentHistId][currentFitTypeIndex - 1]->Update();
   hists[currentHistId]->Fit(fits[currentHistId][currentFitTypeIndex - 1], "RQMN");
}

void GUIFit::ActivatePoint(const int pointNumber, const double x, const double y) 
{
   isFitPointActive = true; 
   currentActivePointIndex = pointNumber;
   currentActivePointGr->SetPointX(0, x);
   currentActivePointGr->SetPointY(0, y);
}

void GUIFit::DeactivateCurrentActivePoint() 
{
   isFitPointActive = false; 
   currentActivePointIndex = -1;
   currentActivePointGr->SetPointX(0, -1.);
   currentActivePointGr->SetPointY(0, -1.);
}

void GUIFit::PrintHelp()
{
   std::cout << std::endl;
   std::cout << " ╔══════════════════════════════════════════════════════════════╗\n"
                " ║                         General info                         ║\n"
                " ║                                                              ║\n"
                " ║ Move cursor to the canvas to focus on the window. This will  ║\n" 
                " ║ make the keyboard configuration work.                        ║\n"
                " ║                                                              ║\n"
                " ║ Window focus may be removed when switching histograms.       ║\n"
                " ║ Moving a mouse over the canvas will bring it back.           ║\n"
                " ║                                                              ║\n"
                " ║ When you focus on a fit you can grab one of the points to    ║\n"
                " ║ move it vertically. This will change the background fit.     ║\n"
                " ║                                                              ║\n"
                " ║ When changing the approximation all parameters except for    ║\n"
                " ║ parameters for backgroud approximations are printed.         ║\n"
                " ╟──────────────────────────────────────────────────────────────╢\n"
                " ║                    Keyboard configuration                    ║\n"
                " ║                                                              ║\n"
                " ║ h:    Print help (general info and keyboar configuration)    ║\n"
                " ║ j, k: Switch focus between histograms                        ║\n"
                " ║ 1-9:  Switch focus between fits                              ║\n"
                " ║ f:    Perform fit with free backround fit parameters         ║\n"
                " ║ n:    Reset background fit parameters                        ║\n"
                " ║ p:    Write results in specified files                       ║\n"
                " ╚══════════════════════════════════════════════════════════════╝" << std::endl;
   std::cout << std::endl;
}

void GUIFit::Start()
{
   if (isStartSuccessfull) return;

   if (currentHistId < 0) 
   {
      std::cout << "\033[1m\033[31mError:\033[0m No histograms were added" << std::endl;
      exit(1);
   }

   bool areFitsOk = true;
   // Chesking fits
   for (unsigned int i = 0; i < hists.size(); i++)
   {
      for (unsigned int j = 0; j < fits[i].size(); j++)
      {
         if (!fits[i][j])
         {
            std::cout << "\033[1m\033[35mWarning:\033[0m Fit was not added for histogram "\
                         "index " << i << " for fit type index " << j << std::endl;
            areFitsOk = false;
         }
         if (!fitsBG[i][j])
         {
            std::cout << "\033[1m\033[35mWarning:\033[0m Background fit was not added "\
                         "for histogram index " << i << " for fit type index " << j << std::endl;
            areFitsOk = false;
         }
      }
   }

   if (!areFitsOk) 
   {
      std::cout << "\033[32mInfo:\033[0m Stopping the program due to abscent fits" << std::endl;
      exit(1);
   }

   fitNames.SetNDC();
   chi2NDF.SetNDC();

   if (currentActivePointGr->GetN() == 0) 
   {
      currentActivePointGr->AddPoint(-1., -1.);
      currentActivePointGr->SetMarkerSize(2.5);
      currentActivePointGr->SetMarkerStyle(89);
      currentActivePointGr->SetMarkerColor(activeColor);
      currentActivePointGr->SetEditable(kFALSE);
   }

   PrintHelp();
   currentHistId = 0;
   isStartSuccessfull = true;
}

void GUIFit::Draw(bool doDrawHist)
{
   if (doDrawHist)
   {
      DeactivateCurrentActivePoint();
      currentFitTypeIndex = 0;

      hists[currentHistId]->Draw();

      SetActiveFit();
   }

   for (unsigned long i = 0; i < fits[currentHistId].size(); i++)
   {
      fits[currentHistId][i]->Draw("SAME");
      fitsBG[currentHistId][i]->Draw("SAME");
   }

   if (isFitPointActive) currentActivePointGr->Draw("P");
   if (bgPointsGr->GetN() > 0) bgPointsGr->Draw("P");

   gPad->GetListOfPrimitives()->Remove(&fitNames);
   gPad->GetListOfPrimitives()->Remove(&chi2NDF);

   if (currentFitTypeIndex > 0 && currentFitTypeIndex <= fits[currentHistId].size())
   {
      std::stringstream chi2ndf;
      chi2ndf << std::fixed << std::setprecision(2) << 
                 fits[currentHistId][currentFitTypeIndex - 1]->GetChisquare()/
                 fits[currentHistId][currentFitTypeIndex - 1]->GetNDF();

      fitNames.SetText(0.2, 0.15, fits[currentHistId][currentFitTypeIndex - 1]->GetName());
      chi2NDF.SetText(0.75, 0.8, ("#chi^2/NDF " + chi2ndf.str()).c_str());

      fitNames.Draw();
      chi2NDF.Draw();
   }

   gPad->Modified();
   gPad->Update();

   gPad->GetFrame()->SetBit(TBox::kCannotMove);
}

#endif /* ROOT_TOOLS_GUI_FIT_CPP */
