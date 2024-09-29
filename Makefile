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
.PHONY: all clean

exe_targets: AnalyzeHeatMaps AnalyzeEmbedding AnalyzeSingleTrack

all: lib/TCanvasPrinter.o \
			 lib/GUIFit.o \
			 lib/TCanvasPrinter.so \
			 lib/GUIFit.so \
			 lib/libTCanvasPrinter.a \
			 lib/libGUIFit.a
	@echo "All done"

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

lib/%.so: lib/%.o
	@$(ECHO) Building CXX shared library $@
	$(CXX) -shared -o $@ $<

lib/lib%.a: lib/%.o
	@$(ECHO) Building CXX static library $@
	ar rcs $@ $<

# other

clean: 
	@echo Cleaning
	rm -rf lib

endif
