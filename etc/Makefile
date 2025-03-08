# Set the shell.
SHELL=/usr/bin/env bash

# Include the configuration.
-include Makefile.inc

# Rules without physical targets (secondary expansion for specific rules).
.SECONDEXPANSION:
.PHONY: all clean

all: ROOTTools
	@echo "All done"

ROOTTools: TF1Tools TCanvasTools ThrObj GUIDistrCutter2D
TCanvasTools: $(ROOT_TOOLS_LIB_DIR)/libTF1Tools.so
TF1Tools: $(ROOT_TOOLS_LIB_DIR)/libTCanvasTools.so
GUIFit: $(ROOT_TOOLS_LIB_DIR)/libGUIFit.so
ThrObj: $(ROOT_TOOLS_LIB_DIR)/libThrObj.so
TFileTools: $(ROOT_TOOLS_LIB_DIR)/libTFileTools.so
GUIDistrCutter2D: $(ROOT_TOOLS_LIB_DIR)/libGUIDistrCutter2D.so

.SILENT:

# ROOTTools

$(ROOT_TOOLS_LIB_DIR):
	mkdir -p $@

$(ROOT_TOOLS_LIB_DIR)/%.o: src/%.cpp | $(ROOT_TOOLS_LIB_DIR)
	@echo Building CXX object $@
	$(CXX) $< $(CXX_COMMON_LIB) -o $@ $(ROOT_TOOLS_INCLUDE) $(ROOT_INCLUDE) `$(ROOT_CONFIG) --glibs`

$(ROOT_TOOLS_LIB_DIR)/lib%.so: $(ROOT_TOOLS_LIB_DIR)/%.o
	@echo Building CXX shared library $@
	$(CXX) -shared -o $@ $<

# other

clean: 
	@echo Cleaning
	rm -rf $(ROOT_TOOLS_LIB_DIR)/*.so $(ROOT_TOOLS_LIB_DIR)/*.o
