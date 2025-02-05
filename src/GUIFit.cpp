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

ROOTTools::GUIFit::GUIFit() 
{
   bgPointsGr = new TGraph();
   currentActivePointGr = new TGraph();
};

void ROOTTools::GUIFit::AddFitType(const std::string& outputFileName)
{
   system(("cp " + outputFileName + " " + outputFileName + ".backup").c_str());
   outputFilesNames.push_back(outputFileName);
   outputFiles.push_back(std::ofstream());
}

void ROOTTools::GUIFit::AddHist(TH1F *dataHist, const std::string& xVal)
{
   fits.resize(fits.size()+1);
   bgFits.resize(bgFits.size()+1);
   bgFitParNumbersStart.resize(bgFitParNumbersStart.size()+1);
   
   currentHistId = 0;
   hists.push_back((TH1F *) dataHist->Clone());
   histsXValues.push_back(xVal);
}

void ROOTTools::GUIFit::AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber, const unsigned int histNumber)
{
   fits[histNumber].push_back((TF1 *) fit->Clone());
   bgFits[histNumber].push_back((TF1 *) bgFit->Clone());
   bgFitParNumbersStart[histNumber].push_back(bgFitParStartNumber);

   for (int i = 0; i < bgFit->GetNpar(); i++)
   {
      bgFits[histNumber].back()->ReleaseParameter(i);
      fits[histNumber].back()->ReleaseParameter(bgFitParStartNumber+i);
   }
}

void ROOTTools::GUIFit::AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), const int bgFitParStartNumber, const unsigned int histNumber)
{
   double xMin, xMax;
   fit->GetRange(xMin, xMax);
   
   fits[histNumber].push_back(new TF1(fit->GetName(), fcn, xMin, xMax, fit->GetNpar()));
   bgFits[histNumber].push_back((TF1 *) bgFit->Clone());
   bgFitParNumbersStart[histNumber].push_back(bgFitParStartNumber);


   for (int i = 0; i < fit->GetNpar(); i++)
   {
      fits[histNumber].back()->SetParameter(i, fit->GetParameter(i));
      
      double parMin, parMax;
      fit->GetParLimits(i, parMin, parMax);
      fits[histNumber].back()->SetParLimits(i, parMin, parMax);
   }

   for (int i = 0; i < bgFit->GetNpar(); i++)
   {
      bgFits[histNumber].back()->ReleaseParameter(i);
      fits[histNumber].back()->ReleaseParameter(bgFitParStartNumber+i);
   }
}

//Adding fit for the last hist added
void ROOTTools::GUIFit::AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber)
{
   AddFit(fit, bgFit, bgFitParStartNumber, hists.size()-1);
}

//Adding fit for the last hist added
void ROOTTools::GUIFit::AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), const int bgFitParStartNumber)
{
   AddFit(fit, bgFit, fcn, bgFitParStartNumber, hists.size()-1);
}

