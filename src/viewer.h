/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file viewer.h
 * @brief The functions specific to the hex-viewer.
 * @details All the functions that make the extraction of the raw data possible.
 * @note Still a work in progress and currently not very useful.
 * @def PRINT_START
 * The lowest hexidecimal value that corresponds with a printable character.
 * @def PRINT_END
 * The highest hexidecimal value that corresponds with a printable character.
 */
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "commonSupport.h"
#define PRINT_START 0x20
#define PRINT_END 0x7E
using namespace std;
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
	ifstream::pos_type size;
	/**
	 * @brief Checks whether or not a piece of data is printable.
	 * @details It checks the hexidecimal value of a piece of data and determines whether or not it is a printable character.
	 * @param data
	 * The piece of data to check.
	 * @returns <tt> \b true </tt> if the piece is a printable character, <tt> \b false </tt> otherwise.
	 */
	bool isPrintable(unsigned int data);
	/**
	 * @brief Reads the input file.
	 * @details It reads in all the data from the file and stores it into an array of characters to make messing with the data easier.
	 * @param &file
	 * The file to read in.
	 * @returns A <tt>char</tt> array containing all the data in the file.
	 */
	unsigned char * readFile(ifstream &file);
	/**
	 * @brief Gets the size of the file we are currently dealing with.
	 * @details It gives us the file's size so that we can more easily refer to it in various ways.
	 * @returns The file size.
	 */
	inline ifstream::pos_type getSize();
	/**
	 * @brief Stores the size of the file we are currently dealing with.
	 * @details It stores the file's size for us so that we can more easily refer to it in various ways.
	 * @param &file
	 * The file we are currently dealing with.
	 */
	inline void setSize(ifstream &file);
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
	void runView(string inputFile, ifstream &file, ofstream &out);
	/**
	 * @brief Writes the hexidecimal data to a file.
	 * @details Writes the printable hexidecimal data to a file to allow for analysis later on. Not very useful on its own, but useful when combined with the raw data.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writePrintableHex(unsigned char data[], ofstream &out);
	/**
	 * @brief Writes the character data to a file.
	 * @details Writes the printable character data to a file to allow for analysis later on. This allows us to parse the data later on and turn it into usable information.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writePrintableChar(unsigned char data[], ofstream &out);
	/**
	 * @brief Writes the hexidecimal data to a file.
	 * @details Writes the raw hexidecimal data to a file to allow for analysis later on.
	 * @param data[]
	 * The data contained in the file.
	 * @param &out
	 * The file stream to use for the output. This is so that we are not making multiple streams and use as few as possible.
	 */
	void writeRawHex(unsigned char data[], ofstream &out);
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
	void writeRawChar(unsigned char data[], ofstream &out);
	bool isPrintable(unsigned int data){
		return ((PRINT_START <= data) && (PRINT_END >= data));
	}	
	unsigned char * readFile(ifstream &file){
		unsigned char * data = new unsigned char[getSize()];
		file.seekg(0, ios::beg);
		for(unsigned int i = 0; i < getSize(); ++i)
			file >> data[i];
		//file.read(data, getSize()); //Read function for binary files. Will need to test this against the old algorithm and compare the results.
		return data;
	}
	inline ifstream::pos_type getSize(){
		return size;
	}
	inline void setSize(ifstream &file){
		size = file.tellg();
	}
	void runView(string inputFile, ifstream &file, ofstream &out){
		unsigned char * memblock;
		file.open(inputFile.c_str(), ios::in|ios::binary|ios::ate);
		if(file.is_open()){
			setSize(file);
			memblock = readFile(file);
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printable.txt"));
			#else
				out.open(common::inputName(inputFile, " - printable"));
			#endif
			common::writeLabel("Char data", out);
			writePrintableChar(memblock, out);
			out << endl;
			common::writeLabel("Hex data", out);
			writePrintableHex(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - rawChar.txt"));
			#else
				out.open(common::inputName(inputFile, " - rawChar"));
			#endif
			common::writeLabel("Raw char data", out);
			writeRawChar(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - rawHex.txt"));
			#else
				out.open(common::inputName(inputFile, " - rawHex"));
			#endif
			common::writeLabel("Raw hex data", out);
			writeRawHex(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printableChar.txt"));
			#else
				out.open(common::inputName(inputFile, " - printableChar"));
			#endif
			writePrintableChar(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printableHex.txt"));
			#else
				out.open(common::inputName(inputFile, " - printableHex"));
			#endif
			writePrintableHex(memblock, out);
			out.close();
			file.close();
			delete[] memblock;
		}
		else
			cout << "Unable to open file";
	}
	void writePrintableHex(unsigned char data[], ofstream &out){
		bool flag = false;
		for(unsigned int i = 0; i < getSize(); ++i){
			if(isPrintable((unsigned int)data[i])){
				out << hex << (unsigned int)data[i] << "\t";
				flag = true;
			}
			else{
				if(flag){
					out << endl;
					flag = false;
				}
			}
		}
	}
	void writePrintableChar(unsigned char data[], ofstream &out){
		bool flag = false;
		for(unsigned int i = 0; i < getSize(); ++i){
			if(isPrintable((unsigned int)data[i])){
				out << data[i];
				flag = true;
			}
			else{
				if(flag){
					out << endl;
					flag = false;
				}
			}
		}
	}
	void writeRawHex(unsigned char data[], ofstream &out){
		for(unsigned int i = 0; i < getSize(); ++i)
			out << hex << (unsigned int)data[i] << "\t";
	}
	void writeRawChar(unsigned char data[], ofstream &out){
		for(unsigned int i = 0; i < getSize(); ++i)
			out << data[i];
		//out.write(data, getSize()); //Write function for binary files. Will need to test this against the old algorithm and compare the results.
	}
	/*END OF LINE*/
}
