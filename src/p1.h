// --------------------------------------------------------------------------
//
// p1.h
//
// CS5130 Advanced Algorithms
//
// Sharlee Climer, August 2023
//
// --------------------------------------------------------------------------



#ifndef HELLO_H
#define HELLO_H


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>

#include "timer.h"

const bool VERBOSE = 0; // set to '1' for a lot of output to screen, '0' otherwise
const bool QUIET = 0; // set to '1' to minimize output to screen, '0' otherwise 

const int STRING_LENGTH = 100; // number of chars allocated for each string
const bool LOG_FILE = 1; // set to 1 to print out logfile


inline void warning(const char* p) { fprintf(stderr,"Warning: %s \n",p); }
inline void fatal(const char* string) {fprintf(stderr,"\n\nFatal: %s\n\n",string); exit(1); }



#endif
