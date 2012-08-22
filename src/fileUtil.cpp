#include <string>
#include "fileUtil.h"
void util::createMast(parser::fileFormat::file fileA, std::vector<parser::fileFormat::file> masters){
	(unsigned int)fileA.flags = (unsigned int)fileA.flags | 1;
	std::string ids;
	bool test = false;
	for(unsigned int i = 0; i < fileA.groups.length(); ++i){
		for(unsigned int j = 0; j < fileA.groups[i].records.length(); ++j){
			for(unsigned int k = 0; k < masters.groups.length(); ++k){
				if(!test){
					for(unsigned int l = 0; l < masters.groups[k].records.length(); ++l){
						if(fileA.groups[i].records[j].ID == masters.groups[k].records[l].ID){
							ids += fileA.groups[i].records[j].ID;
							test = true;
							break;
						}
					}
				}
			}
			test = false;
		}
	}
	ids += 0;
}
