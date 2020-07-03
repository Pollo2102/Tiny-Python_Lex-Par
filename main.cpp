#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <istream>

int main(int argc, char const *argv[])
{
    if (argc != 2)
    {
        std::cout << "Correct usage: <executable> <input file>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cout << "Error opening the specified file: " << argv[1] << "\n";
        return 1;
    }


    Lexer lexer(file);

    Parser parser(lexer);

    Ast::NodePtr root = parser.parse();

    std::cout << "Parsing complete\n";

    Ast::SymbolTable symTable;

    // Ast::toString(root);

    Ast::eval(root, symTable);

    // Token tk = lexer.getNextToken();

    // while (tk != Token::Eof)
    // {
    //     std::cout << "Token: " << lexer.tokenToString(tk) << "  Lexema: " << lexer.getText() << "\n";
    //     tk = lexer.getNextToken();
    // }
    
    return 0;
}
