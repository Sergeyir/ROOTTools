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
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH1F>>> container_th1;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH2F>>> container_th2;
   std::vector<std::unique_ptr<ROOT::TThreadedObject<TH3F>>> container_th3;
   
   std::vector<std::string> container_tfile_dir;
   std::array<std::vector<int>, 3> container_tfile_dir_iter;
   
   void Clear()
   {
      container_th1.clear();
      container_th2.clear();
      container_th3.clear();
   }

   void AddTFileDir(std::string directory, const int thr_obj_type)
   {
      int dir_iter = -999;
      if (directory != "")
      {
         for (long unsigned int i = 0; i < container_tfile_dir.size(); i++)
         {
            if (container_tfile_dir[i] == directory) 
            {
               dir_iter = i;
            }
         }
         if (dir_iter == -999) 
         {
            dir_iter = container_tfile_dir.size();
            container_tfile_dir.push_back(directory);
         }
      }
      container_tfile_dir_iter[thr_obj_type].push_back(dir_iter);
   }

   void Write()
   {
      for (auto& thr_obj : container_th1) 
      {
         static_cast<std::shared_ptr<TH1>>(thr_obj->Merge())->Clone()->Write();
      }
      for (auto& thr_obj : container_th2) 
      {
         static_cast<std::shared_ptr<TH2>>(thr_obj->Merge())->Clone()->Write();
      }
      for (auto& thr_obj : container_th3) 
      {
         static_cast<std::shared_ptr<TH3>>(thr_obj->Merge())->Clone()->Write();
      }
      Clear();
   }

   void Write(std::string file_name)
   {
      TFile outfile(file_name.c_str(), "RECREATE");
      outfile.cd();
      if (container_tfile_dir.size() == 0) Write();
      else
      {
         for (std::string dir : container_tfile_dir)
         {
            outfile.mkdir(dir.c_str());
         }
         for (long unsigned int i = 0; i < container_th1.size(); i++)
         {
            if (container_tfile_dir_iter[0][i] != -999) outfile.cd(
               container_tfile_dir[container_tfile_dir_iter[0][i]].c_str());
            else outfile.cd();
            static_cast<std::shared_ptr<TH1>>(container_th1[i]->Merge())->Clone()->Write();
         }
         for (long unsigned int i = 0; i < container_th2.size(); i++)
         {
            if (container_tfile_dir_iter[1][i] != -999) outfile.cd(
               container_tfile_dir[container_tfile_dir_iter[1][i]].c_str());
            else outfile.cd();
            static_cast<std::shared_ptr<TH1>>(container_th2[i]->Merge())->Clone()->Write();
         }
         for (long unsigned int i = 0; i < container_th3.size(); i++)
         {
            if (container_tfile_dir_iter[2][i] != -999) outfile.cd(
               container_tfile_dir[container_tfile_dir_iter[2][i]].c_str());
            else outfile.cd();
            static_cast<std::shared_ptr<TH1>>(container_th3[i]->Merge())->Clone()->Write();
         }
      }
   }
} ThrObjHolder;

template<class T>
class ThrObj
{
   private:
   std::unique_ptr<ROOT::TThreadedObject<T>> thr_obj;
   std::shared_ptr<T> obj_ptr;
   int hist_holder_iter;

   public:
   
   //TH1 methods
   ThrObj(std::string name, std::string title,
      const int xnbins, const double xmin, const double xmax,
      std::string tfile_directory = "")
   {
      ThrObjHolder.container_th1.push_back(
         std::unique_ptr<ROOT::TThreadedObject<T>>(
            new ROOT::TThreadedObject<T>(
            name.c_str(), title.c_str(), 
            xnbins, xmin, xmax)));

      hist_holder_iter = ThrObjHolder.container_th1.size();
      obj_ptr = ThrObjHolder.container_th1.back()->Get();

      ThrObjHolder.AddTFileDir(tfile_directory, 0);
   }

   void Fill(const double xval, const double weight)
   {
      obj_ptr->Fill(xval, weight);
   }

   //TH2 methods
   ThrObj(std::string name, std::string title,
      const int xnbins, const double xmin, const double xmax, 
      const int ynbins, const double ymin, const double ymax,
      std::string tfile_directory = "")
   {
      ThrObjHolder.container_th2.push_back(
         std::unique_ptr<ROOT::TThreadedObject<T>>(
         new ROOT::TThreadedObject<T>(
            name.c_str(), title.c_str(), 
            xnbins, xmin, xmax, 
            ynbins, ymin, ymax)));
      
      obj_ptr = ThrObjHolder.container_th2.back()->Get();
      hist_holder_iter = ThrObjHolder.container_th2.size();

      ThrObjHolder.AddTFileDir(tfile_directory, 1);
   }

   //TH3 methods
   ThrObj(std::string name, std::string title,
      const int xnbins, const double xmin, const double xmax, 
      const int ynbins, const double ymin, const double ymax,
      const int znbins, const double zmin, const double zmax,
      std::string tfile_directory = "")
   {
      ThrObjHolder.container_th3.push_back(
         std::unique_ptr<ROOT::TThreadedObject<T>>(
         new ROOT::TThreadedObject<T>(
            name.c_str(), title.c_str(), 
            xnbins, xmin, xmax, 
            ynbins, ymin, ymax,
            znbins, zmin, zmax)));
      
      obj_ptr = ThrObjHolder.container_th3.back()->Get();
      hist_holder_iter = ThrObjHolder.container_th3.size();
      
      ThrObjHolder.AddTFileDir(tfile_directory, 2);
   }

   void Write()
   {
      static_cast<std::shared_ptr<TH1>>(thr_obj->Merge())->Clone()->Write();
   }

   std::shared_ptr<T> Get()
   {
      return obj_ptr;
   }
};

#endif
