#pragma once
#include <vector>
#include <string>

using std::vector;
using std::string;

// node structure for states
struct Node {
    vector<vector<int>> puzzle;
    int gn; // cost of start -> finish (or current)
    int hn; // heuristic cost

    int fn() const { // A* : f = g + h
        return gn + hn;
    } 

    bool operator<(const Node& other) const{ // for min-heap behavior
        return fn() > other.fn();  
    }
};

string puzzleToString(const vector<vector<int>>& p);

vector<vector<vector<int>>> expand(const vector<vector<int>>& puzzle);

vector<vector<int>> generateRandomPuzzle();