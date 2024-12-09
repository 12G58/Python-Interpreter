# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(LightweightPythonInterpreter)

# Set C standard
set(CMAKE_C_STANDARD 99)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.c
    src/tokenizer.c
    src/parser.c
    src/vm.c
)

# Add executable
add_executable(lightweight_python_interpreter ${SOURCES})

