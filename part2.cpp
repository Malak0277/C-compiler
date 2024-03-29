#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//Global variables
const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
string fileLoc = "test.cpp";

struct Token {
    string type;
    string stringValue;
    int intValue;
};

vector<string> symbolTable = {
        "auto", "break", "case", "char", "const", "continue","default", "do", "double", "else", "enum",
        "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed",
        "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
};        //identifiers start from index 32

//prototypes
void readFromFile(const string&, char[]);
string getTokenType(int);
Token getNextToken();
void getState(char, int &, int &, bool &);
string getLexeme(char*&, char *);
Token tokenize(char*, char*, int);
int inSymbolTable(string);
void printSymbolTable();

int main() {
    readFromFile(fileLoc, buffer1);

    Token next = getNextToken();

    while (next.type != "UNKNOWN") { 
        if (next.stringValue == "")
            cout << '<' << next.type << ", " << next.intValue << "> " << endl;
        else
            if(next.type == "keyword" || next.type == "LITERAL")
                cout << "<" << next.type << ", " << next.stringValue << "> " << endl;
            else
                cout << "<" << next.type << ", " << next.stringValue << " > " << endl;

        next = getNextToken();
    }

    printSymbolTable();
    return 0;
}


void readFromFile(const string& program, char buffer[]) {
    static int loc = 0; //location in file
    ifstream file(program, ios::binary);

    // Check if the opening was successful
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        exit(0);
    }

    file.seekg(loc); //start reading from last location
    file.read(buffer, BUF_SIZE-1);

    // Determine the number of characters read
    streamsize bytesRead = file.gcount();

    // update the location in the file
    loc += bytesRead;

    if (file.eof()) {
        buffer[bytesRead] = EOF;
    }

    buffer[BUF_SIZE-1] = EOF; //place sentinels at buffer end

    file.close();
}

string getTokenType(int type) {
    switch (type) {
        case 1: return "ID"; break;
        case 2: return "DEC"; break;
        case 3: return "BIN"; break;
        case 4: return "OCT"; break;
        case 5: return "HEX"; break;
        case 6: return "RELOP"; break; //>, <, >=, <=, ==, !=
        case 7: return "OPERATOR"; break; //+, -, *, /, %, ++, --,  ?:, &, ->, .
        case 8: return "ASSIGNING_OP"; break; //=, +=, -=, *=, /=. %=, &=, |=, ^=, <<=, >>=
        case 10: return "PUNCTUATION"; break; // {} () [] , ; 
        case 11: return "LITERAL"; break;
        case 12: return "LOGICAL_OP"; break; //&&, ||, !
        case 13: return "BITWISE_OP"; break;  // |, ~, ^, <<, >>
        default: return "UNKNOWN"; break; // Handle the case if type is not found
    }
}

Token getNextToken() {
    static char *forward = buffer1, *start = buffer1; //buffer pointers
    int state = 1, lexemeType = -1;
    char c;
    bool retrackFlag = false;
  
    //handling the states
    do{
        c = *forward;

        //dump comments and whitespaces
        if(state == 1 && lexemeType == -1 && start != forward) 
            string dump = getLexeme(start, forward);
            
        getState(c, state, lexemeType, retrackFlag);

        if(state == -1){ //error state
            cerr<<"\nInvalid token: ";  //do we need to save it as a token? (we are not)
            cout << getLexeme(start, forward) << endl << endl;
            state = 1; //back to initial state
        }
        
        //retrack forward ptr
        if(retrackFlag)
            retrackFlag = false;
        else{
            if(*forward != EOF)
                forward++; 
            //advancing forward ptr when meeting EOF
            if (*forward == EOF) {
                if (forward == &buffer1[BUF_SIZE - 1]){
                    readFromFile(fileLoc, buffer2);
                    forward = buffer2;
                }
                else if (forward == &buffer2[BUF_SIZE - 1]){
                    readFromFile(fileLoc, buffer1);
                    forward = buffer1;
                }
                else {
                    break;
                }
            
            } 
        }
        
        
    }while(state != 1 || lexemeType == -1);


    //check if terminated before reaching final state
    if(!(state == 1 || state == 0 || state == 23)){
        cerr << "\nToken incomplete: ";
        while(start!= forward){
            cout<<*start;
            start++;
        }
        cout << endl;
        Token token;
        token.type = "UNKNOWN";
        return token;
    }

    return tokenize(start, forward, lexemeType);

}

