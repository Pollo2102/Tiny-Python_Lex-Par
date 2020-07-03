#ifndef _EXPR_PARSER_H
#define _EXPR_PARSER_H

#include "lexer.h"
#include "AST_classes.h"


class Parser {
public:
	Parser(Lexer &lexer): lexer (lexer)	{ }
	
	Ast::NodePtr parse();

private:
	Ast::NodePtr expr();
	Ast::NodePtr exprP();


	Ast::NodePtr assign();

	void func_call();

	Ast::NodePtr PrintFunc();
	Ast::NodePtr PrintArgs();
	Ast::NodePtr Optl_Print_Args();
	Ast::NodePtr Optl_Print_ArgsP();

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
	Ast::NodePtr term();
	Ast::NodePtr termP(Ast::NodePtr);
	Ast::NodePtr factor();
	Ast::NodePtr factorP();

private:
	Token currToken;
	Lexer &lexer;
};

#endif