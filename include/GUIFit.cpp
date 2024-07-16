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

#include "ErrorHandler.hpp"

#include "TPad.h"
#include "TH1.h"
#include "TLatex.h"
#include "TGraph.h"
#include "TF1.h"

struct
{
	std::vector<TH1F *> hists;
	std::vector<std::string> histsXValues;

	std::vector<std::string> outputFilesNames;
	std::vector<ofstream> outputFiles;
	
	std::vector<std::vector<TF1 *>> fits, bgFits;
	std::vector<std::vector<int>> bgFitParNumbersStart;

	TLatex fitsNames = TLatex(0.2, 0.2, "");
	TLatex chi2NDF = TLatex(0.8, 0.9, "");
	
	//graph that contains points for background tweaking
	TGraph *bgPointsGr = new TGraph();
	
	int currentHistId = -1;
	int currentFitModeId = 0;
	
	bool isFitPointActive = false;
	int currentActivePointId = -1;
	TGraph *currentActivePointGr = new TGraph();

	Color_t activeColor = kRed+1;

	void AddFitType(std::string outputFileName)
	{
		system(("cp " + outputFileName + " " + outputFileName + ".backup").c_str());
		CheckOutputFile(outputFileName, std::ios::app);
		outputFilesNames.push_back(outputFileName);
		outputFiles.push_back(ofstream("../../tmp/a.txt"));
	}
	
	void AddHist(TH1F *dataHist, std::string xVal)
	{
		fits.resize(fits.size()+1);
		bgFits.resize(bgFits.size()+1);
		bgFitParNumbersStart.resize(bgFitParNumbersStart.size()+1);
		
		currentHistId = 0;
		hists.push_back((TH1F *) dataHist->Clone());
		histsXValues.push_back(xVal);
	}
	
