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
