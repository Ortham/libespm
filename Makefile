#OBJECTS1 =																			src/viewerRedoRel.o
OBJECTS =																			src/skyrimParserRedoRel.o src/commonSupport.o src/parser.o src/parserClasses.o src/viewer.o
OBJECTS2 =																			src/commonSupport.cpp src/parser.cpp src/parserClasses.cpp src/viewer.cpp
#all :																				Viewer Viewer.exe Parse Parse.exe
all :																				Parse Parse.exe
#all :																				Parse Release
#Viewer :																			$(OBJECTS1)
#																					g++ -o Viewer $(OBJECTS1)
#Viewer.exe :																		src/viewerRedoRel.cpp
#																					i686-pc-mingw32-g++ -o Viewer.exe src/viewerRedoRel.cpp -static
#src/viewerRedoRel.o :																src/commonSupport.h
Parse :																				$(OBJECTS)
																					g++ -o Parse $(OBJECTS)
Parse.exe :																			src/skyrimParserRedoRel.cpp $(OBJECTS)
																					i686-pc-mingw32-g++ -o Parse.exe src/skyrimParserRedoRel.cpp $(OBJECTS2) -static
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
