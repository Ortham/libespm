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

#include "src/fileFormat.h"
#include <iostream>
using namespace std;
int main(int argc, char *argv[]){
	common::options::setGame("Skyrim");
	ifstream input("opts/input");
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
	return 0;
}
