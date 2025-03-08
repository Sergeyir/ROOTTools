CXX=g++ `$(ROOT_CONFIG) --cflags`
CXX_COMMON_LIB=-Wall -Werror -Wpedantic -pipe -O2 -fPIC -c

ROOT_INCLUDE=-I${ROOT_PATH}/include
ROOT_CONFIG=${ROOT_PATH}/bin/root-config

ROOT_TOOLS_BUILD_DIR=./build
ROOT_TOOLS_LIB_DIR=$(ROOT_TOOLS_BUILD_DIR)/lib
ROOT_TOOLS_INCLUDE=-I ./include
ROOT_TOOLS_LIB=-L $(ROOT_TOOLS_LIB_DIR) -Wl,-rpath,$(ROOT_TOOLS_LIB_DIR) -lTCanvasTools -lFitTools -lThrObj
