vector<pair<string, string>> symbolTable = {
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
----------------------------
Token getNextToken(const char* buffer1, const char* buffer2) {
    static int STindex = 32;
    char *forward = buffer1, *start = buffer1;
    int state = 1, tokenIndex = 0, lexemeType = -1;
	char c;
    bool eofFlag = false, retrackFlag = false;
    string lexeme;
	
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

        } else if (state == 1) start = forward;

        tokenArray[tokenIndex].type = getTokenType(lexemeType);

        if(lexemeType == 1) {
            int inTable = isInTable(lexeme);
            if (inTable >=0 && inTable < 32)
                tokenArray[tokenIndex].type = "keyword";
            if (inTable < STindex)
                tokenArray[tokenIndex].intValue = inTable;
            else {
                symbolTable[STindex] = lexeme;
                tokenArray[tokenIndex].intValue = STindex++;
            }
        }
            /*
            1 - int - ptr
            2/3/4/5 - int - number
            6-12 - String - content
            */
        else if(lexemeType >= 2 || lexemeType <= 5)
            tokenArray[tokenIndex].intValue = stoi(lexeme);
        else if(lexemeType >= 6 || lexemeType <= 12)
            tokenArray[tokenIndex].stringValue = lexeme;
        tokenIndex++;


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
                break;///////////3shan ytl3 yd5ol tany loop
            case 1:
                if (isspace(c))
                    state = 0;
                else if (isalpha(c) || c == '_')
                    state = 2;
                else if (isdigit(c))
                    state = 3;
                else if (c == '/')
                    state = 20;
                else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ',' || ';' || '[' || ']'||'.')// -> t7t m3 ba2y el minuses
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
                else if (c == '^')
                    state = 28;
                else if (c=='~')
                    lexemeType=13;
                else if (c=='?')
                    state = 31;
                else if (c=='.')
                    lexemeType=10;
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
                if (c == '='|c=='>') {
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
                if (c == '&')
                    lexemeType = 12;
                else if(c=='=')
                    lexemeType = 8;
                else
                    retrackFlag = true;
                state =1;
                break;

            case 19:
                if (c == '|') {
                    lexemeType = 12;
                } else if(c==0)
                    lexemeType = 8;
                else
                    retrackFlag = true;
                state = 1;
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
                if (c != '"') {
                    state = 24;
                }
                else
                    lexemeType=11;
                state =1;
                break;


                /*case 25: // string
                    if (c == '\'') { //edit
                        lexemeType = 11;
                        state = 1;
                    }
                    break;  //nothing will cause an error*/

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
            case 28:
                if(c=='=')
                    lexemeType=8;
                else
                    retrackFlag = true;
                state = 1;
                break;
            case 29:
                if(c=='=')
                    lexemeType=8;
                else
                    retrackFlag=true;
                lexemeType=13;
                state=1;
                break;
            case 30:
                if(c=='=')
                    lexemeType=8;
                else
                    retrackFlag=true;
                lexemeType=13;
                state=1;
                break;
            case 31:
                if(c==':'){
                    lexemeType=7;
                    state =1;}
                else{
                    state = 27;
                }
                break;

        }
    }

	if(state != 1){
        cerr << "token incomplete: ";
        while(start!= forward)
            cout<<start++;
        cout << endl;
		return -1;
    }
	
}

