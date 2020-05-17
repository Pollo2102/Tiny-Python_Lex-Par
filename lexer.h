#ifndef __LEXER_H__
#define __LEXER_H__

#include <iostream>
#include <string>
#include <fstream>
#include <stack>
#include "build/tokens.h"

class Lexer
{
public:
    Lexer(std::istream& input): input(input) {}

    std::string getText() { return text; }
    int getNextChar() { return input.get(); }
    void ungetChar(int ch) { input.unget(); }

    void reportError(int ch) {
        std::cerr << "Error: Invalid character: '" << ch << "'\n";
    }

    Token getNextToken();
    Token findKeyword(const std::string& str);
    static const char *tokenToString(Token tk);

private:
    std::istream& input;
    std::string text;
    std::stack<int> tr_wh;
};

#endif