/*
 * fileUtil.h
 * This file is part of Parse
 *
 * Copyright (C) 2012 - deaths_soul (MCP)
 *
 * Parse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Parse is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Parse. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file fileUtil.h
 * @brief Contains general utility functions for the game files.
 * @details Contains the functions related to things like merging, master creation, and so forth.
 */
#pragma once
#include "fileFormat.h"
namespace util{
	void createMast(parser::fileFormat::file &fileA, std::vector<parser::fileFormat::file> masters);
	void createONAM(parser::fileFormat::file &fileA, std::vector<parser::fileFormat::file> masters);
	void revCreateMast(parser::fileFormat::file &fileA);
	void revCreateONAM(parser::fileFormat::file &fileA);
}