	void AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber, const unsigned int histNumber)
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

	void AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), const int bgFitParStartNumber, const unsigned int histNumber)
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
	void AddFit(TF1 *fit, TF1 *bgFit, const int bgFitParStartNumber)
	{
		AddFit(fit, bgFit, bgFitParStartNumber, hists.size()-1);
	}

	//Adding fit for the last hist added
	void AddFit(TF1 *fit, TF1 *bgFit, Double_t (*fcn)(Double_t *, Double_t *), const int bgFitParStartNumber)
	{
		AddFit(fit, bgFit, fcn, bgFitParStartNumber, hists.size()-1);
	}

	void SetBgPoints()
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

	void SetActiveFit()
	{
		bool isActiveSetCheck = false;
		
		for (int i = 0; i < fits[currentHistId].size(); i++)
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

	void PerformFreeFit()
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
	
	void ResetFit()
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

	bool IsActivePoint() 
	{
		return isFitPointActive;
	}
	
	void ActivatePoint(const int pointNumber, const double x, const double y) 
	{
		isFitPointActive = true; 
		currentActivePointId = pointNumber;
		currentActivePointGr->SetPointX(0, x);
		currentActivePointGr->SetPointY(0, y);
	}
	
	void DeactivatePoint() 
	{
		isFitPointActive = false; 
		currentActivePointId = -1;
		currentActivePointGr->SetPointX(0, -1.);
		currentActivePointGr->SetPointY(0, -1.);
	}

	void PrintHelp()
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

	void Draw(bool doDrawHist = true, bool isFirstDraw = true)
	{	
		if (isFirstDraw)
		{
			if (currentHistId < 0) PrintError("No histograms were added");
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
		
		for (int i = 0; i < fits[currentHistId].size(); i++)
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
} GUIFitPar;

void GUIFit()
{
	const int event = gPad->GetEvent();
	const int px = gPad->GetEventX();
	const int py = gPad->GetEventY();
	const double x = gPad->PadtoX(gPad->AbsPixeltoX(px));
	const double y = gPad->PadtoX(gPad->AbsPixeltoY(py));
	
	switch (event)
	{
		case kKeyPress:
			if (px > 47 && px < 58) 
			{
				GUIFitPar.currentFitModeId = px-48;
				GUIFitPar.DeactivatePoint();
				GUIFitPar.SetActiveFit();
			}
			
			switch (px)
			{
				case 'j':
					if (GUIFitPar.hists.size() == 1)
					{
						PrintWarning("Cannot change histogram since only one was added");
					}
					else if (GUIFitPar.currentHistId < GUIFitPar.hists.size()-1)
					{
						GUIFitPar.currentHistId++;
					}
					else
					{
						GUIFitPar.currentHistId = 0;
					}

					GUIFitPar.Draw(true, false);
					
					break;

				case 'k':
					if (GUIFitPar.hists.size() == 1)
					{
						PrintWarning("Cannot change histogram since only one was added");
					}
					else if (GUIFitPar.currentHistId > 0)
					{
						GUIFitPar.currentHistId--;
					}
					else
					{
						GUIFitPar.currentHistId = GUIFitPar.hists.size()-1;
					}

					GUIFitPar.Draw(true, false);
					
					break;
					
				case 'p':
					for (int j = 0; j < GUIFitPar.outputFiles.size(); j++)
					{
						GUIFitPar.outputFiles[j].close();
						GUIFitPar.outputFiles[j].open(GUIFitPar.outputFilesNames[j]);
						
						for (int i = 0; i < GUIFitPar.hists.size(); i++)
						{
							GUIFitPar.outputFiles[j] << GUIFitPar.histsXValues[i] << " ";

							double xMin, xMax;
							GUIFitPar.fits[i][j]->GetRange(xMin, xMax);

							const double xShift = (xMax - xMin)/
								(static_cast<double>(GUIFitPar.bgFits[i][j]->GetNpar())-1.);
							
							GUIFitPar.outputFiles[j] << 
								GUIFitPar.bgFits[i][j]->GetParameter(0) << " ";
							
							for (int k = 1; k < GUIFitPar.bgFits[i][j]->GetNpar() - 1; k++)
							{
								const double xPos = xMin + xShift*static_cast<double>(k);
								GUIFitPar.outputFiles[j] << GUIFitPar.bgFits[i][j]->GetParameter(k) << " ";
							}
							
							GUIFitPar.outputFiles[j] << 
								GUIFitPar.bgFits[i][j]->GetParameter(
								GUIFitPar.bgFits[i][j]->GetNpar() - 1) << std::endl;
						}
					}
					PrintInfo("BG approximation parameters were written");
					break;

				case 'f':
					GUIFitPar.PerformFreeFit();
					break;

				case 'n':
					GUIFitPar.ResetFit();
					break;

				case 'h':
					GUIFitPar.PrintHelp();
					break;
			}
				
			break;
			
			
		case kButton1Down:
			if (!GUIFitPar.IsActivePoint()) 
			{
				for (int i = 0; i < GUIFitPar.bgPointsGr->GetN(); i++)
				{
				const double markerPositionX = gPad->XtoPixel(GUIFitPar.bgPointsGr->GetPointX(i));
				const double markerPositionY = gPad->YtoPixel(GUIFitPar.bgPointsGr->GetPointY(i));

				if (pow(markerPositionX - px, 2) + pow(markerPositionY - py, 2) < 
						pow(4.*GUIFitPar.bgPointsGr->GetMarkerSize(), 2)) 
				{
						GUIFitPar.ActivatePoint(i, 
							GUIFitPar.bgPointsGr->GetPointX(i), 
							GUIFitPar.bgPointsGr->GetPointY(i));
					}
				}
			}
			break;

		case kButton1Motion:
			if (GUIFitPar.IsActivePoint())
			{
				GUIFitPar.currentActivePointGr->SetPointY(0, y);
				GUIFitPar.bgPointsGr->SetPointY(GUIFitPar.currentActivePointId, y);
				
				GUIFitPar.bgPointsGr->Fit(
					GUIFitPar.bgFits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1], "RQNM");
				
				for (int i = 0; i < GUIFitPar.bgFits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->GetNpar(); i++)
				{
					GUIFitPar.fits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->FixParameter(
						GUIFitPar.bgFitParNumbersStart[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]+i, 
						GUIFitPar.bgFits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->GetParameter(i));
				}
				GUIFitPar.fits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->Update();
				
				GUIFitPar.hists[GUIFitPar.currentHistId]->Fit(
					GUIFitPar.fits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1], "RQMN");

				GUIFitPar.fits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->Update();

				std::cout << "Fit parameters: ";

				for (int i = 0; i < GUIFitPar.bgFitParNumbersStart[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]; i++)
				{
					std::cout << 
						GUIFitPar.fits[GUIFitPar.currentHistId][GUIFitPar.currentFitModeId-1]->GetParameter(i) 
						<< " ";
				}
				std::cout << std::endl;
			}
			else return;
			break;

		case kButton1Up:
			GUIFitPar.DeactivatePoint();
			break;
	}
	
	GUIFitPar.Draw(false, false);
};

#endif
