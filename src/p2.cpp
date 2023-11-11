// --------------------------------------------------------------------
//
// p2.cpp
//
// CS5130 Advanced Algorithms
//
// Austin hester, Nov. 2023
//
// Adapted from:
//
// Sharlee Climer, Aug. 2023
//
// ---------------------------------------------------------------------

#include <bitset>
/* Re-enable once Clark upgrades to gcc >= 4.9.0
#include <regex>
*/

#include "p2.h"
#include "process_news.h"
#include "news.h"
#include "test.h"


int main(int argc, char** argv) 
{
    if (argc != 2) {
        fatal("\n  Usage:\n    P2 inputFileName.txt | --test  (input file contains list of proposed articles)\n\n");
        exit(1);
    }

    const char* inputFileName = argv[1];

    if (strcmp(inputFileName, "--test") == 0) {
        exit(start_test());
    }

    timer t;
    t.start("Timer started.");

    // new code added here
    char outputFileName[STRING_LENGTH];
    char base[STRING_LENGTH-4]; // base for output file names
    const char outSuffix[] = ".out"; 
      
    // determine output file name
    for (int i = 0; i < STRING_LENGTH; i++) {
        base[i] = inputFileName[i]; // copy input file name
        if (base[i] == '\0') {
            if(i < 5) {
                fatal("Expected output file name to have at least 5 characters");
            }

            base[i-4] = '\0'; // set end of string to not include 4 char suffix 
            break;
        }
    } 
    sprintf(outputFileName, "%s%s", base, outSuffix); // now you can open outputFileName and write out the information

    // Print CLAs unless quiet mode
    if (!QUIET) {
        std::cout << "\nCommand line arguments: \n\t";
        for (int i = 0; i < argc; i++) {
            std::cout << argv[i] << " ";
        }
        std::cout << "\n\n";
        std::cout << "Input will be read from '" << inputFileName << "'.\n";
    }

    // Initialize the News Collection, which holds the proposed articles.
    NewsProgram news = NewsProgram();

    // Process the file.
    // actualArticleCount holds the actual number read line-by-line from the file.
    int actualArticleCount = process_file(inputFileName, &news);


    // Display info before processing
    std::cout << "Number of Articles:  " << actualArticleCount << std::endl;
    std::cout << "Number of Reporters: " << news.numReporters << std::endl;

    // Verify Integrity
    if (actualArticleCount != news.numArticles) {
        cleanupNews(&news, actualArticleCount);
        fatal("Loaded article count does not match header.");
    }

    // Generate and score all possible solutions
    PlausibleSolution incumbent = PlausibleSolution();
    bool anyFeasibleFound = explore_solution_space(news, &incumbent);

    // Stop time
    t.stop("\nTimer stopped.");
    double compTime;
    compTime = t.timeVal();

    int seconds = 0;
    int min = 0;
    int hour = 0;

    seconds = (int)compTime % 60;
    min = (int)compTime / 60;
    hour = min / 60;
    min = min % 60;

    if (!QUIET) {
        // Print output
        if (anyFeasibleFound) {
            std::cout << "----" << std::endl << "Incumbent Solution:\t" << incumbent.solution;
            std::cout << std::endl << "----" << std::endl;
            std::cout << "Article\tType\tReporter\tCost\tClicks" << std::endl;
            // Loop over our articles
            for (int i = 0; i < news.numArticles; i++) {
                if (incumbent.solution.test(i)) {
                    Article* a = news.articles[i];
                    printf("%s\t%s\t%d\t\t%d\t%d\n", a->id.c_str(), a->type.c_str(), a->reporter, a->cost, a->clicks);
                }
            }
            std::cout << "Total Cost:\t\t" << incumbent.score << std::endl;
            std::cout << "Total Clicks:\t\t" << incumbent.totalClicks << std::endl;

            std::cout << "Type Counts:" << std::endl;
            std::cout << "\tGlobal:\t\t" << incumbent.typeCounts["G"] << std::endl;
            std::cout << "\tLocal:\t\t" << incumbent.typeCounts["L"] << std::endl;
            std::cout << "\tScience:\t" << incumbent.typeCounts["S"] << std::endl;
            std::cout << "\tEntertainment:\t" << incumbent.typeCounts["E"] << std::endl;

            std::cout << "Reporter Counts:" << std::endl;
            for (int i = 1; i <= news.numReporters; i++) {
                std::cout << "\t\t" << i << ":\t" << incumbent.reporterCounts[i] << std::endl;
            }
        } else {
            std::cout << "----" << std::endl << "No Feasible Solution Found";
            std::cout << std::endl << "----" << std::endl;
        }
        std::cout << "Computation Time: " << hour << " hours, " << min << " minutes, " << seconds << " seconds.\n" << std::endl;
    }

    // Output to file
    FILE* outputFile;

    if ((outputFile = fopen(outputFileName, "w+")) == NULL) {
        // Cleanup dynamically created data
        cleanupNews(&news, news.numArticles);
        fatal("Unable to open output file.");
    }

    fprintf(outputFile, "Article\tType\tReporter\tCost\tClicks\n");
    // Loop over our articles
    for (int i = 0; i < news.numArticles; i++) {
        if (incumbent.solution.test(i)) {
            Article* a = news.articles[i];
            fprintf(outputFile, "%s\t\t%s\t\t%d\t\t\t%d\t\t%d\n", a->id.c_str(), a->type.c_str(), a->reporter, a->cost, a->clicks);
        }
    }
    fprintf(outputFile, "Total Cost: %d\n", incumbent.score);
    fprintf(outputFile, "Total Clicks: %d\n", incumbent.totalClicks);
    fprintf(outputFile, "Computation Time: %d hours, %d minutes, %d seconds.\n", hour, min, seconds);

    fclose(outputFile);

    std::cout << "Output successfully to " << outputFileName << std::endl;

    // Cleanup dynamically created data
    cleanupNews(&news, news.numArticles);

    return 0;
}

