#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int BUF_SIZE = 4096;
char buffer1[BUF_SIZE], buffer2[BUF_SIZE];

char* readFromFile(const string& fileName, char buffer[]) {

    ifstream inputFile(fileName, ios::binary);

    // Check if the opening was successful
    if (!inputFile.is_open()) {
        cerr << "Error opening file" << endl;
        return nullptr;
    }

    inputFile.read(buffer, BUF_SIZE);

    // Check if the read was successful
    if (!inputFile) {
        cerr << "Error reading from file: " << fileName << endl;
        return nullptr;
    }

    inputFile.close();

    char* ptr;
    ptr = buffer;
    return ptr;
}