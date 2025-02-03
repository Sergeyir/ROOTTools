/** 
 *  @file   CheckFileForNan.hpp 
 *  @brief  Contains useful set of functions to work with TFile objects
 *
 *  Not yet finished
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/

#ifndef TFILE_TOOLS_HPP
#define TFILE_TOOLS_HPP

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

class CheckFileForNan
{
   public:
   CheckFileForNan(const std::string &fileName);
   bool ContainsNan(const std::string &dirName = "");
   
   private: 
   TFile *file;
   void PrintNanInfo(const std::string& objName, const std::string& dirName);
};

#endif /* TFILE_TOOLS_HPP */
