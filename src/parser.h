/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file parser.h
 * @brief Contains the parser specific functions.
 * @details The main library of functions that the parser uses to dig through the files that the hex-viewer has generated after picking apart the inputted files.
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
//#include "fileFormat.h"
using namespace std;
/**
 * @namespace parser
 * @brief Contains the parser specific functions.
 * @details Contains all the necessary functions that only the parser relies on in order to parse through the files to extract the records from the game files.
 */
namespace parser{
	/**
	 * @var fileName
	 * The file name.
	 */
	extern string fileName;
	/**
	 * @var header
	 * The file header.
	 */
	extern string header;
	/**
	 * @brief Checks whether or not a file is a BSA.
	 * @details It checks the file's header to see if it matches "BSA", which is the current header for BSAs.
	 * @param &file
	 * The file to be checked.
	 * @returns <tt> \b true </tt> if the file is a BSA, <tt> \b false </tt> otherwise.
	 */
	bool isBSA(ifstream &file);
	/**
	 * @brief Checks whether or not a file has the ESM extension;
	 * @details It checks the file's extension to see if it matches ".esm"
	 * @returns <tt> \b true </tt> if the file has the ESM extension, <tt> \b false </tt> otherwise.
	 */
	bool isESM();
	/**
	 * @brief Checks whether or not a file has the ESP extension;
	 * @details It checks the file's extension to see if it matches ".esp"
	 * @returns <tt> \b true </tt> if the file has the ESP extension, <tt> \b false </tt> otherwise.
	 */
	bool isESP();
	/**
	 * @brief Checks whether or not a file has the ESS extension;
	 * @details It checks the file's extension to see if it matches ".ess"
	 * @returns <tt> \b true </tt> if the file has the ESS extension, <tt> \b false </tt> otherwise.
	 */
	bool isESS();
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
	 * @brief Checks whether or not a file is a saved game.
	 * @details It checks the file's header to see if it matches "TESV_SAVEGAME" or "TES4SAVEGAME", which are the current headers for saved game files.
	 * @param &file
	 * The file to be checked.
	 * @returns <tt> \b true </tt> if the file is a saved game, <tt> \b false </tt> otherwise.
	 */
	bool isSave(ifstream &file);
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
	 * @brief Gets the file name.
	 * @details Is here to allow for future compartmentalization.
	 * @returns The file name.
	 * @todo Set things up as an object for each file, possibly, to allow for multiple uses of various functions without causing problems.
	 */
	inline string getFileName();
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
	/**
	 * @brief Sets the file header.
	 * @details Is here to allow for future compartmentalization.
	 * @param &file
	 * The file to extract the header from.
	 */
	inline void setFileHeader(ifstream &file);
	/**
	 * @brief Sets the file name.
	 * @details Is here to allow for future compartmentalization.
	 * @param name
	 * The name of the file to store.
	 */
	inline void setFileName(string name);
	inline string getFileHeader(){
		return header;
	}
	inline string getFileName(){
		return fileName;
	}
	inline void setFileHeader(ifstream &file){
		getline(file, header);
	}
	inline void setFileName(string name){
		fileName = name;
	}
	/*END OF LINE*/
}
