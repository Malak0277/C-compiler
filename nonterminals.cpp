#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <map>
#include <algorithm> // for std::isspace
#include <cctype>    // for std::isspace


using namespace std;





class NonTerminalClass {
private:
    string nonTerminal;
    vector<string> first;
    vector<string> follow;
    vector<vector<string>> grammar;
    vector<vector<string>> foreignGrammar; // All the grammar where this non-terminal appears
    vector<string> foreignStart; // All the non-terminals where this non-terminal appears

public:
    static map<string, NonTerminalClass> nonTerminalObject;

    NonTerminalClass() {}   
    NonTerminalClass(string n, const vector<vector<string>>& rules) : nonTerminal(n), grammar(rules){}

    vector<string> getFirst(){
        if (first.size() == 0) 
            setFirst();
        return first;
    }

    vector<string> getFollow(){
        return follow;
    }

    vector<vector<string>> getForeignGrammar(){
        return foreignGrammar;
    }

    void setForeignGrammar(vector<string> g){
        foreignGrammar.push_back(g);
    }



    vector<string> getForeignStart(){
        return foreignStart;
    }

    void setForeignStart(string s){
        foreignStart.push_back(s);
    }


    void extractNonTerminals(){
        for(int i = 0; i < grammar.size(); i++) // each grammar rule
            for(int j = 0; j < grammar[i].size(); j++){  // each word per rule
                if(!isTerminal(grammar[i][j])){
                    nonTerminalObject[grammar[i][j]].setForeignGrammar(grammar[i]);
                    nonTerminalObject[grammar[i][j]].setForeignStart(nonTerminal);                    
                }
            }
    }


    void setFirst(){
        vector<string> firstVect;
        for (int i = 0; i < grammar.size(); i++){
            string firstItem = grammar[i][0]; 

            if (isTerminal(firstItem))
                firstVect.push_back(firstItem);
            else{
                vector<string> nextNonTerminalFirst = nonTerminalObject[firstItem].getFirst();
                for(int i = 0; i < nextNonTerminalFirst.size(); i++){
                    firstVect.push_back(nextNonTerminalFirst[i]);
                }
            }
        }

        first = firstVect; 
    }

/*
    void setFollow(){
        vector<string> followVect;

        for(int i = 0; i < foreignGrammar.size(); i++)
            for(int j = 0; j < foreignGrammar[i].size(); j++)
                string word = foreignGrammar[i][j];
                if(word == nonTerminal)

        for (int i = 0; i < symbols.size(); i++) //to loop through the symbols
            for(int j = 0; j < symbols[i].size; j++) // to loop through the grammar in a symbol
                for(int nonTerminal = 0; nonTerminal < symbols[i][j].size; nonTerminal++){ // to loop inside the grammar
                    if (symbols[i][j][nonTerminal] == t)
                        nonTerminal start = symbols[i];
                        if(t == start)
                            followVect.push_back("$");

                        if(nonTerminal != symbols[i][j].size() -1){
                            auto after = symbols[i][j][nonTerminal+1];

                            if (typeid(after).name() == typeid(string).name()) //if it is a terminal
                                followVect.push_back(after);
                            else // if it is a non-terminal
                                vector<string> vect = first(after);
                                for(int i = 0; i < vect.size(); i++)
                                    if(vect[i] == "#")
                                        followVect.push_back(follow(start));
                                    else
                                        followVect.push_back(vect[i])
                        }
                        else //if it is the last element
                            followVect.push_back(follow(start));
                }
        follow = removeDuplicates(followVect);
    }

*/

private:

    bool isTerminal(const string& str) {
        return (str.size() >= 2 && str.front() == '"' && str.back() == '"');
    }

    vector<string> removeDuplicates(const vector<string>& arr) {
        vector<string> uniqueArr;

        // Iterate through the array and add unique elements to the uniqueArr
        for (const string& value : arr) {
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

vector<vector<string>> grammarExtractor(string line, string &ntName, vector<string>& terminals){
    vector<vector<string>> grammarRules;


    // Split the line based on "->"
    vector<string> parts = split(line, "->");
    if (parts.size() != 2) {
        cerr << "Invalid line format: " << line << endl;
        return grammarRules;
    }

    vector<string> result;
    istringstream iss(parts[0]);
    string token;

    // Read each token separated by whitespace and store it in the result vector
    while (iss >> token) {
        result.push_back(token);
    }

    if(result.size() != 1) {
        cerr << "Invalid line format: " << line << endl;
        return grammarRules;
    }
    
    ntName = result[0];

    // Split the line based on "|"
    vector<string> subparts = split(parts[1], "|");
    for (const auto& subpart : subparts) {
        if (all_of(subpart.begin(), subpart.end(), ::isspace)) {
            cerr << "Invalid line format: " << line << endl;
            return grammarRules;
        }
        vector<string> rule;
        vector<string> words = split(subpart, " ");
        for (const auto& word : words) 
            if (!word.empty()) {
            rule.push_back(word);
            if (word.front() == '"' && word.back() == '"') {
                // Strip the double quotes and add to terminals
                terminals.push_back(word);
            } 
            
        }
        grammarRules.push_back(rule);
        
    }

    return grammarRules; 
}



map<string, NonTerminalClass> NonTerminalClass::nonTerminalObject;

int main() {
    vector<string> nonTerminalList;
    vector<string> terminalList;    


    ifstream file("cfg.cfg");
    if (!file.is_open()) {
        cerr << "Failed to open file 'c.cfg'.\n";
        return 1;
    }
    
          

    string line;
    while (getline(file, line)) {
        if (line.empty() || all_of(line.begin(), line.end(), ::isspace)) {
            continue;
        }
        string nonTerminal;
        vector<vector<string>> grammar = grammarExtractor(line, nonTerminal, terminalList);

        if(grammar.size() != 0){
            nonTerminalList.push_back(nonTerminal);
            NonTerminalClass::nonTerminalObject.insert({nonTerminal, NonTerminalClass(nonTerminal, grammar)});

        }

    }



    file.close();




    for (int i = 0; i < nonTerminalList.size(); i++){
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].extractNonTerminals();
        
        /*
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].setFirst();
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].setFollow();
        */
           
    }

    
// TESTS
    //foreign grammar
    for (int i = 0; i < nonTerminalList.size(); i++){
        vector<string> k = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getForeignStart();
        vector<vector<string>> r = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getForeignGrammar();
        cout << nonTerminalList[i] << endl;
        cout << "------" << endl;

        for(int i = 0; i < r.size(); i++){
                cout << k[i] << " -> ";
            for(int j = 0; j < r[i].size(); j++)
                cout << r[i][j] << " ";
    
            cout << endl;
        }
        cout << endl << endl;
    }
    //first
    for (int i = 0; i < nonTerminalList.size(); i++){
        cout << nonTerminalList[i] << " => ";
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].setFirst();
        vector<string> fl= NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFirst();
        for(int i = 0; i < fl.size(); i++)
                cout << fl[i] << " ";
        cout << endl;

    }


}



