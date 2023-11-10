# Makefile for P2
# Austin Hester

# Repo directories
INCDIR=inc
SRCDIR=src
OBJDIR=obj
TARGETDIR=bin

# Libraries
LIBS=-lstdc++

# Target
TARGET=P2

# Compiler options
CC=g++
CFLAGS=-g -Wall -std=c++11
CFLAGS+= -I $(INCDIR)
CFLAGS+= $(LIBS)
#DEPS = $(wildcard $(INCDIR)/*.hpp)
SOURCES= $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, %.o, $(SOURCES))

$(info OBJECTS: $(OBJECTS))
$(info SOURCES: $(SOURCES))
$(info DEPS: $(DEPS))

.PHONY: all clean

default: all

all: $(TARGET)

#TODO put objects in OBJDIR
$(OBJECTS): $(SOURCES)
	${CC} -c $^ -I $(INCDIR)

#TODO put executable in $(TARGETDIR)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJECTS) $(wildcard $(INCDIR)/*.gch)

destroy:
	$(RM) $(OBJECTS) P2 P2.exe $(wildcard $(INCDIR)/*.gch)

# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
