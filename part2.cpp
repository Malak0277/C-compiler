#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

//Notes:
//symbol table
//dynamic arrays

//static int tokenIndex = 0;
const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];

struct Token {
    string type;
    string stringValue;
    int intValue;
};
Token tokenArray[1000];		//one element = type + value	//DYNAMIC
//Token* tokenArray = new Token[1000];
//vector<Token> tokenArray;

//dynamic array:     //std::string** symbolTable = new std::string*[1000]; for (int i = 0; i < 1000; ++i) symbolTable[i] = new std::string[2];
//vector of vector:  //vector<vector<string>> symbolTable(1000, vector<string>(2));
//vector of array:   //vector<array<int, 2>> symbolTable;
string symbolTable[1000][2]		//index is the entry, lexeme, and type.		//DYNAMIC
     = {
        {"auto", "keyword"},
        {"break", "keyword"},
        {"case", "keyword"},
        {"char", "keyword"},
        {"const", "keyword"},
        {"continue", "keyword"},
        {"default", "keyword"},
        {"do", "keyword"},
        {"double", "keyword"},
        {"else", "keyword"},
        {"enum", "keyword"},
        {"extern", "keyword"},
        {"float", "keyword"},
        {"for", "keyword"},
        {"goto", "keyword"},
        {"if", "keyword"},
        {"int", "keyword"},
        {"long", "keyword"},
        {"register", "keyword"},
        {"return", "keyword"},
        {"short", "keyword"},
        {"signed", "keyword"},
        {"sizeof", "keyword"},
        {"static", "keyword"},
        {"struct", "keyword"},
        {"switch", "keyword"},
        {"typedef", "keyword"},
        {"union", "keyword"},
        {"unsigned", "keyword"},
        {"void", "keyword"},
        {"volatile", "keyword"},
        {"while", "keyword"}
        //identifiers starts from index 32
};

int isInTable(string identifier) {
    int entry = 0;
    while(symbolTable[entry][0] != "") {
        if (symbolTable[entry][0] == identifier) return -1;
        entry++;
    }
    return entry;
}

string getTokenType(int type) {
    static map<int, string> tokenTypes = {
            {1, "ID"},
            {2, "DEC"},
            {3, "BIN"},
            {4, "OCT"},
            {5, "HEX"},
            {6, "RELOP"}, //>, <, >=, <=, <>, ==, !=
            {7, "OPERATOR"}, //+, -, *, /, %, ++, --,  ?:, &,   
            {8, "ASSIGNING_OP"}, //=, +=, -=, *=, /=. %=, &=, |=, ^=, <<=, >>=
            {10, "PUNCTUATION"}, // {} () [] , ; . ->
            {11, "LITERAL"}, 
            {12, "LOGICAL_OP"}, //&&, ||, !
            {13, "BITWISE_OP"}, // |, ~, ^, <<, >>
    };
      

    auto it = tokenTypes.find(type);
    if (it != tokenTypes.end()) {
        return it->second;
    } else {
        return "UNKNOWN";
    }
}

void readFromFile(const string& program, char buffer[]) {
    static int loc = 0;

    ifstream file(program, ios::binary);

    // Check if the opening was successful
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
    }

    file.seekg(loc);
    file.read(buffer, BUF_SIZE-1);

    // Determine the number of characters read
    streamsize bytesRead = file.gcount();

    // update the location in the file
    loc += bytesRead;

    if (file.eof()) {
        buffer[bytesRead] = EOF;
    }

    buffer[BUF_SIZE-1] = EOF;

    file.close();
}


void printSymbolTable(){
    cout << "Index\tKeyword\t\tType" << endl;
    for (int i = 0; i < 32; i++)
        cout << i << "\t" << symbolTable[i][0] << "\t\t" << symbolTable[i][1] << endl;
}


