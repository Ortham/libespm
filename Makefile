OBJECTS =																			src/skyrimParserRedoRel.o src/commonSupport.o src/parser.o src/parserClasses.o src/viewer.o
OBJECTS2 =																			src/commonSupport.cpp src/parser.cpp src/parserClasses.cpp src/viewer.cpp
all :																				Parse Parse.exe
Parse :																				$(OBJECTS) init
																					g++ -o Parse $(OBJECTS)
																					mv $(OBJECTS) Objects
Parse.exe :																			src/skyrimParserRedoRel.cpp $(OBJECTS) init
																					i686-w64-mingw32-g++ -o Parse.exe src/skyrimParserRedoRel.cpp $(OBJECTS2) -static
init :
																					mkdir -p Objects
src/skyrimParserRedoRel.o src/commonSupport.o src/parser.o src/viewer.o :			src/constants.h
src/skyrimParserRedoRel.o :															src/commonSupport.h src/parserClasses.h src/viewer.h
src/commonSupport.o src/viewer.o :													src/commonSupport.h
src/parser.o src/parserClasses.o :													src/parser.h
src/parserClasses.o :																src/parserClasses.h
src/viewer.o :																		src/viewer.h
Release :																			Parse
																					7z a -t7z -m0=lzma2 -mx=9 -ms=on -i@fileList Parser.7z
.PHONY :																			clean
clean :
																					rm Parse $(OBJECTS) Parser.7z
																					rm -f -r Objects