void getState(char c, int &state, int &lexemeType, bool &retrackFlag){  //Hager
    switch(state) {
            case 0:
                if (isspace(c))
                    state = 0;
                else {
                    state = 1;
                    lexemeType = -1; //to indicate that even though final state is reached(state 1) no token is going to be taken
                    retrackFlag = true;
                }
                break;
            case 1:
                if (isspace(c))
                    state = 0;
                else if (isalpha(c) || c == '_')
                    state = 2;
                else if (c == '0')
                    state = 8;
                else if (isdigit(c))
                    state = 3;
                else if (c == '/')
                    state = 20;
                else if (c == '"')
                    state = 24;
                else if (c == '\'')
                    state = 25;
                else if (c == '<')
                    state = 12;
                else if (c == '>')
                    state = 13;
                else if (c == '=')
                    state = 14;
                else if (c == '*' || c == '%')
                    state = 15;
                else if (c == '+')
                    state = 16;
                else if (c == '-')
                    state = 17;
                else if (c == '&')
                    state = 18;
                else if (c == '|')
                    state = 19;
                else if (c == '!')
                    state = 26;
                else if (c == '^')
                    state = 28;
                else if (c=='~')
                    lexemeType=13;
                else if (c=='?')
                    lexemeType=7;
                else if(c==':')
                    lexemeType=7;
                else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ',' || c == ';' || c == '[' || c == ']')
                    lexemeType = 10;
                else if(c == '.')
                    lexemeType = 7;
                else if(c == EOF)
                    break;
                else
                    state = 27;
                break;

            case 2:
                if (isalnum(c) || c == '_')
                    state = 2;
                else {
                    lexemeType = 1;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 3:
                if (isdigit(c))
                    state = 3;
                else if (c == '.')
                    state = 4;
                else if (c == 'e' || c == 'E')
                    state = 5;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 2;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 4:
                if (isdigit(c))
                    state = 4;
                else if (c == 'e' || c == 'E')
                    state = 5;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 2;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 5:
                if (isdigit(c))
                    state = 7;
                else if (c == '+' || c == '-')
                    state = 6;
                else
                    state = 27;
                break;

            case 6:
                if (isdigit(c))
                    state = 7;
                else
                    state = 27;
                break;

            case 7:
                if (isdigit(c))
                    state = 7;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 2;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 8:
                if (c == 'b' || c == 'B')
                    state = 9;
                else if (c == 'x' || c == 'X')
                    state = 10;
                else if (isdigit(c))
                    state = 11;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 2;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 9:
                if (c == '0' || c == '1')
                    state = 9;
                else if (isalnum(c))
                    state = 27;
                else {
                    lexemeType = 3;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 10:
                if (isdigit(c) || c == 'A' || c == 'a' || c == 'B' || c == 'b' || c == 'C' || c == 'c'
                    || c == 'D' || c == 'd' || c == 'E' || c == 'e' || c == 'F' || c == 'f')
                    state = 10;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 5;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 11:
                if (c == '8' || c == '9')
                    state = 27;
                else if (isdigit(c))
                    state = 11;
                else if (isalpha(c))
                    state = 27;
                else {
                    lexemeType = 4;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 12:
                if (c == '=') {
                    lexemeType = 6;
                    state = 1;
                }
                else if (c=='<')
                    state = 29;
                else {
                    lexemeType = 6;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 13:
                if (c == '=') {
                    lexemeType = 6;
                    state = 1;
                }
                else if (c=='>')
                    state = 30;
                else {
                    lexemeType = 6;
                    retrackFlag = true;
                    state = 1;
                }
                break;


            case 14:
                if (c == '=') {
                    lexemeType = 6;
                    state = 1;
                } else {
                    lexemeType = 8;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 15:
                if (c == '=') {
                    lexemeType = 8;
                    state = 1;
                } else {
                    lexemeType = 7;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 16:
                if (c == '+') {
                    lexemeType = 7;
                    state = 1;
                } else if (c == '=') {
                    lexemeType = 8;
                    state = 1;
                }
                else {
                    lexemeType = 7;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 17:
                if (c == '-') {
                    lexemeType = 7;
                    state = 1;
                } else if (c == '=') {
                    lexemeType = 8;
                    state = 1;
                } 
                else if(c == '>'){
                    lexemeType = 7;
                    state = 1;
                }
                else {
                    lexemeType = 7;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 18:
                if (c == '&')
                    lexemeType = 12;
                else if(c=='=')
                    lexemeType = 8;
                else{
                    lexemeType = 7;
                    retrackFlag = true;
                }
                state =1;
                break;

            case 19:
                if (c == '|') {
                    lexemeType = 12;
                } else if(c=='=')
                    lexemeType = 8;
                else{
                    lexemeType = 13;
                    retrackFlag = true;
                }
                state = 1;
                break;
                

            case 20:
                if (c == '/')
                    state = 23;
                else if (c == '*')
                    state = 21;
                else{
                    retrackFlag = true; 
                    state = 15; //going to be treated as divide operator and not a comment
                }
                break;

            case 21:
                if (c == '*')
                    state = 22;
                else
                    state = 21;
                break;

            case 22:
                if (c == '*')
                    state = 22;
                else if (c == '/') {
                    lexemeType = -1; //no token returned, just move the start
                    state = 1;
                } else
                    state = 21;
                break;

            case 23:
                if (c == '\n') {  
                    lexemeType = -1; //no token returned, just move the start
                    state = 1;
                }
                break;

            case 24: // string
                if (c == '\\')
                    state = 33;
                else if (c != '"')
                    state = 24;
                else{
                    lexemeType=11;
                    state = 1;
                }
                break;


            case 25: // char
                if (c == '\'')
                    state = 27;
                else if(c == '\\')
                    state = 31;
                else
                    state = 32;
                break;

            case 26:
                if (c == '=') {
                    lexemeType = 6;
                    state = 1;
                } else {
                    lexemeType = 12;
                    state = 1;
                    retrackFlag = true;
                }
                break;

            case 27:   //Error state
                if(isspace(c) || c == ';'){
                    state = -1; //erros indicator for lexeme to get discarded
                    retrackFlag = true;
                }
                break;
            case 28:
                if(c=='=')
                    lexemeType=8;
                else{
                    retrackFlag = true;
                    lexemeType=13;
                }
                state = 1;
                break;
            case 29:
                if(c=='=')
                    lexemeType=8;
                else{
                    retrackFlag = true;
                    lexemeType=13;
                }
                state=1;
                break;
            case 30:
                if(c=='=')
                    lexemeType=8;
                else{
                    retrackFlag = true;
                    lexemeType=13;
                }
                state=1;
                break;

            case 31:
                if (c == 'n' || c == 't' || c == 'r' || c == '\'' || c == '\"' || c == '\\' || c == '0' || c == 'a' || c == 'b' || c == 'f' || c == 'v') 
                    state = 32;
                else
                    state = 27;
                break;

            case 32:
                if (c == '\''){
                    state = 1;
                    lexemeType = 11;
                }
                else
                    state = 27;
                break;

            case 33:
                state = 24;
            break;
                

        }

}

string getLexeme(char*& start, char *forward){
    string lexeme;
    while (1) { 
        if (*start == EOF) 
            if (start == &buffer1[BUF_SIZE - 1])
                start = buffer2;
            else if (start == &buffer2[BUF_SIZE - 1])
                start = buffer1;
    
    if(start == forward) break;
    lexeme += *start;
    start++;
    }
    return lexeme;
}

Token tokenize(char *start, char* forward, int lexemeType){
    static int STindex = 32;
    Token token;
    string lexeme = getLexeme(start, forward);

    if(lexemeType >= 2 && lexemeType <= 5)
        token.type = "NUM";
    else
        token.type = getTokenType(lexemeType);

    if(lexemeType == 1) {  //keywords or identifier
        int inTable = inSymbolTable(lexeme); //get position in symbol table
        
        if (inTable >= 0 && inTable < 32){ // keyword
            token.type = "keyword";
            token.stringValue = lexeme;
        }
        else if (inTable < STindex) // already existing identifier
            token.intValue = inTable;
        else { // new identifier
            symbolTable.push_back(lexeme);
            token.intValue = STindex++;
        }
    }
    //Numbers
    else if(lexemeType == 2){ //dec 
        token.intValue = stoi(lexeme);
    } 
    else if(lexemeType == 3){ //bin
        const char* charPtr = &lexeme[2];
        token.intValue = stoi(charPtr, NULL, 2);
    } 
    else if(lexemeType == 4 || lexemeType == 5){ //oct + hex
        const char* charPtr = &lexeme[0];
        token.intValue = stoi(charPtr, NULL, 0);
    }
    else if(lexemeType >= 6 && lexemeType <= 13) //others
        token.stringValue = lexeme;

    return token;
}

int inSymbolTable(string identifier) {
    int entry = 0;
    while(entry < symbolTable.size()) {
        if (symbolTable[entry] == identifier) break;
        entry++;
    }
    return entry;
}

void printSymbolTable() {
    if (symbolTable.size() == 32) return;
    cout << endl;
    cout << "Entry\tIdentifier" << endl;
    cout << "------------------"<< endl;
    for (int i = 32; i < symbolTable.size(); i++)
        cout << i << "\t" << symbolTable[i] << endl;
}