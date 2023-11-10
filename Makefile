# Makefile for P1
# Austin Hester

# Repo directories
INCDIR=src
SRCDIR=src
OBJDIR=obj
TARGET  = P1
TARGETDIR=bin

# Compiler options
CC=g++
CFLAGS=-g -Wall -std=c++11 -lstdc++
CFLAGS+= -I $(INCDIR)
CFLAGS+= $(OPENCV_CFLAGS)
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
	${CC} -c $^

#TODO put executable in $(TARGETDIR)
$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OBJECTS) $(wildcard $(INCDIR)/*.gch)

destroy:
	$(RM) $(OBJECTS) P1 P1.exe $(wildcard $(INCDIR)/*.gch)

# https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
