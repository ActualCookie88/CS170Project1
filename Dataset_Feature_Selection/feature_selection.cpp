#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <chrono>
#include <iomanip>

using namespace std;

/* ////////////////////////////////////////////////////////////////////

SCAN FILE FOR FEATURES/INSTANCES

//////////////////////////////////////////////////////////////////// */

vector<vector<double>> dataSet;
vector<vector<vector<double>>> featureDistance;
int numFeatures = 0;

// scan file and load data set + number of features
bool scanData(string fileName) {
    ifstream inputFile(fileName);

    // error handling
    if (!inputFile.is_open()) {
        cout << "Error: could not open file.\n";
        return false;
    }

    string line;

    // scan file
    while(getline(inputFile, line)) {
        stringstream ss(line);
        vector<double> instance;
        double value;

        // scan row
        while(ss >> value) {
            instance.push_back(value);
        }

        if(!instance.empty()) {
            dataSet.push_back(instance);
        }
    }

    inputFile.close();

    if(!dataSet.empty()) {
        numFeatures = dataSet[0].size() - 1; // number of columns - class #
    }

    return true;
}

/* ////////////////////////////////////////////////////////////////////

HELPERS

//////////////////////////////////////////////////////////////////// */

// precompute squared distances for each feature
void precomputeDistances() {
    int n = dataSet.size();

    featureDistance.resize(n, vector<vector<double>>(n, vector<double>(numFeatures + 1)));

    for(int i = 0; i < n; i++) {
        for(int k = 0; k < n; k++) {
            for(int f = 1; f <= numFeatures; f++) {
                double diff = dataSet[i][f] - dataSet[k][f];
                featureDistance[i][k][f] = diff * diff;
            }
        }
    }
}

