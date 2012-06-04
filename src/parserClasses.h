/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file parserClasses.h
 * @brief Contains the parser specific classes (such as the file we're working with).
 * @details Holds the objects for the parser to work with, such as files, so that we can work with multiple files in a single run and not need to re-run the program.
 * @attention Do not include parser.h if using this file.
 */
#pragma once
#include "parser.h"
namespace parser{
	class GameFile{
		private:
			/**
			 * @var fileName
			 * The file name.
			 */
			std::string fileName;
			/**
			 * @var header
			 * The file header.
			 */
			std::string header;
		public:
			/**
			 * @brief Checks whether or not a file is a BSA.
			 * @details It checks the file's header to see if it matches "BSA", which is the current header for BSAs.
			 * @param &file
			 * The file to be checked.
			 * @returns <tt> \b true </tt> if the file is a BSA, <tt> \b false </tt> otherwise.
			 */
			bool isBSA(std::ifstream &file);
			/**
			 * @brief Checks whether or not a file has the ESM extension;
			 * @details It checks the file's extension to see if it matches ".esm"
			 * @returns <tt> \b true </tt> if the file has the ESM extension, <tt> \b false </tt> otherwise.
			 */
			bool isESM();
			/**
			 * @brief Checks whether or not a file has the ESP extension;
			 * @details It checks the file's extension to see if it matches ".esp"
			 * @returns <tt> \b true </tt> if the file has the ESP extension, <tt> \b false </tt> otherwise.
			 */
			bool isESP();
			/**
			 * @brief Checks whether or not a file has the ESS extension;
			 * @details It checks the file's extension to see if it matches ".ess"
			 * @returns <tt> \b true </tt> if the file has the ESS extension, <tt> \b false </tt> otherwise.
			 */
			bool isESS();
			/**
			 * @brief Checks whether or not a file is a mod (plugin).
			 * @details It checks the file's header to see if it matches "TES4" or "TES3", which are the current headers for mod files.
			 * @param &file
			 * The file to be checked.
			 * @returns <tt> \b true </tt> if the file is a mod, <tt> \b false </tt> otherwise.
			 */
			bool isMod(std::ifstream &file);
			/**
			 * @brief Checks whether or not a file is a saved game.
			 * @details It checks the file's header to see if it matches "TESV_SAVEGAME" or "TES4SAVEGAME", which are the current headers for saved game files.
			 * @param &file
			 * The file to be checked.
			 * @returns <tt> \b true </tt> if the file is a saved game, <tt> \b false </tt> otherwise.
			 */
			bool isSave(std::ifstream &file);
			/**
			 * @brief Gets the file header.
			 * @details Is here to allow for future compartmentalization.
			 * @returns The file header.
			 * @todo Set things up as an object for each file, possibly, to allow for multiple uses of various functions without causing problems.
			 */
			inline std::string getFileHeader();
			/**
			 * @brief Gets the file name.
			 * @details Is here to allow for future compartmentalization.
			 * @returns The file name.
			 * @todo Set things up as an object for each file, possibly, to allow for multiple uses of various functions without causing problems.
			 */
			inline std::string getFileName();
			/**
			 * @brief Sets the file header.
			 * @details Is here to allow for future compartmentalization.
			 * @param &file
			 * The file to extract the header from.
			 */
			inline void setFileHeader(std::ifstream &file);
			/**
			 * @brief Sets the file name
			 * @details Is here to allow for future compartmentalization.
			 * @param name
			 * The name of the file to store.
			 */
			inline void setFileName(std::string name);
	};
	inline std::string GameFile::getFileHeader(){
		return header;
	}
	inline std::string GameFile::getFileName(){
		return fileName;
	}
	inline void GameFile::setFileHeader(std::ifstream &file){
		getline(file, header);
	}
	inline void GameFile::setFileName(std::string name){
		fileName = name;
	}
	/*END OF LINE*/
}
