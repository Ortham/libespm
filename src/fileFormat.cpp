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
unsigned int espm::delimiterLength;
unsigned int espm::flagLength;
unsigned int espm::groupStampLength;
unsigned int espm::groupTypeLength;
unsigned int espm::IDLength;
unsigned int espm::revLength;
unsigned int espm::decompSizeLength;
unsigned int espm::fieldSizeLength;
unsigned int espm::groupSizeLength;
unsigned int espm::headSizeLength;
unsigned int espm::recSizeLength;
unsigned int espm::stuffzLength;
unsigned int espm::verLength;

espm::field::field() : name(NULL), size(0), data(NULL) {
	name = new char[getDelimiterLength()];
	//size varies so can't allocate data memory.
}

espm::field::field(field const &other) {
	name = new char[getDelimiterLength()];
	size = other.size;
	data = new char[size];
	
	memcpy(name, other.name, getDelimiterLength());
	memcpy(data, other.data, size);
}
	
espm::field::~field() {
	delete [] name;
	delete [] data;
}

espm::field& espm::field::operator= (field other) {
	size = other.size;
	std::swap(name, other.name);
	std::swap(data, other.data);
	return *this;
}

espm::item::_record::_record()
	: recName(NULL), 
	  size(0), 
	  flags(0), 
	  ID(0), 
	  recID(NULL), 
	  revision(NULL), 
	  version(NULL), 
	  stuffz(NULL), 
	  decompSize(0) {

	recName = new char[getDelimiterLength()];
	recID = new char[getIDLength()];
	revision = new char[getRevLength()];
	version = new char[getVerLength()];
	stuffz = new char[getStuffzLength()];

}

espm::item::_record::_record(espm::item::_record const &other) {
	recName = new char[getDelimiterLength()];
	recID = new char[getIDLength()];
	revision = new char[getRevLength()];
	version = new char[getVerLength()];
	stuffz = new char[getStuffzLength()];
	
	size = other.size;
	flags = other.flags;
	ID = other.ID;
	decompSize = other.decompSize;
	fields = other.fields;
	
	memcpy(recName, other.recName, getDelimiterLength());
	memcpy(recID, other.recID, getIDLength());
	memcpy(revision, other.revision, getRevLength());
	memcpy(version, other.version, getVerLength());
	memcpy(stuffz, other.stuffz, getStuffzLength());
}

espm::item::_record::~_record() {
	delete [] recName;
	delete [] recID;
	delete [] revision;
	delete [] version;
	delete [] stuffz;
}

espm::item::_record& espm::item::_record::operator= (espm::item::_record other) {
	size = other.size;
	flags = other.flags;
	ID = other.ID;
	decompSize = other.decompSize;
	
	std::swap(recName, other.recName);
	std::swap(recID, other.recID);
	std::swap(revision, other.revision);
	std::swap(version, other.version);
	std::swap(stuffz, other.stuffz);
	std::swap(fields, other.fields);
	
	return *this;
}

espm::item::_group::_group() 
	: groupHeader(NULL), 
	  groupSize(0), 
	  groupName(NULL), 
	  type(NULL), 
	  stamp(NULL), 
	  stuffz1(NULL), 
	  version(NULL), 
	  stuffz2(NULL) {
				
	groupHeader = new char[getDelimiterLength()];
	groupName = new char[getDelimiterLength()];
	type = new char[getGroupTypeLength()];
	stamp = new char[getGroupStampLength()];
	stuffz1 = new char[getStuffzLength()];
	version = new char[getVerLength()];
	stuffz2 = new char[getStuffzLength()];		
}

espm::item::_group::_group(espm::item::_group const &other) {
	groupHeader = new char[getDelimiterLength()];
	groupName = new char[getDelimiterLength()];
	type = new char[getGroupTypeLength()];
	stamp = new char[getGroupStampLength()];
	stuffz1 = new char[getStuffzLength()];
	version = new char[getVerLength()];
	stuffz2 = new char[getStuffzLength()];		
	
	groupSize = other.groupSize;
	
	memcpy(groupHeader, other.groupHeader, getDelimiterLength());
	memcpy(groupName, other.groupName, getDelimiterLength());
	memcpy(type, other.type, getGroupTypeLength());
	memcpy(stamp, other.stamp, getGroupStampLength());
	memcpy(stuffz1, other.stuffz1, getStuffzLength());
	memcpy(version, other.version, getVerLength());
	memcpy(stuffz2, other.stuffz2, getStuffzLength());
	
	items = other.items;
}

espm::item::_group::~_group() {
	delete [] groupHeader;
	delete [] groupName;
	delete [] type;
	delete [] stamp;
	delete [] stuffz1;
	delete [] version;
	delete [] stuffz2;
}

