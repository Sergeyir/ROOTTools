// $HEADER$
//------------------------------------------------------------------------------------------------
//                                    CheckTFileForNan declaration
//------------------------------------------------------------------------------------------------
// CheckTFileForNan
//
// ** Free and open code for anyone to use **
//
// Author: Sergei Antsupov
// Email: antsupov0124@gmail.com
//
/**
 * Basic tool for checking root files (TFile) for Nan containing objects
 **/
//------------------------------------------------------------------------------------------------

#ifndef CHECK_TFILE_FOR_NAN_HPP
#define CHECK_TFILE_FOR_NAN_HPP

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

#endif /* CHECK_TFILE_FOR_NAN_HPP */
