/*
 * fileFormat.h
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
 * @file fileFormat.h
 * @brief Contains the file format stuff.
 * @details A support library to hold the general information on the format of a specific file type, as opposed to the general patterns. I think...
 * It's missing some notes, but I'll get them included later.
 * The notes missing include, but are not limited to, the observation that the main TES4 section's size value corresponds to 
 */
#pragma once
#include <sstream>
#include <iostream>
#include "commonSupport.h"
namespace parser{
	/**
	 * @namespace fileFormat
	 * @brief Contains file format stuff.
	 * @details This is for the stuff relating to the file format itself. Currently, it's a bit sparse and includes mainly notes and observations, but the idea is that
	 * this is where the main programs will output a parsed format to in various structs.
	 */
	namespace fileFormat{
		/**
		 * @var delimiterLength
		 * The length of the delimiter in bytes.
		 */
		extern unsigned int delimiterLength;
		/**
		 * @var flagLength
		 * The length of the flag field in bytes.
		 */
		extern unsigned int flagLength;
		extern unsigned int IDLength;
		extern unsigned int revLength;
		/**
		 * @var sizeLength
		 * The length of the size field in bytes.
		 */
		extern unsigned int sizeLength;
		extern unsigned int stuffzLength;
		extern unsigned int verLength;
		/**
		 * @struct TES_Format
		 * @brief The plugin format.
		 * @details This contains the data for the plugin files in a fairly generic format to allow the files for past and
		 * future games to be easily parsed with little to no changes.
		 * @note The following three structs are here just for reference purposes and may be removed in the future to clean things up.
		 */
		struct TES_Format{
			/*These two numbers will need to be changed to non-hard-coded numbers in order to accommodate format changes.
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
			std::string author;
			unsigned char MAST[4];
			unsigned short MAST_size;
			std::string masterFile;
			unsigned char DATA[4];
			unsigned short DATA_size;
			unsigned int moreStuff4;
			unsigned char INTV[4];
			unsigned short INTV_size;
			unsigned int moreStuff5;
		};
		struct BASE_FORMAT{
			unsigned char * record;
			unsigned short size;
			//unsigned char data[size];
		};
		struct BASE_HEADER_FORMAT{
			unsigned char header[4];
			unsigned short size;
			unsigned char stuff[18];
			BASE_FORMAT base;
		};
		struct field{
			char * name;
			unsigned int size;
			char * data;
		};
		/**
		 * @struct record
		 * @brief An individual record.
		 * @details This contains the data for an individual record.
		 * @note May need to add in the ability for a record to store records themselves in cases where they contain some.
		 * Though, I'm probably getting myself confused here with the fact that the terminology isn't being used consistently at all and is, quite frankly, crap.
		 */
		struct record{
			//unsigned char * name;
			std::string name;
			char * recName; //will replace std::string name once I rewrite the functions that use std::string name to work with the char type instead
			unsigned int size;
			unsigned int flags;
			unsigned int ID;
			char * recID; //will replace unsigned int ID once I rewrite the functions that use unsigned int ID to work with the char type instead (this may require that the type table thing gets further along)
			char * revision;
			char * version;
			char * stuffz;
			unsigned int decompSize;
			/*unsigned*/ char * data;
			std::vector<field> fields;
		};
		/**
		 * @struct group
		 * @brief A group of records.
		 * @details This contains the data for a group of records, as each group is composed of a list of records.
		 * @note May add a group-list as well, as some groups may themselves contain groups. Something I'll think about.
		 */
		struct group{
			char * groupHeader;
			unsigned int groupSize;
			char * groupName;
			char * type;
			char * stamp;
			char * stuffz1;
			char * version;
			char * stuffz2;
			std::vector<group> groups;
			std::vector<record> records;
		};
		/**
		 * @struct file
		 * @brief The file itself.
		 * @details This represents a file (specifically a mod file, the others will come in time), which consists of a lump of data with records and then the list of groups.
		 */
		struct file{
			char * header;
			unsigned int size;
			unsigned int flags;
			char * ID;
			char * revision;
			char * version;
			char * stuffz;
			std::vector<record> records;
			std::vector<field> fields;
			std::vector<group> groups;
		};
		//temporary file read function, will most likely change in the future
		void readFile(std::ifstream &input, file &File1);
		//very much temporary
		struct record readRecord(std::ifstream &input, record &Record1);
		/**
		 * @brief Checks to see if a record is compressed.
		 * @details Checks the flag on the record to see if the compression flag is set.
		 * @param &recordA
		 * The record that we want to check.
		 * @returns <tt> \b true </tt> if the record is compressed, <tt> \b false </tt> otherwise.
		 */
		bool isCompressed(record &recordA);
		/**
		 * @brief Checks to see if a plugin file is a 'master'.
		 * @details Checks the flag on the plugin file to see if the 'master' flag is set (may be useful, no telling).
		 * @param &fileA
		 * The plugin file that we want to check.
		 * @returns <tt> \b true </tt> if the plugin file is a 'master', <tt> \b false </tt> otherwise.
		 */
		bool isMaster(file &fileA);
		/*inline*/struct file getFile();
		/**
		 * @brief Reads the flags.
		 * @details Reads the flags from various sections based on the length of the flag section.
		 * @param &file
		 * The file that is open.
		 * @returns The flags in a C-String, since their format can change and this enables them to be stored for whatever use.
		 */
		unsigned int readFlags(std::ifstream &file);
		/**
		 * @brief Reads a record name.
		 * @details Reads the record name based on the length for the delimiter.
		 * @param &file
		 * The file that is open.
		 * @returns The record name in a C-String, since their format can change and this enables them to be stored for whatever use.
		 */
		unsigned char * readRecord(std::ifstream &file);
		/**
		 * @brief Reads the data in a record.
		 * @details Reads the record data based on the size section of the record.
		 * @param &file
		 * The file that is open.
		 * @returns The record data in a C-String, since their format can change and this enables them to be stored for whatever use.
		 */
		unsigned char * readRecordData(std::ifstream &file);
		/**
		 * @brief Reads the size of a record.
		 * @details Reads the record size based on the length of the size section.
		 * @param &file
		 * The file that is open.
		 * @returns The record size as an unsigned int.
		 * @note The type of the size may need tweaking in case it changes to support a value larger than an unsigned int can contain.
		 */
		unsigned int readSize(std::ifstream &file);
		std::vector<char *> getMasters(file &File1);
		/**
		 * @brief Gets the length of the delimiter.
		 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 * @returns The length of the delimiter.
		 */
		inline unsigned int getDelimiterLength();
		/**
		 * @brief Gets the length of the flag field.
		 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 * @returns The length of the flag field.
		 */
		inline unsigned int getFlagLength();
		inline unsigned int getIDLength();
		inline unsigned int getRevLength();
		/**
		 * @brief Gets the length of the size field.
		 * @details Is here to allow for cases where the length of the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 * @returns The length of the size field.
		 */
		inline unsigned int getSizeLength();
		inline unsigned int getStuffzLength();
		inline unsigned int getVerLength();
		/**
		 * @brief Sets the length of the delimiter.
		 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setDelimiterLength(unsigned int length);
		/**
		 * @brief Sets the length of the delimiter.
		 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setDelimiterLength2();
		/**
		 * @brief Sets the length of the flag field.
		 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setFlagLength(unsigned int length);
		/**
		 * @brief Sets the length of the flag field.
		 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setFlagLength2();
		inline void setIDLength();
		inline void setRevLength();
		/**
		 * @brief Sets the length of the size field.
		 * @details Is here to allow for cases where the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setSizeLength(unsigned int length);
		/**
		 * @brief Sets the length of the size field.
		 * @details Is here to allow for cases where the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
		 * pass on the new length to properly read the files.
		 */
		inline void setSizeLength2();
		inline void setStuffzLength();
		inline void setVerLength();
//		inline file getFile(){
//			return File;
//		}
		inline unsigned int getDelimiterLength(){
			return delimiterLength;
		}
		inline unsigned int getFlagLength(){
			return flagLength;
		}
		inline unsigned int getIDLength(){
			return IDLength;
		}
		inline unsigned int getRevLength(){
			return revLength;
		}
		inline unsigned int getSizeLength(){
			return sizeLength;
		}
		inline unsigned int getStuffzLength(){
			return stuffzLength;
		}
		inline unsigned int getVerLength(){
			return verLength;
		}
		inline void setDelimiterLength(unsigned int length){
			delimiterLength = length;
		}
		inline void setDelimiterLength2(){
			std::stringstream(common::structVals[common::options::game]["Length"][0]) >> delimiterLength;
		}
		inline void setFlagLength(unsigned int length){
			flagLength = length;
		}
		inline void setFlagLength2(){
			std::stringstream(common::structVals[common::options::game]["FlagLength"][0]) >> flagLength;
		}
		inline void setIDLength(){
			std::stringstream(common::structVals[common::options::game]["IDLength"][0]) >> IDLength;
		}
		inline void setRevLength(){
			std::stringstream(common::structVals[common::options::game]["RevLength"][0]) >> revLength;
		}
		inline void setSizeLength(unsigned int length){
			sizeLength = length;
		}
		inline void setSizeLength2(){
			std::stringstream(common::structVals[common::options::game]["SizeLength"][0]) >> sizeLength;
		}
		inline void setStuffzLength(){
			std::stringstream(common::structVals[common::options::game]["StuffzLength"][0]) >> stuffzLength;
		}
		inline void setVerLength(){
			std::stringstream(common::structVals[common::options::game]["VerLength"][0]) >> verLength;
		}
	}
}
