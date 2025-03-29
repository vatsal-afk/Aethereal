# Compiler
CXX = g++
CXXFLAGS = -Wall -Iinclude -Ivendor/imgui

# Source files
SRC = application.cpp src/glad.c \
      vendor/imgui/imgui.cpp \
      vendor/imgui/imgui_demo.cpp \
      vendor/imgui/imgui_draw.cpp \
      vendor/imgui/imgui_tables.cpp \
      vendor/imgui/imgui_widgets.cpp \
      vendor/imgui/imgui_impl_glfw.cpp \
      vendor/imgui/imgui_impl_opengl3.cpp

# Output binary
TARGET = app

# Libraries
LIBS = -lglfw -ldl -lGL

# Default target
all: $(TARGET)

# Compile and link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Clean build files
clean:
	rm -f $(TARGET)
