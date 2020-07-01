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
    Lexer(std::istream& input): input(input) { 
            indentation_stack.push(0); 
            checking_indentation = false;
        }

    std::string getText() { return text; }
    int getNextChar() { return checking_indentation ? '@' : input.get(); }
    void ungetChar(int ch) { input.unget(); }

    void reportError(int ch) {
        std::cerr << "Error: Invalid character: '" << ch << "'\n";
    }

    Token getNextToken();
    Token findKeyword(const std::string& str);
    Token indentation_check();
    Token contains_dedentation();
    Token skip_eol();
    static const char *tokenToString(Token tk);

private:
    std::istream& input;
    std::string text;
    std::stack<int> tr_wh;
    std::stack<int> indentation_stack;
    int current_indentation;
    bool checking_indentation;
};

#endif