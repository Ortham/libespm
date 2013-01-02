/*
 * fileUtil.h
 * This file is part of libespm
 *
 * Copyright (C) 2012 - deaths_soul (MCP)
 *
 * libespm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libespm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libespm. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file fileUtil.h
 * @brief Contains general utility functions for the game files.
 * @details Contains the functions related to things like merging, master creation, and so forth.
 */
#ifndef __FILE_UTIL_H__
#define __FILE_UTIL_H__

#include <vector>
#include "constants.h"
#include "fileFormat.h"
/**
 * @namespace util
 * @brief Contains utility functions.
 * @details Contains functions to perform various operations on the game files themselves.
 */
namespace util{
	/**
	 * @brief Sets the flag for a plugin file for it to be a 'master'.
	 * @details Sets the flag for a plugin file for it to be a 'master'.
	 * @param &fileA
	 * The <tt> struct </tt> representing the file we want to make a 'master'.
	 */
	void createMast(espm::file &fileA);
	/**
	 * @brief Creates the ONAM record.
	 * @details Scans through a list of masters and a file to see if any of the records' ID are identical and also checks to see if the record names are in the ONAM entry
	 * in the value list and adds the ID to the ONAM entry if they are.
	 * @param &fileA
	 * The <tt> struct </tt> representing the file we want to generate ONAM stuff for.
	 * @param masters
	 * The list of masters to scan.
	 */
	void createONAM(espm::file &fileA, std::vector<espm::file> masters);
	/**
	 * @brief Un-sets the flag for a plugin file for it to be a 'master'.
	 * @details Un-sets the flag for a plugin file for it to be a 'master'.
	 * @param &fileA
	 * The <tt> struct </tt> representing a file we want to convert from a 'master'.
	 */
	void revCreateMast(espm::file &fileA);
	/**
	 * @brief Removes ONAM stuff.
	 * @details Scans through the top-level record list in a plugin file and removes the ONAM record.
	 * @param &fileA
	 * The <tt> struct </tt> representing the file we want to remove the ONAM stuff for.
	 */
	void revCreateONAM(espm::file &fileA);
	/*END OF LINE*/
}

#endif /* __FILE_UTIL_H__ */
