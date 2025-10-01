#!/bin/bash

# Check if the 'build' directory exists.
if [ -d "build" ]; then
    # If it exists, print a message and remove it.
    echo "Found existing 'build' directory. Removing it..."
    rm -rf build
fi

# Create a new, empty build directory.
echo "Creating build directory..."
mkdir build

# Navigate into the build directory.
cd build

# Run CMake to configure the project from the parent directory.
# The ".." tells CMake to look for CMakeLists.txt one level up.
cmake ..

# Compile the project.
make

# Run the final executable.
# The program will be located in the current directory (build).
echo "Running the program..."
./main