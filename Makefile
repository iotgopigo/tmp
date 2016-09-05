#############################################################################
# Project makefile should define the following BEFORE including this file:
# SRC_FILES - a list of all source files
# LIB_NAME - output name
# INC_DIRS - a list of additional include directories
# LIB_DIRS - a list of additional library directories
# USED_LIBS - a list of libraries to link with
# DEFINES - [Optional] additional preprocessor defines
# CXXFLAGS - [Optional] additional flags for the compiler
# LDFLAGS - [Optional] additional flags for the linker
#############################################################################

# マクロ定義部
CXX      = g++
CXXFLAGS = -O3 -Wall
BUILD_DIR= ./bin
LDFLAGS	 =
INCLUDES = -I/usr/local/include -I. -I/usr/local/include/opencv2 -I/usr/local/include/opencv
LIBS	 = -L/usr/lib -L/usr/lib/i386-linux-gnu -lm -L/usr/local/lib -lopencv_highgui -lopencv_imgproc -lopencv_core 
TARGET   = main
SRCS	 = main.cpp
OBJS     = main.o

# 生成規則
all:	$(BUILD_DIR) $(patsubst %,$(BUILD_DIR)/%,$(TARGET))

$(BUILD_DIR) :
	mkdir -p $@

$(BUILD_DIR)/%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< $(LIBS) $(INCLUDES)

$(BUILD_DIR)/$(TARGET): $(patsubst %,$(BUILD_DIR)/%,$(OBJS))
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS) $(INCLUDES)

.PHONY: all clean
