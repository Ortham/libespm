#pragma once
namespace parser{
	namespace fileFormat{
		/**
		 * @struct TES_Format
		 * @brief The plugin format.
		 * @details This contains the data for the plugin files in a fairly generic format to allow the files for past and
		 * future games to be easily parsed with little to no changes.
		 */
		struct TES_Format{
			/*These two numbers will need to be changed to non-hardcoded numbers in order to accomodate format changes.
			 *Assuming HEDR remains constant, which there is a good chance of, this is fairly trivial.
			 *<obj_name>.read(<var>, <size>) will read in the data to the variables after the initial parse of the main file to determine the individual values.
			 *The family of ato<desired>(<have>) is used to convert it to the proper format.
			 *For the initial parse, we go through and run a count between some predefined constants and use the count as the amount of data to read in.
			 *The predefined constants will probably correspond somewhat to the initial parse of the main file that we use to get just the record listings.
			 *After we have the listings, we could go through and have the system automatically create the corresponding variables for them and then import the
			 *generated code into the main program.
			 *...
			 *This may actually work to parse through all their past and future titles autonomously.
			 *
			 *However, this won't take the data-types into account. That's a different problem. The big question is, can it take the data-types into account on its own?
			 *For Skyrim, CNAM and MAST are followed by 2 hex bits that denote the size of the data including the null character.
			 *However, this may be different for past and future games so this is something that shouldn't be relied upon.
			 *The only thing that can be relied upon is that the records themselves will most likely have the same format (3 or 4 capital letters) across all games.
			 *
			 *
			 *The problem with figuring out data-types has been solved, kind of. Check every bit of data and see if it's a printable character and check the
			 *last one as denoted by the record size to make sure it's null.
			 */
			unsigned char headerString[4];
			unsigned short recordSize;
			unsigned int stuff;
			unsigned char HEDR[4];
			unsigned short HEDRSize;
			unsigned int moreStuff;
			unsigned char CNAM[4];
			unsigned short CNAM_size;
			string author;
			unsigned char MAST[4];
			unsigned short MAST_size;
			string masterFile;
			unsigned char DATA[4];
			unsigned short DATA_size;
			unsigned int moreStuff4;
			unsigned char INTV[4];
			unsigned short INTV_size;
			unsigned int moreStuff5;
		};
		struct BASE_FORMAT{
			unsigned char header[4];
			unsigned short size;
			unsigned int stuff;
		}
	}
}
