/*
 * test.cpp
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

#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "commonSupport.h"
#include "fileFormat.h"
using namespace std;
int main(int argc, char *argv[]){
	/*time_t start, end;
	start = time(NULL);*/
	string game = argv[1];
	common::options::setGame(game);
	ifstream input("opts/input");
	common::readOptions(input);
	input.close();
	/*end = time(NULL);
	cout << "Time taken to read configuration file: " << (end - start) << " seconds." << endl;
	start = time(NULL);*/
	input.open(argv[2], ios::binary);
	struct espm::file File;
	espm::readFile(input, File);
	/*end = time(NULL);
	cout << "Time taken to read input file: " << (end - start) << " seconds." << endl;*/
	if(espm::isMaster(File))
		cout << "true" << endl;
	else
		cout << "false" << endl;
	input.close();
	cout.write(File.header, 4) << endl;
	cout << File.size << endl;
	cout << hex << File.flags << endl << dec;
	cout << (unsigned int&)*(File.ID) << endl;
	cout << (unsigned int&)*(File.revision) << endl;
	cout << (unsigned short&)*(File.version) << endl;
	cout << (unsigned short&)*(File.stuffz) << endl;
	for(unsigned long long i = 0; i < File.fields.size(); ++i){
		cout.write(File.fields[i].name, 4) << endl;
		cout << File.fields[i].size << endl;
		if(strncmp("MAST", File.fields[i].name, 4) == 0)
			cout << "Master: ";
		cout << File.fields[i].data << endl;
	}
	/*start = time(NULL);*/
	for(unsigned long long i = 0; i < File.items.size() - 1; ++i){
		cout.write(File.items[i].group.groupHeader, 4) << endl;
		cout << "----------------" << endl;
		cout.write(File.items[i].group.groupName, 4) << endl;
		espm::iterate(File.items[i]);
	}
	/*end = time(NULL);
	cout << "Time taken to iterate over all of the items: " << (end - start) << " seconds." << endl;
	start = time(NULL);*/
	vector<char *> masters;
	masters = espm::getMasters(File);
	/*end = time(NULL);
	cout << "Time taken to get masters: " << (end - start) << " seconds." << endl;*/
	for(unsigned long long i = 0; i < masters.size(); ++i)
		cout << "Master " << i << ": " << masters[i] << endl;
	cout << "Num Items: " << File.items.size() << endl;
	/*start = time(NULL);*/
	vector<espm::item> records;
	records = espm::getRecords(File);
	/*end = time(NULL);
	cout << "Time taken to get the records: " << (end - start) << " seconds." << endl;*/
	cout << "Num Records: " << records.size() << endl;
	espm::item recTest;
	/*start = time(NULL);*/
	//Params are: File, fieldName, fieldData, length of search terms WITH terminating character
	recTest = espm::getRecordByFieldD(File, "EDID", "EnchFortifyAlchemyConstantSelf", 31);
	/*end = time(NULL);
	cout << "Time taken to get the record by the field data: " << (end - start) << " seconds." << endl;*/
	cout << recTest.record.recName << endl;
	/*start = time(NULL);*/
	recTest = espm::getRecordByID(File, 0x0008b65c);
	/*end = time(NULL);
	cout << "Time taken to get the record by the ID: " << (end - start) << " seconds." << endl;*/
	cout << recTest.record.recName << endl;
	espm::item groupTest;
	/*start = time(NULL);*/
	groupTest = espm::getGroupByName(File, "SPEL");
	/*end = time(NULL);
	cout << "Time taken to get the group by name: " << (end - start) << " seconds." << endl;*/
	cout << groupTest.group.groupName << endl;
	vector<espm::item> groups;
	/*start = time(NULL);*/
	groups = espm::getGroups(File);
	/*end = time(NULL);
	cout << "Time taken to get the groups: " << (end - start) << " seconds." << endl;*/
	cout << "Num Groups: " << groups.size() << endl;
	/*END OF LINE*/
	return 0;
}
