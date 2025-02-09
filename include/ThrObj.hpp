/** 
 *  @file   ThrObj.hpp 
 *  @brief  Contains useful set of containers to simplify and/or speed up the process of writing multithreaded applications with ROOT TThreadedObject<T> objects
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
   /// @namespace ThrObjHolder container that stores histograms of ThrObj class
   namespace ThrObjHolder
   { 
      /*! @brief Call this function to merge and write histograms in the current open TDirectory (i.e. gDirectory)
       */
      void Write();
      /*! @brief Call this function to merge and write histograms in the specified file which will be overwritten if it already exists, otherwise it will be created
       * @param[in] outputFileName name of the output file
       */
      void Write(const std::string& outputFileName);

      // other functions and variables below are not intended for the user 
      // and are called/accessed automaticaly

      /// Not intended for user. Adds TH1F histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH1F> AddHistogram(ROOT::TThreadedObject<TH1F> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH2F histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH2F> AddHistogram(ROOT::TThreadedObject<TH2F> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH3F histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH3F> AddHistogram(ROOT::TThreadedObject<TH3F> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH1D histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH1D> AddHistogram(ROOT::TThreadedObject<TH1D> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH2D histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH2D> AddHistogram(ROOT::TThreadedObject<TH2D> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH3D histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH3D> AddHistogram(ROOT::TThreadedObject<TH3D> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH1L histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH1L> AddHistogram(ROOT::TThreadedObject<TH1L> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH2L histogram to the corresponding container; this function is called in ThrObj constructor
      std::shared_ptr<TH2L> AddHistogram(ROOT::TThreadedObject<TH2L> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Adds TH3L histogram to the corresponding container; this function is called in ThrObj constructo
      std::shared_ptr<TH3L> AddHistogram(ROOT::TThreadedObject<TH3L> *hist, 
                                         const std::string& directory);
      /// Not intended for user. Not intended for user. Clears containers with histogram after histograms were merged and written 
      void Clear();
      /// Not intended for user. Adds index of ThrObj to the directory index. This function is called in AddHistogram function
      void AddTFileDirectory(const std::string& directory, const int thrObjIndex);

      /// container for TH1F histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1F>>> containerTH1F;
      /// container for TH2F histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2F>>> containerTH2F;
      /// container for TH3F histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3F>>> containerTH3F;
      /// container for TH1D histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1D>>> containerTH1D;
      /// container for TH2D histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2D>>> containerTH2D;
      /// container for TH3D histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3D>>> containerTH3D;
      /// container for TH1L histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1L>>> containerTH1L;
      /// container for TH2L histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2L>>> containerTH2L;
      /// container for TH3L histograms
      std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3L>>> containerTH3L;

      /// containers of TFile directory names
      std::vector<std::string> containerTFileDir;
      // indices of the TFile directories to the histograms
      std::array<std::vector<int>, 9> containerTFileDirIndex;
   };

   /*! @class ThrObj
    * @brief Class ThrObj can be used to simplify the work with TThreadedObject histograms in multithreaded applications
    *
    * This class is especially useful when working with TTreeProcessorMT
    *
    * Examples on usage will be added later
    */
   template<typename T>
   class ThrObj
   {
      public:
      /*! @brief Constructor for TH1F, TH1D, and TH1L types
       *
       * @param[in] name name of the histogram
       * @param[in] title title of the histogram
       * @param[in] xNBins number of bins on X axis
       * @param[in] xLow lower edge of the first bin on X axis
       * @param[in] xUp upper edge of the last bin on X axis
       * @param[in] fileDirectory directory in which histogram will be written. If it does not exist it will be created
       */
      ThrObj(const std::string& name, const std::string& title,
             const int xNBins, const double xLow, const double xMax,
             const std::string& fileDirectory = "");
      /*! @brief Constructor for TH2F, TH2D, and TH2L types
       *
       * @param[in] name name of the histogram
       * @param[in] title title of the histogram
       * @param[in] xNBins number of bins on X axis
       * @param[in] xLow lower edge of the first bin on X axis
       * @param[in] xUp upper edge of the last bin on X axis
       * @param[in] yNBins number of bins on Y axis
       * @param[in] yLow lower edge of the first bin on Y axis
       * @param[in] yUp upper edge of the last bin on Y axis
       * @param[in] fileDirectory directory in which histogram will be written. If it does not exist it will be created
       */
      ThrObj(const std::string& name, const std::string& title,
             const int xNBins, const double xLow, const double xMax, 
             const int yNBins, const double yMin, const double yMax,
             const std::string& fileDirectory = "");
      /*! @brief Constructor for TH3F, TH3D, and TH3L types
       *
       * @param[in] name name of the histogram
       * @param[in] title title of the histogram
       * @param[in] xNBins number of bins on X axis
       * @param[in] xLow lower edge of the first bin on X axis
       * @param[in] xUp upper edge of the last bin on X axis
       * @param[in] yNBins number of bins on Y axis
       * @param[in] yLow lower edge of the first bin on Y axis
       * @param[in] yUp upper edge of the last bin on Y axis
       * @param[in] zNBins number of bins on Z axis
       * @param[in] zLow lower edge of the first bin on Z axis
       * @param[in] zUp upper edge of the last bin on Z axis
       * @param[in] fileDirectory directory in which histogram will be written. If it does not exist it will be created
       */
      ThrObj(const std::string& name, const std::string& title,
             const int xNBins, const double xLow, const double xMax, 
             const int yNBins, const double yMin, const double yMax,
             const int zNBins, const double zMin, const double zMax,
             const std::string& fileDirectory = "");
      /*! @brief Returns shared pointer to the object
       *
       * This pointer is used to create one intance of TThreadedObject on a single thread for uninterrupted access to the object which makes writes safer and faster. Different instances from all threads can be merged after the process on each thread is finished
       */
      std::shared_ptr<T> Get();
      protected:
      /// Pointer to the TThreadedObject
      std::shared_ptr<T> objPtr;
   };
};

#endif /* ROOT_TOOLS_THR_OBJ_HPP */
