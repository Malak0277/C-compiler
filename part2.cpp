#include <iostream>
#include <string>
#include <fstream>
#include <deque>
#include <string>
using namespace std;

const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
string SymbolTable[1000][2]; //index is the entry		//DYNAMIC
//first 20 (or sth) rows are keywords

enum TokenType {
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    SEMICOLON,
};

struct Token {
    TokenType Type;
	int value;
};

const int ARRAY_SIZE = 1000;
Token tokenArray[ARRAY_SIZE];		//DYNAMIC

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

/*
void Token_Type(const std::string& token) { 
	static int Entry = 0;
	switch(token) {
	case 
	
	default:
		throw runtime_error("Invalid token: " + token);
	}

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
		tokenArray[Entry] = {token,TokenType::IDENTIFIER};
	else if(isComment(token))
		cout << "(comment, " << token << ")";
	else
		throw std::runtime_error("Invalid token: " + token);
}
*/


int main(){
	char *forward = buffer1, *start = buffer1;

	readFromFile("../m.txt", buffer1);
	//readFromFile("../m.txt", buffer2);
		
	bool eofFlag = false;
	while(!eofFlag) {

		switch(forward++){
		case EOF:
			if(forward == buffer1[BUF_SIZE-1])
				readFromFile("../m.txt", buffer2);
			else if (forward == buffer2[BUF_SIZE-1])
				readFromFile("../m.txt", buffer1);
			else 
				eofFlag = true;
			break;
			
		default:
			
		}

	}
	
    return 0;
}