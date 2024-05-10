#include <iostream>
#include <string>
#include <vector>
using namespace std;

    void square (int a){
        cout << "ll";
    }


#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector<std::string> splitOnWhitespace(const std::string& str) {
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string token;

    // Read each token separated by whitespace and store it in the result vector
    while (iss >> token) {
        result.push_back(token);
    }

    return result;
}

int main() {
    std::string input = "This is a test string";
    std::vector<std::string> words = splitOnWhitespace(input);

    // Print the words
    for (const auto& word : words) {
        std::cout << word << std::endl;
    }
    cout << words.size();

    return 0;
}


// FUNCTION CALL
//inline, anonymous, function inside function
//memory -> variables(initialized aw la? plus no3ha eh?); call (kanet void aw la)


/*
    int x = 1;
    x = 3;
    y = 1;

    */