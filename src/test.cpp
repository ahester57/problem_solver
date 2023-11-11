// --------------------------------------------------------------------------
//
// test.cpp
//
// CS5130 Advanced Algorithms
//
// Austin Hester, November 2023
//
// --------------------------------------------------------------------------

#include <string>
#include "test.h"


/* start_test();
 *
 * Displays Test Report on screen.
 * 
 * Output: Pass/Fail (0/1).
 */
int start_test()
{
    const char* inputFileNames[] = {
        "examples/ex1.txt",
        "examples/ex2.txt",
        "examples/ex3.txt",
        "examples/ex4.txt",
        "examples/ex5.txt",
        "examples/ex6.txt",
        "examples/ex7.txt",
        "examples/ex8.txt",
        "examples/ex9.txt",
        "examples/ex10.txt",
        "examples/ex11.txt",
        "examples/ex12.txt",
        "examples/ex13.txt"
    };

    const int expects[] = {
        551,
        580,
        1028,
        870,
        1478,
        1431,
        1107,
        1069,
        1007,
        337,
        551,
        423,
        433,
    };

    return _test_files(inputFileNames, expects, 13);
}


/* _test_files(inputFileNames[], expects[], numTestFiles);
 *
 * Input: A list of input filenames to test and their expected scores.
 * Output: Pass/Fail (0/1).
 */
int _test_files(const char** inputFileNames, const int* expects, const int numTestFiles)
{
    timer t;
    t.start("Timer started.");

    // Disable standard-out. (https://stackoverflow.com/a/30185095)
    std::cout.setstate(std::ios_base::failbit);

    bool overall_truth = 0;
    for (int i = 0; i < numTestFiles; i++) {
        NewsProgram news = NewsProgram();
        int actualArticleCount = process_file(inputFileNames[i], &news);
        cleanupNews(&news, actualArticleCount);
        bool this_truth = actualArticleCount == news.numArticles;
        if (!this_truth) {
            std::cerr << __FUNCTION__ << " failed on line " << __LINE__
                        << ": actualArticleCount == news.numArticles failed" << std::endl
                        << "\tFor inputFileName='" << inputFileNames[i] << "'" << std::endl;
        }
        // TODO scoring
    }

    // Re-enable standard-out.
    std::cout.clear();

    // Stop time
    t.stop("\nTimer stopped.");
    double compTime;
    compTime = t.timeVal();

    int seconds = 0;
    int min = 0;
    int hour = 0;

    seconds = (int) compTime % 60;
    min = (int) compTime / 60;
    hour = min / 60;
    min = min % 60;

    std::cout << "Computation Time: " << hour << " hours, " << min << " minutes, " << seconds << " seconds.\n" << std::endl;

    return overall_truth;
}
