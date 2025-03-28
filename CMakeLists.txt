cmake_minimum_required(VERSION 3.10)
project(AttitudeIndicator VERSION 1.0.0)

find_package(OpenGL REQUIRED)

add_executable(AttitudeIndicator 
src/config.h 
src/main.cpp 
src/glad.c
src/graphics/Shader.cpp
src/graphics/Shader.h
src/graphics/Circle.h
src/graphics/Circle.cpp
src/graphics/Rectangle.h
src/graphics/Rectangle.cpp
src/graphics/QuarterCircle.h
src/graphics/QuarterCircle.cpp
src/graphics/RoundedRectangle.h
src/graphics/RoundedRectangle.cpp
src/graphics/AltitudeLines.h
src/graphics/AltitudeLines.cpp
)

target_include_directories(AttitudeIndicator PRIVATE dependencies graphics)

target_link_directories(AttitudeIndicator PRIVATE dependencies graphics)
target_link_libraries(AttitudeIndicator glfw3 OpenGL::GL)
