/*
 * fileFormat.cpp
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
 * @file fileFormat.cpp
 * @brief Contains functions relating to the file format
 * @details Part of the parser namespace, this contains functions relating to the file format specifically.
 */
#include "fileFormat.h"
#include <cstdlib>
#include <cstring>
#include <zlib.h>
struct parser::fileFormat::field Field;
struct parser::fileFormat::record Record;
struct parser::fileFormat::group Group;
struct parser::fileFormat::file File;
unsigned int parser::fileFormat::delimiterLength;
unsigned int parser::fileFormat::flagLength;
unsigned int parser::fileFormat::IDLength;
unsigned int parser::fileFormat::revLength;
unsigned int parser::fileFormat::sizeLength;
unsigned int parser::fileFormat::stuffzLength;
unsigned int parser::fileFormat::verLength;
void parser::fileFormat::readFile(std::ifstream &input, parser::fileFormat::file &File1){
	unsigned int count = 0;
	setDelimiterLength2();
	setFlagLength2();
	setIDLength();
	setSizeLength2();
	setVerLength();
	setRevLength();
	setStuffzLength();
	File1.header = new char[getDelimiterLength()];
	File1.size = 0;
	File1.flags = 0;
	File1.ID = new char[getIDLength()];
	File1.revision = new char[getRevLength()];
	File1.version = new char[getVerLength()];
	File1.stuffz = new char[getStuffzLength()];
	input.read(File1.header, getDelimiterLength());
	input.read((char *)&(File1.size), getDelimiterLength());
	//count += input.gcount();
	input.read((char *)&(File1.flags), getFlagLength());
	//count += input.gcount();
	input.read(File1.ID, getIDLength());
	//count += input.gcount();
	input.read(File1.revision, getRevLength());
	//count += input.gcount();
	input.read(File1.version, getVerLength());
	//count += input.gcount();
	input.read(File1.stuffz, getStuffzLength());
	//count += input.gcount();
	while(count < File1.size){
		Field.name = new char[getDelimiterLength()];
		Field.size = 0;
		input.read(Field.name, getDelimiterLength());
		count += input.gcount();
		input.read((char*)&(Field.size), getSizeLength());
		count += input.gcount();
		Field.data = new char[Field.size];
		count += input.gcount();
		input.read(Field.data, Field.size);
		count += input.gcount();
		File1.fields.push_back(Field);
	}
}
//This function needs work, but it'll basically be for reading the record. The return type may or may not stay, I haven't decided yet. I'll worry about that once I
//have the group stuff figured out. Next step is to figure out the compressed size.
struct parser::fileFormat::record parser::fileFormat::readRecord(std::ifstream &input, parser::fileFormat::record &Record1){
	unsigned int count = 0;
	Record1.recName = new char[getDelimiterLength()];
	Record1.size = 0;
	Record1.flags = 0;
	Record1.recID = new char[getIDLength()];
	Record1.revision = new char[getRevLength()];
	Record1.version = new char[getVerLength()];
	Record1.stuffz = new char[getStuffzLength()];
	Record1.decompSize = 0;
	input.read(Record1.recName, getDelimiterLength());
	input.read((char *)&(Record1.size), getDelimiterLength());
	count += input.gcount();
	input.read((char *)&(Record1.flags), getFlagLength());
	count += input.gcount();
	input.read(Record1.recID, getIDLength());
	count += input.gcount();
	input.read(Record1.revision, getRevLength());
	count += input.gcount();
	input.read(Record1.version, getVerLength());
	count += input.gcount();
	input.read(Record1.stuffz, getStuffzLength());
	count += input.gcount();
	if(isCompressed(Record1)){
		//read in compressed data and uncompress
	}
	while(count < Record1.size){
		Field.name = new char[getDelimiterLength()];
		Field.size = 0;
		input.read(Field.name, getDelimiterLength());
		input.read((char*)&(Field.size), getSizeLength());
		count += input.gcount();
		Field.data = new char[Field.size];
		input.read(Field.data, Field.size);
		count += input.gcount();
		Record1.fields.push_back(Field);
	}
	return Record1;
}
bool parser::fileFormat::isCompressed(parser::fileFormat::record &recordA){
	//if(((unsigned int)recordA.flags & 0x00040000) == 0x00040000)
	if((recordA.flags & strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
bool parser::fileFormat::isMaster(parser::fileFormat::file &fileA){
	//if(((unsigned int)fileA.flags & 0x00000001) == 0x00000001)
	if((fileA.flags & strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
unsigned int parser::fileFormat::readFlags(std::ifstream &file){
	unsigned int data;
	file.read((char *)&data, getFlagLength());
	return data;
}
unsigned char * parser::fileFormat::readRecord(std::ifstream &file){
	unsigned char * data;
	file.read((char *)data, getDelimiterLength());
	return data;
}
unsigned char * parser::fileFormat::readRecordData(std::ifstream &file){
	unsigned char * data;
	//file.read(data, size); //size refers to a future storage place for all the stuff we're reading in and I haven't decided on how to handle that yet
	return data;
}
unsigned int parser::fileFormat::readSize(std::ifstream &file){
	unsigned int size = 0;
	for(unsigned int i = 0; i < getSizeLength(); ++i)
		size += file.get();
	return size;
} 
//struct parser::fileFormat::file getFile(){
//	struct file File1;
//	return File1;
//}
std::vector<char *> parser::fileFormat::getMasters(parser::fileFormat::file &File1){
	std::vector<char *> masters;
	for(int i = 0; i < File1.fields.size(); ++i)
		if(strncmp("MAST", File1.fields[i].name, 4) == 0)
			masters.push_back(File1.fields[i].data);
	return masters;
}
