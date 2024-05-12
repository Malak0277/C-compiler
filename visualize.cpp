#include "nonterminals.cpp"
#include "lexical analyzer.cpp"

class parse_tree_node {
public:
    bool is_terminal;
    string label;
    deque<parse_tree_node*> children;
    parse_tree_node( string l, bool t) : label(l), is_terminal(t) {}
};

std::ostream& operator<<(std::ostream& out, parse_tree_node* node) {
    out << "\"" << (void*)node << "\" [label=\"" << node->label << "\"]";
    return out;
}

void visualize_parse_tree(parse_tree_node* root);
void visualize_parse_tree_aux(parse_tree_node* node, std::ostream& out);

parse_tree_node* ll1_first_follow();
int main(int argc, char const* argv[]) {

    std::cout << "\nNow reading the file: " << argv[1] << std::endl;
    read_file(argv[1], buf);


    parse_tree_node* parse_tree_root_node;


    std::cout << "FIRST-FOLLOW style LL(1) predictive parser\n" << std::endl;
    parse_tree_root_node = ll1_first_follow();


    std::cout << (parse_tree_root_node ? "Parsing successful.\n" : "Parsing failed.\n") << std::endl;
    if (parse_tree_root_node) visualize_parse_tree(parse_tree_root_node);

    return 0;

}

void visualize_parse_tree(parse_tree_node* root) {

    std::ofstream dot_file_out("parse_tree.dot", std::ios::trunc);

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

void visualize_parse_tree_aux(parse_tree_node* node, std::ostream& out) {

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






parse_tree_node* ll1_first_follow() {

    map <pair<string, string>, vector<string>> table = parseTable(nonTerminalList, terminalList);
    
    stack<parse_tree_node*> parsing_stack;//child ,label,terminal 
    parsing_stack.push(new parse_tree_node("$", true)); // We always place $ at the bottom of the stack
    parsing_stack.push(new parse_tree_node("stmt-sequence", false)); // Followed by the top non-terminal of our CFG
    parse_tree_node* root = parsing_stack.top(); // Let's keep track of this top node so we can return it later

    Token token = getNextToken();

    
    int counter =0
    while (parsing_stack.top()->label != "$" && token.type != "UNKOWN") {
        parse_tree_node* top_of_stack = parsing_stack.top();                               

        if (top_of_stack->is_terminal) {  // 
            if (true) {
                if (counter == 2) {
                    if (!(token.stringValue.equals("("))) {
                        token.datatype = sepdt;
                    }
                    else
                        token.datatype = "function";
                    counter = 0;

                }// int num1,num2,num3


                if (token.type == "identifier") {
                    if (counter == 1)
                        coutnter++;
                    else {
                        int i = inSymbolTable(token.stringValue);
                        if (i == 0) {
                            
                            cout << "identifier must have datatype" << endl;
                            return nullptr;
                        }
                    }

                } 
                if (token.type.equals("keyword")) {
                    int c = inSymbolTable(token.stringValue)
                        if (c < 8) { //int float ndfn
                            counter++;
                            string sepdt = token.stringValue;

                        }

                }
            }

            
            
            
            // Check if it matches the current token
            if ((top_of_stack->label == token.stringValue)|| // KEYWORDSSSS && operations
                (top_of_stack->label ==token.type.to_lower() ))//el literials && numbers && identifiers 
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

            if (table.find(make_pair(top_of_stack->label, token1))) != table.end()) { // check grammar exists

                // If non-zero, first of all pop the stack to remove the current non-terminal that is at the top
                parsing_stack.pop();

                // Then, push its production items into the stack in reverse order (such that leftmost is at the top)
                // Note that if this production produces epsilon, this loop won't run, as size() will be 0
                vector <string> rule = table[make_pair(top_of_stack->label,Token1)]
                for (int i = rule.size() - 1; i >= 0; i--) {  
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
                cout << "Synax error at " << token.stringValue <<  endl;
                cout << top_of_stack->label << " -> ";
                for (const string& str : rule) {
                    cout << str << ' ';
                }
                cout << endl;
                return nullptr;
            }
        }
    }

    if (parsing_stack.top()->label == "$" && buffer_offset == 0) return root;
    else return nullptr;
    
}