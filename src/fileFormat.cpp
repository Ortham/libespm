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
#include "parser.h"
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
	///@todo Split this stuff out in a separate function.
	//The readFile function should really only contain the routines to call the functions that will read in the
	//actual data. It is not practical to assume that the whole file is desired to be read in to memory every single time.
	//So, we'll have a function to read in the header thing and then a loop to read in groups until the file is completely in memory.
	//The function to read the groups will read in records along with sub-groups. If the start corresponds to another group, then we repeat the operation until we hit
	//record.
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
	input.read((char *)&(File1.flags), getFlagLength());
	input.read(File1.ID, getIDLength());
	input.read(File1.revision, getRevLength());
	input.read(File1.version, getVerLength());
	input.read(File1.stuffz, getStuffzLength());
	while(count < File1.size){
		Field.name = new char[getDelimiterLength()];
		Field.size = 0;
		input.read(Field.name, getDelimiterLength());
		count += input.gcount();
		input.read((char*)&(Field.size), getSizeLength());
		count += input.gcount();
		Field.data = new char[Field.size];
		input.read(Field.data, Field.size);
		count += input.gcount();
		File1.fields.push_back(Field);
	}
}
//This function needs work, but it'll basically be for reading the record. The return type may or may not stay, I haven't decided yet. I'll worry about that once I
//have the group stuff figured out. Next step is to figure out the compressed size.
struct parser::fileFormat::record parser::fileFormat::readRecord(std::ifstream &input, parser::fileFormat::record &Record1){
	///@todo finish up the compression handling and have two methods of reading it in until some efficiency analysis is done. One method for the compressed record
	///and one for the non-compressed record. For the latter, we could just read in the data straight from the file. For the former, we'll need to do some work before we assign
	///the uncompressed data.
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
	input.read((char *)&(Record1.flags), getFlagLength());
	input.read(Record1.recID, getIDLength());
	input.read(Record1.revision, getRevLength());
	input.read(Record1.version, getVerLength());
	input.read(Record1.stuffz, getStuffzLength());
	if(isCompressed(Record1)){
		//read in compressed data and uncompress
		/*For the compressed stuff, the size of the record is the number of bytes for meat after you get through all the information stuff like flags.
		 *That's the raw size, not the actual size. The actual size is contained in the 4 bytes (which are counted in the raw size) after the informational stuff. 
		 *That's the uncompressed size of the data.*/
	}
	while(count < Record1.size){
		Field.name = new char[getDelimiterLength()];
		Field.size = 0;
		input.read(Field.name, getDelimiterLength());
		count += input.gcount();
		input.read((char*)&(Field.size), getSizeLength());
		count += input.gcount();
		Field.data = new char[Field.size];
		input.read(Field.data, Field.size);
		count += input.gcount();
		Record1.fields.push_back(Field);
	}
	return Record1;
}
unsigned int parser::fileFormat::readRecord(std::ifstream &input, parser::fileFormat::record &Record1){
	///@todo finish up the compression handling and have two methods of reading it in until some efficiency analysis is done. One method for the compressed record
	///and one for the non-compressed record. For the latter, we could just read in the data straight from the file. For the former, we'll need to do some work before we assign
	///the uncompressed data.
	unsigned int count = 0;
	unsigned int totalCount = 0;
	Record1.recName = new char[getDelimiterLength()];
	Record1.size = 0;
	Record1.flags = 0;
	Record1.recID = new char[getIDLength()];
	Record1.revision = new char[getRevLength()];
	Record1.version = new char[getVerLength()];
	Record1.stuffz = new char[getStuffzLength()];
	Record1.decompSize = 0;
	input.read(Record1.recName, getDelimiterLength());
	totalCount += input.gcount();
	input.read((char *)&(Record1.size), getDelimiterLength());
	totalCount += input.gcount();
	input.read((char *)&(Record1.flags), getFlagLength());
	totalCount += input.gcount();
	input.read(Record1.recID, getIDLength());
	totalCount += input.gcount();
	input.read(Record1.revision, getRevLength());
	totalCount += input.gcount();
	input.read(Record1.version, getVerLength());
	totalCount += input.gcount();
	input.read(Record1.stuffz, getStuffzLength());
	totalCount += input.gcount();
	if(isCompressed(Record1)){
		//read in compressed data and uncompress
		/*For the compressed stuff, the size of the record is the number of bytes for meat after you get through all the information stuff like flags.
		 *That's the raw size, not the actual size. The actual size is contained in the 4 bytes (which are counted in the raw size) after the informational stuff. 
		 *That's the uncompressed size of the data.*/
	}
	while(count < Record1.size){
		Field.name = new char[getDelimiterLength()];
		Field.size = 0;
		input.read(Field.name, getDelimiterLength());
		count += input.gcount();
		input.read((char*)&(Field.size), getSizeLength());
		count += input.gcount();
		Field.data = new char[Field.size];
		input.read(Field.data, Field.size);
		count += input.gcount();
		Record1.fields.push_back(Field);
	}
	totalCount += count;
	return totalCount;
}
unsigned int parser::fileFormat::readGroup(std::ifstream &input, parser::fileFormat::group &Group1){
	unsigned int count = 0;
	Group1.groupHeader = new char[getDelimiterLength()];
	Group1.groupSize = 0;
	Group1.groupName = new char[getDelimiterLength()];
	Group1.type = new char[getDelimiterLength()];
	Group1.stamp = new char[2];
	Group1.stuffz1 = new char[2];
	Group1.version = new char[getVerLength()];
	Group1.stuffz2 = new char[2];
	//TODO replace the above with appropriate functions and change the use of DelmiterLength for just delimiters
	input.read(Group1.groupHeader, getDelimiterLength());
	count += input.gcount();
	input.read((char*)&Group1.groupSize, getDelimiterLength());
	count += input.gcount();
	input.read(Group1.groupName, getDelimiterLength());
	count += input.gcount();
	input.read(Group1.type, getDelimiterLength());
	count += input.gcount();
	input.read(Group1.stamp, 2);
	count += input.gcount();
	input.read(Group1.stuffz1, 2);
	count += input.gcount();
	input.read(Group1.version, getVerLength());
	count += input.gcount();
	input.read(Group1.stuffz2, 2);
	count += input.gcount();
	//read in pre-meat stuffz
	//count += groupStuffz; //this is due to the groupSize being the size of the entire block, will need a secondary counter for records to return back to this function
	char * temp;
	temp = new char[4];
	while(count < Group1.groupSize){
		input.read(temp, getDelimiterLength());
		if(parser::isGRUP(temp)){ //will probably need to change this so that we don't have a dependency on parser.h/parser.cpp; may not change it, we'll see
			input.unget();
			input.unget();
			input.unget();
			input.unget();
			struct group groupNew; //or something;
			count += readGroup(input, groupNew);
			Group1.groups.push_back(groupNew);
		}
		else{
			input.unget();
			input.unget();
			input.unget();
			input.unget();
			struct record recordNew;
			count += readRecord(input, recordNew); //or something
			Group1.records.push_back(recordNew);
		}
	}
	return count;
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
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
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
