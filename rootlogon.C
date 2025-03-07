// This file is used for quick check with ROOT6 interpreter
{
   gInterpreter->AddIncludePath("include");
   
   gSystem->Load("lib/libTCanvasTools.so");
   //gSystem->Load("lib/libGUIFit.so");
   gSystem->Load("lib/libThrObj.so");
   //gSystem->Load("lib/libTFileTools.so");
   gSystem->Load("lib/libGUIDistrCutter2D.so");
}
