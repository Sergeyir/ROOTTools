/** 
 *  @file   CheckFileForNan.hpp 
 *  @brief  Contains useful set of functions to work with TFile objects
 *
 *  Not finished yet
 *
 *  This file is a part of a project ROOTTools (https://github.com/Sergeyir/ROOTTools).
 *
 *  @author Sergei Antsupov (antsupov0124@gmail.com)
 **/
#include "TFileTools.hpp"

#ifndef ROOT_TOOLS_TFILE_TOOLS_CPP
#define ROOT_TOOLS_TFILE_TOOLS_CPP

ROOTTools::CheckFileForNan::CheckFileForNan(const std::string &fileName)
{
   if (!std::ifstream(fileName).is_open()) std::cout << "Error: File " << fileName <<
                                                     " does not exist" << std::endl;
   file = TFile::Open(fileName.c_str());
}

bool ROOTTools::CheckFileForNan::ContainsNan(const std::string& dirName)
{
   bool containsNan = false;

   TIter keyList(file->GetListOfKeys());
   TKey *key;

   if (dirName != "") file->cd(dirName.c_str());
   else file->cd();


   //for (int i = 0; i < currentDirList->GetEntries(); i++)
   while ((key = (TKey*)keyList())) 
   {
      //TClass *obj = gROOT->GetClass(key->GetClassName());
      TClass *obj = gROOT->GetClass(key->GetClassName());

      std::cout << ((TH3 *) key->ReadObj())->Integral() << std::endl;
      if (obj->IsFolder()) continue;
      /*
      {
         std::cout << obj->GetName() << std::endl;
         break;
         containsNan = ContainsNan(obj->GetName());
      }
      */
      else if (obj->InheritsFrom("TH1"))
      {
         TH1 *hist = (TH1 *) obj->Clone();
         std::cout << obj->GetName() << std::endl;
         if (std::isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
      else if (obj->InheritsFrom("TH2F"))
      {
         std::cout << obj->GetName() << std::endl;
         TH2 *hist = (TH2 *) obj->Clone();
         if (std::isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
      else if (obj->InheritsFrom("TH3F"))
      {
         std::cout << obj->GetName() << std::endl;
         TH3 *hist = (TH3 *) obj->Clone();
         if (std::isnan(hist->Integral())) 
         {
            PrintNanInfo(obj->GetName(), dirName);
            containsNan = true;
         }
      }
   }

   return containsNan;
}

void ROOTTools::PrintNanInfo(const std::string& objName, const std::string& dirName)
{
   std::cout << "Info: object " << objName << " contains NaN";
   if (dirName != "") std::cout << " in directory " << dirName;
   std::cout << std::endl;
}

#endif /* ROOT_TOOLS_TFILE_TOOLS_CPP */
