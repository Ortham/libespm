WOPTS =									-I/usr/i686-w64-mingw32/sys-root/mingw/include \
										-L/usr/i686-w64-mingw32/sys-root/mingw/lib


OBJDIR =								obj


SRCDIR =								src


OBJECTS =								$(OBJDIR)/skyrimParserRedoRel.o \
										$(OBJDIR)/commonSupport.o \
										$(OBJDIR)/parser.o \
										$(OBJDIR)/parserClasses.o \
										$(OBJDIR)/viewer.o


OBJECTS2 =								$(SRCDIR)/commonSupport.cpp \
										$(SRCDIR)/parser.cpp \
										$(SRCDIR)/parserClasses.cpp \
										$(SRCDIR)/viewer.cpp


TESTOBJ =								$(OBJDIR)/commonSupport.o \
										$(OBJDIR)/fileFormat.o \
										$(OBJDIR)/parser.o \
										$(OBJDIR)/test.o


TESTOBJ2 =								$(SRCDIR)/commonSupport.cpp \
										$(SRCDIR)/fileFormat.cpp \
										$(SRCDIR)/parser.cpp


$(OBJDIR)/%.o :							$(SRCDIR)/%.cpp
										g++ -c -o $@ $<
$(OBJECTS) \
$(TESTOBJ1) :							| $(OBJDIR)
$(OBJDIR) :
										mkdir $(OBJDIR)


all :									Parse Parse.exe
Parse :									$(OBJECTS)
										g++ $^ -o $@
Parse.exe :								$(SRCDIR)/skyrimParserRedoRel.cpp $(OBJECTS2)
										i686-w64-mingw32-g++ $^ -o $@ -static
Test :									$(TESTOBJ) -lz
										g++ $^ -o $@
Test.exe :								$(SRCDIR)/test.cpp $(TESTOBJ2)
										i686-w64-mingw32-g++ $^ $(WOPTS) -lz -o $@ -static

$(OBJDIR)/commonSupport.o \
$(OBJDIR)/parser.o \
$(OBJDIR)/skyrimParserRedoRel.o \
$(OBJDIR)/viewer.o :					$(SRCDIR)/constants.h

$(OBJDIR)/commonSupport.o \
$(OBJDIR)/fileFormat.o \
$(OBJDIR)/skyrimParserRedoRel.o \
$(OBJDIR)/test.o \
$(OBJDIR)/viewer.o :					$(SRCDIR)/commonSupport.h

$(OBJDIR)/fileFormat.o \
$(OBJDIR)/test.o :						$(SRCDIR)/fileFormat.h

$(OBJDIR)/fileFormat.o \
$(OBJDIR)/parser.o \
$(OBJDIR)/parserClasses.o :				$(SRCDIR)/parser.h

$(OBJDIR)/parserClasses.o \
$(OBJDIR)/skyrimParserRedoRel.o :		$(SRCDIR)/parserClasses.h

$(OBJDIR)/skyrimParserRedoRel.o \
$(OBJDIR)/viewer.o :					$(SRCDIR)/viewer.h

Release :								Parse
										7z a -t7z -m0=lzma2 -mx=9 -ms=on -i@fileList Parser.7z
.PHONY :								clean
clean :
										rm -f Parse $(OBJECTS) Parser.7z Parse.exe
										rm -f Test Test.exe $(TESTOBJ)
										rm -f -r $(OBJDIR)
