/*
 * parser.cpp
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
 * @file parser.cpp
 * @brief Contains the parser specific functions.
 * @details The main library of functions that the parser uses to dig through the files that the hex-viewer has generated after picking apart the inputted files.
 */
#include "constants.h"
#include "parser.h"
//#include "fileFormat.h"
std::string parser::fileName;
std::string parser::header;
bool parser::hasValidHeader(char * fileName){
	std::ifstream file(fileName);
	if(file.is_open() && file.good()){
		std::string head;
		file >> head;
		file.close();
		if((isMod(head) && (isESP(fileName) || isESM(fileName))) || (isSave(head) && isESS(fileName)) || isBSA(head))
			return true;
	}
	file.close();
	return false;
}
bool parser::isBSA(std::ifstream &file){
	return (getFileHeader().compare(0, 3, "BSA") == 0);
}
bool parser::isBSA(std::string head){
	return (head.compare(0, 3, "BSA") == 0);
}
bool parser::isBSA(char * head1){
	std::string head(head1);
	return (head.compare(0, 3, "BSA") == 0);
}
bool parser::isBSA2(std::ifstream &file){
	return (getFileHeader().compare(0, common::structVals[common::options::game]["BSAHead"][0].length() + 1, common::structVals[common::options::game]["BSAHead"][0]) == 0);
}
bool parser::isBSA2(std::string head){
	return (head.compare(0, common::structVals[common::options::game]["BSAHead"][0].length() + 1, common::structVals[common::options::game]["BSAHead"][0]) == 0);
}
bool parser::isESM(){
	size_t pos;
	pos = getFileName().find(".esm");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESM(std::string fileName){
	size_t pos;
	pos = fileName.find(".esm");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESM(char * fileName1){
	std::string fileName(fileName1);
	size_t pos;
	pos = fileName.find(".esm");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESM2(){
	size_t pos;
	pos = getFileName().find(common::structVals[common::options::game]["MastExt"][0]);
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESM2(std::string fileName){
	size_t pos;
	pos = fileName.find(common::structVals[common::options::game]["MastExt"][0]);
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
bool parser::isESP(std::string fileName){
	size_t pos;
	pos = fileName.find(".esp");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESP(char * fileName1){
	size_t pos;
	std::string fileName(fileName1);
	pos = fileName.find(".esp");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESP2(){
	size_t pos;
	pos = getFileName().find(common::structVals[common::options::game]["PlugExt"][0]);
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESP2(std::string fileName){
	size_t pos;
	pos = fileName.find(common::structVals[common::options::game]["PlugExt"][0]);
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
bool parser::isESS(std::string fileName){
	size_t pos;
	pos = fileName.find(".ess");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESS(char * fileName1){
	size_t pos;
	std::string fileName(fileName1);
	pos = fileName.find(".ess");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESS2(){
	size_t pos;
	pos = getFileName().find(common::structVals[common::options::game]["SaveExt"][0]);
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isESS2(std::string fileName){
	size_t pos;
	pos = fileName.find(common::structVals[common::options::game]["SaveExt"][0]);
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::isGRUP(std::string data){
	if(isRecord(data))
		return ((data[0] == 'G') && (data[1] == 'R') && (data[2] == 'U') && (data[3] == 'P'));
}
bool parser::isGRUP(char * data1){
	std::string data(data1);
	if(isRecord(data))
		return ((data[0] == 'G') && (data[1] == 'R') && (data[2] == 'U') && (data[3] == 'P'));
}
bool parser::isGRUP2(std::string data){
	if(isRecord(data))
		return (data == common::structVals[common::options::game]["Group"][0]);
}
bool parser::isMod(std::ifstream &file){
	//return ((getFileHeader().compare(0, 4, "TES4") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? (isESM() || isESP()) : false));
	return (((getFileHeader().compare(0, 4, "TES4") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) && ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") != 0) && (getFileHeader().compare(0, 12, "TES4SAVEGAME") != 0))) ? (isESM() || isESP()) : false;
}
bool parser::isMod(std::string head){
	return (((head.compare(0, 4, "TES4") == 0) || (head.compare(0, 4, "TES3") == 0)) && ((head.compare(0, 13, "TESV_SAVEGAME") != 0) && (head.compare(0, 12, "TES4SAVEGAME") != 0))) ? true : false;
}
bool parser::isMod(char * head1){
	std::string head(head1);
	return (((head.compare(0, 4, "TES4") == 0) || (head.compare(0, 4, "TES3") == 0)) && ((head.compare(0, 13, "TESV_SAVEGAME") != 0) && (head.compare(0, 12, "TES4SAVEGAME") != 0))) ? true : false;
}
bool parser::isMod2(std::ifstream &file){
	//return ((getFileHeader().compare(0, 4, "TES4") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? (isESM() || isESP()) : false));
	return ((getFileHeader().compare(0, common::structVals[common::options::game]["PlugHead"][0].length() + 1, common::structVals[common::options::game]["PlugHead"][0]) == 0) && (getFileHeader().compare(0, common::structVals[common::options::game]["SaveHead"][0].length() + 1, common::structVals[common::options::game]["SaveHead"][0]) != 0)) ? (isESM2() || isESP2()) : false;
}
bool parser::isMod2(std::string head){
	return ((head.compare(0, common::structVals[common::options::game]["PlugHead"][0].length() + 1, common::structVals[common::options::game]["PlugHead"][0]) == 0) && (head.compare(0, common::structVals[common::options::game]["SaveHead"][0].length() + 1, common::structVals[common::options::game]["SaveHead"][0]) != 0)) ? true : false;
}
bool parser::isOp(std::string data){
	return (data[0] == '.' && data[1] == '?');
}
bool parser::isOp(char * data1){
	std::string data(data1);
	return (data[0] == '.' && data[1] == '?');
}
bool parser::isRecord(std::string data){
	return ((data[0] >= RANGE_MIN && data[0] <= RANGE_MAX) && (data[1] >= RANGE_MIN && data[1] <= RANGE_MAX) && (data[2] >= RANGE_MIN && data[2] <= RANGE_MAX));
}
bool parser::isRecord(char * data1){
	std::string data(data1);
	return ((data[0] >= RANGE_MIN && data[0] <= RANGE_MAX) && (data[1] >= RANGE_MIN && data[1] <= RANGE_MAX) && (data[2] >= RANGE_MIN && data[2] <= RANGE_MAX));
}
bool parser::isSave(std::ifstream &file){
	//return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? isESS() : false));
	return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) ? isESS() : false;
}
bool parser::isSave(std::string head){
	return ((head.compare(0, 13, "TESV_SAVEGAME") == 0) || (head.compare(0, 12, "TES4SAVEGAME") == 0) || (head.compare(0, 4, "TES3") == 0)) ? true : false;
}
bool parser::isSave(char * head1){
	std::string head(head1);
	return ((head.compare(0, 13, "TESV_SAVEGAME") == 0) || (head.compare(0, 12, "TES4SAVEGAME") == 0) || (head.compare(0, 4, "TES3") == 0)) ? true : false;
}
bool parser::isSave2(std::ifstream &file){
	//return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || ((getFileHeader().compare(0, 4, "TES3") == 0) ? isESS() : false));
	return (getFileHeader().compare(0, common::structVals[common::options::game]["SaveHead"][0].length() + 1, common::structVals[common::options::game]["SaveHead"][0]) == 0) ? isESS2() : false;
}
bool parser::isSave2(std::string head){
	return (head.compare(0, common::structVals[common::options::game]["SaveHead"][0].length() + 1, common::structVals[common::options::game]["SaveHead"][0]) == 0) ? true : false;
}
bool parser::isVar(std::string data){
	return (data[0] == ':' && data[1] == ':');
}
bool parser::isVar(char * data1){
	std::string data(data1);
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
int parser::countLeading(char * line1){
	std::string line(line1);
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
