/*
 * parserClasses.cpp
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
 * @file parserClasses.cpp
 * @brief Contains the parser specific classes (such as the file we're working with).
 * @details Holds the objects for the parser to work with, such as files, so that we can work with multiple files in a single run and not need to re-run the program.
 * @attention Do not include parser.h if using this file.
 */
#include "parserClasses.h"
bool parser::GameFile::isBSA(std::ifstream &file){
	return (getFileHeader().compare(0, 3, "BSA") == 0);
}
bool parser::GameFile::isESM(){
	size_t pos;
	pos = getFileName().find(".esm");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::GameFile::isESP(){
	size_t pos;
	pos = getFileName().find(".esp");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::GameFile::isESS(){
	size_t pos;
	pos = getFileName().find(".ess");
	if(pos == std::string::npos)
		return false;
	return true;
}
bool parser::GameFile::isMod(std::ifstream &file){
	return (((getFileHeader().compare(0, 4, "TES4") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) && ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") != 0) && (getFileHeader().compare(0, 12, "TES4SAVEGAME") != 0))) ? (isESM() || isESP()) : false;
}
bool parser::GameFile::isMod(std::string head){
	return (((head.compare(0, 4, "TES4") == 0) || (head.compare(0, 4, "TES3") == 0)) && ((head.compare(0, 13, "TESV_SAVEGAME") != 0) && (head.compare(0, 12, "TES4SAVEGAME") != 0))) ? true : false;
}
bool parser::GameFile::isMod(char * head1){
	std::string head(head1);
	return (((head.compare(0, 4, "TES4") == 0) || (head.compare(0, 4, "TES3") == 0)) && ((head.compare(0, 13, "TESV_SAVEGAME") != 0) && (head.compare(0, 12, "TES4SAVEGAME") != 0))) ? true : false;
}
bool parser::GameFile::isSave(std::ifstream &file){
	return ((getFileHeader().compare(0, 13, "TESV_SAVEGAME") == 0) || (getFileHeader().compare(0, 12, "TES4SAVEGAME") == 0) || (getFileHeader().compare(0, 4, "TES3") == 0)) ? isESS() : false;
}
bool parser::GameFile::isSave(std::string head){
	return ((head.compare(0, 13, "TESV_SAVEGAME") == 0) || (head.compare(0, 12, "TES4SAVEGAME") == 0) || (head.compare(0, 4, "TES3") == 0)) ? true : false;
}
bool parser::GameFile::isSave(char * head1){
	std::string head(head1);
	return ((head.compare(0, 13, "TESV_SAVEGAME") == 0) || (head.compare(0, 12, "TES4SAVEGAME") == 0) || (head.compare(0, 4, "TES3") == 0)) ? true : false;
}
/*END OF LINE*/
