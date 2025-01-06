//calls automatically when root is called in the directory rootlogon.C is located
{
   gInterpreter->AddIncludePath("include");
   
   gSystem->Load("lib/libGUIFit.so");
   gSystem->Load("lib/libTCanvasPrinter.so");
}
