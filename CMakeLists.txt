cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_CMAKE})

set(CMAKE_CXX_STANDARD 23)
project(LearnOpenGL)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

set(TARGET app)

add_executable(${TARGET})

file(GLOB SOURCES src/*.cpp)

target_sources(${TARGET} PRIVATE ${SOURCES})

target_include_directories(${TARGET} PRIVATE include)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(
    ${TARGET} PRIVATE 
        glfw glad::glad glm::glm imgui::imgui assimp::assimp
)
