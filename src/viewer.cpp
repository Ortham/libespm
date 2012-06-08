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
		common::writeLabel("Char data", out);
		writePrintableChar(memblock, out);
		out << std::endl;
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
/*END OF LINE*/
