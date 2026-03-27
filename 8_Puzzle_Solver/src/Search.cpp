#include "../header/Search.hpp"
#include "../header/Puzzle.hpp"
#include "../header/Heuristics.hpp"
#include "../header/globals.hpp"
#include <queue>
#include <unordered_map>
#include <iostream>
#include <limits>

using namespace::std;

SearchResult runSearch(const vector<vector<int>>& puzzle_, int algorithm, bool printSteps) {
    vector<string> algs = {
        "UNIFORM COST SEARCH",
        "the MISPLACED TILE HEURISTIC",
        "the MANHATTAN DISTANCE HEURISTIC"
    };
    priority_queue<Node> pq;
    unordered_map<string, int> bestGn;

    // initial state/node, establish gn and hn
    Node init;
    init.puzzle = puzzle_;
    init.gn = 0;

    if(algorithm == 1) init.hn = uniformHeuristic(puzzle_);
    else if(algorithm == 2) init.hn = misplacedHeuristic(puzzle_);
    else if(algorithm == 3) init.hn = manhattanHeuristic(puzzle_);
    else return SearchResult{"ERROR", -1, -1, -1, false};

    pq.push(init);

    int numExpanded = 0;
    int queueSize = 1;

    // start loop
    while(!pq.empty()) {
        queueSize = max(queueSize, (int)pq.size());
        Node curr = pq.top();
        pq.pop();

        // print only for algorithms 1-3
        if(printSteps) {
            cout << "The best state to expand with g(n) = " << curr.gn 
                << " and h(n) = " << curr.hn << " is:\n";
            displayPuzzle(curr.puzzle, 2);
            cout << "Expanding this node...\n\n";
        }

        // convert 2D puzzle to string for hashing
        string s = puzzleToString(curr.puzzle);
        
        // if current state not visited yet, expand
        if(bestGn.find(s) != bestGn.end() && bestGn[s] <= curr.gn) continue;
        bestGn[s] = curr.gn;

        // check if puzzle is solved
        if(curr.puzzle == solution) {
            return SearchResult{algs[algorithm-1], curr.gn, numExpanded, queueSize, true};
        }
        
        // continue expanding
        numExpanded++;
        vector<vector<vector<int>>> children = expand(curr.puzzle);

        // loop over every child
        for(auto &childPuzzle : children) {
            int newGn = curr.gn + 1;
            string st = puzzleToString(childPuzzle);
            if(bestGn.count(st) && bestGn[st] <= newGn) continue;

            Node child;
            child.puzzle = childPuzzle;
            child.gn = newGn;
            if(algorithm == 1) child.hn = uniformHeuristic(childPuzzle);
            else if(algorithm == 2) child.hn = misplacedHeuristic(childPuzzle);
            else if(algorithm == 3) child.hn = manhattanHeuristic(childPuzzle);
            pq.push(child);
        }
    }
    // no solution found
    return SearchResult{algs[algorithm-1], -1, numExpanded, queueSize, false};
}

// general search interface
void generalSearch(const vector<vector<int>>& puzzle_, int algorithm, bool runAll) {
    // run all algorithms and print results
    if(runAll) {
        vector<SearchResult> results;
        for(int alg = 1; alg <= 3; alg++) {
            auto res = runSearch(puzzle_, alg, false);
            results.push_back(res);
        }

        cout << "====== SEARCH RESULTS ======\n";
        for(auto &res : results) {
            cout << "Goal state reached! Used " << res.name << ":\n";
            if(res.success) {
                cout << "  Solution Depth: " << res.solutionDepth << "\n"
                     << "  Nodes Expanded: " << res.nodesExpanded << "\n"
                     << "  Max Queue Size: " << res.maxQueueSize << "\n";
            } 
            else {
                cout << "  FAILED TO FIND SOLUTION!\n";
            }
            cout << endl;
        }
        return;
    }

    // run one algorithm and print results
    SearchResult res;
    char choice;
    cout << "Print intermediate steps? (y/n): ";
    cin >> choice;cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool printSteps = (choice == 'y');
    res = runSearch(puzzle_, algorithm, printSteps);

    border();
    cout << "====== SEARCH RESULTS ======\n";
    cout << "Goal state reached! Used " << res.name << ":\n";
    if(res.success) {
        cout << "  Solution Depth: " << res.solutionDepth << "\n"
             << "  Nodes Expanded: " << res.nodesExpanded << "\n"
             << "  Max Queue Size: " << res.maxQueueSize << "\n";
    } 
    else {
        cout << "  FAILED TO FIND SOLUTION!\n";
    }
    cout << endl;
}