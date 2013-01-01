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
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <zlib.h>
#include "fileFormat.h"
#include "parser.h"
struct parser::fileFormat::field Field; //This may go away in favor of using it only in the functions, but we shall see
unsigned int parser::fileFormat::delimiterLength;
unsigned int parser::fileFormat::flagLength;
unsigned int parser::fileFormat::groupStampLength;
unsigned int parser::fileFormat::groupTypeLength;
unsigned int parser::fileFormat::IDLength;
unsigned int parser::fileFormat::revLength;
unsigned int parser::fileFormat::decompSizeLength;
unsigned int parser::fileFormat::fieldSizeLength;
unsigned int parser::fileFormat::groupSizeLength;
unsigned int parser::fileFormat::headSizeLength;
unsigned int parser::fileFormat::recSizeLength;
unsigned int parser::fileFormat::stuffzLength;
unsigned int parser::fileFormat::verLength;
bool parser::fileFormat::getRecordByFieldD(parser::fileFormat::item &Item, char * fieldName, char * fieldData, parser::fileFormat::item &Record, unsigned int length){
	for(unsigned int i = 0; i < Item.group.items.size(); ++i){
		if(Item.group.items[i].type == RECORD){
			for(unsigned int j = 0; j < Item.group.items[i].record.fields.size(); ++j){
				if(strncmp(fieldName, Item.group.items[i].record.fields[j].name, getDelimiterLength()) == 0 && length == Item.group.items[i].record.fields[j].size && strncmp(fieldData, Item.group.items[i].record.fields[j].data, length) == 0){
					Record = Item.group.items[i];
					return true;
				}
			}
		}
		getRecordByFieldD(Item.group.items[i], fieldName, fieldData, Record, length);
	}
	return false;
}
bool parser::fileFormat::isCompressed(parser::fileFormat::item &Record){
	//if(((unsigned int)recordA.flags & 0x00040000) == 0x00040000)
	if((Record.record.flags & strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
bool parser::fileFormat::isMaster(parser::fileFormat::file &File){
	//if(((unsigned int)fileA.flags & 0x00000001) == 0x00000001)
	if((File.flags & strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
parser::fileFormat::item parser::fileFormat::getRecordByFieldD(parser::fileFormat::file &File, char * fieldName, char * fieldData, unsigned int length){
	item Record;
	for(unsigned int i = 0; i < File.items.size(); ++i){
		if(getRecordByFieldD(File.items[i], fieldName, fieldData, Record, length))
			return Record;
	}
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned char * parser::fileFormat::readRecord(std::ifstream &file){
	unsigned char * data;
	file.read((char *)data, getDelimiterLength());
	return data;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned char * parser::fileFormat::readRecordData(std::ifstream &file){
	unsigned char * data;
	//file.read(data, size); //size refers to a future storage place for all the stuff we're reading in and I haven't decided on how to handle that yet
	return data;
}
unsigned int parser::fileFormat::readField(std::ifstream &input, parser::fileFormat::field &Field1){
	Field1.name = new char[getDelimiterLength()];
	Field1.size = 0;
	unsigned int count = 0;
	input.read(Field1.name, getDelimiterLength());
	count += getDelimiterLength();
	//input.read((char*)&(Field1.size), getFieldSizeLength());
	input.read(reinterpret_cast<char*>(&Field1.size), getFieldSizeLength());
	count += getFieldSizeLength();
	Field1.data = new char[Field1.size];
	input.read(Field1.data, Field1.size);
	count += Field1.size;
	return count;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned int parser::fileFormat::readFlags(std::ifstream &file){
	unsigned int data;
	file.read((char *)&data, getFlagLength());
	return data;
}
unsigned int parser::fileFormat::readGroup(std::ifstream &input, parser::fileFormat::item &Group){
	unsigned int count = 0;
	Group.type = GROUP;
	Group.group.groupHeader = new char[getDelimiterLength()];
	Group.group.groupSize = 0;
	Group.group.groupName = new char[getDelimiterLength()];
	Group.group.type = new char[getGroupTypeLength()];
	Group.group.stamp = new char[getGroupStampLength()];
	Group.group.stuffz1 = new char[getStuffzLength()];
	Group.group.version = new char[getVerLength()];
	Group.group.stuffz2 = new char[getStuffzLength()];
	input.read(Group.group.groupHeader, getDelimiterLength());
	count += getDelimiterLength();
	input.read(reinterpret_cast<char*>(&Group.group.groupSize), getGroupSizeLength());
	count += getGroupSizeLength();
	input.read(Group.group.groupName, getDelimiterLength());
	count += getDelimiterLength();
	input.read(Group.group.type, getGroupTypeLength());
	count += getGroupTypeLength();
	input.read(Group.group.stamp, getGroupStampLength());
	count += getGroupStampLength();
	input.read(Group.group.stuffz1, getStuffzLength());
	count += getStuffzLength();
	input.read(Group.group.version, getVerLength());
	count += getVerLength();
	input.read(Group.group.stuffz2, getStuffzLength());
	count += getStuffzLength();
	//read in pre-meat stuffz
	//count += groupStuffz; //this is due to the groupSize being the size of the entire block, will need a secondary counter for records to return back to this function
	char * temp;
	temp = new char[4];
	while(count < Group.group.groupSize){
		input.read(temp, getDelimiterLength());
		if(parser::isGRUP(temp)){ //will probably need to change this so that we don't have a dependency on parser.h/parser.cpp; may not change it, we'll see
			for(unsigned int i = 0; i < getDelimiterLength(); ++i)
				input.unget();
			struct item groupNew;
			groupNew.type = GROUP;
			count += readGroup(input, groupNew);
			Group.group.items.push_back(groupNew);
//			delete groupNew.groupHeader;
//			delete groupNew.groupName;
//			delete groupNew.type;
//			delete groupNew.stamp;
//			delete groupNew.stuffz1;
//			delete groupNew.version;
//			delete groupNew.stuffz2;
		}
		else{
			for(unsigned int i = 0; i < getDelimiterLength(); ++i)
				input.unget();
			struct item recordNew;
			recordNew.type = RECORD;
			count += readRecord(input, recordNew);
			Group.group.items.push_back(recordNew);
//			delete recordNew.recName;
//			delete recordNew.recID;
//			delete recordNew.revision;
//			delete recordNew.version;
//			delete recordNew.stuffz;
		}
	}
	return count;
}
unsigned int parser::fileFormat::readRecord(std::ifstream &input, parser::fileFormat::item &Record){
//	struct field Field;
	unsigned int count = 0;
	unsigned int totalCount = 0;
	Record.type = RECORD;
	Record.record.recName = new char[getDelimiterLength()];
	Record.record.size = 0;
	Record.record.flags = 0;
	Record.record.recID = new char[getIDLength()];
	Record.record.revision = new char[getRevLength()];
	Record.record.version = new char[getVerLength()];
	Record.record.stuffz = new char[getStuffzLength()];
	Record.record.decompSize = 0;
	input.read(Record.record.recName, getDelimiterLength());
	totalCount += getDelimiterLength();
	input.read(reinterpret_cast<char*>(&Record.record.size), getRecSizeLength());
	totalCount += getRecSizeLength();
	input.read(reinterpret_cast<char*>(&Record.record.flags), getFlagLength());
	totalCount += getFlagLength();
	input.read(Record.record.recID, getIDLength());
	totalCount += getIDLength();
	input.read(Record.record.revision, getRevLength());
	totalCount += getRevLength();
	input.read(Record.record.version, getVerLength());
	totalCount += getVerLength();
	input.read(Record.record.stuffz, getStuffzLength());
	totalCount += getStuffzLength();
	if(isCompressed(Record)){
		//read in compressed data and uncompress
		/*For the compressed stuff, the size of the record is the number of bytes for meat after you get through all the information stuff like flags.
		 *That's the raw size, not the actual size. The actual size is contained in the 4 bytes (which are counted in the raw size) after the informational stuff.
		 *That's the uncompressed size of the data.*/
		input.read(reinterpret_cast<char*>(&Record.record.decompSize), getDecompSizeLength());
		totalCount += getDecompSizeLength();
		char * decData;
		decData = new char[Record.record.decompSize]; //may change the size to Record1.decompSize * 2; though, the zlib documentation isn't clear if it shrinks the memory block down
		char * data;
		data = new char[Record.record.size];
		unsigned int compSize = Record.record.size - getDecompSizeLength();
		input.read(data, compSize); //may need a gcount here; definietly change it so that decompSize works off a config value for its size
		totalCount += compSize;
		uncompress((Bytef*)decData, (uLongf*)&Record.record.decompSize, (Bytef*)data, compSize);
		/*Now we need to treat the block of uncompressed data the same as if the record was not compressed in the first place*/
		while(count < Record.record.decompSize){
			Field.name = new char[getDelimiterLength()];
			Field.size = 0;
			memcpy(Field.name, decData, getDelimiterLength());
			count += getDelimiterLength();
			for(int i = 0; i < getDelimiterLength(); ++i)
				decData++;
			memcpy(reinterpret_cast<char*>(&Field.size), decData, getFieldSizeLength());
			count += getFieldSizeLength();
			for(int i = 0; i < getFieldSizeLength(); ++i)
				decData++;
			Field.data = new char[Field.size];
			memcpy(Field.data, decData, Field.size);
			count += Field.size;
			for(int i = 0; i < Field.size; ++i)
				decData++;
			Record.record.fields.push_back(Field);
//			delete Field.name;
//			delete Field.data;
		}
//		delete decData;
//		delete data;
	}
	else{
		while(count < Record.record.size){
			count += readField(input, Field);
			Record.record.fields.push_back(Field);
//			delete Field.name;
//			delete Field.data;
		}
		totalCount += count;
	}
	return totalCount;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned int parser::fileFormat::readSize(std::ifstream &file){
	unsigned int size = 0;
	for(unsigned int i = 0; i < getFieldSizeLength(); ++i)
		size += file.get();
	return size;
}
std::vector<char *> parser::fileFormat::getMasters(parser::fileFormat::file &File){
	std::vector<char *> masters;
	for(int i = 0; i < File.fields.size(); ++i)
		if(strncmp("MAST", File.fields[i].name, 4) == 0)
			masters.push_back(File.fields[i].data);
	return masters;
}
std::vector<parser::fileFormat::item> parser::fileFormat::getRecords(parser::fileFormat::file &File){
	std::vector<item> records;
	for(unsigned int i = 0; i < File.items.size(); ++i)
		getRecords(records, File.items[i]);
	return records;
}
void parser::fileFormat::getRecords(std::vector<parser::fileFormat::item> &records, parser::fileFormat::item &Item){
	for(unsigned int i = 0; i < Item.group.items.size(); ++i){
		if(Item.group.items[i].type == RECORD)
			records.push_back(Item);
		getRecords(records, Item.group.items[i]);
	}
}
void parser::fileFormat::init(){
	setDelimiterLength2();
	setFlagLength2();
	setGroupStampLength();
	setGroupTypeLength();
	setIDLength();
	setRevLength();
	setDecompSizeLength();
	setFieldSizeLength();
	setGroupSizeLength();
	setHeadSizeLength();
	setRecSizeLength();
	setStuffzLength();
	setVerLength();
}
void parser::fileFormat::iterate(parser::fileFormat::item &Group){
	for(unsigned int i = 0; i < Group.group.items.size(); ++i){
		std::cout << "Nested ";
		if(Group.group.items[i].type == GROUP){
			std::cout.write(Group.group.items[i].group.groupHeader, 4) << std::endl;
			std::cout << "----------------" << std::endl;
			std::cout << "Nested ";
			std::cout.write(Group.group.items[i].group.groupName, 4) << std::endl;
		}
		else{
			std::cout << "Nested Rec";
			std::cout << std::endl;
			std::cout << "-----------";
			std::cout << std::endl;
			std::cout.write(Group.group.items[i].record.recName, 4) << std::endl;
			for(unsigned int k = 0; k < Group.group.items[i].record.fields.size(); ++k){
				std::cout << "\t\t";
				std::cout.write(Group.group.items[i].record.fields[k].name, 4) << std::endl;
				//std::cout << "\t\t\t" << Group.group.items[i].record.fields[k].data << std::endl;
			}
		}
		iterate(Group.group.items[i]);
	}
}
void parser::fileFormat::readFile(std::ifstream &input, parser::fileFormat::file &File){
	init();
	readHeaderThing(input, File);
	while(input.good()){
		struct item Item;
		readGroup(input, Item);
		File.items.push_back(Item);
//		delete Group.groupHeader;
//		delete Group.groupName;
//		delete Group.type;
//		delete Group.stamp;
//		delete Group.stuffz1;
//		delete Group.version;
//		delete Group.stuffz2;
	}
}
void parser::fileFormat::readHeaderThing(std::ifstream &input, parser::fileFormat::file &File){
//	struct field Field;
	unsigned int count = 0;
	File.header = new char[getDelimiterLength()];
	File.size = 0;
	File.flags = 0;
	File.ID = new char[getIDLength()];
	File.revision = new char[getRevLength()];
	File.version = new char[getVerLength()];
	File.stuffz = new char[getStuffzLength()];
	input.read(File.header, getDelimiterLength());
	input.read(reinterpret_cast<char*>(&File.size), getHeadSizeLength());
	input.read(reinterpret_cast<char*>(&File.flags), getFlagLength());
	input.read(File.ID, getIDLength());
	input.read(File.revision, getRevLength());
	input.read(File.version, getVerLength());
	input.read(File.stuffz, getStuffzLength());
	while(count < File.size){
		count += readField(input, Field);
		File.fields.push_back(Field);
//		delete Field.name;
//		delete Field.data;
	}
}
/*END OF LINE*/
