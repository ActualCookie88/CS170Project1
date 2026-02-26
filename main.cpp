#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

/* ////////////////////////////////////////////////////////////////////

SCAN FILE FOR FEATURES/INSTANCES

//////////////////////////////////////////////////////////////////// */

vector<vector<double>> dataSet;
int numFeatures = 0;

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

// for terminal display purposes
void border() {
    cout << endl;
    cout << "//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////" << endl;
    cout << endl;
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

double distance() {

}

/* ////////////////////////////////////////////////////////////////////

ALGORITHMS

//////////////////////////////////////////////////////////////////// */

void forwardSelection() {

}

void backwardElimination() {

}

/* ////////////////////////////////////////////////////////////////////

MAIN

//////////////////////////////////////////////////////////////////// */

int main() {\
    border();
    cout << "Feature Selection Algorithm" << endl << endl;

    string fileName;
    int choice;
    
    cout << "Enter dataset filename: ";
    cin >> fileName;
    cout << endl;

    if(!scanData(fileName)) {
        return 1;
    }
        
    cout << "This dataset has " << numFeatures << " features with " << dataSet.size() << " instances.\n";

    return 0;
}