void ROOTTools::GUIFit::Start()
{
   const int event = gPad->GetEvent();
   const int px = gPad->GetEventX();
   const int py = gPad->GetEventY();
   const double y = gPad->PadtoX(gPad->AbsPixeltoY(py));
   
   switch (event)
   {
      case kKeyPress:
         if (px > 47 && px < 58) 
         {
            currentFitModeId = px-48;
            DeactivatePoint();
            SetActiveFit();
         }
         
         switch (px)
         {
            case 'j':
               if (hists.size() == 1)
               {
                  printf("\033[1m\033[35mWarning:\033[0m Cannot change histogram since only one was added\n");
               }
               else if (currentHistId < hists.size()-1)
               {
                  currentHistId++;
               }
               else
               {
                  currentHistId = 0;
               }

               Draw(true, false);
               
               break;

            case 'k':
               if (hists.size() == 1)
               {
                  printf("\033[1m\033[35mWarning:\033[0m Cannot change histogram since only one was added\n");
               }
               else if (currentHistId > 0)
               {
                  currentHistId--;
               }
               else
               {
                  currentHistId = hists.size()-1;
               }

               Draw(true, false);
               
               break;
               
            case 'p':
               for (long unsigned int j = 0; j < outputFiles.size(); j++)
               {
                  outputFiles[j].close();
                  outputFiles[j].open(outputFilesNames[j]);
                  
                  for (long unsigned int i = 0; i < hists.size(); i++)
                  {
                     outputFiles[j] << histsXValues[i] << " ";

                     double xMin, xMax;
                     fits[i][j]->GetRange(xMin, xMax);

                     outputFiles[j] << 
                        bgFits[i][j]->GetParameter(0) << " ";
                     
                     for (int k = 1; k < bgFits[i][j]->GetNpar() - 1; k++)
                     {
                        outputFiles[j] << bgFits[i][j]->GetParameter(k) << " ";
                     }
                     
                     outputFiles[j] << 
                        bgFits[i][j]->GetParameter(
                        bgFits[i][j]->GetNpar() - 1) << std::endl;
                  }
               }
               printf("\033[32mInfo:\033[0m BG approximation parameters were written\n");
               break;

            case 'f':
               PerformFreeFit();
               break;

            case 'n':
               ResetFit();
               break;

            case 'h':
               PrintHelp();
               break;
         }
            
         break;
         
         
      case kButton1Down:
         if (!IsActivePoint()) 
         {
            for (int i = 0; i < bgPointsGr->GetN(); i++)
            {
            const double markerPositionX = gPad->XtoPixel(bgPointsGr->GetPointX(i));
            const double markerPositionY = gPad->YtoPixel(bgPointsGr->GetPointY(i));

            if (pow(markerPositionX - px, 2) + pow(markerPositionY - py, 2) < 
                  pow(4.*bgPointsGr->GetMarkerSize(), 2)) 
            {
                  ActivatePoint(i, 
                     bgPointsGr->GetPointX(i), 
                     bgPointsGr->GetPointY(i));
               }
            }
         }
         break;

      case kButton1Motion:
         if (IsActivePoint())
         {
            currentActivePointGr->SetPointY(0, y);
            bgPointsGr->SetPointY(currentActivePointId, y);
            
            bgPointsGr->Fit(
               bgFits[currentHistId][currentFitModeId-1], "RQNM");
            
            for (int i = 0; i < bgFits[currentHistId][currentFitModeId-1]->GetNpar(); i++)
            {
               fits[currentHistId][currentFitModeId-1]->FixParameter(
                  bgFitParNumbersStart[currentHistId][currentFitModeId-1]+i, 
                  bgFits[currentHistId][currentFitModeId-1]->GetParameter(i));
            }
            fits[currentHistId][currentFitModeId-1]->Update();
            
            hists[currentHistId]->Fit(
               fits[currentHistId][currentFitModeId-1], "RQMN");

            fits[currentHistId][currentFitModeId-1]->Update();

            std::cout << "Fit parameters: ";

            for (int i = 0; i < bgFitParNumbersStart[currentHistId][currentFitModeId-1]; i++)
            {
               std::cout << 
                  fits[currentHistId][currentFitModeId-1]->GetParameter(i) 
                  << " ";
            }
            std::cout << std::endl;
         }
         else return;
         break;

      case kButton1Up:
         DeactivatePoint();
         break;
   }
   
   Draw(false, false);
};

ROOTTools::GUIFit::~GUIFit() 
{
   delete bgPointsGr;
   delete currentActivePointGr;
};

void ROOTTools::GUIFit::SetBgPoints()
{
   double xMin, xMax;
   fits[currentHistId][currentFitModeId-1]->GetRange(xMin, xMax);
   
   //resetting bg points graph
   delete bgPointsGr;
   bgPointsGr = new TGraph();
   
   bgPointsGr->SetEditable(kFALSE);
   
   bgPointsGr->SetMarkerStyle(89);
   bgPointsGr->SetMarkerSize(3.);
   bgPointsGr->SetMarkerColor(activeColor);
   
   const double xShift = (xMax - xMin)/
      (static_cast<double>(bgFits[currentHistId][currentFitModeId-1]->GetNpar())-1.);
      
   bgPointsGr->AddPoint(xMin + 1e-15, 
      bgFits[currentHistId][currentFitModeId-1]->Eval(xMin));
   for (int j = 1; j < bgFits[currentHistId][currentFitModeId-1]->GetNpar()-1; j++)
   {
      const double xPos = xMin+xShift*static_cast<double>(j);
      bgPointsGr->AddPoint(xPos, bgFits[currentHistId][currentFitModeId-1]->Eval(xPos));
    }
   bgPointsGr->AddPoint(xMax - 1e-15, 
      bgFits[currentHistId][currentFitModeId-1]->Eval(xMax));
}

