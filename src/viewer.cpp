/*
 * viewer.cpp
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
 * @file viewer.cpp
 * @brief The functions specific to the hex-viewer.
 * @details All the functions that make the extraction of the raw data possible.
 * @note Still a work in progress and currently not very useful.
 */
#include "constants.h"
#include "viewer.h"
#include "commonSupport.h"
std::ifstream::pos_type viewer::size;
bool viewer::isPrintable(unsigned int data){
	return ((PRINT_START <= data) && (PRINT_END >= data));
}
unsigned char * viewer::readFile(std::ifstream &file){
	unsigned char * data = new unsigned char[getSize()];
	file.seekg(0, std::ios::beg);
	for(unsigned int i = 0; i < getSize(); ++i)
		file >> data[i];
	//file.read(data, getSize()); //Read function for binary files. Will need to test this against the old algorithm and compare the results.
	return data;
}
unsigned char * viewer::readFile2(std::ifstream &file){
	unsigned char * data = new unsigned char[getSize()];
	file.seekg(0, std::ios::beg);
	file.read((char *)data, getSize());
	return data;
}
void viewer::runView(std::string inputFile, std::ifstream &file, std::ofstream &out){
	unsigned char * memblock;
	file.open(inputFile.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if(file.is_open()){
		setSize(file);
		memblock = readFile(file);
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - printable.txt"));
		#else
			out.open(common::inputName(inputFile, " - printable"));
		#endif
		common::writeLabel((std::string)"Char data", out);
		writePrintableChar(memblock, out);
		out << std::endl;
		common::writeLabel((std::string)"Hex data", out);
		writePrintableHex(memblock, out);
		out.close();
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - rawChar.txt"));
		#else
			out.open(common::inputName(inputFile, " - rawChar"));
		#endif
		common::writeLabel((std::string)"Raw char data", out);
		writeRawChar(memblock, out);
		out.close();
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - rawHex.txt"));
		#else
			out.open(common::inputName(inputFile, " - rawHex"));
		#endif
		common::writeLabel((std::string)"Raw hex data", out);
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
		std::cout << "Unable to open file" << std::endl;
}
void viewer::runView(char * inputFile1, std::ifstream &file, std::ofstream &out){
	std::string inputFile(inputFile1);
	unsigned char * memblock;
	file.open(inputFile.c_str(), std::ios::in|std::ios::binary|std::ios::ate);
	if(file.is_open()){
		setSize(file);
		memblock = readFile(file);
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - printable.txt"));
		#else
			out.open(common::inputName(inputFile, " - printable"));
		#endif
		common::writeLabel((std::string)"Char data", out);
		writePrintableChar(memblock, out);
		out << std::endl;
		common::writeLabel((std::string)"Hex data", out);
		writePrintableHex(memblock, out);
		out.close();
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - rawChar.txt"));
		#else
			out.open(common::inputName(inputFile, " - rawChar"));
		#endif
		common::writeLabel((std::string)"Raw char data", out);
		writeRawChar(memblock, out);
		out.close();
		#ifdef __WIN32__
			out.open(common::inputName(inputFile, " - rawHex.txt"));
		#else
			out.open(common::inputName(inputFile, " - rawHex"));
		#endif
		common::writeLabel((std::string)"Raw hex data", out);
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
		std::cout << "Unable to open file" << std::endl;
}
void viewer::writePrintableHex(unsigned char data[], std::ofstream &out){
	bool flag = false;
	for(unsigned int i = 0; i < getSize(); ++i){
		if(isPrintable(static_cast<unsigned int>(data[i]))){
			out << std::hex << static_cast<unsigned int>(data[i]) << "\t";
			flag = true;
		}
		else{
			if(flag){
				out << std::endl;
				flag = false;
			}
		}
	}
}
void viewer::writePrintableChar(unsigned char data[], std::ofstream &out){
	bool flag = false;
	for(unsigned int i = 0; i < getSize(); ++i){
		if(isPrintable(static_cast<unsigned int>(data[i]))){
			out << data[i];
			flag = true;
		}
		else{
			if(flag){
				out << std::endl;
				flag = false;
			}
		}
	}
}
void viewer::writeRawHex(unsigned char data[], std::ofstream &out){
	for(unsigned int i = 0; i < getSize(); ++i)
		out << std::hex << static_cast<unsigned int>(data[i]) << "\t";
}
void viewer::writeRawChar(unsigned char data[], std::ofstream &out){
	for(unsigned int i = 0; i < getSize(); ++i)
		out << data[i];
	//out.write(data, getSize()); //Write function for binary files. Will need to test this against the old algorithm and compare the results.
}
void viewer::writeRawChar2(unsigned char data[], std::ofstream &out){
	out.write((char *)data, getSize());
}
/*END OF LINE*/
