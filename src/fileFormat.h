/*
 * fileFormat.h
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
 * @file fileFormat.h
 * @brief Contains the file format stuff.
 * @details A support library to hold the general information on the format of a specific file type, as opposed to the general patterns. I think...
 * It's missing some notes, but I'll get them included later.
 * The notes missing include, but are not limited to, the observation that the main TES4 section's size value corresponds to.
 */
#ifndef __FILE_FORMAT_H__
#define __FILE_FORMAT_H__

#include <fstream>
#include <sstream>
#include <vector>
#include "constants.h"
#include "commonSupport.h"
/**
 * @namespace fileFormat
 * @brief Contains file format stuff.
 * @details This is for the stuff relating to the file format itself. Currently, it's a bit sparse and includes mainly notes and observations, but the idea is that
 * this is where the main programs will output a parsed format to in various structs.
 */
namespace espm{
	enum Type{
		GROUP,
		RECORD
	};
	/**
	 * @var delimiterLength
	 * The length of the delimiter in bytes.
	 */
	extern unsigned int delimiterLength;
	/**
	 * @var flagLength
	 * The length of the flag field in bytes.
	 */
	extern unsigned int flagLength;
	/**
	 * @var groupStampLength
	 * The length of the group time-stamp field in bytes.
	 */
	extern unsigned int groupStampLength;
	/**
	 * @var groupTypeLength
	 * The length of the group type field in bytes.
	 */
	extern unsigned int groupTypeLength;
	/**
	 * @var IDLength
	 * The length of the ID field in bytes.
	 */
	extern unsigned int IDLength;
	/**
	 * @var revLength
	 * The length of the revision (?) field in bytes.
	 */
	extern unsigned int revLength;
	/**
	 * @var decompSizeLength
	 * The length of the decompressed size field in bytes.
	 */
	extern unsigned int decompSizeLength;
	/**
	 * @var fieldSizeLength
	 * The length of the size field for fields in bytes.
	 */
	extern unsigned int fieldSizeLength;
	/**
	 * @var headSizeLength
	 * The length of the size field for the file header in bytes.
	 */
	extern unsigned int headSizeLength;
	/**
	 * @var groupSizeLength
	 * The length of the size field for groups in bytes.
	 */
	extern unsigned int groupSizeLength;
	/**
	 * @var recSizeLength
	 * The length of the size field for records in bytes.
	 */
	extern unsigned int recSizeLength;
	/**
	 * @var stuffzLength
	 * The length of some unknown field in bytes.
	 */
	extern unsigned int stuffzLength;
	/**
	 * @var verLength
	 * The length of the version (?) field in bytes
	 */
	extern unsigned int verLength;
	/**
	 * @struct field
	 * @brief A field.
	 * @details Contains the data for a field for which a record contains.
	 */
	struct field{
		char * name;
		unsigned int size;
		char * data;
	};
	struct item{
		enum Type type;
		struct{
			char * recName;
			unsigned int size;
			unsigned int flags;
			unsigned int ID;
			char * recID;
			char * revision;
			char * version;
			char * stuffz;
			unsigned int decompSize;
			char * data;
			std::vector<field> fields;
		}record;
		struct{
			char * groupHeader;
			unsigned int groupSize;
			char * groupName;
			char * type;
			char * stamp;
			char * stuffz1;
			char * version;
			char * stuffz2;
			std::vector<item> items;
		}group;
	};
	/**
	 * @struct file
	 * @brief The file itself.
	 * @details This represents a file (specifically a mod file, the others will come in time), which consists of a lump of data with records and then the list of groups.
	 */
	struct file{
		char * header;
		unsigned int size;
		unsigned int flags;
		char * ID;
		char * revision;
		char * version;
		char * stuffz;
		std::vector<field> fields;
		std::vector<item> items;
	};
	bool getRecordByFieldD(item &Item, char * fieldName, char * fieldData, item &Record, unsigned int length);
	/**
	 * @brief Checks to see if a record is compressed.
	 * @details Checks the flag on the record to see if the compression flag is set.
	 * @param &Record
	 * The record that we want to check.
	 * @returns <tt> \b true </tt> if the record is compressed, <tt> \b false </tt> otherwise.
	 */
	bool isCompressed(item &Record);
	bool isGRUP(char * data);
	/**
	 * @brief Checks to see if a plugin file is a 'master'.
	 * @details Checks the flag on the plugin file to see if the 'master' flag is set (may be useful, no telling).
	 * @param &File
	 * The plugin file that we want to check.
	 * @returns <tt> \b true </tt> if the plugin file is a 'master', <tt> \b false </tt> otherwise.
	 */
	bool isMaster(file &File);
	item getRecordByFieldD(file &File, char * fieldName, char * fieldData, unsigned int length);
	/**
	 * @brief Reads a record name.
	 * @details Reads the record name based on the length for the delimiter.
	 * @param &file
	 * The file that is open.
	 * @returns The record name in a C-String, since their format can change and this enables them to be stored for whatever use.
	 */
	unsigned char * readRecord(std::ifstream &file);
	/**
	 * @brief Reads the data in a record.
	 * @details Reads the record data based on the size section of the record.
	 * @param &file
	 * The file that is open.
	 * @returns The record data in a C-String, since their format can change and this enables them to be stored for whatever use.
	 */
	unsigned char * readRecordData(std::ifstream &file);
	/**
	 * @brief Gets the length of the delimiter.
	 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the delimiter.
	 */
	inline unsigned int getDelimiterLength();
	/**
	 * @brief Gets the length of the flag field.
	 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the flag field.
	 */
	inline unsigned int getFlagLength();
	inline unsigned int getGroupStampLength();
	inline unsigned int getGroupTypeLength();
	/**
	 * @brief Gets the length of the ID field.
	 * @details Is here to allow for cases where the length of the ID field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the ID field.
	 */
	inline unsigned int getIDLength();
	/**
	 * @brief Gets the length of the revision field.
	 * @details Is here to allow for cases where the length of the revision field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the revision field.
	 */
	inline unsigned int getRevLength();
	/**
	 * @brief Gets the length of the size field.
	 * @details Is here to allow for cases where the length of the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the size field.
	 */
	inline unsigned int getDecompSizeLength();
	inline unsigned int getFieldSizeLength();
	inline unsigned int getGroupSizeLength();
	inline unsigned int getHeadSizeLength();
	inline unsigned int getRecSizeLength();
	/**
	 * @brief Gets the length of the unknown field.
	 * @details Is here to allow for cases where the length of the unknown field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the unknown field.
	 */
	inline unsigned int getStuffzLength();
	/**
	 * @brief Gets the length of the version field.
	 * @details Is here to allow for cases where the length of the version field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 * @returns The length of the version field.
	 */
	inline unsigned int getVerLength();
	unsigned int readField(std::ifstream &input, field &Field1);
	/**
	 * @brief Reads the flags.
	 * @details Reads the flags from various sections based on the length of the flag section.
	 * @param &file
	 * The file that is open.
	 * @returns The flags in a C-String, since their format can change and this enables them to be stored for whatever use.
	 */
	unsigned int readFlags(std::ifstream &file);
	unsigned int readGroup(std::ifstream &input, item &Group);
	unsigned int readRecord(std::ifstream &input, item &Record);
	/**
	 * @brief Reads the size of a record.
	 * @details Reads the record size based on the length of the size section.
	 * @param &file
	 * The file that is open.
	 * @returns The record size as an unsigned int.
	 * @note The type of the size may need tweaking in case it changes to support a value larger than an unsigned int can contain.
	 */
	unsigned int readSize(std::ifstream &file);
	/**
	 * @brief Gets the 'masters' of a plugin.
	 * @details Gets a list of all of the 'masters' of a file.
	 * @param &File
	 * The file, respresented as a struct, that we want to get the 'masters' of. This can currently only be called after we read in the data from a file.
	 * However, after the planned refactoring, this will hopefully be much saner to use. I hope...
	 * @returns A list of the 'masters' of a file.
	 */
	std::vector<char *> getMasters(file &File);
	std::vector<item> getRecords(file &File);
	void getRecords(std::vector<item> &records, item &Item);
	void init();
	void iterate(item &Group);
	void readFile(std::ifstream &input, file &File);
	void readHeaderThing(std::ifstream &input, file &File);
	/**
	 * @brief Sets the length of the delimiter.
	 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setDelimiterLength(unsigned int length);
	/**
	 * @brief Sets the length of the delimiter.
	 * @details Is here to allow for cases where the delimiter isn't the 4-character standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setDelimiterLength2();
	/**
	 * @brief Sets the length of the flag field.
	 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setFlagLength(unsigned int length);
	/**
	 * @brief Sets the length of the flag field.
	 * @details Is here to allow for cases where the length of the flag field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setFlagLength2();
	inline void setGroupStampLength();
	inline void setGroupTypeLength();
	/**
	 * @brief Sets the length of the ID field.
	 * @details Is here to allow for cases where the length of the ID field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setIDLength();
	/**
	 * @brief Sets the length of the revision field.
	 * @details Is here to allow for cases where the length of the revision field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setRevLength();
	/**
	 * @brief Sets the length of the size field.
	 * @details Is here to allow for cases where the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setSizeLength(unsigned int length);
	/**
	 * @brief Sets the length of the size field.
	 * @details Is here to allow for cases where the size field isn't the 2-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setDecompSizeLength();
	inline void setFieldSizeLength();
	inline void setGroupSizeLength();
	inline void setHeadSizeLength();
	inline void setRecSizeLength();
	/**
	 * @brief Sets the length of the stuffz field.
	 * @details Is here to allow for cases where the length of the stuffz field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setStuffzLength();
	/**
	 * @brief Sets the length of the version field.
	 * @details Is here to allow for cases where the length of the version field isn't the n-byte standard that we have now. In case it changes, all that will need to be done is to
	 * pass on the new length to properly read the files.
	 */
	inline void setVerLength();
	inline unsigned int getDelimiterLength(){
		return delimiterLength;
	}
	inline unsigned int getFlagLength(){
		return flagLength;
	}
	inline unsigned int getGroupStampLength(){
		return groupStampLength;
	}
	inline unsigned int getGroupTypeLength(){
		return groupTypeLength;
	}
	inline unsigned int getIDLength(){
		return IDLength;
	}
	inline unsigned int getRevLength(){
		return revLength;
	}
	inline unsigned int getDecompSizeLength(){
		return decompSizeLength;
	}
	inline unsigned int getFieldSizeLength(){
		return fieldSizeLength;
	}
	inline unsigned int getGroupSizeLength(){
		return groupSizeLength;
	}
	inline unsigned int getHeadSizeLength(){
		return headSizeLength;
	}
	inline unsigned int getRecSizeLength(){
		return recSizeLength;
	}
	inline unsigned int getStuffzLength(){
		return stuffzLength;
	}
	inline unsigned int getVerLength(){
		return verLength;
	}
	inline void setDelimiterLength(unsigned int length){
		delimiterLength = length;
	}
	inline void setDelimiterLength2(){
		std::stringstream(common::structVals[common::options::game]["Length"][0]) >> delimiterLength;
	}
	inline void setFlagLength(unsigned int length){
		flagLength = length;
	}
	inline void setFlagLength2(){
		std::stringstream(common::structVals[common::options::game]["FlagLength"][0]) >> flagLength;
	}
	inline void setGroupStampLength(){
		std::stringstream(common::structVals[common::options::game]["GroupStampLength"][0]) >> groupStampLength;
	}
	inline void setGroupTypeLength(){
		std::stringstream(common::structVals[common::options::game]["GroupTypeLength"][0]) >> groupTypeLength;
	}
	inline void setIDLength(){
		std::stringstream(common::structVals[common::options::game]["IDLength"][0]) >> IDLength;
	}
	inline void setRevLength(){
		std::stringstream(common::structVals[common::options::game]["RevLength"][0]) >> revLength;
	}
	inline void setSizeLength(unsigned int length){
		fieldSizeLength = length;
	}
	inline void setDecompSizeLength(){
		std::stringstream(common::structVals[common::options::game]["DecompSizeLength"][0]) >> decompSizeLength;
	}
	inline void setFieldSizeLength(){
		std::stringstream(common::structVals[common::options::game]["FieldSizeLength"][0]) >> fieldSizeLength;
	}
	inline void setGroupSizeLength(){
		std::stringstream(common::structVals[common::options::game]["GroupSizeLength"][0]) >> groupSizeLength;
	}
	inline void setHeadSizeLength(){
		std::stringstream(common::structVals[common::options::game]["HeadSizeLength"][0]) >> headSizeLength;
	}
	inline void setRecSizeLength(){
		std::stringstream(common::structVals[common::options::game]["RecSizeLength"][0]) >> recSizeLength;
	}
	inline void setStuffzLength(){
		std::stringstream(common::structVals[common::options::game]["StuffzLength"][0]) >> stuffzLength;
	}
	inline void setVerLength(){
		std::stringstream(common::structVals[common::options::game]["VerLength"][0]) >> verLength;
	}
	/*END OF LINE*/
}	

#endif /* __FILE_FORMAT_H__ */
