// --------------------------------------------------------------------------
//
// test.h
//
// CS5130 Advanced Algorithms
//
// Austin Hester, November 2023
//
// --------------------------------------------------------------------------



#ifndef TEST_H
#define TEST_H


#include "process_news.h"


/* start_test();
 *
 * Displays Test Report on screen, testing hard-coded filenames.
 * 
 * Output: Pass/Fail (0/1).
 */
int start_test();

/* _test_files(inputFileNames[], expects[], numTestFiles);
 *
 * Input: A list of input filenames to test and their expected scores.
 * Output: Pass/Fail (0/1).
 */
int _test_files(const char** inputFileNames, const int* expects, const int numTestFiles);

#endif
