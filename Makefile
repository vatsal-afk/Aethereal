# Compiler
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iinclude -Ivendor/imgui -Ivendor/glad

# Source files
SRC = src/core/main.cpp \
      src/core/Renderer.cpp \
      src/core/Shader.cpp \
      src/core/InputManager.cpp \
      src/glad.c \
      vendor/imgui/imgui.cpp \
      vendor/imgui/imgui_demo.cpp \
      vendor/imgui/imgui_draw.cpp \
      vendor/imgui/imgui_tables.cpp \
      vendor/imgui/imgui_widgets.cpp \
      vendor/imgui/imgui_impl_glfw.cpp \
      vendor/imgui/imgui_impl_opengl3.cpp

# Output binary
TARGET = app

# Libraries (for Linux)
LIBS = -lglfw -ldl -lGL -lX11 -lpthread

# Default target
all: $(TARGET)

# Compile and link
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

# Clean build files
clean:
	rm -f $(TARGET)
