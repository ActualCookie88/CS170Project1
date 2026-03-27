#pragma once
#include <vector>
#include <unordered_map>
#include <utility>

using std::vector;
using std::unordered_map;
using std::pair;

// Goal state
extern vector<vector<int>> solution;

// Tile positions for heuristics
extern unordered_map<int, pair<int,int>> solPositions;

// Predefined depth puzzles (optional)
extern vector<vector<vector<int>>> depths;

// Utility functions
void border();
void displayPuzzle(const vector<vector<int>>& puzzle, int type);
bool isSolvable(const vector<vector<int>>& puzzle);