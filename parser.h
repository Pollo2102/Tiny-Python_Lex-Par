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


	void assign();

	void func_call();

	void PrintFunc();
	void PrintArgs();
	void Optl_Print_Args();
	void Optl_Print_ArgsP();

	void Args_List();
	void Args_ListP();

	void ForCall();
	void DefCall();
	void Return_Stmt();
	void Cond_Stmt();
	void Else_Stmt();

	void Func_Code();
	void Func_CodeP();

	void Array();
	void Cond_KeyWd();
	void Comparison();
	void DedentProd();
	void ArithOps();
	void Rel_Op();
	void term();
	void termP();
	void factor();
	void factorP();

private:
	Token currToken;
	Lexer &lexer;
};

#endif