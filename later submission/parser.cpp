#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream> 
#include <map>
#include <algorithm> // for isspace
#include <cctype>    // for isspace
#include "lexical analyzer.cpp"
#include <deque>
#include <stack>

using namespace std;





class NonTerminalClass {
private:
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
                    firstGrammar.insert({pair.first, grammar[i]});
                
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

    bool static isTerminal(const string& str) {
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
                else
                    myMap.insert({{NT[i], pair.first}, pair.second});
            }
        }
    }

    return myMap;
}




vector<string> getFromParseTable(map <pair <string, string>, vector<string>> table, string nt, string token){
    return table[make_pair(nt, token)];
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





vector<vector<string>> cfgExtractor(string line, string &ntName, vector<string>& terminals){
    vector<vector<string>> grammarRules;
    vector<vector<string>> emptyRule;


    // Split the line based on "->"
    vector<string> parts = split(line, "->");
    if (parts.size() != 2) {
        cerr << "Invalid line format: " << line << endl;
        return emptyRule;
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
        return emptyRule;
    }
    
    ntName = result[0];

    // Split the line based on "|"
    vector<string> subparts = split(parts[1], "|");
    for (const auto& subpart : subparts) {
        if (all_of(subpart.begin(), subpart.end(), ::isspace)) {
            cerr << "Invalid line format: " << line << endl;
            return emptyRule;
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
                        return emptyRule;
                    }

                terminals.push_back(word);
            } 
            
        }
        grammarRules.push_back(rule);
        
    }

    return grammarRules; 
}

string printFloat(double num) {
    // Convert the float to a string with fixed precision
    string numString = std::to_string(num);
    
    // Find the position of the decimal point
    size_t decimalPos = numString.find('.');
    
    // If there is a decimal point
    if (decimalPos != string::npos) {
        // Find the position of the last non-zero digit
        size_t lastNonZero = numString.find_last_not_of('0');
        
        // If the last non-zero digit is after the decimal point
        if (lastNonZero > decimalPos) {
            return numString.substr(0, lastNonZero + 1);
        }
    }
    
    // Otherwise, print the original float
    cout << num;
}
/////////////////////////// STACK ///////////////////////////////////


class parse_tree_node {
public:
    bool is_terminal;
    string label;
    deque<parse_tree_node*> children;
    int uniqueKey;
    static int currentUniqueKey;
    string value;
    parse_tree_node(string l, bool t = true) : label(l), is_terminal(t) {
        uniqueKey = currentUniqueKey++;
    }
};
int parse_tree_node::currentUniqueKey = 0;

ostream& operator<<(ostream& out, parse_tree_node* node) {
    out << "\"" << (void*)node << "\" [label=\"" << node->label << "\"]";
    return out;
}

void visualize_parse_tree(parse_tree_node* root);
void visualize_parse_tree_aux(parse_tree_node* node, ostream& out);
//void drawNode(parse_tree_node* node, ofstream& file);
parse_tree_node* ll1_first_follow(vector<string>, vector<string>);




void visualize_parse_tree(parse_tree_node* root) {

    ofstream dot_file_out("parse_tree.dot", ios::trunc);

    dot_file_out << "digraph G {\n";
    dot_file_out << "  node [shape=box];\n"; //customize it
    visualize_parse_tree_aux(root, dot_file_out);
    dot_file_out << "}";
    dot_file_out.close();

    system("dot -Tpdf ./parse_tree.dot -oparse_tree.pdf");
    system("dot -Tpng parse_tree.dot -o parse_tree.png");
    system("parse_tree.pdf");

}


void visualize_parse_tree_aux(parse_tree_node* node, ostream& out) {
    string str;
    if(node -> value == "#")
        str = "ε";
    else if (node -> is_terminal)
        str = (node->value).substr(1, (node->value).length() - 2);
    else 
        str = node->value;

    out << "  \"" << node->uniqueKey << "\" [label=\"" << str << "\"]\n";
    for (auto child : node->children) {
        out << "  \"" << node->uniqueKey << "\" -> \"" << child->uniqueKey << "\"\n";
        visualize_parse_tree_aux(child, out);
    }
}




string startNT = "statement_list";

