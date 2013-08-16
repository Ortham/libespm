/*
 * fileUtil.cpp
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

#include <cstdlib>
#include "commonSupport.h"
#include "fileUtil.h"
void util::createMast(espm::file &File){
	File.flags |= strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0);
}
void util::createONAM(espm::file &File, std::vector<espm::file> masters){
	std::string ids;
	bool test = false;
	for(unsigned long long i = 0; i < File.items.size(); ++i){
		for(unsigned long long j = 0; j < File.items[i].group.records.size(); ++j){
			if(common::options::contains("ONAM", File.items[i].group.records[j].name)){
				for(unsigned long long k = 0; k < masters.size(); ++k){
					for(unsigned long long l = 0; l < masters[k].items.size(); ++l){
						for(unsigned long long m = 0; m < masters[k].items[l].group.records.size(); ++m){
							if(File.groups[i].records[j].ID == masters[k].items[l].group.records[m].ID){
								ids += File.items[i].group.records[j].ID;
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
	std::vector<espm::record>::iterator it = File.records.begin();
	while((*it).name != "INTV" || (*it).name != "INCC" || it != File.records.end())
		++it;
	espm::record ONAM;
	ONAM.name = "ONAM";
	ONAM.size = ids.length() + 1;
	ONAM.data = ids.c_str();
	File.records.insert(it, ONAM);
}
void util::revCreateMast(espm::file &File){
	File.flags ^= strtoul(common::structVals[common::options::game]["MastFlag"][0].c_str(), NULL, 0);
}
void util::revCreateONAM(espm::file &File){
	std::vector<espm::record>::iterator it = File.records.begin();
	while((*it).name != "ONAM" || it != File.records.end())
		++it;
	File.records.erase(it);
}
/*END OF LINE*/
