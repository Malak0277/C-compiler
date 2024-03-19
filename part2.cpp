#include <iostream>
#include <string>
#include <fstream>
#include <deque>
#include <string>
using namespace std;
// assuming ^ indicates eof 

enum TokenType {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SEMICOLON,
};

const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
char token[100];
//string Symboltable[][3]; //Symbol table is a two dimensional array to store

struct Data {
    std::string tokenn;
    TokenType Type;
    };

const int ARRAY_SIZE = 1000;
Data dataArray[ARRAY_SIZE];

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
        buffer[bytesRead] = '^';
    }

    buffer[BUF_SIZE-1] = '^';

    file.close();

}

/*
void fff(){
    char *forward=buffer1, *start=buffer1;
    start=forward=readFromFile("program.txt", buffer1);
    deque<string> myDeque;
    bool Flag = false;
    while(!Flag){
        if(*forward==' '){
            int i =0;
            while((forward-1)!=start){
                token[i] = *start;
                start++;
                i++;
            }
            token[i] = '\0';
            myDeque.push_back(token);//pushes string not char
            //myDeque.push_back("^");
        }

        switch(*forward++){
        case '^':
            if(*forward== buffer1[BUF_SIZE-1])
                forward=readFromFile("program.txt", buffer2);
            else if (*forward== buffer2[BUF_SIZE-1])
                forward=readFromFile("program.txt", buffer1);
            else 
                Flag=true;

    }

    }
    while(myDeque.size() > 0){
        Token_Type(myDeque.front());
        myDeque.pop_front();
    }
    

}

*/
/*
void Token_Type(const std::string& token)
{ 
  static int Entry=0;
  if(isOperator(token))
    cout << "(operator, " << token << ")";
  else if(isSeparator(token))
    cout << "(separator, " << token << ")";
  else if(isKeyword(token))
    cout << "(keyword, " << token << ")";
  else if(isStatement(token))
    cout << "(statement, " << token << ")";
  else if(isLiteral(token))
    cout << "(literal, " << token << ")";
  else if(isID(token))
    cout << "(identifier, " << token << ")";
    dataArray[Entry]={token,TokenType::IDENTIFIER};
  else if(isComment(token))
    cout << "(comment, " << token << ")";
  else
    throw std::runtime_error("Invalid token: " + token);
}

*/


int main(){

    readFromFile("../m.txt", buffer1);
    readFromFile("../m.txt", buffer2);

    
}