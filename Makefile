# Set the shell.
SHELL=/usr/bin/env bash

# Include the configuration.
-include Makefile.inc

_mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
I := $(patsubst %/,%,$(dir $(_mkfile_path)))

ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
     -nrRf $(firstword $(MAKEFILE_LIST)) \
     ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
N := x
C = $(words $N)$(eval N := x $N)
ECHO = python3 $(I)/makefile_progress.py --stepno=$C --nsteps=$T
endif

# Rules without physical targets (secondary expansion for specific rules).
.SECONDEXPANSION:
.PHONY: all clean TCanvasPrinter GUIFit

exe_targets: AnalyzeHeatMaps AnalyzeEmbedding AnalyzeSingleTrack

all: TCanvasPrinter GUIFit
	@echo "All done"

TCanvasPrinter: lib/libTCanvasPrinter.so
GUIFit: lib/libGUIFit.so

.SILENT:

# ROOTTools

lib:
	mkdir -p $@

lib/TCanvasPrinter.o: src/TCanvasPrinter.cpp | lib
	@$(ECHO) Building CXX object $@
	$(CXX) $< $(CXX_COMMON_LIB) -o $@ $(ROOT_INCLUDE) `$(ROOT_CONFIG) --glibs`

lib/GUIFit.o: src/GUIFit.cpp | lib
	@$(ECHO) Building CXX object $@
	$(CXX) $< $(CXX_COMMON_LIB) -o $@ $(ROOT_INCLUDE) `$(ROOT_CONFIG) --glibs`

lib/lib%.so: lib/%.o
	@$(ECHO) Building CXX shared library $@
	$(CXX) -shared -o $@ $<

# other

clean: 
	@echo Cleaning
	rm -rf lib

endif
