OBJECTS =																			src/skyrimParserRedoRel.o src/commonSupport.o src/parser.o src/parserClasses.o src/viewer.o
OBJECTS2 =																			src/commonSupport.cpp src/parser.cpp src/parserClasses.cpp src/viewer.cpp
TESTOBJ =																			src/commonSupport.o src/fileFormat.o test.o
TESTOBJ2 =																			src/commonSupport.cpp src/fileFormat.cpp
all :																				Parse Parse.exe
Parse :																				$(OBJECTS) init
																					g++ -o Parse $(OBJECTS)
																					mv $(OBJECTS) Objects
Parse.exe :																			src/skyrimParserRedoRel.cpp init
																					i686-w64-mingw32-g++ -o Parse.exe src/skyrimParserRedoRel.cpp $(OBJECTS2) -static
Test :																				$(TESTOBJ) init
																					g++ -o Test $(TESTOBJ)
																					mv $(TESTOBJ) Objects
Test.exe :																			test.cpp init
																					i686-w64-mingw32-g++ -o Test.exe test.cpp $(TESTOBJ2) -static
init :
																					mkdir -p Objects
src/skyrimParserRedoRel.o src/commonSupport.o src/parser.o src/viewer.o :			src/constants.h
src/skyrimParserRedoRel.o :															src/commonSupport.h src/parserClasses.h src/viewer.h
src/commonSupport.o src/viewer.o test.o src/fileFormat.o :							src/commonSupport.h
src/parser.o src/parserClasses.o :													src/parser.h
src/parserClasses.o :																src/parserClasses.h
test.o :																			src/fileFormat.h
src/viewer.o :																		src/viewer.h
Release :																			Parse
																					7z a -t7z -m0=lzma2 -mx=9 -ms=on -i@fileList Parser.7z
.PHONY :																			clean
clean :
																					rm -f Parse $(OBJECTS) Parser.7z Parse.exe
																					rm -f Test Test.exe $(TESTOBJ)
																					rm -f -r Objects
