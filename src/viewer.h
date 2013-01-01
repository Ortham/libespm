/*
 * viewer.h
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
 * @file viewer.h
 * @brief The functions specific to the hex-viewer.
 * @details All the functions that make the extraction of the raw data possible.
 * @note Still a work in progress and currently not very useful.
 */
#ifndef __VIEWER_H__
#define __VIEWER_H__

#include <fstream>
#include <string>
#include "constants.h"
/**
 * @namespace viewer
 * @brief Contains the viewer specific functions.
 * @details Contains all the necessary functions that only the viewer relies on in order to extract the raw data from files.
 * @note Still a work in progress and currently not very useful.
 */
namespace viewer{
	/**
	 * @var size
	 * The size of the file.
	 */
	extern std::ifstream::pos_type size;
	/**
	 * @brief Checks whether or not a piece of data is printable.
	 * @details It checks the hexadecimal value of a piece of data and determines whether or not it is a printable character.
	 * @param data
	 * The piece of data to check.
	 * @returns <tt> \b true </tt> if the piece is a printable character, <tt> \b false </tt> otherwise.
	 */
	bool isPrintable(unsigned int data);
	/**
	 * @brief Gets the size of the file we are currently dealing with.
	 * @details It gives us the file's size so that we can more easily refer to it in various ways.
	 * @returns The file size.
	 */
	inline std::ifstream::pos_type getSize();
	/**
	 * @brief Reads the input file.
	 * @details It reads in all the data from the file and stores it into an array of characters to make messing with the data easier.
	 * @param &file
	 * The file to read in.
	 * @returns A <tt>char</tt> array containing all the data in the file.
	 */
	unsigned char * readFile(std::ifstream &file);
	/**
	 * @brief Reads the input file.
	 * @details It reads in all the data from the file and stores it into an array of characters to make messing with the data easier.
	 * @param &file
	 * The file to read in.
	 * @returns A <tt>char</tt> array containing all the data in the file.
	 */
	unsigned char * readFile2(std::ifstream &file);
	/**
	 * @brief Stores the size of the file we are currently dealing with.
	 * @details It stores the file's size for us so that we can more easily refer to it in various ways.
	 * @param &file
	 * The file we are currently dealing with.
	 */
	inline void setSize(std::ifstream &file);
	/**
	 * @brief The main program
	 * @details Runs the hex-viewer. Included as kind of a ready-made implementation of the API here.
	 * @param inputFile
	 * The name of the file to extract data from.
	 * @param &file
	 * The file stream to use for the input. This is so that we are not making multiple streams and use as few as possible.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 * @deprecated This is here mainly for legacy reasons in case folks have some desire to use a program that spits out 5 very large files.
	 * It is recommended to implement the API instead of using this function unless you \em really want to.
	 */
	void runView(std::string inputFile, std::ifstream &file, std::ofstream &out);
	/**
	 * @brief The main program
	 * @details Runs the hex-viewer. Included as kind of a ready-made implementation of the API here.
	 * @param *inputFile1
	 * The name of the file to extract data from.
	 * @param &file
	 * The file stream to use for the input. This is so that we are not making multiple streams and use as few as possible.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 * @deprecated This is here mainly for legacy reasons in case folks have some desire to use a program that spits out 5 very large files.
	 * It is recommended to implement the API instead of using this function unless you \em really want to.
	 */
	extern "C" void runView(char * inputFile1, std::ifstream &file, std::ofstream &out);
	/**
	 * @brief Writes the character data to a file.
	 * @details Writes the printable character data to a file to allow for analysis later on. This allows us to parse the data later on and turn it into usable information.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writePrintableChar(unsigned char data[], std::ofstream &out);
	/**
	 * @brief Writes the hexadecimal data to a file.
	 * @details Writes the printable hexadecimal data to a file to allow for analysis later on. Not very useful on its own, but useful when combined with the raw data.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writePrintableHex(unsigned char data[], std::ofstream &out);
	/**
	 * @brief Writes the character data to a file
	 * @details Writes the raw character data to a file. Essentially makes a backup copy of the data we are using.
	 * In the case of the data array containing all of and only the data from the file, it backs up the file with the exact data contained.
	 * This is here to ensure that no OS or other functions change the data and to ensure that we have a clean backup copy just in case.
	 * However, in order to use the result as backup, one must manually delete the label and the newlines added in if they added a label in.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeRawChar(unsigned char data[], std::ofstream &out);
	/**
	 * @brief Writes the character data to a file.
	 * @details Writes the raw character data to a file. Essentially makes a backup copy of the data we are using.
	 * In the case of the data array containing all of and only the data from the file, it backs up the file with the exact data contained.
	 * This is here to ensure that no OS or other functions change the data and to ensure that we have a clean backup copy just in case.
	 * However, in order to use the result as backup, one must manually delete the label and the newlines added in if they added a label in.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeRawChar2(unsigned char data[], std::ofstream &out);
	/**
	 * @brief Writes the hexadecimal data to a file.
	 * @details Writes the raw hexadecimal data to a file to allow for analysis later on.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeRawHex(unsigned char data[], std::ofstream &out);
	inline std::ifstream::pos_type getSize(){
		return size;
	}
	inline void setSize(std::ifstream &file){
		size = file.tellg();
	}
	/*END OF LINE*/
}

#endif /* __VIEWER_H__ */
