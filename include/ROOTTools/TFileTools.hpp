/** 
 *  @file   CheckFileForNan.hpp 
 *  @brief  Contains useful set of functions to work with TFile objects
 *
 *  Not finished yet; to be added to ROOTTools.hpp after completion
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/

#ifndef ROOT_TOOLS_TFILE_TOOLS_HPP
#define ROOT_TOOLS_TFILE_TOOLS_HPP

#include <string>
#include <iostream>
#include <fstream>

#include "TROOT.h"
#include "TKey.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

/// @namespace ROOTTools ROOTTools namespace
namespace ROOTTools
{
   /*! @class CheckFileForNan
    * @brief class CheckFileForNan can be used to check if the file contains objects with Nan values
    *
    * Not finished yet
    */
   class CheckFileForNan
   {
      public:
      CheckFileForNan(const std::string &fileName);
      bool ContainsNan(const std::string &dirName = "");
      protected: 
      TFile *file;
      void PrintNanInfo(const std::string& objName, const std::string& dirName);
   };
}

#endif /* ROOT_TOOLS_TFILE_TOOLS_HPP */
