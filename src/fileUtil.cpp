#include <string>
#include "fileUtil.h"
void util::createMast(parser::fileFormat::file &fileA, std::vector<parser::fileFormat::file> masters){
	(unsigned int)fileA.flags |= 1;
}
void util::createONAM(parser::fileFormat::file &fileA, std::vector<parser::fileFormat::file> masters){
	std::string ids;
	bool test = false;
	for(unsigned int i = 0; i < fileA.groups.length(); ++i){
		for(unsigned int j = 0; j < fileA.groups[i].records.length(); ++j){
			if(isONAM(fileA.groups[i].records[j].name)){
				for(unsigned int k = 0; k < masters.length(); ++k){
					for(unsigned int l = 0; l < masters[k].groups.length(); ++l){
						for(unsigned int m = 0; m < masters[k].groups[l].records.length(); ++m){
							if(fileA.groups[i].records[j].ID == masters[k].groups[l].records[m].ID){
								ids += fileA.groups[i].records[j].ID;
								test = true;
								break;
							}
						}
						if(test)
							break;
					}
					test = false;
				}
			}
		}
	}
	ids += "00";
	vector<parser::fileFormat::record>::iterator it = fileA.records.begin();
	while(*it.name != "INTV" || *it.name != "INCC" || it != fileA.records.end())
		++it;
	parser::fileFormat::record ONAM;
	ONAM.name = "ONAM";
	ONAM.size = ids.length() + 1;
	ONAM.data = ids.c_str();
	fileA.records.insert(it, ONAM);
}
void util::revCreateMast(parser::fileFormat::file &fileA){
	(unsigned int)fileA.flags ^= 1;
}
void util::revCreateONAM(parser::fileFormat::file &fileA){
	vector<parser::fileFormat::record>::iterator it = fileA.records.begin();
	while(*it.name != "ONAM" || it != fileA.records.end())
		++it;
	fileA.records.erase(it);
}
