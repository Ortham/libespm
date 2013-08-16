# Libespm

A free software library for reading and writing .esp and .esm plugin files.

## Introduction

Libespm can currently read data from plugins for the following games:

* TES III: Morrowind
* TES IV: Oblivion
* TES V: Skyrim
* Fallout 3
* Fallout: New Vegas

Libespm was first created by deaths_soul (AKA MCP), then forked by myself (WrinklyNinja) and pretty much rewritten, though the structure of the library has only changed a little.

Libespm requires the Boost.Filesystem (built) and Boost.Iostreams (headers only) libraries, yaml-cpp and zlib. At the moment the library is header-only, so just make sure that those libraries are accessible in your project build settings.
