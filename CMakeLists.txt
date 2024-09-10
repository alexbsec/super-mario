cmake_minimum_required(VERSION 3.10)
project(SuperMarioClone)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include /usr/local/Cellar/sfml/2.6.1/include)

# Add the source files
add_executable(SuperMario src/main.cpp src/Player.cpp)

# Link SFML libraries
target_link_libraries(SuperMario /usr/local/Cellar/sfml/2.6.1/lib/libsfml-graphics.dylib
                                    /usr/local/Cellar/sfml/2.6.1/lib/libsfml-window.dylib
                                    /usr/local/Cellar/sfml/2.6.1/lib/libsfml-system.dylib)

