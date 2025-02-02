// This file is used for quick check with ROOT6 interpreter
{
   gInterpreter->AddIncludePath("include");
   
   gSystem->Load("lib/libFitTools.so");
   gSystem->Load("lib/libGUIFit.so");
   gSystem->Load("lib/libTCanvasPrinter.so");
   gSystem->Load("lib/libThrObj.so");
}
