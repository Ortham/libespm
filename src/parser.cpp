/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file parser.cpp
 * @brief Contains the parser specific functions.
 * @details The main library of functions that the parser uses to dig through the files that the hex-viewer has generated after picking apart the inputted files.
 */
#include "constants.h"
#include "parser.h"
//#include "fileFormat.h"
std::string parser::fileName;
std::string parser::header;
bool parser::isBSA(std::ifstream &file){
	return (getFileHeader().compare(0, 3, "BSA") == 0);
}
bool parser::isESM(){
	size_t pos;
	pos = getFileName().find(".esm");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESP(){
	size_t pos;
	pos = getFileName().find(".esp");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESS(){
	size_t pos;
	pos = getFileName().find(".ess");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isGRUP(std::string data){
	if(isRecord(data))
		return ((data[0] == 'G') && (data[1] == 'R') && (data[2] == 'U') && (data[3] == 'P'));
}
bool parser::isMod(std::ifstream &file){
	//return ((getFileHeader().compare(0, 4, "TES4") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? (isESM() || isESP()) : false));
	return (((getFileHeader().compare(0, 4, "TES4") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) && ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") != 0) && (getFileHeader().compare(0, 12, "TES4SAVEGAME") != 0))) ? (isESM() || isESP()) : false;
}
bool parser::isOp(std::string data){
	return (data[0] == '.' && data[1] == '?');
}
bool parser::isRecord(std::string data){
	return ((data[0] >= RANGE_MIN && data[0] <= RANGE_MAX) && (data[1] >= RANGE_MIN && data[1] <= RANGE_MAX) && (data[2] >= RANGE_MIN && data[2] <= RANGE_MAX));
}
bool parser::isSave(std::ifstream &file){
	//return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? isESS() : false));
	return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) ? isESS() : false;
}
bool parser::isVar(std::string data){
	return (data[0] == ':' && data[1] == ':');
}
int parser::countLeading(std::string line){
	int count = 0;
	for(int i = 0; i < line.size(); ++i){
		if(line[i] >= RANGE_MIN && line[i] <= RANGE_MAX)
			break;
		++count;
	}
	return count;
}
void parser::eraseLeading(std::string &line){
	int leading = countLeading(line);
	line.erase(0, leading);
}
void parser::eraseTrailing(std::string &line){
	if(line.size() >= 4){
		if(line[3] >= RANGE_MIN && line[3] <= RANGE_MAX)
			line.erase(4, line.size());
		else if(!(line[3] >= 0x61 && line[3] <= 0x7A))
			if(line.compare(0, 5, "TIF__") != 0)
				line.erase(3, line.size());
	}
}
/*END OF LINE*/
