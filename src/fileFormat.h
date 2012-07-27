/*
 * fileFormat.h
 * This file is part of Parse
 *
 * Copyright (C) 2012 - deaths_soul(MCP)
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
 * @file fileFormat.h
 * @brief Contains the file format stuff.
 * @details A support library to hold the general information on the format of a specific file type, as opposed to the general patterns. I think...
 * It's missing some notes, but I'll get them included later.
 * The notes missing include, but are not limited to, the observation that the main TES4 section's size value corresponds to 
 */
#pragma once
namespace parser{
	/**
	 * @namespace fileFormat
	 * @brief Contains file format stuff.
	 * @details This is for the stuff relating to the file format itself. Currently, it's a bit sparse and includes mainly notes and observations, but the idea is that
	 * this is where the main programs will output a parsed format to in various structs.
	 */
	namespace fileFormat{
		/**
		 * @struct TES_Format
		 * @brief The plugin format.
		 * @details This contains the data for the plugin files in a fairly generic format to allow the files for past and
		 * future games to be easily parsed with little to no changes.
		 */
		struct TES_Format{
			/*These two numbers will need to be changed to non-hardcoded numbers in order to accomodate format changes.
			 *Assuming HEDR remains constant, which there is a good chance of, this is fairly trivial.
			 *<obj_name>.read(<var>, <size>) will read in the data to the variables after the initial parse of the main file to determine the individual values.
			 *The family of ato<desired>(<have>) is used to convert it to the proper format.
			 *For the initial parse, we go through and run a count between some predefined constants and use the count as the amount of data to read in.
			 *The predefined constants will probably correspond somewhat to the initial parse of the main file that we use to get just the record listings.
			 *After we have the listings, we could go through and have the system automatically create the corresponding variables for them and then import the
			 *generated code into the main program.
			 *...
			 *This may actually work to parse through all their past and future titles autonomously.
			 *
			 *However, this won't take the data-types into account. That's a different problem. The big question is, can it take the data-types into account on its own?
			 *For Skyrim, CNAM and MAST are followed by 2 hex bits that denote the size of the data including the null character.
			 *However, this may be different for past and future games so this is something that shouldn't be relied upon.
			 *The only thing that can be relied upon is that the records themselves will most likely have the same format (3 or 4 capital letters) across all games.
			 *
			 *
			 *The problem with figuring out data-types has been solved, kind of. Check every bit of data and see if it's a printable character and check the
			 *last one as denoted by the record size to make sure it's null.
			 */
			/*
			 * .? is the delimiter in the files used to denote a new entry, from the looks of it.
			 * I can use that to extract only that data. Though, I need to check on the records
			 * This is only for the executables, though
			 * Read in file, string = getline(), if string[0] == '.' && string[1] == '?' write to another file, else skip line
			 * Boom, there's the parser code
			 * Er, erase string[0] and string[1] first and then write. That way, we have just the entries and not the delimiters :p
			 * Records are not delimited at all, unfortunately
			 * However, they are at least three capital letters on the same line.
			 * string = getline(), if length < 3 then not a record, if length >= 3 then check all to make sure they're either a capital letter or a number
			 * Capital letters have a specific ASCII value range
			 * 65 - 90 in base 10
			 * 41 - 5A in hex
			 * Output to 2 different files. 1 for operations and 1 for records.
			 * Ops
			 * Records
			 * If there's at least 4 capital letters in the entry, write it to the file. The whole line as there are some entries with qualifiers
			 * 2-
			 * 20
			 * 11 + 16 + 16 + 16 + 16 + 16 + 2 = 13 + 5 * 16 = 80 + 13 = 93 - 20 = 74
			 * Er 73
			 * Bingo
			 * HEADER
			 * LENGTH (TOTAL_LENGTH - NUM_SUBHEADERS)
			 * DATA
			 * What's before the data is beyond me...
			 * Ok, that's the basic format for all their games. And there is no reason to assume it'll change. 
			 * If it does, a simple change to the code is all that's necessary I think
			 * I need to sit down with a pad of paper and a pen and think this through. I've got everything I need, for the most part.
			 * I just need to sit down and work it out. After that, I can worry about the unknowns.
			*/
			unsigned char headerString[4];
			unsigned short recordSize;
			unsigned int stuff;
			unsigned char HEDR[4];
			unsigned short HEDRSize;
			unsigned int moreStuff;
			unsigned char CNAM[4];
			unsigned short CNAM_size;
			string author;
			unsigned char MAST[4];
			unsigned short MAST_size;
			string masterFile;
			unsigned char DATA[4];
			unsigned short DATA_size;
			unsigned int moreStuff4;
			unsigned char INTV[4];
			unsigned short INTV_size;
			unsigned int moreStuff5;
		};
		struct BASE_FORMAT{
			unsigned char record[4];
			unsigned short size;
			unsigned char data[size];
		};
		struct BASE_HEADER_FORMAT{
			unsigned char header[4];
			unsigned short size;
			unsigned char stuff[18];
			BASE_FORMAT base;
		};
	}
}
