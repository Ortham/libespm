/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file skyrimParserRedoRel.cpp
 * @brief The main program that manages all the parsing.
 * @details All the parsing of the records is managed here and passes arguments to functions and the 
 * hex-viewer in order to extract all the record information from the EXE and ESMs/ESPs.
 */
#include <iostream>
#include <fstream>
#include <string>
#include "constants.h"
#include "commonSupport.h"
#include "parserClasses.h"
#include "viewer.h"
using namespace std;
/**
 * @brief The main program
 * @param argc
 * The number of parameters passed to the program
 * @param argv[]
 * The file name passed to the program
 * @returns 0 if executed properly.
 */
int main(int argc, char *argv[]){
	if(argc != 2)
		cout << "Usage: " << argv[0] << " <filename>\n";
	else{
		string inputFile = argv[1];
		parser::setFileName(inputFile);
		ifstream file; 
		ofstream out;
		/*START HEX-VIEWER*/
		unsigned char * memblock;
		file.open(inputFile.c_str(), ios::in|ios::binary|ios::ate);
		if(file.is_open()){
			viewer::setSize(file);
			memblock = viewer::readFile(file);
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printable.txt"));
			#else
				out.open(common::inputName(inputFile, " - printable"));
			#endif
			common::writeLabel("Char data", out);
			viewer::writePrintableChar(memblock, out);
			out << endl;
			common::writeLabel("Hex data", out);
			viewer::writePrintableHex(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - rawChar.txt"));
			#else
				out.open(common::inputName(inputFile, " - rawChar"));
			#endif
			common::writeLabel("Raw char data", out);
			viewer::writeRawChar(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - rawHex.txt"));
			#else
				out.open(common::inputName(inputFile, " - rawHex"));
			#endif
			common::writeLabel("Raw hex data", out);
			viewer::writeRawHex(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printableChar.txt"));
			#else
				out.open(common::inputName(inputFile, " - printableChar"));
			#endif
			viewer::writePrintableChar(memblock, out);
			out.close();
			#ifdef __WIN32__
				out.open(common::inputName(inputFile, " - printableHex.txt"));
			#else
				out.open(common::inputName(inputFile, " - printableHex"));
			#endif
			viewer::writePrintableHex(memblock, out);
			out.close();
			file.close();
			delete[] memblock;
		}
		else
			cout << "Unable to open file";
		/*END HEX-VIEWER*/
		string line;
		#ifdef __WIN32__
			file.open(common::inputName(inputFile, " - printableChar.txt"));
		#else
			file.open(common::inputName(inputFile, " - printableChar"));
		#endif
		if(file.is_open()){
			parser::setFileHeader(file);
			line = parser::getFileHeader();
			if(parser::isMod(file)){
				#ifdef __WIN32__
					out.open(common::inputName(inputFile, " - Records.txt"));
				#else
					out.open(common::inputName(inputFile, " - Records"));
				#endif
				parser::eraseLeading(line);
				line.erase(4, line.size());
				out << line << endl;
				while(file.good()){
					getline(file, line);
					parser::eraseLeading(line);
					if(line.size() >= 3){
						if(parser::isRecord(line)){
							parser::eraseTrailing(line);
							out << line << endl;
						}
					}
				}
				file.close();
				out.close();
				#ifdef __WIN32__
					file.open(common::inputName(inputFile, " - Records.txt"));
					out.open(common::inputName(inputFile, " - Extracted Records.txt"));
					
				#else
					file.open(common::inputName(inputFile, " - Records"));
					out.open(common::inputName(inputFile, " - Extracted Records"));
				#endif
				if(file.is_open()){
					while(file.good()){
						getline(file, line);
						if(line.size() <= 4)
							out << line << endl;
					}
					out.close();
				}	
				file.close();
			}
			else if(parser::isSave(file)){
				#ifdef __WIN32__
					out.open(common::inputName(inputFile, " - Variables.txt"));
				#else
					out.open(common::inputName(inputFile, " - Variables"));
				#endif
				if(parser::getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0)
					line.erase(13, line.size());
				else if(parser::getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0)
					line.erase(12, line.size());
				else if(parser::getFileHeader().compare(0, 4, "TES3") == 0)
					line.erase(4, line.size());
				out << line << endl;
				while(file.good()){
					getline(file, line);
					if(parser::isVar(line))
						out << line << endl;
				}
				out.close();
				file.close();
			}
			else{
				#ifdef __WIN32__
					out.open(common::inputName(inputFile, " - Ops.txt"));
					ofstream records(common::inputName(inputFile, " - Records.txt"));
				#else
					out.open(common::inputName(inputFile, " - Ops"));
					ofstream records(common::inputName(inputFile, " - Records"));
					
				#endif
				if(parser::isOp(line)){
					line.erase(0, 2);
					out << line << endl;
				}
				else
					if(line.size() >= 3)
						if(parser::isRecord(line))
							records << line << endl;
				while(file.good()){
					getline(file, line);
					if(parser::isOp(line)){
						line.erase(0, 2);
						out << line << endl;
					}
					else
						if(line.size() >= 3)
							if(parser::isRecord(line))
								records << line << endl;
				}
				out.close();
				records.close();
			}
		}
		file.close();
	}
	/*END OF LINE*/
	return 0;
}
