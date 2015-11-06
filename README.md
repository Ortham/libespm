Libespm2
========

[![Build Status](https://travis-ci.org/WrinklyNinja/libespm2.svg?branch=master)](https://travis-ci.org/WrinklyNinja/libespm2)

A free software library for reading Elder Scrolls Plugin (`.esp`) and Elder Scrolls Master (`.esm`) files. It's a work-in-progress rewrite of libespm using test-driven development, with the goals of:

* Removing unused code (ie. that isn't called in usage by libloadorder or LOOT)
* Providing some confidence that the code actually works through automated testing
* Reducing the amount of duplicate code that has built up between libloadorder and LOOT for handling plugin files.

Because libespm2 has a tight focus on providing only what's necessary for libloadorder and LOOT to function, its functionality is very limited, and as such it's probably not suitable for wider use.

## Build Instructions

Libespm2 itself is a header-only library, so if you want to use it, no building is necessary.

If you want to run its test suite, build files can be generated using [CMake](http://cmake.org). The [Travis config file](.travis.yml) documents the process of building on Linux with GCC.

### Requirements

* [Boost](http://www.boost.org) Filesystem v1.55.0
* [Google Test](https://github.com/google/googletest) v1.7.0
