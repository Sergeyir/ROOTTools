# Overview

This is a simple set of useful classes and functions to use in any cpp application that uses [ROOT]{https://root.cern/} applications on linux.

# Requirements

- GNU GCC++17 or newer.
- ghostscript - can be installed with any package manager.
- [ROOT]{https://root.cern/} V.6.00 or newer compiled with c++17 or newer.

To check version of c++ that was used for compilation of ROOT run (you may need to head in bin directory in ROOT install directory if ROOT was not intalled with package manager)

```sh 
root-config --cflags
```

If [ROOT6]{https://root.cern/} was not installed with a package manager on your system you also need to set $ROOT_INCLUDE and $ROOT_CONFIG in your profile (.bashrc, .zshrc or other) or configure the paths in Makefile.am.

# Installing

Run in your working directory to copy the repository

```sh
git clone https://github.com/Sergeyir/ROOTTools --depth=1
```

Run in the downloaded directory to compile the libraries (you can use option -jN to compile on multiple threads)

```sh 
make
```

# Documentation

You can view the detailed documentation https://sergeyir.github.io/ROOTTools/ (see Namespaces / Namespaces List / ROOTTools). Since this repository is not designed to be used on it's own and is intended to be used as a set of libraries every class, method, and variable are all in ROOTTools namespace scope.

If the link is unaccesible you can generate the documentation yourself with doxygen. To do this run while in root of the repository

```sh
doxygen
```

To view the generated html file open html/index.html with any browser or simply run to automaticaly open it on linux

```sh
xdg-open html/index.html
```

# Usage

In order to use functions and classes from this project while compiling link libraries libTCanvasTools.so, libFitTools.so, libGUIFit.so, libThrObj.so, libTFileTools.so (see $ROOT_TOOLS_LIB in Makefile and Makefile.inc for more detail) and don't forget to include the needed header files (see list of files in documentation https://sergeyir.github.io/ROOTTools/files.html).
