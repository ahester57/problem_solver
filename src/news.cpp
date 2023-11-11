// Austin Hester, CS 5130, Oct. 2023

#include <stdlib.h>
#include <iostream>
#include <map>

#include "news.h"



/* scoreSolution(&ps, np);
 *
 * Input: A plausible solution and a news program to score against.
 * Output: The cost associated with the given solution.
 */
void scoreSolution(PlausibleSolution* ps, const NewsProgram np)
{
    // Cost stats
    int totalCost = 0;
    // Counting stats
    ps->totalClicks = 0;
    ps->totalArticles = 0;
    ps->typeCounts["G"] = 0;
    ps->typeCounts["L"] = 0;
    ps->typeCounts["S"] = 0;
    ps->typeCounts["E"] = 0;
    // Initialize maps
    for (int i = 1; i <= np.numReporters; i++) {
        ps->reporterCounts[i] = 0;
    }

    // Loop and score (compute)
    for (int i = 0; i < np.numArticles; i++) {
        if (ps->solution.test(i)) {
            totalCost += np.articles[i]->cost;
            ps->totalClicks += np.articles[i]->clicks;
            ps->totalArticles++;
            ps->typeCounts[np.articles[i]->type]++;
            ps->reporterCounts[np.articles[i]->reporter]++;
        }
    }
    // Type Profit
    int profit = 0;
    for (std::map<std::string, int>::iterator iter = ps->typeCounts.begin(); iter != ps->typeCounts.end(); ++iter) {
        if (iter->second > 1) {
            profit += 150; 
        }
    }
    // Reporter Labor
    int labor = 0;
    for (std::map<int, int>::iterator iter = ps->reporterCounts.begin(); iter != ps->reporterCounts.end(); ++iter) {
        if (iter->second > 1) {
            labor += 100 * (iter->second - 1);
        }
    }
    ps->score = totalCost - profit + labor;
}


/* checkFeasible(&ps, np);
 *
 * Input: A plausible solution and a news program to score against.
 * Output: The feasibility of the given solution.
 */
void checkFeasible(PlausibleSolution* ps, const NewsProgram np)
{
    // Click Minimum
    if (ps->totalClicks < np.minClicks) {
        ps->isFeasible = false;
        return;
    }
    // Type Requirement
    int halfwayLimit = ps->totalArticles / 2;
    for (std::map<std::string, int>::iterator iter = ps->typeCounts.begin(); iter != ps->typeCounts.end(); ++iter) {
        if (iter->second == 0) {
            ps->isFeasible = false;
            return;
        }
        if (iter->second > halfwayLimit) {
            ps->isFeasible = false;
            return;
        }
    }
    // Reporter Requirement
    for (std::map<int, int>::iterator iter = ps->reporterCounts.begin(); iter != ps->reporterCounts.end(); ++iter) {
        if (iter->second == 0) {
            ps->isFeasible = false;
            return;
        }
    }
    ps->isFeasible = true;
}

/* cleanupNews
 * 
 *
 */
void cleanupNews(NewsProgram* np, int numArticles)
{
    // Cleanup dynamically created data
    for (int i = 0; i < numArticles; i++) {
        try { delete np->articles[i]; } catch (...) {}
    }
    try { free(np->articles); } catch (...) {}
}

