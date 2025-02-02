/** 
 *  @file   ThrObj.hpp 
 *  @brief  Contains useful set of containers to simplify and/or speed up the process of writing multithreaded applications with ROOT
 *
 *  In order to use these containers libThrObj.so must be loaded
 *
 *  This file is a part of a project CppTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#ifndef ROOT_TOOLS_THR_OBJ_HPP
#define ROOT_TOOLS_THR_OBJ_HPP

#include <vector>
#include <array>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include "ROOT/TThreadedObject.hxx"

/// @namespace ROOTTools ROOTTools namespace
namespace ROOTTools
{
   namespace ThrObjHolder
   { 
      std::shared_ptr<TH1F> AddHistogram(ROOT::TThreadedObject<TH1F> *hist, 
                                         const std::string& directory);
      std::shared_ptr<TH2F> AddHistogram(ROOT::TThreadedObject<TH2F> *hist, 
                                         const std::string& directory);
      std::shared_ptr<TH3F> AddHistogram(ROOT::TThreadedObject<TH3F> *hist, 
                                         const std::string& directory);
      
      std::shared_ptr<TH1D> AddHistogram(ROOT::TThreadedObject<TH1D> *hist, 
                                         const std::string& directory);
      std::shared_ptr<TH2D> AddHistogram(ROOT::TThreadedObject<TH2D> *hist, 
                                         const std::string& directory);
      std::shared_ptr<TH3D> AddHistogram(ROOT::TThreadedObject<TH3D> *hist, 
                                         const std::string& directory);
      void Clear();
      void AddTFileDirectory(const std::string& directory, const int thrObjIndex);
      void Write();
      void Write(const std::string& outputFileName);

      // containers that store histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1F>>> containerTH1F;
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2F>>> containerTH2F;
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3F>>> containerTH3F;
      
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1D>>> containerTH1D;
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2D>>> containerTH2D;
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3D>>> containerTH3D;

      // containers of TFile directory names
      std::vector<std::string> containerTFileDir;
      // indices of the TFile directories to the histograms
      std::array<std::vector<int>, 6> containerTFileDirIndex;
   };

   template<typename T>
   class ThrObj
   {
      public:
      //TH1 methods
      ThrObj(const std::string& name, const std::string& title,
             const int xnbins, const double xmin, const double xmax,
             const std::string& fileDirectory = "");
      //TH2 methods
      ThrObj(const std::string& name, const std::string& title,
             const int xnbins, const double xmin, const double xmax, 
             const int ynbins, const double ymin, const double ymax,
             const std::string& fileDirectory = "");
      //TH3 methods
      ThrObj(const std::string& name, const std::string& title,
             const int xnbins, const double xmin, const double xmax, 
             const int ynbins, const double ymin, const double ymax,
             const int znbins, const double zmin, const double zmax,
             const std::string& fileDirectory = "");
      std::shared_ptr<T> Get();
      protected:
      std::shared_ptr<T> objPtr;
   };
};

#endif /* ROOT_TOOLS_THR_OBJ_HPP */
