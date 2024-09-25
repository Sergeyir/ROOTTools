// $HEADER$
//------------------------------------------------------------------------------------------------
//                            ThrObj declaration and realisation
//------------------------------------------------------------------------------------------------
// ThrObj - threaded object 
//
// ** Free and open code for anyone to use **
//
// Author: Sergei Antsupov
// Email: antsupov0124@gmail.com
//
/**
 * Basic container for storing and handling ROOT TThreadedObject instances
 **/
//------------------------------------------------------------------------------------------------

#ifndef THR_OBJ_HPP
#define THR_OBJ_HPP

#include <vector>
#include <array>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "ROOT/TThreadedObject.hxx"

struct
{
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1F>>> containerTH1F;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2F>>> containerTH2F;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3F>>> containerTH3F;
   
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1D>>> containerTH1D;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2D>>> containerTH2D;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3D>>> containerTH3D;

   std::vector<std::string> containerTFileDir;
   std::array<std::vector<int>, 6> containerTFileDirIndex;
   
   std::shared_ptr<TH1F> AddHistogram(
      ROOT::TThreadedObject<TH1F> *hist, 
      const std::string& directory) 
   {
      containerTH1F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH1F>>(hist));
      AddTFileDirectory(directory, 0);
      return containerTH1F.back()->Get();
   };
   
   std::shared_ptr<TH2F> AddHistogram(
      ROOT::TThreadedObject<TH2F> *hist, 
      const std::string& directory) 
   {
      containerTH2F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH2F>>(hist));
      AddTFileDirectory(directory, 1);
      return containerTH2F.back()->Get();
   };
   
   std::shared_ptr<TH3F> AddHistogram(
      ROOT::TThreadedObject<TH3F> *hist, 
      const std::string& directory) 
   {
      containerTH3F.push_back(std::unique_ptr<ROOT::TThreadedObject<TH3F>>(hist));
      AddTFileDirectory(directory, 2);
      return containerTH3F.back()->Get();
   };
   
   std::shared_ptr<TH1D> AddHistogram(
      ROOT::TThreadedObject<TH1D> *hist, 
      const std::string& directory) 
   {
      containerTH1D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH1D>>(hist));
      AddTFileDirectory(directory, 3);
      return containerTH1D.back()->Get();
   };
   
   std::shared_ptr<TH2D> AddHistogram(
      ROOT::TThreadedObject<TH2D> *hist, 
      const std::string& directory) 
   {
      containerTH2D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH2D>>(hist));
      AddTFileDirectory(directory, 4);
      return containerTH2D.back()->Get();
   };
   
   std::shared_ptr<TH3D> AddHistogram(
      ROOT::TThreadedObject<TH3D> *hist, 
      const std::string& directory) 
   {
      containerTH3D.push_back(std::unique_ptr<ROOT::TThreadedObject<TH3D>>(hist));
      AddTFileDirectory(directory, 5);
      return containerTH3D.back()->Get();
   };
   
   void Clear()
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

   void AddTFileDirectory(const std::string& directory, const int thrObjIndex)
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

   void Write()
   {
      for (auto& thrObj : containerTH1F) 
      {
         static_cast<std::shared_ptr<TH1F>>(thrObj->Merge())->Clone()->Write();
      }
      for (auto& thrObj : containerTH2F) 
      {
         static_cast<std::shared_ptr<TH2F>>(thrObj->Merge())->Clone()->Write();
      }
      for (auto& thrObj : containerTH3F) 
      {
         static_cast<std::shared_ptr<TH3F>>(thrObj->Merge())->Clone()->Write();
      }
      for (auto& thrObj : containerTH1D) 
      {
         static_cast<std::shared_ptr<TH1D>>(thrObj->Merge())->Clone()->Write();
      }
      for (auto& thrObj : containerTH2D) 
      {
         static_cast<std::shared_ptr<TH2D>>(thrObj->Merge())->Clone()->Write();
      }
      for (auto& thrObj : containerTH3D) 
      {
         static_cast<std::shared_ptr<TH3D>>(thrObj->Merge())->Clone()->Write();
      }
      
      Clear();
   }

   void Write(const std::string& outputFileName)
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
         Clear();
      }
   }
   
} ThrObjHolder;

template<class T>
class ThrObj
{
   private:
   std::shared_ptr<T> objPtr;

   public:
   
   //TH1 methods
   ThrObj(const std::string& name, const std::string& title,
          const int xnbins, const double xmin, const double xmax,
          const std::string& directoryTFile = "")
   {
      objPtr = ThrObjHolder.AddHistogram(
         new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
            xnbins, xmin, xmax), 
         directoryTFile);
   }

   //TH2 methods
   ThrObj(const std::string& name, const std::string& title,
          const int xnbins, const double xmin, const double xmax, 
          const int ynbins, const double ymin, const double ymax,
          const std::string& directoryTFile = "")
   {
      objPtr = ThrObjHolder.AddHistogram(
         new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
            xnbins, xmin, xmax, 
            ynbins, ymin, ymax),
         directoryTFile);
   }

   //TH3 methods
   ThrObj(const std::string& name, const std::string& title,
          const int xnbins, const double xmin, const double xmax, 
          const int ynbins, const double ymin, const double ymax,
          const int znbins, const double zmin, const double zmax,
          const std::string& directoryTFile = "")
   {
      objPtr = ThrObjHolder.AddHistogram(
         new ROOT::TThreadedObject<T>(name.c_str(), title.c_str(), 
            xnbins, xmin, xmax, 
            ynbins, ymin, ymax,
            znbins, zmin, zmax),
         directoryTFile);
   }

   std::shared_ptr<T> Get()
   {
      return objPtr;
   }
};

#endif /*THR_OBJ_HPP*/
