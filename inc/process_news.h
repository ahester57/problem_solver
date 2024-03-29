// --------------------------------------------------------------------------
//
// process_news.h
//
// CS5130 Advanced Algorithms
//
// Austin Hester, November 2023
//
// --------------------------------------------------------------------------



#ifndef PROCESS_NEWS_H
#define PROCESS_NEWS_H


#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <string.h>

#include "p2.h"
#include "news.h"


/* process_file(inputFileName, news*);
 *
 * Input: An input filename and a news program to build.
 * Output: The number of actual rows loaded.
 */
int process_file(const char* inputFileName, NewsProgram* news);

/* explore_solution_space(news, plausible solution*);
 *
 * Input: An initialized news plan and a dummy solution.
 * Output: Modifies incumbent to optimal while returning whether any feasible solution was found.
 */
bool explore_solution_space(const NewsProgram news, PlausibleSolution* incumbent);

#endif
