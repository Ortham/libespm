/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file commonSupport.h
 * @brief Contains the functions common to the viewer and parser.
 * @details A support library to hold functions that both programs use to help with keeping down clutter.
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;
/**
 * @namespace common
 * @brief The common stuff to all programs.
 * @details Contains and will contain all functions and variables that are shared between the parser and viewer to reduce clutter and possibly other stuff.
 */
namespace common{
	/**
	 * @brief Creates the output file names.
	 * @details Takes the base file name and prepends it to the appropriate strings to help identify what the files contain.
	 * @param base
	 * The base file name.
	 * @param addon
	 * The text used to identify the file.
	 * @returns A C-string of the combined form, which is also easily passed to the I/O functions in the standard library.
	 */
	const char* inputName(string base, string addon);
	/**
	 * @brief Counts up the amount of junk in front of the lines.
	 * @details This is to account for excess characters being added to the front of the desired values, and is used to determine how much stuff to delete.
	 * @param line
	 * The line that is desired to have its leading junk counted up and possibly removed.
	 * @returns The number of characters that are junk at the beginning of the line.
	 */
	int countLeading(string line);
	//int numLines(ifstream &in);
	/**
	 * @brief Calls the hex-viewer.
	 * @details Passes the file to the hex-viewer, which then proceeds to grab the hex data and the char data and spits it all out to appropriate files.
	 * @param fileName
	 * The file name to pass to the hex-viewer.
	 * @note The file name must be enclosed in quotes for files that include whitespace in their names.
	 * @bug Call doesn't work on Windows.
	 */
	void callViewer(string fileName);
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
	 * @brief Writes a label to the output file.
	 * @details Writes the label specifying the data type to the output file to make things a bit more clear.
	 * @param label
	 * The label that should be written to the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeLabel(string label, ofstream &out);
	/*END OF LINE*/
}
