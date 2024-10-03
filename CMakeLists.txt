cmake_minimum_required(VERSION 3.5)

project(DebugObject)

find_package(tsl-robin-map CONFIG REQUIRED)

add_compile_options(/std:c++20 /Zc:preprocessor)

add_executable(DebugObject DebugObject.cpp Object.h Object.cpp ObjectBaseInfo.h ObjectManager.cpp ObjectManager.h DefaultCreateObject.h)

target_link_libraries(DebugObject PRIVATE tsl::robin_map)
