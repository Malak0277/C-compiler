#include <iostream>
#include <string>
#include <fstream>
#include <deque>
using namespace std;
//kslna  nkaml tom isA

const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];
char token[100];
char* readFromFile(const string& program, char buffer[]) {



    ifstream inputFile(program, ios::binary);

    // Check if the opening was successful
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return nullptr;
    }

    inputFile.read(buffer, BUF_SIZE);

    // Check if the read was successful
    if (!inputFile) {
        cerr << "Error reading from file: " << program << endl;
        return nullptr;
    }

    inputFile.close();

    char* forward;
    forward = buffer;
    return forward;

}

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

}
