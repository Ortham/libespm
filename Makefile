OBJECTS =							Objects/skyrimParserRedoRel.o \
									Objects/commonSupport.o \
									Objects/parser.o \
									Objects/parserClasses.o \
									Objects/viewer.o
																						
																						
OBJECTS2 =							src/commonSupport.cpp \
									src/parser.cpp \
									src/parserClasses.cpp \
									src/viewer.cpp
																						
																						
TESTOBJ =							Objects/commonSupport.o \
									Objects/fileFormat.o \
									Objects/test.o
																					
																					
TESTOBJ2 =							src/commonSupport.cpp \
									src/fileFormat.cpp


all :								Parse Parse.exe
Parse :								init $(OBJECTS)
									g++ -o Parse $(OBJECTS)
Parse.exe :							src/skyrimParserRedoRel.cpp
									i686-w64-mingw32-g++ -o Parse.exe src/skyrimParserRedoRel.cpp $(OBJECTS2) -static
Test :								init $(TESTOBJ)
									g++ -o Test $(TESTOBJ)
Test.exe :							src/test.cpp
									i686-w64-mingw32-g++ -o Test.exe src/test.cpp $(TESTOBJ2) -static
init :
									mkdir -p Objects																					
Objects/%.o :						src/%.cpp
									g++ -c -o $@ $<																					
Objects/skyrimParserRedoRel.o \
Objects/commonSupport.o \
Objects/parser.o \
Objects/viewer.o :					src/constants.h
Objects/skyrimParserRedoRel.o :		src/commonSupport.h \
									src/parserClasses.h \
									src/viewer.h
Objects/commonSupport.o \
Objects/viewer.o \
Objects/test.o \
Objects/fileFormat.o :				src/commonSupport.h
Objects/parser.o \
Objects/parserClasses.o :			src/parser.h
Objects/parserClasses.o :			src/parserClasses.h
Objects/test.o :					src/fileFormat.h
Objects/viewer.o :					src/viewer.h
Release :							Parse
									7z a -t7z -m0=lzma2 -mx=9 -ms=on -i@fileList Parser.7z
.PHONY :							clean
clean :
									rm -f Parse $(OBJECTS) Parser.7z Parse.exe
									rm -f Test Test.exe $(TESTOBJ)
									rm -f -r Objects