void ROOTTools::GUIFit::SetActiveFit()
{
   bool isActiveSetCheck = false;
   
   for (unsigned long i = 0; i < fits[currentHistId].size(); i++)
   {
      if (i == currentFitModeId-1)
      {
         isActiveSetCheck = true;
         
         fits[currentHistId][i]->SetLineColor(activeColor);
         bgFits[currentHistId][i]->SetLineColor(activeColor);
         fits[currentHistId][i]->SetLineWidth(4);
         bgFits[currentHistId][i]->SetLineWidth(4);   

         SetBgPoints();
      }
      else
      {
         fits[currentHistId][i]->SetLineColor(kGray+1);
         bgFits[currentHistId][i]->SetLineColor(kGray+1);
         fits[currentHistId][i]->SetLineWidth(1);
         bgFits[currentHistId][i]->SetLineWidth(1);
      }
   }

   if (!isActiveSetCheck)
   {
      delete bgPointsGr;
      bgPointsGr = new TGraph();
   }
}

void ROOTTools::GUIFit::PerformFreeFit()
{
   if (currentFitModeId == 0 || currentFitModeId > fits[currentHistId].size()) return;
   
   for (int i = 0; i < bgFits[currentHistId].back()->GetNpar(); i++)
   {
      fits[currentHistId][currentFitModeId-1]->ReleaseParameter(
         bgFitParNumbersStart[currentHistId][currentFitModeId-1]+i);
   }
   
   hists[currentHistId]->Fit(fits[currentHistId][currentFitModeId-1], "RQMNB");
   
   for (int i = 0; i < bgFits[currentHistId].back()->GetNpar(); i++)
   {
      bgFits[currentHistId][currentFitModeId-1]->SetParameter(i, 
         fits[currentHistId][currentFitModeId-1]->GetParameter(
         bgFitParNumbersStart[currentHistId][currentFitModeId-1]+i));
   }
   
   SetBgPoints();
}

void ROOTTools::GUIFit::ResetFit()
{
   if (currentFitModeId == 0 || currentFitModeId > fits[currentHistId].size()) return;
   
   double xMin, xMax;
   fits[currentHistId][currentFitModeId-1]->GetRange(xMin, xMax);
   
   //resetting bg points graph
   delete bgPointsGr;
   bgPointsGr = new TGraph();
   
   bgPointsGr->SetEditable(kFALSE);
   
   bgPointsGr->SetMarkerStyle(89);
   bgPointsGr->SetMarkerSize(3.);
   bgPointsGr->SetMarkerColor(activeColor);
   
   const double xShift = (xMax - xMin)/
      (static_cast<double>(bgFits[currentHistId][currentFitModeId-1]->GetNpar())-1.);
      
   bgPointsGr->AddPoint(xMin + 1e-15, 
      hists[currentHistId]->GetBinContent(hists[currentHistId]->GetXaxis()->FindBin(xMin)));
   for (int j = 1; j < bgFits[currentHistId][currentFitModeId-1]->GetNpar()-1; j++)
   {
      const double xPos = xMin+xShift*static_cast<double>(j);
      bgPointsGr->AddPoint(xPos, 
         hists[currentHistId]->GetBinContent(hists[currentHistId]->GetXaxis()->FindBin(xPos)));
   }
   bgPointsGr->AddPoint(xMax - 1e-15, 
      hists[currentHistId]->GetBinContent(hists[currentHistId]->GetXaxis()->FindBin(xMax)));
   
   bgPointsGr->Fit(bgFits[currentHistId][currentFitModeId-1], "RQMN");
   
   for (int i = 0; i < bgFits[currentHistId][currentFitModeId-1]->GetNpar(); i++)
   {
      fits[currentHistId][currentFitModeId-1]->FixParameter(
         bgFitParNumbersStart[currentHistId][currentFitModeId-1]+i, 
         bgFits[currentHistId][currentFitModeId-1]->GetParameter(i));
   }
   fits[currentHistId][currentFitModeId-1]->Update();
   hists[currentHistId]->Fit(
      fits[currentHistId][currentFitModeId-1], "RQMN");
}

bool ROOTTools::GUIFit::IsActivePoint() 
{
   return isFitPointActive;
}

void ROOTTools::GUIFit::ActivatePoint(const int pointNumber, const double x, const double y) 
{
   isFitPointActive = true; 
   currentActivePointId = pointNumber;
   currentActivePointGr->SetPointX(0, x);
   currentActivePointGr->SetPointY(0, y);
}

