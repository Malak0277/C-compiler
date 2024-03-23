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
    IDENTIFIER=1,
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
	int state = 1; 
	char c; 
	bool eofFlag = false, retrackFlag = false;
	int lexemeType=-1 ;
	

	readFromFile("../m.txt", buffer1);
	//readFromFile("../m.txt", buffer2);
		
	
	while(!eofFlag) {

		switch(*forward){
		case EOF:
			if(*forward == buffer1[BUF_SIZE-1])
				readFromFile("../m.txt", buffer2);
			else if (*forward == buffer2[BUF_SIZE-1])
				readFromFile("../m.txt", buffer1);
			else 
				eofFlag = true;
			break;
			
		default:
			
		}
		c = *forward;
		forward++;
		// if(isspace(c)){
		// 	state
		// }
		switch (state)
		{
		case 1: 
			
			if(isalpha(c)|| c=='_')
				state = 2;
			else if(isdigit(c)){
				state = 3;
			}
		break;
		case 2: 
			if(isalnum(c) || c=='_')
				state =2;
			else if(isspace(c))
			{
				lexemeType = 1;
				retrackFlag =true;
				state = 0; //case 0 111111111111111111111111111111111111111111111111111111111111111111111111111
			}
			else{
				lexemeType = 1;
				retrackFlag =true;
				state = 1;
			}
			break;
		case 3:
			if(c==0)
			  	state =;
			
			else if (isdigit(c))
				state = 3;
			else if (c=='.')
				state= 4;
			else if (c=='e'||c=='E')
				state= 5;
			else if(isalpha(c))  //12d
				state = ; //error
			else {
			    lexemeType = 2;
				retrackFlag =true;
				state = 1;
			     }	
		case 4:
		    if (isdigit(c))
				state = 4;
			else if (c=='e'||c=='E')
				state= 5;
			else if(isalpha(c))  //12d
				state = ; //error
			else {
			    lexemeType = 2;
				retrackFlag =true;
				state = 1;
			     }
		case 5:
		    if (isdigit(c))
				state = 7;
			else if(c=='+'|| c=='-')
			 	state = 6;
			else 
			 	state = ; // error
		case 6:
			if (isdigit(c))
				state = 7;
			
			else 
			 	state = ; // error
		case 7:
			if (isdigit(c))
				state = 7;
			else if(isalpha(c))  //12d
				state = ; //error
			else {
			    lexemeType = 2;
				retrackFlag =true;
				state = 1;
			     }    
			
		default:
			break;
		}

	}
	
    return 0;
}