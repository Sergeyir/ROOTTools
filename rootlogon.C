//calls automatically when root is called in the directory rootlogon.C is located
{
   gInterpreter->AddIncludePath("include");
   
   gSystem->Load("lib/GUIFit.so");
   gSystem->Load("lib/TCanvasPrinter.so");
}