parse_tree_node* ll1_first_follow(vector<string> nonTerminalList, vector<string> terminalList) {

    map <pair<string, string>, vector<string>> table = ParseTable(nonTerminalList, terminalList);
    
    stack<parse_tree_node*> parsing_stack; //child ,label,terminal 
    parsing_stack.push(new parse_tree_node("$", true)); 
    parsing_stack.push(new parse_tree_node(startNT, false)); 
    parse_tree_node* root = parsing_stack.top(); 
    root->value = root->label;

    int counter = 0;
    string sepdt;
    string store;

    Token token = getNextToken();

    while (parsing_stack.top()->label != "$" && token.type != "UNKNOWN") {

        if (parsing_stack.top()->is_terminal) {  // 
                  
            //Addition value in symbol table
    

            string stackTop = parsing_stack.top()->label;
            string token1 = token.stringValue;
            string token2 = to_string((int)token.numValue);
            token1 = '"' + token1 + '"' ;
            token2 = '"' + token2 + '"' ;
            string tokenType = token.type;
            for (char& c : tokenType) 
                c = tolower(c);
            tokenType = '"' + tokenType + '"' ;
            
            if ((stackTop == token1)||(stackTop == token2)||(stackTop == tokenType)) 
            {
                parsing_stack.pop();
                stackTop = parsing_stack.top()->label;
            }
            else { 
                cout << "Syntax error at " << token1 << endl;
                return root;
            }

            token = getNextToken();
        }// Otherwise, if it is non-terminal,
        else {

            string mytoken; 
            if(token.type == "NUM" || token.type == "LITERAL" || token.type == "ID"){
                mytoken = token.type;
                for (char& c : mytoken) 
                    c = tolower(c);
            }
            else{
                mytoken = token.stringValue;
            }
            mytoken = '"' + mytoken + '"' ;

            vector<string> rule = table[make_pair(parsing_stack.top()->label, mytoken)];

            parse_tree_node* parent = parsing_stack.top();
            

            if(rule.size() == 0){
                map <string, vector<string>> f = NonTerminalClass::nonTerminalObject[parsing_stack.top()->label].getFirst();
                bool found = false;
                for (const auto& pair : f) 
                    if(pair.first == "\"#\""){
                        parsing_stack.pop();
                        parse_tree_node* node = new parse_tree_node("#", true);
                        node -> value = "#";
                        parent->children.push_front(node);
                        found = true;
                        break;
                    }

                if(!found)
                    return root;
            }else{
                if(rule[0] == "\"#\""){
                    parsing_stack.pop();

                    parse_tree_node* node = new parse_tree_node("#", true);
                    node -> value = "#";
                    parent->children.push_front(node);
                    
                }
                
                else if (table.find(make_pair(parsing_stack.top()->label, mytoken)) != table.end()) { // check grammar exists

                    parsing_stack.pop();
                    
                    for(int i = rule.size() - 1; i >= 0; i--) {  
                        string production_item = rule[i];
                        bool is_terminal = NonTerminalClass::isTerminal(production_item);

                        string mylabel = "";
                        if (production_item == "\"literal\""){
                            mylabel = token.stringValue;
                            mylabel = "(\\" + mylabel.substr(0, mylabel.length() - 1) + "\\\")\\";
                            mylabel = production_item.substr(0, production_item.length() - 1)+ "\\n" + mylabel;
                        }
                        else if(production_item == "\"id\""){
                            int index = token.numValue;
                            mylabel = symbolTable[index];
                            mylabel = "(\\" + mylabel+ "\\)\\";
                            mylabel = production_item.substr(0, production_item.length() - 1)+ "\\n" + mylabel;
                        }
                        else if(production_item == "\"num\""){
                            double num = token.numValue;
                            if(num == (int) num)
                                mylabel = to_string((int)num);
                            else 
                            mylabel = printFloat(num);
                            mylabel = mylabel + '"';
                            mylabel = production_item.substr(0, production_item.length() - 1)+ "\\n" + mylabel;

                        }
                        else
                            mylabel = production_item;

                        // Now create a node for it
                        parse_tree_node* node = new parse_tree_node(production_item, is_terminal);
                        node -> value = mylabel;
                        parsing_stack.push(node);
                        parent->children.push_front(node);

                    }
                    // Otherwise, parsing failed, return a nullptr for the parse tree
                }
                else {
                    std::cout << "Synax error at " << token.stringValue <<  endl;
                    std::cout << parsing_stack.top()->label << " -> ";
                    for (const string& str : rule) {
                        std::cout << str << ' ';
                    }
                    std::cout << endl;
                    return root;
                }
            
            }
        }
    }
    
    if(parsing_stack.size() == 2){
        parse_tree_node* parent = parsing_stack.top();

        parsing_stack.pop();

        parse_tree_node* node = new parse_tree_node("#", true);
        node -> value = "#";
        parent->children.push_front(node);
    }
    //return root;
    if (parsing_stack.top()->label == "$" && token.type == "UNKNOWN") return root; 
    else return root;
    
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




    // initialize class objects
    terminalList.push_back("$");
    //SETTERS
    for (int i = 0; i < nonTerminalList.size(); i++){
        NonTerminalClass::nonTerminalObject[nonTerminalList[i]].extractNonTerminals();
    }

    
    for (int i = 0; i < nonTerminalList.size(); i++){
        map <string, vector<string>> first = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFirst();  
    }

    bool o;
    for (int i = 0; i < nonTerminalList.size(); i++){
        if (i == 0)     
            o = true;
        else 
            o = false;
        vector<string> follow = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFollow(o);
        
    }


// Print tokens
cout << "Parse Table" << endl;
map <pair <string, string>, vector<string>> parseTable = ParseTable(nonTerminalList, terminalList);



for(int i = 0; i < nonTerminalList.size(); i++){ 
    cout << nonTerminalList[i] << endl;
    cout << "*********************" << endl;
    for(int j = 0; j < terminalList.size(); j++)
        if(parseTable.find(make_pair(nonTerminalList[i], terminalList[j])) != parseTable.end()){
            cout << terminalList[j] << " : ";
                for (const string& str : parseTable[make_pair(nonTerminalList[i], terminalList[j])]) 
                    cout << str << " ";
                cout << endl;
        }
   cout << endl;
}


    std::cout << "\nNow reading the file: " << endl;
    readFromFile(fileLoc, buffer1);



    parse_tree_node* parse_tree_root_node;


    cout << "FIRST-FOLLOW style LL(1) predictive parser\n" << endl;
    parse_tree_root_node = ll1_first_follow(nonTerminalList, terminalList);
    //parse_tree_root_node = parse2();



    cout << (parse_tree_root_node ? "Parsing successful.\n" : "Parsing failed.\n") << endl;
    if (parse_tree_root_node) visualize_parse_tree(parse_tree_root_node);



printSymbolTable();






    
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
*/
    
/*
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
*/

/*

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




}



