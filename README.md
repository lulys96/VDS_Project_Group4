# VDS_Project_Group4
![Workflow Badge](https://github.com/lulys96/VDS_Project_Group4/workflows/GTest/badge.svg)

## Building a static library:
In order to compile the BDD package into a static library, navigate to root-folder.
Then run:
```
mkdir build && cd build
cmake ../
make
```

This will create the static library called "*libManager.a*" inside 
the build/src/ directory

## How to run the unittests:
After running make, there will be a executable located in the build/src/test/
inside this folder run the command:
```
./VDSProject_test
```
This will run all unittests within the gtest-suite.

## How to run the acceptance tests of the entire package:
