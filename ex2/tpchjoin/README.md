# FDE Exercise 2.5

## Task

Implement the missing parts of the function sum_extendedprice in src/sum.cpp.
You can use the test provided in test/test_sum.cpp to check if your implementation works
correctly.

## Build
A configuration file is provided to build this project with CMake.
This allows you to build the project in the terminal but also
provides the option to use Jetbrains CLion or Microsoft Visual Studio
and other IDEs.

Building from Terminal:
Start in the project directory.
To create a build for debugging:
```
mkdir -p build/debug
cd build/debug
cmake -DCMAKE_BUILD_TYPE=Debug ../..
make
```

To create a build for performance measurements, start in the project directory:
```
mkdir -p build/release
cd build/release
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ../..
make
```

This creates the binaries test_all and main.

Make sure your builds are not failing! <br/>
*Left Sidebar > CI /CD > Pipelines*