espm::item::_group& espm::item::_group::operator= (espm::item::_group other) {
	groupSize = other.groupSize;
	
	std::swap(groupHeader, other.groupHeader);
	std::swap(groupName, other.groupName);
	std::swap(type, other.type);
	std::swap(stamp, other.stamp);
	std::swap(stuffz1, other.stuffz1);
	std::swap(version, other.version);
	std::swap(stuffz2, other.stuffz2);
	std::swap(items, other.items);
	
	return *this;
}

espm::file::file() 
	: header(NULL), 
	  size(0), 
	  flags(0), 
	  ID(NULL), 
	  revision(NULL), 
	  version(NULL), 
	  stuffz(NULL) {
		  
	header = new char[getDelimiterLength()];
	ID = new char[getIDLength()];
	revision = new char[getRevLength()];
	version = new char[getVerLength()];
	stuffz = new char[getStuffzLength()];
}

espm::file::file(espm::file const &other) {
	header = new char[getDelimiterLength()];
	ID = new char[getIDLength()];
	revision = new char[getRevLength()];
	version = new char[getVerLength()];
	stuffz = new char[getStuffzLength()];
	
	size = other.size;
	flags = other.flags;
	fields = other.fields;
	items = other.items;
	
	memcpy(header, other.header, getDelimiterLength());
	memcpy(ID, other.ID, getIDLength());
	memcpy(revision, other.revision, getRevLength());
	memcpy(version, other.version, getVerLength());
	memcpy(stuffz, other.stuffz, getStuffzLength());
}

espm::file::~file() {
	delete [] header;
	delete [] ID;
	delete [] revision;
	delete [] version;
	delete [] stuffz;
}

espm::file& espm::file::operator= (espm::file other) {
	size = other.size;
	flags = other.flags;
	
	std::swap(header, other.header);
	std::swap(ID, other.ID);
	std::swap(revision, other.revision);
	std::swap(version, other.version);
	std::swap(stuffz, other.stuffz);
	std::swap(fields, other.fields);
	std::swap(items, other.items);
	
	return *this;
}

