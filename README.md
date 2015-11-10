Libespm2
========

[![Build Status](https://travis-ci.org/WrinklyNinja/libespm2.svg?branch=master)](https://travis-ci.org/WrinklyNinja/libespm2)

A free software library for reading Elder Scrolls Plugin (`.esp`) and Elder Scrolls Master (`.esm`) files. It can currently parse plugins for the following games:

* TES III: Morrowind
* TES IV: Oblivion
* TES V: Skyrim
* Fallout 3
* Fallout: New Vegas

Libespm2 is a test-driven development rewrite of [libespm](https://github.com/WrinklyNinja/libespm) that focuses on providing a useful API to [libloadorder](https://github.com/WrinklyNinja/libloadorder) and [LOOT](https://github.com/loot/loot), rather than a general-purpose plugin parser.

## Requirements

* [Boost](http://www.boost.org) Filesystem and Locale libraries (tested with v1.55.0 and v1.59.0)

## Build Instructions

Libespm2 itself is a header-only library, so if you want to use it, only Boost's Filesystem library needs to be built.

Libespm2's test suite also requires [Google Test](https://github.com/google/googletest) (tested with v1.7.0), and build files can be generated using [CMake](http://cmake.org). The [Travis config file](.travis.yml) documents the process of building on Linux with GCC. The process on Windows with MSVC is equivalent.
