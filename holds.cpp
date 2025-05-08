#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <random>
#include <iterator>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage:      " << argv[0] << " <input_file>" << endl;
        return 1;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    vector<string> lines;
    string line;

    while (getline(inputFile, line)) {
        istringstream iss(line);
        vector<string> words((istream_iterator<string>(iss)), istream_iterator<string>());

        if (!words.empty()) {
            words.pop_back(); // Remove the last word
            ostringstream oss;
            for (size_t i = 0; i < words.size(); ++i) {
                if (i > 0) oss << " ";
                oss << words[i];
            }
            lines.push_back(oss.str());
        }
    }

    inputFile.close();

    if (lines.size() < 15) {
        cerr << "Error: File contains fewer than 15 valid lines after processing." << endl;
        return 1;
    }

    // Shuffle the lines
    random_device rd;
    mt19937 g(rd());
    shuffle(lines.begin(), lines.end(), g);

    // Print the first 15 lines
    for (int i = 0; i < 15; ++i) {
        cout << lines[i] << endl;
    }

    return 0;
}