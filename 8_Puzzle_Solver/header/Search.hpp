#pragma once
#include <vector>
#include <string>
using std::vector;
using std::string;

// struct to store search results
struct SearchResult {
    string name;
    int solutionDepth;
    int nodesExpanded;
    int maxQueueSize;
    bool success;
    vector<vector<vector<int>>> path;
};

SearchResult runSearch(const vector<vector<int>>& puzzle_, int algorithm, bool printSteps, bool printSolution);

void generalSearch(const vector<vector<int>>& puzzle_, int algorithm, bool runAll);

