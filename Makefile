#OBJECTS1 =						src/viewerRedoRel.o
OBJECTS2 =						src/skyrimParserRedoRel.o
#all :							Viewer Viewer.exe Parse Parse.exe
all:							Parse Parse.exe
#Viewer :						$(OBJECTS1)
#								g++ -o Viewer $(OBJECTS1)
#Viewer.exe :					src/viewerRedoRel.cpp
#								i686-pc-mingw32-g++ -o Viewer.exe src/viewerRedoRel.cpp -static
#src/viewerRedoRel.o :			src/commonSupport.h
Parse :							$(OBJECTS2)
								g++ -o Parse $(OBJECTS2)
Parse.exe :						src/skyrimParserRedoRel.cpp $(OBJECTS2)
								i686-pc-mingw32-g++ -o Parse.exe src/skyrimParserRedoRel.cpp -static
src/skyrimParserRedoRel.o :		src/commonSupport.h src/parser.h src/viewer.h
.PHONY :						clean
clean :
								rm Parse Parse.exe
