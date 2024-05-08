#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 

using namespace std;


class NonTerminalClass {
private:
    vector<string> first;
    vector<string> follow;
    vector<vector<string>> grammar;

public:
   
   NonTerminalClass(const vector<vector<string>>& rules) : grammar(rules){}

    vector<string> getFirst(){
        return first;
    }

    vector<string> getFollow(){
        return follow;
    }

    void setFirst(nonTerminal k){
        vector<string> firstVect;
        for (int i = 0; i < k.size(); i++)
            if (typeid(k[i][0]).name() == typeid(string).name())
                firstVect.push_back(k[i][0]);
            else 
                firstVect.push_back(first(k[i][0]));
        
        first = removeDuplicates(firstVect); /////
    }

    void setFollow(nonTerminal t){
        vector<string> followVect;
        for (int i = 0; i < symbols.size(); i++) //to loop through the symbols
            for(int j = 0; j < symbols[i].size; j++) // to loop through the grammar in a symbol
                for(int k = 0; k < symbols[i][j].size; k++){ // to loop inside the grammar
                    if (symbols[i][j][k] == t)
                        nonTerminal start = symbols[i];
                        if(t == start)
                            followVect.push_back("$");

                        if(k != symbols[i][j].size() -1){
                            auto after = symbols[i][j][k+1];

                            if (typeid(after).name() == typeid(string).name()) //if it is a terminal
                                followVect.push_back(after);
                            else // if it is a non-terminal
                                vector<string> vect = first(after);
                                for(int i = 0; i < vect.size(); i++)
                                    if(vect[i] == "€")
                                        followVect.push_back(follow(start));
                                    else
                                        followVect.push_back(vect[i])
                        }
                        else //if it is the last element
                            followVect.push_back(follow(start));
                }
        follow = removeDuplicates(followVect);
    }

private:
    vector<string> removeDuplicates(const vector<string>& arr) {
        vector<string> uniqueArr;

        // Iterate through the array and add unique elements to the uniqueArr
        for (const T& value : arr) {
            if (find(uniqueArr.begin(), uniqueArr.end(), value) == uniqueArr.end()) {
                uniqueArr.push_back(value);
            }
        }
        return uniqueArr;
    }
};




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

vector<vector<string>> grammarExtractor(string line, string &ntName){
        vector<vector<string>> grammarRules;

        if (line.empty()) {
            continue; // Skip processing empty lines
        }

        // Split the line based on "->"
        vector<string> parts = split(line, "->");
        if (parts.size() != 2) {
            cerr << "Invalid line format: " << line << endl;
            continue;
        }
        
        // extracting the nonTerminal
        ntName = parts[0];

        vector<string> subparts = split(parts[1], "|");
        for (const auto& subpart : subparts) {
            vector<string> rule;
            vector<string> words = split(subpart, " ");
            for (const auto& word : words) 
                if (word != "")
                    rule.push_back(word);
            grammarRules.push_back(rule);
            
        }

        return grammarRules; 
}


int main() {
    map<string, NonTerminalClass> nonTerminalMap;
    vector<string> nonTerminalList;

    ifstream file("cfg.cfg");
    if (!file.is_open()) {
        cerr << "Failed to open file 'c.cfg'.\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        string *nonTerminal;
        vector<vector<string>> grammar = grammarExtractor(line, nonTerminal)

        nonTerminalList.push_back(*nonTerminal);
        nonTerminalMap[*nonTerminal] = NonTerminalClass(grammar);
    }

    file.close();


    for (int i = 0; i < nonTerminalList.size(); i++){
        nonTerminalMap[nonTerminalList[i]].setFirst();
        nonTerminalMap[nonTerminalList[i]].setFollow();
    }

}



