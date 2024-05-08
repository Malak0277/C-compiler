#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 

using namespace std;

vector<string> split(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0, end;
    while ((end = str.find(delimiter, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(str.substr(start)); // Get the last token
    return tokens;
}

int main() {
    ifstream file("cfg.cfg");
    if (!file.is_open()) {
        cerr << "Failed to open file 'c.cfg'.\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            continue; // Skip processing empty lines
        }

        // Split the line based on "->"
        vector<string> parts = split(line, "->");
        if (parts.size() != 2) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }
        
        // Print what's before '->'
        cout << parts[0] << " = ";
        cout << "{ ";

        // Split the part after '->' based on '|'
        vector<string> subparts = split(parts[1], "|");
        for (const auto& subpart : subparts) {
            cout << "{" ;
            // Split subpart by spaces
            vector<string> words = split(subpart, " ");
            for (const auto& word : words) {
                if (word != "")
                    cout << word << ", ";
            }
            cout << "}";
        }

        cout << " }" << ";";
        cout << endl << endl;  
    }

    file.close();
    return 0;
}
