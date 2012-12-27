/*
 * commonSupport.h
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
 * @file commonSupport.h
 * @brief Contains the functions common to the viewer and parser.
 * @details A support library to hold functions that both programs use to help with keeping down clutter.
 */
#ifndef __COMMON_SUPPORT_H__
#define __COMMON_SUPPORT_H__

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
/**
 * @namespace common
 * @brief The common stuff to all programs.
 * @details Contains and will contain all functions and variables that are shared between the parser and viewer to reduce clutter and possibly other stuff.
 */
namespace common{
	enum type{
		STRING,
		UNSIGNED_INT,
		SIGNED_INT,
		UNSIGNED_LONG,
		SIGNED_LONG,
		UNSIGNED_SHORT,
		SIGNED_SHORT,
		FLOAT,
		DOUBLE
	};
	/**
	 * @var structVals
	 * The list of the values that can change due to a format change or being game specific.
	 */
	extern std::map<std::string, std::map<std::string, std::vector<std::string> > > structVals;
	/**
	 * @brief Checks to see if a chunk of data is a string.
	 * @details Checks first to see if a chunk of data has a null character at the end, as required for strings, and, if so, checks the rest of the chunk to see if they are
	 * actual printable characters.
	 * @param data[]
	 * The chunk of data to check.
	 * @returns <tt> \b true </tt> if the chunk of data is a string, <tt> \b false </tt> otherwise.
	 */
	bool isString(unsigned char data[]);
	/**
	 * @brief Creates the output file names.
	 * @details Takes the base file name and prepends it to the appropriate strings to help identify what the files contain.
	 * @param base
	 * The base file name.
	 * @param addon
	 * The text used to identify the file.
	 * @returns A C-string of the combined form, which is also easily passed to the I/O functions in the standard library.
	 */
	const char* inputName(std::string base, std::string addon);
	/**
	 * @brief Creates the output file names.
	 * @details Takes the base file name and prepends it to the appropriate strings to help identify what the files contain.
	 * @param *base1
	 * The base file name.
	 * @param *addon1
	 * The text used to identify the file.
	 * @returns A C-string of the combined form, which is also easily passed to the I/O functions in the standard library.
	 */
	extern "C" const char* inputName(char * base1, char * addon1);
	//int numLines(ifstream &in);
	/**
	 * @brief Calls the hex-viewer.
	 * @details Passes the file to the hex-viewer, which then proceeds to grab the hex data and the char data and spits it all out to appropriate files.
	 * @param fileName
	 * The file name to pass to the hex-viewer.
	 * @note The file name must be enclosed in quotes for files that include white-space in their names.
	 * @bug Call doesn't work on Windows.
	 */
	void callViewer(std::string fileName);
	/**
	 * @brief Calls the hex-viewer.
	 * @details Passes the file to the hex-viewer, which then proceeds to grab the hex data and the char data and spits it all out to appropriate files.
	 * @param *fileName1
	 * The file name to pass to the hex-viewer.
	 * @note The file name must be enclosed in quotes for files that include white-space in their names.
	 * @bug Call doesn't work on Windows.
	 */
	extern "C" void callViewer(char * fileName1);
	/**
	 * @brief Erases the junk from the end of a record..
	 * @details It uses the fact that all 4 allowable character have to be an uppercase character to determine if it's a record or a field.
	 * A field is either going to be longer than 4 characters or will not be entirely uppercase characters.
	 * It also accounts for the TIF fields by specifically checking for them, although this can be extended for any format or string in the future.
	 * @param &line
	 * The line to erase the junk from
	 * @todo Extract the fields from the records as well.
	 */
	void eraseTrailing(std::string &line);
	/**
	 * @brief Reads the list of various values into the map.
	 * @details Sets up the list of values that can change from an input file.
	 * @param &file
	 * The file that contains the list of various values per game per value-type.
	 */
	void readOptions(std::ifstream &file);
	/**
	 * @brief Writes a label to the output file.
	 * @details Writes the label specifying the data type to the output file to make things a bit more clear.
	 * @param label
	 * The label that should be written to the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeLabel(std::string label, std::ofstream &out);
	/**
	 * @brief Writes a label to the output file.
	 * @details Writes the label specifying the data type to the output file to make things a bit more clear.
	 * @param *label1
	 * The label that should be written to the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	extern "C" void writeLabel(char * label1, std::ofstream &out);
	/**
	 * @brief Writes out an XML file of the modular values.
	 * @details Takes the map and writes out the contents to an XML file broken up based on game and by the name of the values.
	 * @param &out
	 * The output file to use for the XML generation.
	 */
	void writeXML(std::ofstream &out);
	/**
	 * @brief Associates a string with a value type.
	 * @details Takes a string of a value type and returns the correct enum value for it. This is here for the future table of values so that we can use a saner system instead
	 * of checking each string over and over again.
	 * @param var
	 * The string we want to associate with a value type.
	 * @returns The value type that the string corresponds to.
	 */
	type getType(std::string var);
	namespace options{
		extern std::string game;// = "Default";
		/**
		 * @brief Sets the game name.
		 * @details Sets the game name that we want to use for the configuration file.
		 * @param gameName
		 * The name of the game whose values in the configuration file are going to be used.
		 */
		inline void setGame(std::string gameName){
			game = gameName;
		}
		/**
		 * @brief Gets the game name.
		 * @details Gets the current game name that is being used.
		 * @returns The game name.
		 */
		inline std::string getGame(){
			return game;
		}
		bool contains(std::string opt, std::string val);
	}
	/*END OF LINE*/
}

#endif /* __COMMON_SUPPORT_H__ */
