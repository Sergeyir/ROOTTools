// $SOURCE$
//------------------------------------------------------------------------------------------------
//                                    CheckTFileForNan realisation
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

#include "../include/CheckTFileForNan.hpp"

#ifndef CHECK_TFILE_NAN_CPP
#define CHECK_TFILE_NAN_CPP

CheckFileForNan::CheckFileForNan(const std::string &fileName)
{
   if (!std::ifstream(fileName).is_open()) std::cout << "Error: File " << fileName <<
                                                     " does not exist" << std::endl;
   file = TFile::Open(fileName.c_str());
}

bool CheckFileForNan::ContainsNan(const char *dirName)
{
   if (dirName != "") file->cd(dirName);
   TList *currentDirList = gDirectory()->GetListOfKeys();

   bool containsNan = false;

   for (int i = 0; i < currentDirList.GetEntries(); i++)
   {
      TObject *obj = currentDirList.At(i);
      
      if (currentDirList[i]->isFolder()) 
      {
         containsNan = ContainsNan(obj->GetName())
      }
      else if (obj->InheritsFrom("TH1"))
      {
         TH1 *hist = (TH1 *) obj->Clone();
         if (isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
      else if (obj->InheritsFrom("TH2"))
      {
         TH2 *hist = (TH2 *) obj->Clone();
         if (isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
      else if (obj->InheritsFrom("TH3"))
      {
         TH3 *hist = (TH3 *) obj->Clone();
         if (isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
   }
}

bool CheckFileForNan::ContainsNan(const std::string &dirName) ContainsNan(dirName.c_str());

void PrintNanInfo(const char *objName, const char *dirName)
{
   std::cout << "Info: object " << objName << " contains NaN";
   if (dirName != "") std::cout << " in directory " << dirName;
   std::cout << std::endl;
}

#endif /* CHECK_TFILE_NAN_CPP */