bool espm::getRecordByFieldD(espm::item &Item, char * fieldName, char * fieldData, espm::item &Record, unsigned int length){
	for(unsigned long long i = 0; i < Item.group.items.size(); ++i){
		if(Item.group.items[i].type == RECORD){
			for(unsigned long long j = 0; j < Item.group.items[i].record.fields.size(); ++j){
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
bool espm::getRecordByID(espm::item &Item, unsigned int ID, espm::item &Record){
	for(unsigned long long i = 0; i < Item.group.items.size(); ++i){
		if(Item.group.items[i].type == RECORD){
			if(strncmp(reinterpret_cast<char*>(&ID), Item.group.items[i].record.recID, getIDLength()) == 0){
				Record = Item.group.items[i];
				return true;
			}
		}
		getRecordByID(Item.group.items[i], ID, Record);
	}
	return false;
}
bool espm::isCompressed(espm::item &Record){
	//if(((unsigned int)recordA.flags & 0x00040000) == 0x00040000)
	if((Record.record.flags & strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["CompFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
bool espm::isGRUP(char * data){
	return (strncmp(data, common::structVals[common::options::game]["Group"][0].c_str(), getDelimiterLength()) == 0);
}
bool espm::isMaster(espm::file &File){
	//if(((unsigned int)fileA.flags & 0x00000001) == 0x00000001)
	if((File.flags & strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0)) == strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0))
		return true;
	return false;
}
espm::item espm::getGroupByName(espm::file &File, char * name){
	for(unsigned long long i = 0; i < File.items.size(); ++i)
		if(File.items[i].type == GROUP && (strncmp(File.items[i].group.groupName, name, getDelimiterLength()) == 0))
			return File.items[i];
	item newItem;
	return newItem;
}
espm::item espm::getRecordByFieldD(espm::file &File, char * fieldName, char * fieldData, unsigned int length){
	item Record;
	for(unsigned long long i = 0; i < File.items.size(); ++i)
		if(getRecordByFieldD(File.items[i], fieldName, fieldData, Record, length))
			return Record;
	item newItem;
	return newItem;
}
espm::item espm::getRecordByID(espm::file &File, unsigned int ID){
	item Record;
	for(unsigned long long i = 0; i < File.items.size(); ++i)
		if(getRecordByID(File.items[i], ID, Record))
			return Record;
	item newItem;
	return newItem;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned char * espm::readRecord(std::ifstream &file){
	unsigned char * data;
	file.read((char *)data, getDelimiterLength());
	return data;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned char * espm::readRecordData(std::ifstream &file){
	unsigned char * data;
	//file.read(data, size); //size refers to a future storage place for all the stuff we're reading in and I haven't decided on how to handle that yet
	return data;
}
unsigned int espm::readField(std::ifstream &input, espm::field &Field){
	unsigned int count = 0;
	input.read(Field.name, getDelimiterLength());
	count += getDelimiterLength();
	//input.read((char*)&(Field1.size), getFieldSizeLength());
	input.read(reinterpret_cast<char*>(&Field.size), getFieldSizeLength());
	count += getFieldSizeLength();
	Field.data = new char[Field.size];
	input.read(Field.data, Field.size);
	count += Field.size;
	return count;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned int espm::readFlags(std::ifstream &file){
	unsigned int data;
	file.read((char *)&data, getFlagLength());
	return data;
}
unsigned int espm::readGroup(std::ifstream &input, espm::item &Group){
	unsigned int count = 0;
	Group.type = GROUP;
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
		if(espm::isGRUP(temp)){ //will probably need to change this so that we don't have a dependency on parser.h/parser.cpp; may not change it, we'll see
			for(unsigned int i = 0; i < getDelimiterLength(); ++i)
				input.unget();
			item groupNew;
			groupNew.type = GROUP;
			count += readGroup(input, groupNew);
			Group.group.items.push_back(groupNew);
		}
		else{
			for(unsigned int i = 0; i < getDelimiterLength(); ++i)
				input.unget();
			item recordNew;
			recordNew.type = RECORD;
			count += readRecord(input, recordNew);
			Group.group.items.push_back(recordNew);
		}
	}
	delete [] temp;
	return count;
}
unsigned int espm::readRecord(std::ifstream &input, espm::item &Record){
	unsigned int count = 0;
	unsigned int totalCount = 0;
	Record.type = RECORD;
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
			char * decDataOffset = decData;
			field Field;
			memcpy(Field.name, decDataOffset, getDelimiterLength());
			count += getDelimiterLength();
			for(int i = 0; i < getDelimiterLength(); ++i)
				decDataOffset++;
			memcpy(reinterpret_cast<char*>(&Field.size), decDataOffset, getFieldSizeLength());
			count += getFieldSizeLength();
			for(int i = 0; i < getFieldSizeLength(); ++i)
				decDataOffset++;
			Field.data = new char[Field.size];
			memcpy(Field.data, decDataOffset, Field.size);
			count += Field.size;
			for(int i = 0; i < Field.size; ++i)
				decDataOffset++;
			Record.record.fields.push_back(Field);
		}
		delete [] decData;
		delete [] data;
	}
	else{
		while(count < Record.record.size){
			field Field;
			count += readField(input, Field);
			Record.record.fields.push_back(Field);
		}
		totalCount += count;
	}
	return totalCount;
}
///@todo Look into stripping some of these functions out or rewriting them to be more useful. Right now, they seem to be taking up space more than anything else...
unsigned int espm::readSize(std::ifstream &file){
	unsigned int size = 0;
	for(unsigned int i = 0; i < getFieldSizeLength(); ++i)
		size += file.get();
	return size;
}
std::vector<char *> espm::getMasters(espm::file &File){
	std::vector<char *> masters;
	for(unsigned long long i = 0; i < File.fields.size(); ++i)
		if(strncmp("MAST", File.fields[i].name, 4) == 0)
			masters.push_back(File.fields[i].data);
	return masters;
}
std::vector<espm::item> espm::getGroups(espm::file &File){
	return File.items;
}
std::vector<espm::item> espm::getRecords(espm::file &File){
	std::vector<item> records;
	for(unsigned long long i = 0; i < File.items.size(); ++i)
		getRecords(records, File.items[i]);
	return records;
}
void espm::getRecords(std::vector<espm::item> &records, espm::item &Item){
	for(unsigned long long i = 0; i < Item.group.items.size(); ++i){
		if(Item.group.items[i].type == RECORD)
			records.push_back(Item.group.items[i]);
		getRecords(records, Item.group.items[i]);
	}
}
void espm::init(){
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
void espm::iterate(espm::item &Group){
	for(unsigned long long i = 0; i < Group.group.items.size(); ++i){
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
			for(unsigned long long k = 0; k < Group.group.items[i].record.fields.size(); ++k){
				std::cout << "\t\t";
				std::cout.write(Group.group.items[i].record.fields[k].name, 4) << std::endl;
				//std::cout << "\t\t\t" << Group.group.items[i].record.fields[k].data << std::endl;
			}
		}
		iterate(Group.group.items[i]);
	}
}
void espm::readFile(std::ifstream &input, espm::file &File){
	init();
	readHeaderThing(input, File);
	while(input.good()){
		item Item;
		readGroup(input, Item);
		File.items.push_back(Item);
	}
}
void espm::readHeaderThing(std::ifstream &input, espm::file &File){
	unsigned int count = 0;
	input.read(File.header, getDelimiterLength());
	input.read(reinterpret_cast<char*>(&File.size), getHeadSizeLength());
	input.read(reinterpret_cast<char*>(&File.flags), getFlagLength());
	input.read(File.ID, getIDLength());
	input.read(File.revision, getRevLength());
	input.read(File.version, getVerLength());
	input.read(File.stuffz, getStuffzLength());
	while(count < File.size){
		field Field;
		count += readField(input, Field);
		File.fields.push_back(Field);
	}
}
/*END OF LINE*/