// user input checker
int selectOptionHelper(int min, int max) {
    string input;
    int number;
    while(true) {
        cin >> input;
        cout << endl;
        try {
            number = stoi(input); 
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

// Euclidean Distance formula helper
double euclideanDistance(int row1, int row2, const vector<int>& featureSet, double bestSoFar) {
    double sum = 0.0;

    for(int feature : featureSet) {
        sum += featureDistance[row1][row2][feature];

        // stop early if already worse
        if(sum > bestSoFar) {
            return sum;
        }
    }

    return sum; // removed sqrt (not needed for comparison)
}

// Leave one out cross validation
double leaveOneOut(const vector<int>& featureSet) {
    if(featureSet.empty()) {
        return 0;
    }

    int n = dataSet.size();

    int numCorrectlyClassified = 0;

    // outer loop for each instance
    for(int i = 0; i < n; i++) {
        double labelObjectToClassify = (int)dataSet[i][0];

        double nearestNeighborDist = numeric_limits<double>::infinity();
        int nearestNeighborLoc = -1;
        
        // innter loop to comapare to other instances
        for(int k = 0; k < n; k++) {
            if(k != i) { // dont compare to self
                double distance = euclideanDistance(i, k, featureSet, nearestNeighborDist);

                // update nearest neighbor IF closer
                if(distance <= nearestNeighborDist) {
                    nearestNeighborDist = distance;
                    nearestNeighborLoc = k;
                }
            }
        }

        double neighborLabel = (int)dataSet[nearestNeighborLoc][0];
        if(labelObjectToClassify == neighborLabel) numCorrectlyClassified++; // amount of correct classifications
    }
    // accuracy calculation
    return (double)numCorrectlyClassified / n;
}

// display purposes
void printSet(const vector<int>& featureSet, ostream& out ) {
    out << "{";
    for(int i = 0; i < featureSet.size(); i++) {
        out << featureSet[i];
        if(i != featureSet.size() - 1) {
            out << ",";
        }
    }
    out << "}";
}

/* ////////////////////////////////////////////////////////////////////

ALGORITHMS

//////////////////////////////////////////////////////////////////// */

// Select one feature at each level
void forwardSelection(ofstream& outFile) {
    vector<int> currFeatureSet;
    vector<int> bestSet;
    double bestAccuracy = 0.0;
    
    // outer loop for walking down search tree
    for(int level = 1; level <= numFeatures; level++) {
        cout << "On level " << level << " of the search tree\n";
        outFile << "On level " << level << " of the search tree" << endl;
        int featureToAdd = -1;
        double bestAccuracySoFar = 0.0;

        // inner loop for considering selecting each feature
        for(int k = 1; k <= numFeatures; k++) {
            if(find(currFeatureSet.begin(), currFeatureSet.end(), k) != currFeatureSet.end()) continue; // skip if k already in set

            // test accuracy of selecting feature temporarily
            vector<int> temp = currFeatureSet;
            temp.push_back(k);

            double accuracy = leaveOneOut(temp);

            // cout << "Using feature(s) ";
            // printSet(temp);
            // cout << " accuracy is " << accuracy * 100 << "%\n";

            // update accuracy and best feature
            if(accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToAdd = k;
            }
        }
        // adds best feature to set
        currFeatureSet.push_back(featureToAdd);
        
        // output to terminal
        cout << "Feature set ";
        printSet(currFeatureSet, cout);
        cout << " was best, accuracy is " << bestAccuracySoFar * 100 << "%\n\n";
        // output to seperate file
        outFile << "Feature set ";
        printSet(currFeatureSet, outFile);
        outFile << " was best, accuracy is " << bestAccuracySoFar * 100 << "%" << endl;

        // update best overall
        if(bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currFeatureSet;
        }
    }
    // output to terminal
    cout << "Finished search. Best feature subset is ";
    printSet(bestSet, cout);
    cout << " with accuracy " << bestAccuracy * 100 << "%\n";
    // output to seperate file
    outFile << "Finished search. Best feature subset is ";
    printSet(bestSet, outFile);
    outFile << " with accuracy " << bestAccuracy * 100 << "%" << endl;
}

// Remove one feature at each level
void backwardElimination(ofstream& outFile) {
    vector<int> currFeatureSet;
    vector<int> bestSet;
    double bestAccuracy = 0.0;

    // start with all features
    for(int i = 1; i <= numFeatures; i++) {
        currFeatureSet.push_back(i);
    }

    bestAccuracy = leaveOneOut(currFeatureSet);
    bestSet = currFeatureSet;

    cout << "On level " << numFeatures << " of the search tree\n";
    outFile << "On level " << numFeatures << " of the search tree"<< endl;

    cout << "Using feature(s) ";
    printSet(currFeatureSet, cout);
    cout << " accuracy is " << bestAccuracy * 100 << "%\n\n";

    outFile << "Using feature(s) ";
    printSet(currFeatureSet, outFile);
    outFile << " accuracy is " << bestAccuracy * 100 << "%" << endl;

    // outer loop for walking up search tree
    for(int level = numFeatures - 1; level >= 1; level--) {
        cout << "On level " << level << " of the search tree\n";
        outFile << "On level " << level << " of the search tree" << endl;
        int featureToRemove = -1;
        double bestAccuracySoFar = 0.0;

        // inner loop for considering removing each feature
        for(int k = 0; k < currFeatureSet.size(); k++) {

            // test accuracy of removing feature temporarily
            vector<int> temp = currFeatureSet;
            temp.erase(temp.begin() + k);

            double accuracy = leaveOneOut(temp);

            // cout << "Using feature(s) ";
            // printSet(temp);
            // cout << " accuracy is " << accuracy * 100 << "%\n";

            // update best accuracy and set
            if(accuracy > bestAccuracySoFar) {
                bestAccuracySoFar = accuracy;
                featureToRemove = currFeatureSet[k];
            }
        }
        // remove best feature from set
        currFeatureSet.erase(remove(currFeatureSet.begin(), currFeatureSet.end(),featureToRemove),currFeatureSet.end());

        // output to terminal
        cout << "Feature set ";
        printSet(currFeatureSet, cout);
        cout << " was best, accuracy is " << bestAccuracySoFar * 100 << "%\n\n";
        // output to seperate file
        outFile << "Feature set ";
        printSet(currFeatureSet, outFile);
        outFile << " was best, accuracy is " << bestAccuracySoFar * 100 << "%" << endl;

        // update best overall
        if(bestAccuracySoFar > bestAccuracy) {
            bestAccuracy = bestAccuracySoFar;
            bestSet = currFeatureSet;
        }
    }
    // output to terminal
    cout << "Finished search. Best feature subset is ";
    printSet(bestSet, cout);
    cout << " with accuracy " << bestAccuracy * 100 << "%\n";
    // output to seperate file
    outFile << "Finished search. Best feature subset is ";
    printSet(bestSet, outFile);
    outFile << " with accuracy " << bestAccuracy * 100 << "%" << endl;
}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {
    cout << "Feature Selection Algorithm" << endl << endl;
    string fileName;

    cout << "Choose dataset: \n"
         << "(1) Small Dataset\n"
         << "(2) Large Dataset\n"
         << "(3) Custom Dataset\n";
    int choice = selectOptionHelper(1,3);

    if(choice == 1) {
        fileName = "CS170_Small_DataSet__7.txt";
    }
    else if(choice == 2){
        fileName = "CS170_Large_DataSet__85.txt";
    }
    else {
        cout << "Enter dataset filename: ";
        cin >> fileName;
        cout << endl;
    }

    if(!scanData(fileName)) return 1;

    cout << "Precomputing feature distances... \n\n";
    precomputeDistances();
        
    cout << "This dataset has " << numFeatures << " features with " << dataSet.size() << " instances.\n";
    cout << "Choose algorithm: \n"
         << "(1) Forward Selection\n"
         << "(2) Backward Elimination\n";
    choice = selectOptionHelper(1,2);

    ofstream outFile("output.txt");
    if(!outFile.is_open()) {
        cout << "Error opening output file.\n";
        return 1;
    }
    
    auto start = chrono::high_resolution_clock::now();

    if(choice == 1) {
        forwardSelection(outFile);
    }
    else {
        backwardElimination(outFile);
    }

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed = end - start;
    cout << fixed << setprecision(2);
    cout << "Program runtime: " << elapsed.count() << " seconds\n";

    return 0;
}