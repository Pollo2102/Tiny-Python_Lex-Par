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
	if (currToken == Token::Dedent) {
		currToken = lexer.getNextToken();
		while (currToken == Token::Dedent)
			currToken = lexer.getNextToken();
		while (currToken == Token::Eol) {
			currToken = lexer.getNextToken();
		}
	}
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
	else if (currToken == Token::KwElse) {
		Else_Stmt();
	}
	else if (currToken == Token::KwDef) {
		DefCall();
	}
	else if (currToken == Token::Eof){
		return;
	}
	else if (currToken == Token::KwReturn) {
		Return_Stmt();
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
			std::cout << "Invalid token at assign. Expected OpAssign but got: " << lexer.tokenToString(currToken) << " with value: " << lexer.getText() << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		term();
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

void Parser::Optl_Print_Args()
{
	if (currToken == Token::Comma) {
		currToken = lexer.getNextToken();

		term();
		Optl_Print_ArgsP();
	}
}

void Parser::Optl_Print_ArgsP()
{
	if (currToken == Token::Comma)
	{
		currToken = lexer.getNextToken();
		PrintArgs();
	}
}

void Parser::Args_List()
{
	term();
	Args_ListP();
}

void Parser::Args_ListP()
{
	if (currToken == Token::Comma) {
		currToken = lexer.getNextToken();
		term();
		Args_ListP();
	}
}

void Parser::ForCall()
{
	if (currToken == Token::KwFor) {
		currToken = lexer.getNextToken();

		if (currToken != Token::Identifier) {
			std::cout << "Invalid token at ForCall. Expected Identifier but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::KwIn) {
			std::cout << "Invalid token at ForCall. Expected KwIn but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::Identifier) {
			std::cout << "Invalid token at ForCall. Expected Identifier but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::Open_Par) {
			std::cout << "Invalid token at ForCall. Expected Open_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		term();

		if (currToken != Token::Comma) {
			std::cout << "Invalid token at ForCall. Expected Comma but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		term();

		if (currToken != Token::Close_Par) {
			std::cout << "Invalid token at ForCall. Expected Close_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::Colon) {
			std::cout << "Invalid token at ForCall. Expected Colon but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		Func_Code();
	}
	else {
		std::cout << "Invalid token at ForCall. Expected KwFor but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::DefCall()
{
	if (currToken == Token::KwDef) {
		currToken = lexer.getNextToken();

		if (currToken != Token::Identifier) {
			std::cout << "Invalid token at DefCall. Expected Identifier but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::Open_Par) {
			std::cout << "Invalid token at DefCall. Expected Open_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken == Token::Identifier || currToken == Token::Dec) {
			Args_List();
		}

		if (currToken != Token::Close_Par) {
			std::cout << "Invalid token at DefCall. Expected Close_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		if (currToken != Token::Colon) {
			std::cout << "Invalid token at DefCall. Expected Colon but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		Func_Code();

	}
	else {
		std::cout << "Invalid token at DefCall. Expected KwDef but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Return_Stmt()
{
	if (currToken == Token::KwReturn) {
		currToken = lexer.getNextToken();

		term();
	}
	else {
		std::cout << "Invalid token at Return_Stmt. Expected KwReturn but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Cond_Stmt()
{
	Cond_KeyWd();
	term();

	if (currToken != Token::Colon) {
		std::cout << "Invalid token at Cond_Stmt. Expected Colon but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
	currToken = lexer.getNextToken();

	Func_Code();
}

void Parser::Else_Stmt()
{
	if (currToken == Token::KwElse) {
		currToken = lexer.getNextToken();

		if (currToken != Token::Colon) {
			std::cout << "Invalid token at Else_Stmt. Expected Colon but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}

		currToken = lexer.getNextToken();

		Func_Code();
	}
	else {
		std::cout << "Invalid token at Else_Stmt. Expected KwElse but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Func_Code()
{
	if (currToken == Token::Eol) {
		currToken = lexer.getNextToken();

		if (currToken != Token::Indent) {
			std::cout << "Current token1: " << lexer.tokenToString(currToken) << " with value: " << lexer.getText() << "\n";
			std::cout << "Invalid token at Func_Code. Expected Indent but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		exprP();
		Func_CodeP();

		DedentProd();
		currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at Func_Code. Expected Eol but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Func_CodeP()
{
	if (currToken == Token::Eol) {
		currToken = lexer.getNextToken();
		if (currToken == Token::Dedent) {
			currToken = lexer.getNextToken();
		}
		while (currToken == Token::Eol)
			currToken = lexer.getNextToken();
		exprP();
		Func_CodeP();
	}
}

void Parser::Array()
{
	if (currToken == Token::Identifier) {
		currToken = lexer.getNextToken();

		if (currToken != Token::Open_Br) {
			std::cout << "Invalid token at Array. Expected Open_Br but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		Args_List();

		if (currToken != Token::Close_Br) {
			std::cout << "Invalid token at Array. Expected Close_Br but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

	}
	else {
		std::cout << "Invalid token at Array. Expected Identifier but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Cond_KeyWd()
{
	if (currToken == Token::KwIf || currToken == Token::KwElif || currToken == Token::KwWhile) {
		currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at Cond_KeyWd. Expected KwIf or KwElif or KwWhile but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Comparison()
{
	term();
	Rel_Op();
	term();
}

void Parser::DedentProd()
{
	if (currToken == Token::Dedent || currToken == Token::Eof) {
		currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at DedentProd. Expected Dedent or Eof but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::ArithOps()
{
	if (currToken == Token::OpAdd || currToken == Token::OpSub || currToken == Token::OpMult || currToken == Token::OpDiv
		|| currToken == Token::OpPow || currToken == Token::OpMod) {
			currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at ArithOps. Expected an Arithmetic Operator but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::Rel_Op()
{
	if (currToken == Token::RelEQ || currToken == Token::RelNEQ || currToken == Token::RelGT || currToken == Token::RelGTE
		|| currToken == Token::RelLT || currToken == Token::RelLTE) {
			currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at Rel_Op. Expected a Relational Operator but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::term()
{
	factor();
	termP();
}

void Parser::termP()
{
	if (currToken == Token::OpAdd || currToken == Token::OpSub || currToken == Token::OpMult || currToken == Token::OpDiv
		|| currToken == Token::OpPow || currToken == Token::OpMod) {

		ArithOps();
		factor();
		termP();
	}
	else if (currToken == Token::RelEQ || currToken == Token::RelNEQ || currToken == Token::RelGT || currToken == Token::RelGTE
		|| currToken == Token::RelLT || currToken == Token::RelLTE) {
		Rel_Op();
		factor();
		termP();
	}
}

void Parser::factor()
{
	if (currToken == Token::Identifier || currToken == Token::KwInput || currToken == Token::Literal || currToken == Token::Open_Br) {
		currToken = lexer.getNextToken();

		factorP();
	}
	else if (currToken == Token::Dec) {
		currToken = lexer.getNextToken();
	}
	else {
		std::cout << "Invalid token at factor. Expected an Identifier or a Number but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

void Parser::factorP()
{
	if (currToken == Token::Open_Par) {
		currToken = lexer.getNextToken();
		if (currToken == Token::Identifier || currToken == Token::Dec || currToken == Token::Literal) {
			Args_List();
		}
		if (currToken != Token::Close_Par) {
			std::cout << "Invalid token at factorP. Expected Close_Par but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();
	}
	else if (currToken == Token::Open_Br) {
		currToken = lexer.getNextToken();
		Args_List();

		if (currToken != Token::Close_Br) {
			std::cout << "Invalid token at factorP. Expected Close_Br but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();
	}
}