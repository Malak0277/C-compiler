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
    vector<string> first;
    map <string, vector<string>> firstGrammar;
    vector<string> follow;
    vector<vector<string>> grammar;
    vector<vector<string>> foreignGrammar; // All the grammar where this non-terminal appears
    vector<string> foreignStart; // All the non-terminals where this non-terminal appears
    bool followSignal;

public:
    string nonTerminal; // the name of this non terminal
    static map<string, NonTerminalClass> nonTerminalObject;

    NonTerminalClass() {}   
    NonTerminalClass(string n, const vector<vector<string>>& rules) : nonTerminal(n), grammar(rules){
        followSignal = false;
    }

    map <string, vector<string>> getFirst(){
        if (firstGrammar.size() == 0) 
            setFirst();
        return firstGrammar;
    }

    vector<string> getFollow(bool o = false){
        if (followSignal){
            follow.push_back("$");
            return follow;
        }
        
        if (follow.size() == 0) {
            if(o)
                follow.push_back("$");
            setFollow();
        }
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
        for (int i = 0; i < grammar.size(); i++){
            string firstItem = grammar[i][0]; 

            if (isTerminal(firstItem))
                firstGrammar.insert({firstItem, grammar[i]});

            else{
                map <string, vector<string>> f = nonTerminalObject[firstItem].getFirst();

                for (const auto& pair : f) 
                    firstGrammar.insert({pair.first, pair.second});
                
            }
        }

    }


    void setFollow(){

        for(int i = 0; i < foreignGrammar.size(); i++)
            for(int j = 0; j < foreignGrammar[i].size(); j++){
                string word = foreignGrammar[i][j];
                if(word == nonTerminal){
                    followSignal = true;
                    NonTerminalClass start = nonTerminalObject[foreignStart[i]];
                    if(start.nonTerminal == word)
                        follow.push_back("$");
                    
                    if(j != foreignGrammar[i].size()-1){ //check it is not the last word in the rule
                        string after = foreignGrammar[i][j+1];

                        if (isTerminal(after)) //if it is a terminal
                            follow.push_back(after);
                        else{ // if it is a non-terminal
                            NonTerminalClass afterObj = nonTerminalObject[after];

                            map <string, vector<string>> vect = afterObj.getFirst();

                                                                   

                            for (const auto& pair : vect) 
                                if(pair.first == "\"#\""){
                                    if(start.nonTerminal == word)
                                        continue;
                                    vector<string> nextNonTerminalFollow =  start.getFollow();
                                   // for (const auto& pair : f) 
                                    //firstGrammar.insert({pair.first, pair.second});
                                    for(int k = 0; k < nextNonTerminalFollow.size(); k++)
                                        follow.push_back(nextNonTerminalFollow[k]);
                                }
                                else
                                    follow.push_back(pair.first);
                                                    
                        }
                    }else{ //if it is the last element
                        if(start.nonTerminal == word)
                            continue;
                        vector<string> nextNonTerminalFollow = start.getFollow();
                        for(int h = 0; h < nextNonTerminalFollow.size(); h++)
                            follow.push_back(nextNonTerminalFollow[h]);
                        
                    }
                        
                }

            }

        followSignal = false;
        follow = removeDuplicates(follow);
    }

    bool isTerminal(const string& str) {
        return (str.size() >= 2 && str.front() == '"' && str.back() == '"');
    }

    static vector<string> removeDuplicates(const vector<string>& arr) {
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

map<string, NonTerminalClass> NonTerminalClass::nonTerminalObject;


map <pair <string, string>, vector<string>> ParseTable(vector<string> NT, vector<string> T){
    //map <pair<nt, t>, grammar>
    map <pair <string, string>, vector<string>> myMap;
    vector<string> epsilon;
    epsilon.push_back("\"#\"");

    for(int i = 0; i < NT.size(); i++){ // loop on non-terminals
        NonTerminalClass obj = NonTerminalClass::nonTerminalObject[NT[i]];
        map <string, vector<string>> vect = obj.getFirst();  

        for (const auto& pair :vect){
            if(myMap.find(make_pair(NT[i], pair.first)) != myMap.end())
                cout << "LL1() violated !!!!!!!!" << endl;
            else{
                if(pair.first == "\"#\""){
                    for (int x = 0; x < obj.getFollow().size(); x++)
                            myMap.insert({{NT[i], obj.getFollow()[x]}, epsilon});                
                }
                    
                myMap.insert({{NT[i], pair.first}, pair.second});
            }
        }
    }

    return myMap;
}


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




vector<string> getFromParseTable(map <pair <string, string>, vector<string>> table, string nt, string token){
    return table[make_pair(nt, token)];
}


vector<vector<string>> cfgExtractor(string line, string &ntName, vector<string>& terminals){
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
                for(int i = 1; i < word.size()-1; i++)
                    if (word[i] == '\"'){
                        cerr << "Invalid line format: " << line << endl;
                        return grammarRules;
                    }

                terminals.push_back(word);
            } 
            
        }
        grammarRules.push_back(rule);
        
    }

    return grammarRules; 
}




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
        vector<vector<string>> grammar = cfgExtractor(line, nonTerminal, terminalList);

        if(grammar.size() != 0){
            nonTerminalList.push_back(nonTerminal);
            NonTerminalClass::nonTerminalObject.insert({nonTerminal, NonTerminalClass(nonTerminal, grammar)});

        }

    }
    terminalList = NonTerminalClass::removeDuplicates(terminalList);

    file.close();



    terminalList.push_back("$");
    //SETTERS
    for (int i = 0; i < nonTerminalList.size(); i++){
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].extractNonTerminals();
    }

    cout << "==+++++++++++++++++++++++++++++++++++++++&&&&&&&&&&"<<endl;
    cout << NonTerminalClass::nonTerminalObject[nonTerminalList[0]].nonTerminal << endl;
    bool o;
    for (int i = 0; i < nonTerminalList.size(); i++){
        map <string, vector<string>> first = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFirst();  
        if (i == 0)     
            o = true;
        else 
            o = false;
        vector<string> follow = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFollow(o);
 
    }

    
