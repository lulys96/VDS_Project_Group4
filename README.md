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
To run the acceptance tests, you first need to run the bench with the benchmark-tests.
In order to do this, navigate into the bench folder after running make in the build-folder and run the executable with the benchmark of your choice:
```
cd ./src/bench/
./VDSProject_bench ../../../benchmarks/<benchmarkname>/<benchfile>.bench
```
When the testbench is finished, it will show how the runtime of the computations, VM: the virtual memory needed and RSS: The portion of memory occupied by the process that is in RAM.
To verify the results you need to check the results against the already existing results. The results computed are located in the same build-folder as the executable.
Compare one .txt result file at the time:
```
cd ../verify/
./VDSProject_verify ../bench/results_<benchfile>/txt/<outputname>.txt ../../../src/verify/results_<benchfile>/txt/<outputname>.txt 
```
If the computed output is equivalent for all files, the acceptance test passes.
