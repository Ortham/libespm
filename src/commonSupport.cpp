/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file commonSupport.cpp
 * @brief Contains the functions common to the viewer and parser.
 * @details A support library to hold functions that both programs use to help with keeping down clutter.
 */
#include <cstdlib>
#include <vector>
#include "constants.h"
#include "commonSupport.h"
const char* common::inputName(std::string base, std::string addon){
	//cout << base << endl;
	//cout << addon << endl;
	//cout << base + addon << endl;
	return (base + addon).c_str();
}
int common::countLeading(std::string line){
	int count = 0;
	for(int i = 0; i < line.size(); ++i){
		if(line[i] >= RANGE_MIN && line[i] <= RANGE_MAX)
			break;
		++count;
	}
	return count;
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
void common::eraseLeading(std::string &line){
	int leading = countLeading(line);
	line.erase(0, leading);
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
void common::writeLabel(std::string label, std::ofstream &out){
	out << label << std::endl;
	std::string labelLine;
	for(unsigned int i = 0; i < label.size(); ++i)
		labelLine = labelLine + "-";
	out << labelLine << std::endl;
	out << std::endl;
	out << std::endl;
}
/*END OF LINE*/
