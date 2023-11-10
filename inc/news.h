// --------------------------------------------------------------------------
//
// news.h
//
// CS5130 Advanced Algorithms
//
// Austin Hester, Oct. 2023
//
// --------------------------------------------------------------------------



#ifndef NEWS_H
#define NEWS_H


#include <bitset>
#include <string.h>


const int BIT_LENGTH = 32; // number of chars allocated for each bitstring 


struct Article {
    std::string id;
    std::string type;
    int reporter;
    int cost;
    int clicks;
};

struct NewsProgram {
    int numArticles = -1;
    int numReporters = -1;
    int minClicks = -1;
    Article** articles; 
};

/*
 * Data structure for proposed solution.
 *
 * May convert into dynamic properties in future.
 */
struct PlausibleSolution {
    std::bitset<BIT_LENGTH> solution;
    int score;
    int totalClicks;
    int totalArticles;
    std::map<std::string, int> typeCounts; // 0: G, 1: L, 2: S, 3: E
    std::map<int, int> reporterCounts;
    bool isFeasible;
};


/* scoreSolution(&ps, np);
 *
 * Input: A plausible solution and a news program to score against.
 * Output: The cost associated with the given solution.
 */
void scoreSolution(PlausibleSolution* ps, NewsProgram np);


/* checkFeasible(&ps, np);
 *
 * Input: A plausible solution and a news program to score against.
 * Output: The feasibility of the given solution.
 */
void checkFeasible(PlausibleSolution* ps, NewsProgram np);


void cleanupNews(NewsProgram* np, int numArticles);


#endif
