#include "lexer.h"

enum class State {
    start_q0,
    number_q0,
    number_q1,
    number_q2,
    number_q3,
    number_q4,
    number_q5,
    Arith_Op_q0,
    Arith_Op_q3,
    Ident_q0,
    Ident_q1,
    Ident_q2,
    Literal_q0,
    Literal_q1,
    Rel_Oper_q0,
    Rel_Oper_q1,
    Rel_Oper_q2,
    Rel_Oper_q3,
    Rel_Oper_q4,
    Comment_q0,
    Comment_q1,
    function_elems_q0,
    array_elems_q0,
};

Token Lexer::findKeyword(const std::string& str)
{
    if (str == "if")
        return Token::KwIf;
    else if (str == "def")
        return Token::KwDef;
    else if (str == "elif")
        return Token::KwElif;
    else if (str == "else")
        return Token::KwElse;
    else if (str == "for")
        return Token::KwFor;
    else if (str == "print")
        return Token::KwPrint;
    else if (str == "input")
        return Token::KwInput;
    else if (str == "in")
        return Token::KwIn;
    else if (str == "while")
        return Token::KwWhile;
    else 
        return Token::Ident;
}

Token Lexer::getNextToken()
{
    text = "";
        State state = State::start_q0;
    int ch = getNextChar();

    while (true) {
        switch (state) {
            // start
            case State::start_q0:
                if ((ch == '\n') || (ch == ' ') || (ch == '\t')) {
                    state = State::start_q0;
                    ch = getNextChar();
                }
                else if (ch == EOF) {
                    text += ch;
                    return Token::Eof;
                }
                else {
                    // Trying next automaton 'number
                    state = State::number_q0;
                }
                break;
            // number
            case State::number_q0:
                if (ch == '0') {
                    text += ch;
                    state = State::number_q1;
                    ch = getNextChar();
                }
                else if ((ch >= '1') && (ch <= '9')) {
                    text += ch;
                    state = State::number_q4;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'Arith_Op
                    state = State::Arith_Op_q0;
                }
                break;
            case State::number_q1:
                if ((ch == 'x') || (ch == 'X')) {
                    text += ch;
                    state = State::number_q5;
                    ch = getNextChar();
                }
                else if ((ch == 'b') || (ch == 'B')) {
                    text += ch;
                    state = State::number_q2;
                    ch = getNextChar();
                }
                else if (ch == 'o') {
                    text += ch;
                    state = State::number_q3;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'Arith_Op
                    state = State::Arith_Op_q0;
                }
                break;
            case State::number_q2:
                if ((ch == '0') || (ch == '1')) {
                    text += ch;
                    state = State::number_q2;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Bin;
                }
                break;
            case State::number_q3:
                if ((ch >= '0') && (ch <= '7')) {
                    text += ch;
                    state = State::number_q3;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Oct;
                }
                break;
            case State::number_q4:
                if ((ch >= '0') && (ch <= '9')) {
                    text += ch;
                    state = State::number_q4;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Dec;
                }
                break;
            case State::number_q5:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'f')) || ((ch >= 'A') && (ch <= 'F'))) {
                    text += ch;
                    state = State::number_q5;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return Token::Hex;
                }
                break;
            // Arith_Op
            case State::Arith_Op_q0:
                if (ch == '+') {
                    text += ch;
                    return Token::OpAdd;
                }
                else if (ch == '-') {
                    text += ch;
                    return Token::OpSub;
                }
                else if (ch == '*') {
                    text += ch;
                    state = State::Arith_Op_q3;
                    ch = getNextChar();
                }
                else if (ch == '/') {
                    text += ch;
                    return Token::OpDiv;
                }
                else if (ch == '%') {
                    text += ch;
                    return Token::OpMod;
                }
                else {
                    // Trying next automaton 'Ident
                    state = State::Ident_q0;
                }
                break;
            case State::Arith_Op_q3:
                if (ch == '*') {
                    text += ch;
                    return Token::OpPow;
                }
                else {
                    ungetChar(ch);
                    return Token::OpMult;
                }
                break;
            // Ident
            case State::Ident_q0:
                if ((ch == '_') || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= 'a') && (ch <= 'z'))) {
                    text += ch;
                    state = State::Ident_q1;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'Literal
                    state = State::Literal_q0;
                }
                break;
            case State::Ident_q1:
                if (((ch >= '0') && (ch <= '9')) || ((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || (ch == '_')) {
                    text += ch;
                    state = State::Ident_q1;
                    ch = getNextChar();
                }
                else {
                    ungetChar(ch);
                    return findKeyword(text);
                }
                break;
            // Literal
            case State::Literal_q0:
                if (ch == '"') {
                    text += ch;
                    state = State::Literal_q1;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'Rel_Oper
                    state = State::Rel_Oper_q0;
                }
                break;
            case State::Literal_q1:
                if (ch == '"') {
                    text += ch;
                    return Token::Literal;
                }
                else {
                    text += ch;
                    state = State::Literal_q1;
                    ch = getNextChar();
                }
                break;
            // Rel_Oper
            case State::Rel_Oper_q0:
                if (ch == '!') {
                    text += ch;
                    state = State::Rel_Oper_q3;
                    ch = getNextChar();
                }
                else if (ch == '=') {
                    text += ch;
                    state = State::Rel_Oper_q4;
                    ch = getNextChar();
                }
                else if (ch == '>') {
                    text += ch;
                    state = State::Rel_Oper_q1;
                    ch = getNextChar();
                }
                else if (ch == '<') {
                    text += ch;
                    state = State::Rel_Oper_q2;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'Comment
                    state = State::Comment_q0;
                }
                break;
            case State::Rel_Oper_q1:
                if (ch == '=') {
                    text += ch;
                    return Token::RelGTE;
                }
                else {
                    ungetChar(ch);
                    return Token::RelGT;
                }
                break;
            case State::Rel_Oper_q2:
                if (ch == '=') {
                    text += ch;
                    return Token::RelLTE;
                }
                else {
                    ungetChar(ch);
                    return Token::RelLT;
                }
                break;
            case State::Rel_Oper_q3:
                if (ch == '=') {
                    text += ch;
                    return Token::NEQ;
                }
                else {
                    // Trying next automaton 'Comment
                    state = State::Comment_q0;
                }
                break;
            case State::Rel_Oper_q4:
                if (ch == '=') {
                    text += ch;
                    return Token::RelEQ;
                }
                else {
                    ungetChar(ch);
                    return Token::OpAssign;
                }
                break;
            // Comment
            case State::Comment_q0:
                if (ch == '#') {
                    state = State::Comment_q1;
                    ch = getNextChar();
                }
                else {
                    // Trying next automaton 'function_elems
                    state = State::function_elems_q0;
                }
                break;
            case State::Comment_q1:
                if ((ch == '\n') || (ch == EOF)) {
                    state = State::start_q0;
                    ch = getNextChar();
                }
                else {
                    state = State::Comment_q1;
                    ch = getNextChar();
                }
                break;
            // function_elems
            case State::function_elems_q0:
                if (ch == '(') {
                    text += ch;
                    return Token::Open_Par;
                }
                else if (ch == ')') {
                    text += ch;
                    return Token::Close_Par;
                }
                else if (ch == ':') {
                    text += ch;
                    return Token::Colon;
                }
                else if (ch == ',') {
                    text += ch;
                    return Token::Comma;
                }
                else {
                    // Trying next automaton 'array_elems
                    state = State::array_elems_q0;
                }
                break;
            // array_elems
            case State::array_elems_q0:
                if (ch == ']') {
                    text += ch;
                    return Token::Close_Br;
                }
                else if (ch == '[') {
                    text += ch;
                    return Token::Open_Br;
                }
                else {
                    reportError(ch);
                    ch = getNextChar();
                    state = State::start_q0;
                }
                break;
        }
    }

}

