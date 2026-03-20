#include "header/Search.hpp"
#include "header/Puzzle.hpp"
#include "header/Heuristics.hpp"
#include "header/globals.hpp"

#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <chrono>

using namespace std;

/* ////////////////////////////////////////////////////////////////////

HELPERS

//////////////////////////////////////////////////////////////////// */

// user input checker
int selectOptionHelper(int min, int max) {
    string input;
    int number;
    while(true) {
        cin >> input;
        cout << endl;
        try {
            number = stoi(input); // if fails, throws exception to catch
        } catch(...) {
            cout << "INVALID OPTION. TRY AGAIN: ";
            continue;
        }
        if(number >= min && number <= max) {
            return number;
        } else {
            cout << "INVALID OPTION. TRY AGAIN: ";
        }
    }
}

// depth input checker
vector<vector<int>> depthSelectHelper(int choice) {
    vector<int> depthVals = {0,2,4,8,12,16,20,24};

    auto it = find(depthVals.begin(), depthVals.end(), choice);
    if(it == depthVals.end()){
        cout << "Invalid depth.\n";
        exit(1);
    }

    int index = distance(depthVals.begin(), it);
    return depths[index];
}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {
    border();
    cout << "8-Puzzle Solver\n\n";
    
    // initial user prompt
    cout << "SELECT OPTION: \n" <<
            "(1) Default Puzzle\n" <<
            "(2) Custom Puzzle\n";
    int choice = selectOptionHelper(1, 2);
    vector<vector<int>> puzzle(3, vector<int>(3));

    if(choice == 1) { // default puzzle
        cout << "ENTER DEPTH OF PUZZLE (0,2,4,8,12,16,20,24): ";

        puzzle = depthSelectHelper(selectOptionHelper(0, 24));
    }
    else if(choice == 2) { // custom puzzle
        unordered_set<int> seen;
        for(int i = 0; i < 3; i++) {
            for(int j = 0; j < 3; j++) {
                int tile;
                while(true) {
                    cout << "Enter tile Number (Row " << i + 1 << ", Column " << j + 1 << ") [0-8, no duplicates]: ";
                    cin >> tile;

                    // check valid number
                    if(tile < 0 || tile > 8) {
                        cout << "INVALID: Must be between 0 and 8.\n";
                        continue;
                    }

                    // check for duplicates
                    if(seen.count(tile)) {
                        cout << "INVALID: Tile already used.\n";
                        continue;
                    }

                    // valid input
                    puzzle[i][j] = tile;
                    seen.insert(tile);
                    break;
                }
            }
        }
    }
    else {
        cout << "ERROR";
        return 0;
    }

    border();

    cout << "Selected puzzle: \n";
    displayPuzzle(puzzle, 1);

    // algorithm choice prompt
    cout << "SELECT ALGORITHM: \n" <<
            "(1) Uniform Cost Search\n" <<
            "(2) Misplaced Tile Heuristic (A*)\n" <<
            "(3) Manhattan Distance Heuristic (A*)\n";
    choice = selectOptionHelper(1, 3);
    
    border();

    auto start = chrono::high_resolution_clock::now();
    generalSearch(puzzle, choice);
    auto end = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Execution Time: " << duration << " ms\n";

    return 0;
}

