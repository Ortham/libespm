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

Libespm requires the following Boost libraries:

* Filesystem (built)
* Iostreams
* String Algorithms
* Lexical Cast

At the moment the libespm itself is header-only, so just make sure that those libraries are accessible in your project build settings.

Support for compressed record fields requires zlib to be accessible in your project build settings, and the `USING_ZLIB` preprocessor definition.

## Design Notes

The general idea is of a generic backend that's used to parse (and write) the files, with more specialised structures to interpret the data. Everything gets namespaced according to its specificity, so:

* Generic stuff is found at `espm`.
* Game stuff is found at `espm::tes3`, `espm::tes4`, `espm::tes5`, `espm::fo3`, `espm::fonv` for Morrowind, Oblivion, Skyrim, Fallout 3 and Fallout: New Vegas respectively.
* Specific record and field structures would be found in sub-namespaces of each game's namespace, so Skyrim's TES4 record interface would be the structure `espm::tes5::TES4::Record` while the record's ONAM interface would be the structure `espm::tes5::TES4::ONAM`.

The reasoning is that different records contain different fields for different purposes, and they also contain fields with matching names but different data (eg. for Skyrim, `ONAM` in `TES4` is not the same as `ONAM` in `LCSR`). Records also differ between games, eg. the `TES4` record has no `ONAM` field in Oblivion, but does in Skyrim. Field specialisations therefore need to be defined per-record, and records need to be defined per-game. Hence the namespace structure used.

Of course, although there are quite a lot of differences between games and records, there are also quite a lot of similarities, so to prevent code duplication stuff gets imported across namespaces. For example, the `MAST` fields for the `TES4` record in Oblivion, Fallout 3, Fallout: New Vegas and Skyrim are identical, so they are defined in `espm::tes4::TES4` and the other namespaces just import from there using `using espm::tes4::TES4::MAST;`.
