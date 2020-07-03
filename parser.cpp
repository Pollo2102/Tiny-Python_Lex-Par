#include "parser.h"

Ast::NodePtr Parser::parse()
{
    currToken = lexer.getNextToken();
        Ast::NodePtr node = expr();

		if (currToken != Token::Eof) {
			std::cout << "Syntax error: Invalid token: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		else {
			std::cout << "Input parsed successfully.\n";
			return node;
		}
}

Ast::NodePtr Parser::expr()
{
    while (currToken == Token::Eol) {
		currToken = lexer.getNextToken();
	}
	std::vector<Ast::NodePtr> code;
	code.push_back(exprP());

	while (currToken == Token::Eol) {
		currToken = lexer.getNextToken();
		if (currToken == Token::Eof) {
			break;
		}
		else if (currToken != Token::Eol) {
			code.push_back(exprP());
		}
	}

	if (currToken == Token::Eof) {
		return std::make_shared<Ast::Sequence>(code);
	}
	else {
		std::cout << "Syntax error.\n";
		throw 1;
	}
}

Ast::NodePtr Parser::exprP()
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
		return assign();
	}
	else if (currToken == Token::KwPrint) {
		return PrintFunc();
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
		return nullptr;
	}
	else if (currToken == Token::KwReturn) {
		Return_Stmt();
	}
	else {
		std::cout << "Invalid token at exprP: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

Ast::NodePtr Parser::assign()
{
	if (currToken == Token::Identifier) {
		std::string varName = lexer.getText();

		currToken = lexer.getNextToken();

		if (currToken != Token::OpAssign) {
			std::cout << "Invalid token at assign. Expected OpAssign but got: " << lexer.tokenToString(currToken) << " with value: " << lexer.getText() << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();

		Ast::NodePtr expression = term();

		return std::make_shared<Ast::Assign>(varName, expression);
	}
	else {
		std::cout << "Syntax error at assign.";
		throw 1;
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

Ast::NodePtr Parser::PrintFunc()
{
	if (currToken == Token::KwPrint) {
		currToken = lexer.getNextToken();
		return std::make_shared<Ast::Print>(PrintArgs());
	}
	else {
		std::cout << "Invalid token at PrintFunc. Expected KwPrint but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

Ast::NodePtr Parser::PrintArgs() 
{
	if (currToken == Token::Literal) {
		Ast::NodePtr lit = std::make_shared<Ast::Literal>(lexer.getText());
		currToken = lexer.getNextToken();
		return std::make_shared<Ast::PrintArgs>(lit, Optl_Print_Args());
	}
	else {
		std::cout << "Invalid token at PrintArgs. Expected Literal but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

Ast::NodePtr Parser::Optl_Print_Args()
{
	if (currToken == Token::Comma) {
		currToken = lexer.getNextToken();
		Ast::NodePtr valueI = term();
		return std::make_shared<Ast::PrintArgs>(valueI, Optl_Print_Args());
	}
	else {
		return std::make_shared<Ast::Literal>("\n");
	}
}

Ast::NodePtr Parser::Optl_Print_ArgsP()
{
	if (currToken == Token::Comma)
	{
		currToken = lexer.getNextToken();
		return PrintArgs();
	}
	else {
		return std::make_shared<Ast::Literal>("\n");
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

Ast::NodePtr Parser::term()
{
	Ast::NodePtr valueI = factor();
	return termP(valueI);
}

Ast::NodePtr Parser::termP(Ast::NodePtr valueI) // Redo for AST
{
	if (currToken == Token::OpAdd) {

		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Add>(valueI, termP(valueI2));
	}
	else if (currToken == Token::OpSub) {
		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Sub>(valueI, termP(valueI2));
	}
	else if (currToken == Token::OpMult) {
		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Mult>(valueI, termP(valueI2));
	}
	else if (currToken == Token::OpDiv) {
		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Div>(valueI, termP(valueI2));
	}
	else if (currToken == Token::OpPow) {
		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Pow>(valueI, termP(valueI2));
	}
	else if (currToken == Token::OpMod) {
		ArithOps();
		Ast::NodePtr valueI2 = factor();
		return std::make_shared<Ast::Pow>(valueI, termP(valueI2));
	}
	else if (currToken == Token::RelEQ || currToken == Token::RelNEQ || currToken == Token::RelGT || currToken == Token::RelGTE
		|| currToken == Token::RelLT || currToken == Token::RelLTE) {
		std::string RelOp = lexer.getText();
		Rel_Op();
		Ast::NodePtr ValueI2 = factor();
		return std::make_shared<Ast::Compare>(valueI, RelOp, termP(ValueI2));
	}
	else
		return valueI;
}

Ast::NodePtr Parser::factor()
{
	if (currToken == Token::Identifier) {
		std::string value = lexer.getText();
		currToken = lexer.getNextToken();
		if (currToken != Token::Open_Par && currToken != Token::Open_Br) {
			return std::make_shared<Ast::Ident>(value);
		}
		else {
			return factorP();
		}
	}
	else if (currToken == Token::KwInput) { // Create class
		currToken = lexer.getNextToken();

		return factorP();
	}
	else if (currToken == Token::Literal) {
		std::string value = lexer.getText();
		currToken = lexer.getNextToken();

		factorP();
		return std::make_shared<Ast::Literal>(value);
	}
	else if (currToken == Token::Dec) {
		int value = std::stoi(lexer.getText());
		currToken = lexer.getNextToken();
		return std::make_shared<Ast::Number>(value);
	}
	else {
		std::cout << "Invalid token at factor. Expected an Identifier or a Number but got: " << lexer.tokenToString(currToken) << "\n";
		throw 1;
	}
}

Ast::NodePtr Parser::factorP()
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
		return nullptr; // Finish
	}
	else if (currToken == Token::Open_Br) {
		currToken = lexer.getNextToken();
		Args_List();

		if (currToken != Token::Close_Br) {
			std::cout << "Invalid token at factorP. Expected Close_Br but got: " << lexer.tokenToString(currToken) << "\n";
			throw 1;
		}
		currToken = lexer.getNextToken();
		return nullptr; // Finish
	}
	else {
		std::cout << "Invalid token at factorP. Wrong Token.\n";
		throw 1;
	}
}