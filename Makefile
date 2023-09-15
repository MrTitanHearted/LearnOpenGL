CXX := clang++
CXXFLAGS := -std=c++20 -Wall
TARGET := learnopengl.exe
BUILD_DIR := .\build
SOURCES := .\src\main.cpp .\src\glad.c .\src\shaders.cpp .\src\textures.cpp .\src\buffers.cpp .\src\camera.cpp
INC_DIR := -Iinclude
INCLUDES := .\include\shaders.hpp .\include\textures.hpp .\include\buffers.hpp .\include\camera.hpp
LIB_DIR := -Llib
LDFLAGS := -lOpenGL32 -lglfw3dll -lassimp-vc143-mt

build: $(BUILD_DIR)\$(TARGET)

$(BUILD_DIR)\$(TARGET): $(SOURCES) $(INCLUDES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(BUILD_DIR)\$(TARGET) $(INC_DIR) $(LIB_DIR) $(LDFLAGS)

run:
	$(BUILD_DIR)\$(TARGET)