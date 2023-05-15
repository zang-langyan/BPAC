#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need GLFW (http://www.glfw.org), GLEW (http://glew.sourceforge.net) and Poppler(https://poppler.freedesktop.org) installed:
# Linux:
#   apt-get install libglfw-dev libglew-dev libpoppler-cpp-dev
# Mac OS X:
#   brew install glfw glew poppler
# MSYS2:
#   pacman -S --noconfirm --needed mingw-w64-x86_64-toolchain mingw-w64-x86_64-glfw mingw-w64-x86_64-glew mingw-w64-x86_64-poppler
#

#CXX = g++
#CXX = clang++

# ---------------------------------------------------------------------
#  Directory structure
# ---------------------------------------------------------------------
IMGUI_DIR = include/imgui
BUILT_DIR = built

# Name of executable
EXE = BPAC++

# BPAC++ sources
SOURCES += $(wildcard src/*.cpp)

# imgui sources
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_glfw.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp

# Object files
OBJS = $(addprefix $(BUILT_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))

# ---------------------------------------------------------------------
# System detection
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

# Compiler flags
CXXFLAGS = -std=c++17 -w
# *********************************************************************
# Includes:
# stb_image
CXXFLAGS += -Iinclude

# imgui
INC_IMGUI = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += $(INC_IMGUI)

CXXFLAGS += -g -Wall -Wformat
# *********************************************************************
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3 glew poppler-cpp poppler` 

	CXXFLAGS += `pkg-config --cflags glfw3 glew poppler-cpp poppler`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib 
	#LIBS += -L/usr/local/Cellar/poppler/23.04.0_1/lib
	#LIBS += -lglfw3
	LIBS += -lglfw
	LIBS += -lglew
	LIBS += -lpoppler -lpoppler-cpp

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include 
	CXXFLAGS += -I/usr/local/Cellar/poppler/23.04.0_1/include/poppler/cpp 
	CXXFLAGS += -Wno-deprecated
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32 -lglew -lpoppler -lpoppler-cpp

	CXXFLAGS += `pkg-config --cflags glfw3 glew poppler-cpp poppler`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

# src
$(BUILT_DIR)/%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# imgui
$(BUILT_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILT_DIR)/%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(OBJS): | $(BUILT_DIR)

$(BUILT_DIR):
	mkdir -p $(BUILT_DIR)

clean:
	rm -f $(EXE) $(OBJS)
