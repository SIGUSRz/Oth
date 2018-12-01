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
* --type, -t: [required] game type, 0: player first, 1: compuer first.
* --limit, -l: [required] time limit for each step, __usage not implemented__
* --include, -I: [optional] include board file
