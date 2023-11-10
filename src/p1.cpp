// --------------------------------------------------------------------
//
// p1.cpp
//
// CS5130 Advanced Algorithms
//
// Austin hester, Oct. 2023
//
// Adapted from:
//
// Sharlee Climer, Aug. 2023
//
// ---------------------------------------------------------------------

#include <bitset>
#include <cmath>
#include <limits.h>
#include <map>
/* Re-enable once Clark upgrades to gcc >= 4.9.0
#include <regex>
*/

#include "p1.h"
#include "news.h"



int main(int argc, char ** argv) 
{
    if (argc != 2) {
        fatal("Usage:\n\n   P1 inputFileName.txt  (input file contains list of proposed articles\n\n");
    }

    timer t;
    t.start("Timer started.");

    char* inputFileName = argv[1];  
    FILE* inputFile;

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

    // read in names and print messages
    if ((inputFile = fopen(inputFileName, "r")) == NULL) {
        fatal("Unable to open input file.");
    }

    char tempStrng[STRING_LENGTH]; // string for temporary storage

    /* Re-enable once Clark upgrades to gcc >= 4.9.0
    // Define the regex expressions
    std::regex const countsRegex("^(\\d+)\\s+articles\\s+and\\s+(\\d+)\\s+reporters");
    std::regex const clicksRegex("^(\\d+)\\s+clicks\\s+or\\s+more\\s+required");
    std::regex const articleRegex("^(\\w+)\\s+(\\w+)\\s+(\\d+)\\s+(\\d+)\\s+(\\w+)");
    */

    int lineCount = 0; // hard-coding this since Clark does not support regex

    // Initialize the News Collection, which holds the proposed articles.
    NewsProgram news = NewsProgram();
    int articleCount = 0;

    // Main loop
    while (true) {


        if (feof(inputFile)) {
            break;
        }

        fgets(tempStrng, sizeof(tempStrng), inputFile);

        lineCount++;

        /* Re-enable once Clark upgrades to gcc >= 4.9.0
        std::smatch match; // holds regex group matches
        std::string str(tempStrng); // std::string for the regex searches
        */

        /* Re-enable once Clark upgrades to gcc >= 4.9.0
        if (news.numArticles == -1 && std::regex_search(str, match, countsRegex)) {
        */
        if (lineCount == 1) { // hard-coding since Clark does not support regex
            // Header
            printf("Processing Header\n");
            //for (auto v : match) std::cout << v << "," << std::endl;

            sscanf(tempStrng, "%d articles and %d reporters\n", &news.numArticles, &news.numReporters);

            /* Re-enable once Clark upgrades to gcc >= 4.9.0
            news.numArticles = std::stoi(match[1]);
            news.numReporters = std::stoi(match[2]);
            */

            // Allocate space for the given number of articles
            if (news.numArticles > 0) {
                news.articles = (Article**) malloc(news.numArticles * sizeof(Article*));
            }

            std::cout << "Number of Articles: " << news.numArticles << std::endl;
            std::cout << "Number of Reporters " << news.numReporters << std::endl;

        /* Re-enable once Clark upgrades to gcc >= 4.9.0
        } else if (news.minClicks == -1 && std::regex_search(str, match, clicksRegex)) {
        */
        } else if (lineCount == 2) {
            // Required clicks
            printf("Processing Required Clicks\n");

            sscanf(tempStrng, "%d clicks or more required\n", &news.minClicks);

            /* Re-enable once Clark upgrades to gcc >= 4.9.0
            news.minClicks = std::stoi(match[1]);
            */

            std::cout << "Minimum Clicks: " << news.minClicks << std::endl;

        /* Re-enable once Clark upgrades to gcc >= 4.9.0
        } else if (std::regex_search(str, match, articleRegex)) {
        */
        } else if (lineCount > 3) {
            // (DATA) Article Line
            //printf("Processing Data\n");
            //for (auto v : match) std::cout << v << "," << std::endl;

            // Initialize the new article
            Article* a = new Article();

            char id[STRING_LENGTH];
            char type[STRING_LENGTH];
            sscanf(tempStrng, "%s %s %d %d %d\n", id, type, &a->reporter, &a->cost, &a->clicks);
            a->id = id;
            a->type = type;

            /* Re-enable once Clark upgrades to gcc >= 4.9.0
            a->id = std::string(match[1]);
            a->type = std::string(match[2]);
            a->reporter = std::stoi(match[3]);
            a->cost = std::stoi(match[4]);
            a->clicks = std::stoi(match[5]);
            */

            // Load new article into NewsProgram
            news.articles[articleCount] = a;
            articleCount++;
            //printf("%s %s %d %d %d\n", a->id.c_str(), a->type.c_str(), a->reporter, a->cost, a->clicks);

            // Check if this is last article
            if (articleCount >= news.numArticles) {
                if (VERBOSE) {
                    warning("Article count satisfied.");
                }
                break;
            }

        } else {
            // Extraneous data
            std::cout << "Extraneous data received: " << tempStrng << std::endl;
        }

        //printf("Hello %s!\n", tempStrng);
    }

    // Close input file
    fclose(inputFile);

    // Display info before processing
    std::cout << "Number of Articles: " << articleCount << std::endl;
    std::cout << "Number of Reporters " << news.numReporters << std::endl;

    // Verify Integrity
    if (articleCount != news.numArticles) {
        cleanupNews(&news, articleCount);
        fatal("Loaded article count does not match header.");
    }

    // Generate and score all possible solutions
    PlausibleSolution incumbent = {0, INT_MAX};
    long numPossibilities = pow(2, news.numArticles);
    for (long i = 0; i < numPossibilities; i++) {

        // Create a bitset to represent our choices
        PlausibleSolution soln = {};
        soln.solution = std::bitset<BIT_LENGTH>(i);

        // Score the solution against the proposed articles
        scoreSolution(&soln, news);

        if (VERBOSE) {
            std::cout << "----" << std::endl;
            std::cout << soln.solution << std::endl;
            std::cout << "Score:\t\t" << soln.score << std::endl;
            std::cout << "Clicks:\t\t" << soln.totalClicks << std::endl;
        }

        // Skip if worse than incumbent
        if (soln.score > incumbent.score) {
            continue;
        }
        // Past that, so we are better than the incumbent

        // Check feasibility now
        checkFeasible(&soln, news);

        if (VERBOSE) {
            std::cout << "Feasible:\t" << soln.isFeasible << std::endl;
        }

        if (soln.isFeasible) {
            // At this point, we can update the current to the next incumbent
            incumbent.solution = soln.solution;
            incumbent.score = soln.score;
            incumbent.totalClicks = soln.totalClicks;
            incumbent.typeCounts = soln.typeCounts;
            incumbent.reporterCounts = soln.reporterCounts;
        }
    }


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

