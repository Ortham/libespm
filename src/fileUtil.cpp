#include <string>
#include "fileUtil.h"
void util::createMast(parser::fileFormat::file fileA, std::vector<parser::fileFormat::file> masters){
	(unsigned int)fileA.flags = (unsigned int)fileA.flags | 1;
	std::string ids;
	bool test = false;
	for(unsigned int i = 0; i < fileA.groups.length(); ++i){
		for(unsigned int j = 0; j < fileA.groups[i].records.length(); ++j){
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
	ids += 0;
}
