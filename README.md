# Oth

## Dependencies
A __CLion__ Project with CMake
* Boost 1.68
* gcc 8.2
* C++ 2017 standard

## Installation
Before Run, set path variable
```
DYLD_LIBRARY_PATH=${BOOST_LIBRARY_PATH} 
```

## Parameters
* --help: help
* --black, -b: [required int] black player, 0: AI, 1: human.
* --white, -w: [required int] white player, 0: AI, 1: human.
* --limit, -l: [required float] time limit for each step.
* --include, -I: [optional string] include board file, __usage not implemented__.
