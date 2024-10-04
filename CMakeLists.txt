cmake_minimum_required(VERSION 3.5)

project(DebugObject)

find_package(tsl-robin-map CONFIG REQUIRED)

add_compile_options(/std:c++20 /Zc:preprocessor /fsanitize=address)

add_executable(DebugObject  Main.cpp 

                            DebugObject.h
                            DebugObject.cpp

                            Object.h
                            Object.cpp
                            ObjectBaseInfo.h
                            ObjectManager.h
                            ObjectManager.cpp
                            DefaultCreateObject.h)

target_link_libraries(DebugObject PRIVATE tsl::robin_map)
