#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include "lexer.h"

class Parser {
public:
	Parser(Lexer &lexer): lexer (lexer)	{ }
	
	void parse();

private:
	void expr();
	void exprP();
	void term();
	void factor();
	void assign();
	void PrintFunc();
	void ForCall();
	void Cond_Stmt();
	void func_call();
	void Args_List();
	void PrintArgs();
	void Optl_Print_Args();

private:
	Token currToken;
	Lexer &lexer;
};

#endif