#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <fstream>
#include <sstream>
#include <regex>

#include "lexer.h"
#include "parser.h"

using namespace std;

int main(int argc, char** argv) {
    if(argc < 2) {
        cerr << "Usage: ./transpile <input_file.py>";
        return 1;
    }

    ifstream file;
    file.open(argv[1]);

    if(!file.is_open()) {
        cerr << "Error: Could not open file " << argv[1] << endl;
        return 1;
    }

    ostringstream ss;
    ss << file.rdbuf();
    string source = ss.str();

    file.close();

    // lexer
    std::vector<Lexer::Token> tokens = Lexer::tokenize(&source);

    // parser
    

    // transpile

    return 0;
}