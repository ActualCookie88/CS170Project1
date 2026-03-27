#include "../header/globals.hpp"
#include <iostream>

using std::cout;

vector<vector<int>> solution = { // goal state
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
};

unordered_map<int, pair<int,int>> solPositions = { // each individual position
    {1,{0,0}}, {2,{0,1}}, {3,{0,2}},
    {4,{1,0}}, {5,{1,1}}, {6,{1,2}},
    {7,{2,0}}, {8,{2,1}}
};

vector<vector<vector<int>>> depths = {
{   // depth 0
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 0}
}, { // depth 2
    {1, 2, 3},
    {4, 5, 6},
    {0, 7, 8}
}, { // depth 4
    {1, 2, 3},
    {5, 0, 6},
    {4, 7, 8}
}, { // depth 6
    {1, 3, 6},
    {4, 0, 2},
    {7, 5, 8}
}, { // depth 8
    {1, 3, 6},
    {5, 0, 2},
    {4, 7, 8}
}, { // depth 10
    {1, 5, 0},
    {4, 6, 2},
    {7, 3, 8}
}, { // depth 12
    {1, 3, 6},
    {5, 0, 7},
    {4, 8, 2}
}, { // depth 14
    {7, 1, 6},
    {5, 3, 2},
    {0, 4, 8}
}, { // depth 16
    {1, 6, 7},
    {5, 0, 3},
    {4, 8, 2}
}, { // depth 18
    {5, 2, 6},
    {3, 7, 4},
    {0, 1, 8}
}, { // depth 20
    {7, 1, 2},
    {4, 8, 5},
    {6, 3, 0}
}, { // depth 22
    {5, 7, 3},
    {4, 6, 1},
    {0, 8, 2}
}, { // depth 24
    {0, 7, 2},
    {4, 6, 1},
    {3, 5, 8}
},
};

// terminal display purposes
void border() {
    cout << "\n//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////\n\n";
}

// prints puzzle 
void displayPuzzle(const vector<vector<int>>& puzzle, int type) {
    if(type == 1) {
        cout << "[" << puzzle[0][0] << " " << puzzle[0][1] << " " << puzzle[0][2] << "]\n" <<
            "[" << puzzle[1][0] << " " << puzzle[1][1] << " " << puzzle[1][2] << "]\n" <<
            "[" << puzzle[2][0] << " " << puzzle[2][1] << " " << puzzle[2][2] << "]\n\n";
    }
    else if(type == 2) {
        cout << "   [" << puzzle[0][0] << " " << puzzle[0][1] << " " << puzzle[0][2] << "]\n" <<
            "   [" << puzzle[1][0] << " " << puzzle[1][1] << " " << puzzle[1][2] << "]\n" <<
            "   [" << puzzle[2][0] << " " << puzzle[2][1] << " " << puzzle[2][2] << "]\n\n";
    }
    
}

bool isSolvable(const vector<vector<int>>& puzzle) {
    vector<int> flat;
    for(auto &row : puzzle)
        for(int x : row)
            if(x != 0) flat.push_back(x); // ignore blank

    int inversions = 0;
    for(int i = 0; i < flat.size(); i++) {
        for(int j = i+1; j < flat.size(); j++) {
            if(flat[i] > flat[j]) inversions++;
        }
    }

    return inversions % 2 == 0; // true = solvable
}