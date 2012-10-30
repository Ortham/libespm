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

#include "fileFormat.h"
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
int main(int argc, char *argv[]){
	common::options::setGame("Skyrim");
	ifstream input("../opts/input");
	common::readOptions(input);
	input.close();
	input.open(argv[1], ios::binary);
	struct parser::fileFormat::file File;
	parser::fileFormat::readFile(input, File);
	if(parser::fileFormat::isMaster(File))
		cout << "true" << endl;
	else
		cout << "false" << endl;
	input.close();
	cout << File.header << endl;
	cout << File.size << endl;
	cout << hex << File.flags << endl << dec;
	cout << (unsigned int&)*(File.ID) << endl;
	cout << (unsigned int&)*(File.revision) << endl;
	cout << (unsigned short&)*(File.version) << endl;
	cout << (unsigned short&)*(File.stuffz) << endl;
	for(int i = 0; i < File.fields.size(); ++i){
		cout << File.fields[i].name << endl;
		cout << File.fields[i].size << endl;
		if(strncmp("MAST", File.fields[i].name, 4) == 0)
			cout << "Master: ";
		cout << File.fields[i].data << endl;
	}
	vector<char *> masters;
	masters = parser::fileFormat::getMasters(File);
	for(int i = 0; i < masters.size(); ++i)
		cout << "Master " << i << ": " << masters[i] << endl;
	return 0;
}
