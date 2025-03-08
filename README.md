# Overview

Set of useful functions and classes that can be used in any C++ application to simplify the work with <a href="https://root.cern/">ROOT6</a>'s various classes, such as TF1, TCanvas, TFile, TThreadedObject, etc., or to extend their usage. This repository is still in progress, and some of its features are yet to be finished/added. But everything described in the [documentation](https://sergeyir.github.io/documentation/ROOTTools/) is finished and it works fine.

# Requirements

- GNU GCC++17 or newer.
- ghostscript - can be installed with any package manager.
- [ROOT](https://root.cern/) V.6.00 or newer compiled with C++17 or newer.

To check the version of C++ that was used for the compilation of the ROOT run (you may need to head into the bin directory in the ROOT install directory if ROOT was not installed with the package manager)

```sh 
root-config --cflags
```

If [ROOT6](https://root.cern/) was not installed with a package manager on your system, you also need to set path to the directory in which root was installed ($ROOT_PATH) in your profile (.bashrc, .zshrc, or other) or configure the paths in CMakeLists.txt or etc/Makefile.am.

# Installing

Run in your working directory to copy the repository

```sh
git clone https://github.com/Sergeyir/ROOTTools --depth=1
```

First run in the downloaded directory to generate Makefile

```sh
cmake .
```

Then run to compile the libraries (you can use option -jN to compile on multiple threads)

```sh 
make
```

Or you can use Makefile from etc directory

```sh
cp etc/Makefile* . && make
```

# Documentation

You can view the detailed documentation at https://sergeyir.github.io/documentation/ROOTTools/. (see Namespaces / Namespaces List / ROOTTools). Since this repository is not designed to be used on its own and is intended to be used as a set of libraries, every class, method, and variable is all in the ROOTTools namespace scope.

If the link is inaccessible, you can generate the documentation yourself with Doxygen. To do this run while in the root of the repository

```sh
doxygen
```

To view the generated html file open html/index.html with any browser or simply run to automaticaly open it on linux

```sh
xdg-open html/index.html
```

# Usage

In order to use functions and classes from this project while compiling link libraries libTCanvasTools.so, libFitTools.so, libGUIFit.so, libThrObj.so, libTFileTools.so (see $ROOT_TOOLS_LIB in Makefile and Makefile.inc for more detail or see CMakeLists.txt), and don't forget to include the needed header files (see the list of files in documentation https://sergeyir.github.io/documentation/ROOTTools/files.html).
