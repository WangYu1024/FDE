# FDE Exercise 3.4

## Task

Implement the missing code fragments in src/main.cpp and src/sum.cpp.
See the corresponding header include/sum.hpp for documentation.
You can use the test provided in test/test_sum.cpp to check if your implementation works
correctly.

## Build
A configuration file is provided to build this project with CMake.
This allows you to build the project in the terminal but also
provides the option to use Jetbrains CLion or Microsoft Visual Studio
and other IDEs.

Building from Terminal:
Start in the project directory.
```
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
```

This creates the binaries test_all and main.

Make sure your builds are not failing! <br/>
*Left Sidebar > CI /CD > Pipelines*
