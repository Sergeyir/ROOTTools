/** 
 *  @file   ThrObj.cpp 
 *  @brief  Contains useful set of containers to simplify and/or speed up the process of writing multithreaded applications with ROOT
 *
 *  In order to use these containers libThrObj.so must be loaded
 *
 *  This file is a part of a project CppTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_THR_OBJ_CPP
#define ROOT_TOOLS_THR_OBJ_CPP

#include <vector>
#include <array>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "ROOT/TThreadedObject.hxx"

#include "../include/ThrObj.hpp"

std::shared_ptr<TH1F> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH1F> *hist, 
                                                            const std::string& directory) 
{
   containerTH1F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH1F>>(hist));
   AddTFileDirectory(directory, 0);
   return containerTH1F.back()->Get();
};
      
std::shared_ptr<TH2F> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH2F> *hist, 
                                                            const std::string& directory)
{
   containerTH2F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH2F>>(hist));
   AddTFileDirectory(directory, 1);
   return containerTH2F.back()->Get();
};

std::shared_ptr<TH3F> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH3F> *hist, 
                                                            const std::string& directory) 
{
   containerTH3F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH3F>>(hist));
   AddTFileDirectory(directory, 2);
   return containerTH3F.back()->Get();
};

std::shared_ptr<TH1D> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH1D> *hist, 
                                                            const std::string& directory) 
{
   containerTH1D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH1D>>(hist));
   AddTFileDirectory(directory, 3);
   return containerTH1D.back()->Get();
};

std::shared_ptr<TH2D> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH2D> *hist, 
                                                            const std::string& directory) 
{
   containerTH2D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH2D>>(hist));
   AddTFileDirectory(directory, 4);
   return containerTH2D.back()->Get();
};

std::shared_ptr<TH3D> ROOTTools::ThrObjHolder::AddHistogram(ROOT::TThreadedObject<TH3D> *hist, 
                                                            const std::string& directory) 
{
   containerTH3D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH3D>>(hist));
   AddTFileDirectory(directory, 5);
   return containerTH3D.back()->Get();
};

void ROOTTools::ThrObjHolder::Clear()
{
   containerTH1F.clear();
   containerTH2F.clear();
   containerTH3F.clear();
   
   containerTH1D.clear();
   containerTH2D.clear();
   containerTH3D.clear();
   
   containerTFileDir.clear();
   for (auto& vec : containerTFileDir)
   {
      vec.clear();
   }
}

void ROOTTools::ThrObjHolder::AddTFileDirectory(const std::string& directory, const int thrObjIndex)
{
   int dirIndex = -999;
   if (directory != "")
   {
      for (long unsigned int i = 0; i < containerTFileDir.size(); i++)
      {
         if (containerTFileDir[i] == directory) 
         {
            dirIndex = i;
         }
      }
      if (dirIndex == -999) 
      {
         dirIndex = containerTFileDir.size();
         containerTFileDir.push_back(directory);
      }
   }
   containerTFileDirIndex[thrObjIndex].push_back(dirIndex);
}

void ROOTTools::ThrObjHolder::Write()
{
   for (auto& thrObj : containerTH1F) 
   {
      thrObj->Merge()->Write();
   }
   for (auto& thrObj : containerTH2F) 
   {
      thrObj->Merge()->Write();
   }
   for (auto& thrObj : containerTH3F) 
   {
      thrObj->Merge()->Write();
   }
   for (auto& thrObj : containerTH1D) 
   {
      thrObj->Merge()->Write();
   }
   for (auto& thrObj : containerTH2D) 
   {
      thrObj->Merge()->Write();
   }
   for (auto& thrObj : containerTH3D) 
   {
      thrObj->Merge()->Write();
   }
   
   Clear();
}

void ROOTTools::ThrObjHolder::Write(const std::string& outputFileName)
{
   TFile outputFile(outputFileName.c_str(), "RECREATE");
   outputFile.cd();
   if (containerTFileDir.size() == 0) Write();
   else
   {
      for (std::string dir : containerTFileDir)
      {
         outputFile.mkdir(dir.c_str());
      }
      for (long unsigned int i = 0; i < containerTH1F.size(); i++)
      {
         if (containerTFileDirIndex[0][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[0][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH1F>>(containerTH1F[i]->Merge())->Clone()->Write();
      }
      for (long unsigned int i = 0; i < containerTH2F.size(); i++)
      {
         if (containerTFileDirIndex[1][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[1][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH2F>>(containerTH2F[i]->Merge())->Clone()->Write();
      }
      for (long unsigned int i = 0; i < containerTH3F.size(); i++)
      {
         if (containerTFileDirIndex[2][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[2][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH3F>>(containerTH3F[i]->Merge())->Clone()->Write();
      }
      for (long unsigned int i = 0; i < containerTH1D.size(); i++)
      {
         if (containerTFileDirIndex[3][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[3][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH1D>>(containerTH1D[i]->Merge())->Clone()->Write();
      }
      for (long unsigned int i = 0; i < containerTH2D.size(); i++)
      {
         if (containerTFileDirIndex[4][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[4][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH2D>>(containerTH2D[i]->Merge())->Clone()->Write();
      }
      for (long unsigned int i = 0; i < containerTH3D.size(); i++)
      {
         if (containerTFileDirIndex[5][i] != -999) outputFile.cd(
            containerTFileDir[containerTFileDirIndex[5][i]].c_str());
         else outputFile.cd();
         static_cast<std::shared_ptr<TH3D>>(containerTH3D[i]->Merge())->Clone()->Write();
      }
      Clear();
   }
}
 
template<typename T>
ROOTTools::ThrObj<T>::ThrObj(const std::string& name, const std::string& title,
                             const int xnbins, const double xmin, const double xmax,
                             const std::string& fileDirectory)
{
   objPtr = ThrObjHolder::AddHistogram(new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
                                                                    xnbins, xmin, xmax), 
                                       fileDirectory);
}

template<typename T>
ROOTTools::ThrObj<T>::ThrObj(const std::string& name, const std::string& title,
                             const int xnbins, const double xmin, const double xmax, 
                             const int ynbins, const double ymin, const double ymax,
                             const std::string& fileDirectory)
{
   objPtr = ThrObjHolder::AddHistogram(new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
                                                                    xnbins, xmin, xmax, 
                                                                    ynbins, ymin, ymax),
                                       fileDirectory);
}

template<typename T>
ROOTTools::ThrObj<T>::ThrObj(const std::string& name, const std::string& title,
                             const int xnbins, const double xmin, const double xmax, 
                             const int ynbins, const double ymin, const double ymax,
                             const int znbins, const double zmin, const double zmax,
                             const std::string& fileDirectory)
{
   objPtr = ThrObjHolder::AddHistogram(new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
                                                                    xnbins, xmin, xmax, 
                                                                    ynbins, ymin, ymax,
                                                                    znbins, zmin, zmax),
                                       fileDirectory);
}

template<typename T>
std::shared_ptr<T> ROOTTools::ThrObj<T>::Get()
{
   return objPtr;
}

// explicit instantiations of ROOTTools::ThrObj for different types of histograms
template ROOTTools::ThrObj<TH1F>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const std::string&);
template ROOTTools::ThrObj<TH2F>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const std::string&);
template ROOTTools::ThrObj<TH3F>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const std::string&);
template ROOTTools::ThrObj<TH1D>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const std::string&);
template ROOTTools::ThrObj<TH2D>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const std::string&);
template ROOTTools::ThrObj<TH3D>::ThrObj(const std::string&, const std::string&, 
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const int, const double, const double,
                                         const std::string&);

// explicit instantiations of ROOTTools::ThrObj::Get() for different types of histograms
template std::shared_ptr<TH1F> ROOTTools::ThrObj<TH1F>::Get();
template std::shared_ptr<TH2F> ROOTTools::ThrObj<TH2F>::Get();
template std::shared_ptr<TH3F> ROOTTools::ThrObj<TH3F>::Get();
template std::shared_ptr<TH1D> ROOTTools::ThrObj<TH1D>::Get();
template std::shared_ptr<TH2D> ROOTTools::ThrObj<TH2D>::Get();
template std::shared_ptr<TH3D> ROOTTools::ThrObj<TH3D>::Get();

#endif /* ROOT_TOOLS_THR_OBJ_CPP */