int main() {
    char *forward = buffer1, *start = buffer1;
    int state = 1, tokenIndex = 0;
    char c;
    bool eofFlag = false, retrackFlag = false;
    string lexeme;
    int lexemeType = -1;

    printSymbolTable();
    readFromFile("../m.txt", buffer1);

    while(1) {
        if (retrackFlag) {
            if (forward == buffer1)
                forward = &buffer2[BUF_SIZE - 2];
            else if (forward == buffer2)
                forward = &buffer1[BUF_SIZE - 2];
            else
                forward--;
            retrackFlag = false;
        }

        //get lexeme
        if (state == 1 && lexemeType != -1) {
            int i = 0;
            while (start != forward) {
                if (*start == EOF) {
                    if (start == &buffer1[BUF_SIZE - 1])
                        start = buffer2;
                    if (start == &buffer2[BUF_SIZE - 1])
                        start = buffer1;
                }
                lexeme += *start;   
                start++;
            }
            tokenIndex++;
        } else if (state == 1) start = forward;

            tokenArray[tokenIndex].type = getTokenType(lexemeType);
            if(lexemeType == 1)
              //Mariaaam 
            ;
            /*
            1 - int - ptr
            2/3/4/5 - int - number
            6-12 - String - content
            */
            else if(lexemeType >= 2 || lexemeType <= 5)
              tokenArray[tokenIndex].intValue = stoi(lexeme);
            else if(lexemeType >= 6 || lexemeType <= 12)
              tokenArray[tokenIndex].stringValue = lexeme;
            


        if (*forward == EOF) {
            if (forward == &buffer1[BUF_SIZE - 1])
                readFromFile("../m.txt", buffer2);
            else if (forward == &buffer2[BUF_SIZE - 1])
                readFromFile("../m.txt", buffer1);
            else
                break;
        }

        c = *forward;
        forward++;
        switch (state) {
            case 0:
                if (isspace(c)) 
                    state = 0;
                else {
                    state = 1;
                    lexemeType = -1;
                }
            case 1:
                if (isspace(c))
                    state = 0;   
                else if (isalpha(c) || c == '_')
                    state = 2;
                else if (isdigit(c))
                    state = 3;
                else if (c == '/')
                    state = 20;
                else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ',' || ';' || '[' || ']'||'.')
                    lexemeType = 10;
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
                else if (c == '*' || c == '/' || c == '%')
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
                else
                    state = 27;
                break;

            case 2:
                if (isalnum(c) || c == '_')
                    state = 2;
                else if (isspace(c)) {
                    lexemeType = 1;
                    retrackFlag = true;
                    state = 0; //case 0 111111111111111111111111111111111111111111111111111111111111111111111111111
                } else {
                    lexemeType = 1;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 3:
                if (c == '0')
                    state = 8;
                else if (isdigit(c))
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
                } else {
                    lexemeType = 6;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 13:
                if (c == '=') {
                    lexemeType = 6;
                    state = 1;
                } else {
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
                } else if (c == '0')
                    state = 8;
                else if (isdigit(c))
                    state = 3;
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
                } else if (c == '0')
                    state = 8;
                else if (isdigit(c))
                    state = 3;
                else if(c == '>'){
                     lexemeType = 10;
                     state = 1;
                }
                else {
                    lexemeType = 7;
                    retrackFlag = true;
                    state = 1;
                }
                break;

            case 18:
                if (c == '&') {  
                    lexemeType = 12;  
                    state = 1;
                } else {
                    retrackFlag = true;
                    //state = error;   //not
                }
                break;
                
            case 19:
                if (c == '|') {
                    lexemeType = 12;
                    state = 1;
                } else {
                retrackFlag = true;
                //state = error;
                }
                break; 
            //^

            case 20:
                if (c == '/')
                    state = 23;
                else if (c == '*')   
                    state = 21;
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
                if (c == '/')
                    state = 23;
                else if (c == '\n') {  //////////////
                    lexemeType = -1; //no token returned, just move the start
                    state = 1;
                }
                break;

          case 24: // string
                if (c == '"') {
                  lexemeType = 11;
                  state = 1;
                }
                break;  //nothing will cause an error

          case 25: // string
                if (c == '\'') { //edit
                  lexemeType = 11;
                  state = 1;
                }
                break;  //nothing will cause an error

        /*case 24: // string
                if (isprint(c)) 
                    state = 25;
                else;//state =; //error
                break;
            case 25:
                if (isprint(c))
                    state = 25;
                else if (c == '"') {
                    lexemeType = 11;
                    state = 1;
                } else;//state =; //error
                break;
            */
                

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
              if(isspace(c)){//edit
                char *temp = start;
                cerr<<"invalid token: ";  //do we need to save it as a token?
                while(temp != forward)
                  cout<<temp++;
                cout << endl;
                state = 0;
            break;
            }
    }
  }
  
  if(state != 1){
    cerr << "token incomplete: ";
    while(start!= forward)
        cout<<start++;
    cout << endl;
  }

  cout << "end of file reached" <<endl;

    
    //case token is id:
    int STindex = 32;
    for (int i = 0; i < tokenIndex; i++) {
        int inTable = isInTable(tokenArray[i].value);
        if (inTable >= 0 && inTable < 32) //it is a keyword
            tokenArray[i].type = "keyword";
        else if (tokenArray[i].type == "ID" && inTable == -1) {      //add to ST
            symbolTable[STindex][0] = tokenArray[i].value;
            symbolTable[STindex][1] = tokenArray[i].type;
            tokenArray[i].value = STindex++;    //entry to (index in) symbol table
        } else
            tokenArray[i].value = inTable;      //entry to symbol table
    }

    //kont bgarab print ay haga
    for (int i = 0; i < 3; i++) {
        cout << tokenArray[i].value << " " << tokenArray[i].type << endl;
    }

    //delete[] tokenArray;
    return 0;
}