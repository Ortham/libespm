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
#include <cstdlib>
#include <vector>
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
	 * @brief Writes a label to the output file.
	 * @details Writes the label specifying the data type to the output file to make things a bit more clear.
	 * @param label
	 * The label that should be written to the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeLabel(string label, ofstream &out);
	const char* inputName(string base, string addon){
		//cout << base << endl;
		//cout << addon << endl;
		//cout << base + addon << endl;
		return (base + addon).c_str();
	}
	/*int numLines(ifstream &in){
		///in.seekg(0, ios::beg);
		string line;
		vector<string> lines;
		while(getline(in, line))
			lines.push_back(line);
		//in.seekg(0, ios::beg);
		return lines.size();
	}*/
	void callViewer(string fileName){
		fileName = "\"" + fileName + "\"";
		#ifdef __WIN32__
			const char * callName = (".\\Viewer.exe " + fileName).c_str();
			system(callName);
		#else
			const char * callName = ("./Viewer " + fileName).c_str();
			system(callName);
		#endif
	}
	void writeLabel(string label, ofstream &out){
		out << label << endl;
		string labelLine;
		for(unsigned int i = 0; i < label.size(); ++i)
			labelLine = labelLine + "-";
		out << labelLine << endl;
		out << endl;
		out << endl;
	}
	/*END OF LINE*/
}
