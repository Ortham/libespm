/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file parser.h
 * @brief Contains the parser specific functions.
 * @details The main library of functions that the parser uses to dig through the files that the hex-viewer has generated after picking apart the inputted files.
 * @def RANGE_MIN
 * The lowest hexidecimal value that is allowed. In this case, the lowest allowed would be 0x41 as that corresponds to the capital letter 'A'
 * and all records consist of 3 or 4 capital letters.
 * @def RANGE_MAX
 * The highest hexidecimal value that is allowed. In this case, the highest allowed would be 0x5A as that corresponds to the capital letter 'Z'
 * and all records consist of 3 or 4 capital letters.
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "fileFormat.h"
#define RANGE_MIN 0x41
#define RANGE_MAX 0x5A
using namespace std;
/**
 * @namespace parser
 * @brief Contains the parser specific functions.
 * @details Contains all the necessary functions that only the parser relies on in order to parse through the files to extract the records from the game files.
 */
namespace parser{
	/**
	 * @var header
	 * The file header.
	 */
	string header;
	/**
	 * @brief Checks whether or not a file is a mod (plugin).
	 * @details It checks the file's header to see if it matches "TES4" or "TES3", which are the current headers for mod files.
	 * @param &file
	 * The file to be checked.
	 * @returns <tt> \b true </tt> if the file is a mod, <tt> \b false </tt> otherwise.
	 */
	bool isMod(ifstream &file);
	/**
	 * @brief Checks whether or not a line is an operation.
	 * @details It checks a line to see if it begins with ".?", which denotes an operation.
	 * @note Only for non-mod files (plugins).
	 * @param data
	 * The line to be checked
	 * @returns <tt> \b true </tt> if the line is an operation, <tt> \b false </tt> otherwise.
	 */
	bool isOp(string data);
	/**
	 * @brief Checks whether or not a line is a record.
	 * @details It checks a line to see if it consists of 3 capital letters in the first 3 positions.
	 * @note Only for mod files (plugins).
	 * @param data
	 * The line to be checked
	 * @returns <tt> \b true </tt> if the line is a record, <tt> \b false </tt> otherwise.
	 * @todo Integrate the rest of the checks from the main program to make this function more complete.
	 */
	bool isRecord(string data);
	/**
	 * @brief Checks whether or not a line is a GRUP.
	 * @details It checks a line to see if it is equal to "GRUP".
	 * @note Only for mod files (plugins).
	 * @param data
	 * The line to be checked
	 * @returns <tt> \b true </tt> if the line is a GRUP, <tt> \b false </tt> otherwise.
	 * @todo Integrate the rest of the checks from the main program to make this function more complete.
	 */
	bool isGRUP(string data);
	/**
	 * @brief Checks whether or not a line is a variable in a saved game.
	 * @details It checks a line to see if it begins with "::", which denotes a variable in a saved game.
	 * @note Only for non-mod files (plugins).
	 * @param data
	 * The line to be checked
	 * @returns <tt> \b true </tt> if the line is a variable, <tt> \b false </tt> otherwise.
	 */
	bool isVar(string data);
	/**
	 * @brief Checks whether or not a file is a BSA.
	 * @details It checks the file's header to see if it matches "BSA", which is the current header for BSAs.
	 * @param &file
	 * The file to be checked.
	 * @returns <tt> \b true </tt> if the file is a BSA, <tt> \b false </tt> otherwise.
	 */
	bool isBSA(ifstream &file);
	/**
	 * @brief Checks whether or not a file is a saved game.
	 * @details It checks the file's header to see if it matches "TESV_SAVEGAME" or "TES4SAVEGAME", which are the current headers for saved game files.
	 * @param &file
	 * The file to be checked.
	 * @returns <tt> \b true </tt> if the file is a saved game, <tt> \b false </tt> otherwise.
	 */
	bool isSave(ifstream &file);
	
	/**
	 * @brief Counts up the amount of junk in front of the lines.
	 * @details This is to account for excess characters being added to the front of the desired values, and is used to determine how much stuff to delete.
	 * @param line
	 * The line that is desired to have its leading junk counted up and possibly removed.
	 * @returns The number of characters that are junk at the beginning of the line.
	 */
	int countLeading(string line);
	/**
	 * @brief Gets the file header.
	 * @details Is here to allow for future compartmentalization.
	 * @returns The file header.
	 * @todo Set things up as an object for each file, possibly, to allow for multiple uses of various functions without causing problems.
	 */
	inline string getFileHeader();
	/**
	 * @brief Sets the file header.
	 * @details Is here to allow for future compartmentalization.
	 * @param &file
	 * The file to extract the header from.
	 */
	inline void setFileHeader(ifstream &file);
	/**
	 * @brief Erases the junk from the beginning of a line.
	 * @details It uses the countLeading(string line) function to determine how much to erase.
	 * @param &line
	 * The line to erase the junk from
	 */
	void eraseLeading(string &line);
	/**
	 * @brief Erases the junk from the end of a record..
	 * @details It uses the fact that all 4 allowable character have to be an uppercase character to determine if it's a record or a field.
	 * A field is either going to be longer than 4 characters or will not be entirely uppercase characters.
	 * It also accounts for the TIF fields by speficially checking for them, although this can be extended for any format or string in the future.
	 * @param &line
	 * The line to erase the junk from
	 * @todo Extract the fields from the records as well.
	 */
	void eraseTrailing(string &line);
	bool isMod(ifstream &file){
		return ((getFileHeader().compare(0, 4, "TES4") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0));
	}
	bool isOp(string data){
		return (data[0] == '.' && data[1] == '?');
	}
	bool isRecord(string data){
		return ((data[0] >= RANGE_MIN && data[0] <= RANGE_MAX) && (data[1] >= RANGE_MIN && data[1] <= RANGE_MAX) && (data[2] >= RANGE_MIN && data[2] <= RANGE_MAX));
	}
	bool isGRUP(string data){
		if(isRecord(data))
			return ((data[0] == 'G') && (data[1] == 'R') && (data[2] == 'U') && (data[3] == 'P'));
	}
	bool isVar(string data){
		return (data[0] == ':' && data[1] == ':');
	}
	bool isBSA(ifstream &file){
		return (getFileHeader().compare(0, 3, "BSA") == 0);
	}
	bool isSave(ifstream &file){
		return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0));
	}
	int countLeading(string line){
		int count = 0;
		for(int i = 0; i < line.size(); ++i){
			if(line[i] >= RANGE_MIN && line[i] <= RANGE_MAX)
				break;
			++count;
		}
		return count;
	}
	inline string getFileHeader(){
		return header;
	}
	inline void setFileHeader(ifstream &file){
		getline(file, header);
	}
	void eraseLeading(string &line){
		int leading = countLeading(line);
		line.erase(0, leading);
	}
	void eraseTrailing(string &line){
		if(line.size() >= 4){
			if(line[3] >= RANGE_MIN && line[3] <= RANGE_MAX)
				line.erase(4, line.size());
			else if(!(line[3] >= 0x61 && line[3] <= 0x7A))
				if(line.compare(0, 5, "TIF__") != 0)
					line.erase(3, line.size());
		}
	}
	/*END OF LINE*/
}