void ROOTTools::GUIFit::DeactivatePoint() 
{
   isFitPointActive = false; 
   currentActivePointId = -1;
   currentActivePointGr->SetPointX(0, -1.);
   currentActivePointGr->SetPointY(0, -1.);
}

void ROOTTools::GUIFit::PrintHelp()
{
   std::cout << std::endl;

   std::cout << " ╔══════════════════════════════════════════════════════════════╗" << std::endl;
   std::cout << " ║                         General info                         ║" << std::endl;
   std::cout << " ║                                                              ║" << std::endl;
   std::cout << " ║ Move cursor to the canvas to focus on the window. This will  ║" << std::endl; 
   std::cout << " ║ make the keyboard configuration work.                        ║" << std::endl;
   std::cout << " ║                                                              ║" << std::endl;
   std::cout << " ║ Window focus may be removed when switching histograms.       ║" << std::endl;
   std::cout << " ║ Moving a mouse over the canvas will bring it back.           ║" << std::endl;
   std::cout << " ║                                                              ║" << std::endl;
   std::cout << " ║ When you focus on a fit you can grab one of the points to    ║" << std::endl;
   std::cout << " ║ move it vertically. This will change the background fit.     ║" << std::endl;
   std::cout << " ║                                                              ║" << std::endl;
   std::cout << " ║ When changing the approximation all parameters except for    ║" << std::endl;
   std::cout << " ║ parameters for backgroud approximations are printed.         ║" << std::endl;
   std::cout << " ╟──────────────────────────────────────────────────────────────╢" << std::endl;
   std::cout << " ║                    Keyboard configuration                    ║" << std::endl;
   std::cout << " ║                                                              ║" << std::endl;
   std::cout << " ║ h:    Print help (general info and keyboar configuration)    ║" << std::endl;
   std::cout << " ║ j, k: Switch focus between histograms                        ║" << std::endl;
   std::cout << " ║ 1-9:  Switch focus between fits                              ║" << std::endl;
   std::cout << " ║ f:    Perform fit with free backround fit parameters         ║" << std::endl;
   std::cout << " ║ n:    Reset background fit parameters                        ║" << std::endl;
   std::cout << " ║ p:    Write results in specified files                       ║" << std::endl;
   std::cout << " ╚══════════════════════════════════════════════════════════════╝" << std::endl;
   std::cout << std::endl;
}

void ROOTTools::GUIFit::Draw(bool doDrawHist, bool isFirstDraw)
{   
   if (isFirstDraw)
   {
      if (currentHistId < 0) 
      {
         printf("\033[1m\033[31mError:\033[0m No histograms were added");
      }
      fitsNames.SetNDC();
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
   }
   if (doDrawHist)
   {
      DeactivatePoint();
      currentFitModeId = 0;
      
      hists[currentHistId]->Draw();
         
      SetActiveFit();   
   }
   
   for (unsigned long i = 0; i < fits[currentHistId].size(); i++)
   {
      fits[currentHistId][i]->Draw("SAME");
      bgFits[currentHistId][i]->Draw("SAME");
   }
   
   if (IsActivePoint()) currentActivePointGr->Draw("P");
   if (bgPointsGr->GetN() > 0) bgPointsGr->Draw("P");

   gPad->GetListOfPrimitives()->Remove(&fitsNames);
   gPad->GetListOfPrimitives()->Remove(&chi2NDF);
   
   if (currentFitModeId > 0 && currentFitModeId <= fits[currentHistId].size())
   {
      std::stringstream chi2ndf;
      chi2ndf << std::fixed << std::setprecision(2) << 
         fits[currentHistId][currentFitModeId-1]->GetChisquare()/
         fits[currentHistId][currentFitModeId-1]->GetNDF();
      
      fitsNames.SetText(0.2, 0.15, fits[currentHistId][currentFitModeId-1]->GetName());
      chi2NDF.SetText(0.75, 0.8, ("#chi^2/NDF " + chi2ndf.str()).c_str());
      
      fitsNames.Draw();
      chi2NDF.Draw();
   }
   
   gPad->Modified();
   gPad->Update();
   
   gPad->GetFrame()->SetBit(TBox::kCannotMove);
}

#endif /* ROOT_TOOLS_GUI_FIT_CPP */