// TESTS
/*
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
    cout << endl << "firstttt" << endl << endl;
    for (int i = 0; i < nonTerminalList.size(); i++){
        
        cout << nonTerminalList[i] << " => ";
        
        //NonTerminalClass::nonTerminalObject[nonTerminalList[i]].setFirst();
        map <string, vector<string>> fm = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFirst();  

        for (const auto& pair : fm){
            cout << pair.first << " <<==>> ";
            for(int j = 0; j < pair.second.size(); j++)
                cout << pair.second[j] << " ";
            cout << endl;
        }
        

        


    }

// follow

    cout << endl << "followww" << endl << endl;

    for (int i = 0; i < nonTerminalList.size(); i++){
        cout << nonTerminalList[i] << " => ";
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].setFollow();
        vector<string> fl= NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFollow();       
        for(int i = 0; i < fl.size(); i++)
                cout << fl[i] << " ";
        cout << endl;
    }


*/

// parse table
cout << "DUN...DUN....DUNNNNNNN" << endl;
map <pair <string, string>, vector<string>> parseTable = ParseTable(nonTerminalList, terminalList);



for(int i = 0; i < nonTerminalList.size(); i++){ 
    cout << nonTerminalList[i] << endl;
    cout << "*********************" << endl;
    for(int j = 0; j < terminalList.size(); j++)
        if(parseTable.find(make_pair(nonTerminalList[i], terminalList[j])) != parseTable.end()){
            cout << terminalList[j] << " : ";
                for (const std::string& str : parseTable[make_pair(nonTerminalList[i], terminalList[j])]) 
                    cout << str;
                cout << endl;
        }
   cout << endl;
}


}



