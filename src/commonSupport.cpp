/*
 * commonSupport.cpp
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
 * @file commonSupport.cpp
 * @brief Contains the functions common to the viewer and parser.
 * @details A support library to hold functions that both programs use to help with keeping down clutter.
 */
#include <cstdlib>
#include <stack>
#include "constants.h"
#include "commonSupport.h"
std::map<std::string, std::map<std::string, std::vector<std::string> > > common::structVals;
bool common::isString(unsigned char data[]){
	unsigned int dataSize = sizeof data;
	if(data[dataSize - 1] != '\0')
		return false;
	for(unsigned int i = 0; i < dataSize; ++i)
		if(data[i] < PRINT_START || data[i] > PRINT_END)
			return false;
	return true;
}
const char* common::inputName(std::string base, std::string addon){
	//cout << base << endl;
	//cout << addon << endl;
	//cout << base + addon << endl;
	return (base + addon).c_str();
}
const char* common::inputName(char * base1, char * addon1){
	std::string base(base1);
	std::string addon(addon1);
	//cout << base << endl;
	//cout << addon << endl;
	//cout << base + addon << endl;
	return (base + addon).c_str();
}
/*int common::numLines(ifstream &in){
	///in.seekg(0, ios::beg);
	string line;
	vector<string> lines;
	while(getline(in, line))
		lines.push_back(line);
	//in.seekg(0, ios::beg);
	return lines.size();
}*/
void common::callViewer(std::string fileName){
	fileName = "\"" + fileName + "\"";
	#ifdef __WIN32__
		const char * callName = (".\\Viewer.exe " + fileName).c_str();
		system(callName);
	#else
		const char * callName = ("./Viewer " + fileName).c_str();
		system(callName);
	#endif
}
void common::callViewer(char * fileName1){
	std::string fileName(fileName1);
	fileName = "\"" + fileName + "\"";
	#ifdef __WIN32__
		const char * callName = (".\\Viewer.exe " + fileName).c_str();
		system(callName);
	#else
		const char * callName = ("./Viewer " + fileName).c_str();
		system(callName);
	#endif
}
void common::eraseTrailing(std::string &line){
	if(line.size() >= 4){
		if(line[3] >= RANGE_MIN && line[3] <= RANGE_MAX)
			line.erase(4, line.size());
		else if(!(line[3] >= 0x61 && line[3] <= 0x7A))
			if(line.compare(0, 5, "TIF__") != 0)
				line.erase(3, line.size());
	}
}
void common::readOptions(std::ifstream &file){
	std::string line, line2, test;
	std::map<std::string, std::vector<std::string> > map2;
	std::vector<std::string> data;
	bool firstRun = true;
	while(file.good()){
		std::getline(file, line);
		data.clear();
		if(line[0] == '=' && line[1] == '='){
			if(!firstRun){
				structVals.insert(std::pair<std::string, std::map<std::string, std::vector<std::string> > >(line2, map2));
				map2.clear();
			}
			line2 = line.substr(2, (line.size() - 4));
			firstRun = false;
		}
		else if(!line.empty()){
			file >> test;
			if(test[0] == '{'){
				file >> test;
				while(test[0] != '}'){
					data.push_back(test);
					file >> test;
				}
				map2.insert(std::pair<std::string, std::vector<std::string> >(line, data));
			}
		}
	}
	structVals.insert(std::pair<std::string, std::map<std::string, std::vector<std::string> > >(line2, map2));
}
void common::writeLabel(std::string label, std::ofstream &out){
	out << label << std::endl;
	std::string labelLine;
	for(unsigned int i = 0; i < label.size(); ++i)
		labelLine = labelLine + "-";
	out << labelLine << std::endl;
	out << std::endl;
	out << std::endl;
}
void common::writeLabel(char * label1, std::ofstream &out){
	std::string label(label1);
	out << label << std::endl;
	std::string labelLine;
	for(unsigned int i = 0; i < label.size(); ++i)
		labelLine = labelLine + "-";
	out << labelLine << std::endl;
	out << std::endl;
	out << std::endl;
}
void common::writeXML(std::ofstream &out){
	std::map<std::string, std::map<std::string, std::vector<std::string> > >::iterator it;
	std::map<std::string, std::vector<std::string> >::iterator it2;
	std::stack<std::string> stuff;
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl;
	out << "<Games>" << std::endl;
	for(it = structVals.begin(); it != structVals.end(); ++it){
		out << "\t<" << (*it).first << ">" << std::endl;
		stuff.push((*it).first);
		for(it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2){
			out << "\t\t<" << (*it2).first << ">" << std::endl;
			stuff.push((*it2).first);
			for(int i = 0; i < (*it2).second.size(); ++i){
				out << "\t\t\t<data>\n\t\t\t\t" << (*it2).second[i] << "\n\t\t\t</data>" << std::endl;
			}
			out << "\t\t</" << stuff.top() << ">" << std::endl;
			stuff.pop();
		}
		out << "\t</" << stuff.top() << ">" << std::endl;
		stuff.pop();
	}
	out << "</Games>" << std::endl;
}
bool common::options::contains(std::string opt, std::string val){
	for(unsigned int i = 0; i < structVals[game][opt].size(); ++i)
		if(structVals[game][opt][i] == val)
			return true;
	return false;
}
/*END OF LINE*/
