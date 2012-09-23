/*
 * fileFormat.cpp
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
 * @file fileFormat.cpp
 * @brief Contains functions relating to the file format
 * @details Part of the parser namespace, this contains functions relating to the file format specifically.
 */
#include "fileFormat.h"
bool parser::fileFormat::isCompressed(parser::fileFormat::record &recordA){
	if(((unsigned int)recordA.flags & 0x00040000) == 0x00040000)
		return true;
	return false;
}
unsigned char * parser::fileFormat::readFlags(std::ifstream &file){
	unsigned char * data;
	file.read(data, getFlagLength());
	return data;
}
unsigned char * parser::fileFormat::readRecord(std::ifstream &file){
	unsigned char * data;
	file.read(data, getDelimiterLength());
	return data;
}
unsigned char * parser::fileFormat::readRecordData(std::ifstream &file){
	unsigned char * data;
	file.read(data, size); //size refers to a future storage place for all the stuff we're reading in and I haven't decided on how to handle that yet
	return data;
}
unsigned int parser::fileFormat::readSize(std::ifstream &file){
	unsigned int size = 0;
	for(unsigned int i = 0; i < getSizeLength(); ++i)
		size += file.get();
	return size;
} 
