#include "parser.h"

void Parser::parse()
{
    currToken = lexer.getNextToken();
        expr();

		if (currToken != Token::Eof) {
			std::cout << "Syntax error: Invalid token: " << lexer.tokenToString(currToken) << "\n";
		}
		else {
			std::cout << "Input parsed successfully.\n";
		}
}

void Parser::expr()
{
    while (currToken == Token::Eol) {
		currToken = lexer.getNextToken();
	}
	exprP();

	while (currToken == Token::Eol) {
		currToken = lexer.getNextToken();
		if (currToken == Token::Eof) {
			break;
		}
		else if (currToken != Token::Eol) {
			exprP();
		}
	}

	if (currToken == Token::Eof) {
		return;
	}
}

void Parser::exprP()
{
	if (currToken == Token::Identifier) {
		assign();
	}
	else if (currToken == Token::KwPrint) {
		PrintFunc();
	}
	else if (currToken == Token::KwFor) {
		ForCall();
	}
	else if (currToken == Token::KwIf || currToken == Token::KwElif || currToken == Token::KwWhile)
	{
		Cond_Stmt();
	}
	else {
		std::cout << "Invalid token at exprP: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::assign()
{
	if (currToken == Token::Identifier) {
		currToken = lexer.getNextToken();

		if (currToken != Token::OpAssign) {
			std::cout << "Invalid token at assign. Expected OpAssign but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		factor();
	}
}

void Parser::func_call()
{
	if (currToken == Token::Identifier) {
		currToken = lexer.getNextToken();
		if (currToken != Token::Open_Par) {
			std::cout << "Invalid token at func_call. Expected OpenPar but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();
		if (currToken == Token::Identifier || currToken == Token::Dec) {
			Args_List();	
		}
		if (currToken == Token::Close_Par) {
			currToken = lexer.getNextToken();
			return;
		}
		else {
			std::cout << "Invalid token at func_call. Expected Close_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
	}
	else {
		std::cout << "Invalid token at func_call. Expected Identifier but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::PrintFunc()
{
	if (currToken == Token::KwPrint) {
		currToken = lexer.getNextToken();
		PrintArgs();
	}
	else {
		std::cout << "Invalid token at PrintFunc. Expected KwPrint but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::PrintArgs() 
{
	if (currToken == Token::Literal) {
		currToken = lexer.getNextToken();
		Optl_Print_Args();
	}
	else {
		std::cout << "Invalid token at PrintArgs. Expected Literal but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}