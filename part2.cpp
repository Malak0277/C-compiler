#include <iostream>
#include <string>
#include <fstream>
#include <deque>
#include <string>
using namespace std;

//Notes: // + whitespaces + retracting + lexemeType=-1 and retract_flag=false + retract_flag = true => start = forward
// errors + final_state_digits + sign in b/x/o?


const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
string SymbolTable[1000][2]; //index is the entry		//DYNAMIC
//first 20 (or sth) rows are keywords

enum TokenType {
    ID = 1,
    DEC,
    BIN,
    OCT,
    HEX,
    RELOP,
    OPERATOR, 
    ASSIGNING_OP,
    SEMICOLON,
    SEPERATOR,
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




int main(){
	char *forward = buffer1, *start = buffer1; 
	int state = 1; 
	char c; 
	bool eofFlag = false, retrackFlag = false;
	int lexemeType = -1 ;
	

	readFromFile("../m.txt", buffer1);
	//readFromFile("../m.txt", buffer2);
		
	
	while(1) {
    if(retrackFlag){
      if(forward == buffer1)
        forward = &buffer2[BUF_SIZE-2];
      else if(forward == buffer2)
        forward = &buffer1[BUF_SIZE-2];
      else 
        forward--;

    }

		switch(*forward){
		case EOF:
			if(forward == &buffer1[BUF_SIZE-1])
				readFromFile("../m.txt", buffer2);
			else if (forward == &buffer2[BUF_SIZE-1])
				readFromFile("../m.txt", buffer1);
			else 
				eofFlag = true;
			break;
			
		default:
			
		}

    if(eofFlag)
      break;

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
			else if(isdigit(c))
				state = 3;
			
      else if(c =='/')
        state = 20;

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
			if(c == 0)
			  	state = 8;
			else if (isdigit(c))
				state = 3;
			else if (c=='.')
				state= 4;
			else if (c=='e'||c=='E')
				state= 5;
			else if(isalpha(c))  //12d
				;//state = ; //error
			else {
			  lexemeType = 2;
				retrackFlag = true;
				state = 1;
			  }	
      break;
		case 4:
		  if (isdigit(c))
				state = 4;
			else if (c=='e'|| c=='E')
				state= 5;
			else if(isalpha(c))  //12d
				;//state = ; //error
			else {
			  lexemeType = 2;
				retrackFlag =true;
				state = 1;
			}
    break;
		case 5:
		    if (isdigit(c))
				state = 7;
			else if(c=='+'|| c=='-')
			 	state = 6;
			else 
			 	;//state = ; // error
      break;
		case 6:
			if (isdigit(c))
				state = 7;
			
			else 
			 	;//state = ; // error
      break;
		case 7:
			if (isdigit(c))
				state = 7;
			else if(isalpha(c))  //12d
				;//state = ; //error
			else {
			  lexemeType = 2;
				retrackFlag =true;
				state = 1;
			}    
    break;

    case 8:
      if (c == 'b' || c == 'B')
        state = 9;
      else if (c == 'x' || c == 'X')
        state = 10;
      else if(isdigit(c))
        state = 11;
      else if(isalpha(c))
        ;//state = ; //error
      else 
        lexemeType = 2;
				retrackFlag = true;
				state = 1;
    break;

    case 9:
      if(c == '0' || c == '1')
        state = 9;
      else if(isalnum(c))
        ;//state = ; //error
      else 
        lexemeType = 3;
				retrackFlag = true;
				state = 1;
    break;

    case 10:
      if (isdigit(c) || c == 'A' || c == 'a' || c == 'B' || c == 'b' || c == 'C' || c == 'c' ||
          c == 'D' || c == 'd' || c == 'E' || c == 'e' || c == 'F' || c == 'f'){
            state = 10;
      }
      else if(isalpha)
        ;//state = ; //error
      else 
        lexemeType = 5;
				retrackFlag = true;
				state = 1;
    break;

    case 11: 
      if(c == '8' || c == '9')
        ;//state = ; //error
      else if (isdigit(c))
        state = 11;
      else if(isalpha)
        ;//state = ; //error
      else 
        lexemeType = 4;
				retrackFlag = true;
				state = 1;
    break;




    case 20:
      	if(c == '/') 
          state = 23;
        else if(c == '*')
          state = 21;
			break;

    case 21:
      if(c == '*')
        state = 22;
      else
        state = 21;
      break;

    case 22:
      if(c == '*')
        state = 22;
      else if(c == '/'){
        lexemeType = 0; //no token returned, just move the start
				state = 1;
      }
      else 
        state = 21;
    break;

    case 23:
      if(c == '/') 
          state = 23;
      if(c=='\n'){  //////////////
        lexemeType = 0; //no token returned, just move the start
				state = 1;
      }
      
    break;
		}

	}
	
    return 0;
}

