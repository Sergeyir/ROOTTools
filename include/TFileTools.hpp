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

#include "TKey.h"
#include "TKeyList.h"
#include "TFile.h"
#include "TTree.h"

class CheckFileForNan()
{
   public:
   CheckFileForNan(const std::string &fileName);
   bool ContainsNan(const char *dirName = "");
   bool ContainsNan(const std::string &dirName = "");
   
   private: 
   TFile *file;
   void PrintNanInfo();
}

#endif /* TFILE_TOOLS_HPP */
