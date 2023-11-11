// --------------------------------------------------------------------------
//
// process_news.cpp
//
// CS5130 Advanced Algorithms
//
// Austin Hester, November 2023
//
// --------------------------------------------------------------------------

#include <cmath>
#include <limits.h>

#include "process_news.h"


/* process_file(inputFileName, news*);
 *
 * Input: An input filename and a news program to build.
 * Output: The number of actual rows loaded.
 */
int process_file(const char* inputFileName, NewsProgram* news)
{
    FILE* inputFile;

    // read in names and print messages
    if ((inputFile = fopen(inputFileName, "r")) == NULL) {
        std::cerr << "For inputFileName='" << inputFileName << "'" << std::endl;
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
    int actualArticleCount = 0;

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
            std::cout << "Processing Header" << std::endl;
            //for (auto v : match) std::cout << v << "," << std::endl;

            sscanf(tempStrng, "%d articles and %d reporters\n", &news->numArticles, &news->numReporters);

            /* Re-enable once Clark upgrades to gcc >= 4.9.0
            news.numArticles = std::stoi(match[1]);
            news.numReporters = std::stoi(match[2]);
            */

            // Allocate space for the given number of articles
            if (news->numArticles > 0) {
                news->articles = (Article**) malloc(news->numArticles * sizeof(Article*));
            }

            std::cout << "Number of Articles: " << news->numArticles << std::endl;
            std::cout << "Number of Reporters " << news->numReporters << std::endl;

        /* Re-enable once Clark upgrades to gcc >= 4.9.0
        } else if (news.minClicks == -1 && std::regex_search(str, match, clicksRegex)) {
        */
        } else if (lineCount == 2) {
            // Required clicks
            std::cout << "Processing Required Clicks" << std::endl;

            sscanf(tempStrng, "%d clicks or more required\n", &news->minClicks);

            /* Re-enable once Clark upgrades to gcc >= 4.9.0
            news.minClicks = std::stoi(match[1]);
            */

            std::cout << "Minimum Clicks: " << news->minClicks << std::endl;

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
            news->articles[actualArticleCount] = a;
            actualArticleCount++;
            //printf("%s %s %d %d %d\n", a->id.c_str(), a->type.c_str(), a->reporter, a->cost, a->clicks);

            // Check if this is last article
            if (actualArticleCount >= news->numArticles) {
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

    return actualArticleCount;
}

/* explore_solution_space(news, plausible solution*);
 *
 * Input: An initialized news plan and a dummy solution.
 * Output: Modifies incumbent to optimal while returning whether any feasible solution was found.
 */
bool explore_solution_space(const NewsProgram news, PlausibleSolution* incumbent)
{
    // Generate and score all possible solutions
    incumbent->score = INT_MAX;
    long numPossibilities = pow(2, news.numArticles);
    bool anyFeasibleFound = false;
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
        if (soln.score > incumbent->score) {
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
            anyFeasibleFound = true;
            incumbent->solution = soln.solution;
            incumbent->score = soln.score;
            incumbent->totalClicks = soln.totalClicks;
            incumbent->typeCounts = soln.typeCounts;
            incumbent->reporterCounts = soln.reporterCounts;
        }
    }
    return anyFeasibleFound;
}
