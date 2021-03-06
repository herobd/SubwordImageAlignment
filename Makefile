#############################################################################
# Makefile for building: SubwordImageAlignment
# Generated by qmake (3.0) (Qt 5.2.1)
# Project:  SubwordImageAlignment.pro
# Template: app
#############################################################################

MAKEFILE      = Makefile.Debug

####### Compiler, tools and options

CC            = gcc
CXX           = g++-4.9
DEFINES       = 
CFLAGS        = --g -Wall -W -fPIE $(DEFINES)
CXXFLAGS      = -std=c++11 -g -Wall -W -fPIE $(DEFINES)
INCPATH       = -I/usr/lib/x86_64-linux-gnu/qt5/mkspecs/linux-g++-64 -I. -I./familysearch_documentproject_2013.08.30/src
LINK          = g++
LIBS          = $(SUBLIBS) -L./familysearch_documentproject_2013.08.30/lib/ -ldocumentproj_2013.08.30 -ljpeg -ltiff -lpng -lpthread 

####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = main.cpp 
OBJECTS       = obj/main.o
TARGET        = SubwordImageAlignment


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean:
	- rm -f $(TARGET)
	- rm -f $(OBJECTS)




####### Compile

obj/main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/main.o main.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