const char *Lexer::tokenToString(Token tk)
{
        switch (tk) {
        case Token::Colon: return "Colon";
        case Token::Open_Par: return "Open_Par";
        case Token::OpAssign: return "OpAssign";
        case Token::NEQ: return "NEQ";
        case Token::RelLTE: return "RelLTE";
        case Token::RelGT: return "RelGT";
        case Token::Close_Br: return "Close_Br";
        case Token::RelLT: return "RelLT";
        case Token::RelGTE: return "RelGTE";
        case Token::Close_Par: return "Close_Par";
        case Token::OpMult: return "OpMult";
        case Token::OpDiv: return "OpDiv";
        case Token::OpSub: return "OpSub";
        case Token::OpMod: return "OpMod";
        case Token::KwElse: return "Keyword else";
        case Token::RelEQ: return "RelEQ";
        case Token::Eof: return "Eof";
        case Token::KwIn: return "Keyword in";
        case Token::KwPrint: return "Keyword print";
        case Token::KwDef: return "Keyword def";
        case Token::Bin: return "Bin";
        case Token::KwWhile: return "Keyword while";
        case Token::KwElif: return "Keyword elif";
        case Token::Ident: return "Identifier";
        case Token::OpPow: return "OpPow";
        case Token::KwIf: return "Keyword if";
        case Token::Literal: return "Literal";
        case Token::Oct: return "Oct";
        case Token::KwFor: return "KeyWord for";
        case Token::Dec: return "Dec";
        case Token::Hex: return "Hex";
        case Token::Open_Br: return "Open_Br";
        case Token::Comma: return "Comma";
        case Token::KwInput: return "Keyword input";
        case Token::OpAdd: return "OpAdd";
        default: return "Unknown";
    }

}