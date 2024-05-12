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


/////////////////////////// STACK ///////////////////////////////////


class parse_tree_node {
public:
    bool is_terminal;
    string label;
    deque<parse_tree_node*> children;
    parse_tree_node( string l, bool t) : label(l), is_terminal(t) {}
};

ostream& operator<<(ostream& out, parse_tree_node* node) {
    out << "\"" << (void*)node << "\" [label=\"" << node->label << "\"]";
    return out;
}

void visualize_parse_tree(parse_tree_node* root);
void visualize_parse_tree_aux(parse_tree_node* node, ostream& out);

parse_tree_node* ll1_first_follow(vector<string>, vector<string>);


void visualize_parse_tree(parse_tree_node* root) {

    ofstream dot_file_out("parse_tree.dot", ios::trunc);

    // TODO: Would be nice if you decorated your parse tree, lookup how to do so in graphviz dot notation
    dot_file_out << "digraph G {\n";
    visualize_parse_tree_aux(root, dot_file_out);
    dot_file_out << "}";
    dot_file_out.close();

    // Convert the dot file into a SVG vector image
    system("dot -Tpdf ./parse_tree.dot -oparse_tree.pdf");

    // Open the vector file
    // TODO: Integrate this with your GUI...
    // You'll need to lookup how to open an .svg file inside Qt or whatever GUI framework you are using
    system("parse_tree.pdf");

}

void visualize_parse_tree_aux(parse_tree_node* node, ostream& out) {

    // Need to uniquely identify each node in graphviz
    out << "  " << node << (node->is_terminal ? "[fontcolor=\"red\"][color=\"red\"];\n" : ";\n");

    for (auto child : node->children) {

        bool child_has_children = child->children.size() > 0;
        if (child->is_terminal || (!child->is_terminal && child_has_children)) {

            // Need to uniquely identify each child node too
            out << "  " << child << ";\n";

            // Create edge between node and child unique addresses
            out << "  \"" << (void*)node << "\" -> \"" << (void*)child << "\";\n";

            // Recursively build rest of tree
            visualize_parse_tree_aux(child, out);

        }

    }

}






parse_tree_node* ll1_first_follow(vector<string> nonTerminalList, vector<string> terminalList) {

    map <pair<string, string>, vector<string>> table = ParseTable(nonTerminalList, terminalList);
    
    stack<parse_tree_node*> parsing_stack;//child ,label,terminal 
    parsing_stack.push(new parse_tree_node("$", true)); // We always place $ at the bottom of the stack
    parsing_stack.push(new parse_tree_node("stmt-sequence", false)); // Followed by the top non-terminal of our CFG
    parse_tree_node* root = parsing_stack.top(); // Let's keep track of this top node so we can return it later

    Token token = getNextToken();

    
    int counter = 0;
    string sepdt;
    while (parsing_stack.top()->label != "$" && token.type != "UNKOWN") {
        parse_tree_node* top_of_stack = parsing_stack.top();                               

        if (top_of_stack->is_terminal) {  // 
            if (true) {
                if (counter == 2) {
                    if (token.stringValue != "(") {
                        token.datatype = sepdt;
                    }
                    else
                        token.datatype = "function";
                    counter = 0;

                }// int num1,num2,num3


                if (token.type == "identifier") {
                    if (counter == 1)
                        counter++;
                    else {
                        int i = inSymbolTable(token.stringValue);
                        if (i == 0) {
                            cout << "identifier must have datatype" << endl;
                            return nullptr;
                        }
                    }

                } 
                if (token.type == "keyword"){
                    int c = inSymbolTable(token.stringValue);
                        if(c < 8) { 
                            counter++;
                            sepdt = token.stringValue;

                        }

                }
            }

            
        
            string stackTop = top_of_stack->label.substr(1, stackTop.size()-2);
            for (char& c : token.type) 
                c = tolower(c);
            
            // Check if it matches the current token
            if ((stackTop == token.stringValue)|| // KEYWORDSSSS && operations
                (stackTop ==token.type))//el literials && numbers && identifiers 
                 {
                // If it matches pop the stack, get the next token, and move on to the next iteration
                parsing_stack.pop();
                Token token = getNextToken();
                continue;

                // Otherwise, parsing failed, return a nullptr for the parse tree
            }
            else { 
                cout << "Synax error at " << token.stringValue << endl;
                return nullptr;
            }
        }// Otherwise, if it is a non-terminal,
        else {

            string token1 = token.stringValue;

            // dict outer terminal m3 vector
            // Check if there exists a valid production from this non-terminal for the current Token1
            // We do this by checking how many productions exist for this non-terminal for the current Token1 
            // Since our grammar is LL(1) (i.e., non-ambiguous), there can only be 1 production, or 0 (none)

            vector <string> rule = table[make_pair(top_of_stack->label, token1)];
            if (table.find(make_pair(top_of_stack->label, token1)) != table.end()) { // check grammar exists

                // If non-zero, first of all pop the stack to remove the current non-terminal that is at the top
                parsing_stack.pop();

                // Then, push its production items into the stack in reverse order (such that leftmost is at the top)
                // Note that if this production produces epsilon, this loop won't run, as size() will be 0
                for(int i = rule.size() - 1; i >= 0; i--) {  
                     string production_item = rule[i];

                    // Before I proceed, I want to check if this production item is a terminal or a non-terminal
                    // I can do this by trying to find it in the production dictionary non-terminals keys
                    // If it is not found, then it is a terminal; it is not found if "find()" returns the "end()" iterator
                     bool is_terminal = NonTerminalClass::isTerminal(production_item);

                    // Now I create a node for it
                    parse_tree_node* node = new parse_tree_node(production_item, is_terminal);

                    // Add it to the children of its parent non-terminal
                    // Since I am using a deque, I can push_front instead of push_back
                    // This is to cancel out the backwards iteration of the child nodes
                    top_of_stack->children.push_front(node);

                    // Finally, push it to the stack
                    parsing_stack.push(node);

                }

                // Otherwise, parsing failed, return a nullptr for the parse tree
            }
            else {
                std::cout << "Synax error at " << token.stringValue <<  endl;
                std::cout << top_of_stack->label << " -> ";
                for (const string& str : rule) {
                    std::cout << str << ' ';
                }
                std::cout << endl;
                return nullptr;
            }
        }
    }

    //return root;
    if (parsing_stack.top()->label == "$" && token.type != "UNKNOWN") return root; 
    else return nullptr;
    
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

    bool o;
    for (int i = 0; i < nonTerminalList.size(); i++){
        map <string, vector<string>> first = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFirst();  
        if (i == 0)     
            o = true;
        else 
            o = false;
        vector<string> follow = NonTerminalClass::nonTerminalObject[nonTerminalList[i]].getFollow(o);
 
    }


    cout << "\nNow reading the file: " << endl;
    readFromFile(fileLoc, buffer1);


    parse_tree_node* parse_tree_root_node;


    cout << "FIRST-FOLLOW style LL(1) predictive parser\n" << endl;
    parse_tree_root_node = ll1_first_follow(nonTerminalList, terminalList);


    cout << (parse_tree_root_node ? "Parsing successful.\n" : "Parsing failed.\n") << endl;
    if (parse_tree_root_node) visualize_parse_tree(parse_tree_root_node);

    
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

/*
// parse table
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
*/

}



