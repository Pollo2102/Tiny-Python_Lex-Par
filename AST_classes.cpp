#include "AST_classes.h"
#include <string>

namespace Ast
{

    std::string toString(const NodePtr& node)
    {
        std::string str = "";
        switch (node->getKind())
        {
            case Kind::Sequence:
            {
                str = "";
                for(const auto& s: reinterpret_cast<Sequence *>(node.get())->getStatements())
                {
                    str += toString(s) + "\n";
                }
                return str;
            }
            
            case Kind::Assign:
            {
                Assign * asgn = reinterpret_cast<Assign *>(node.get());
                return  asgn->getVar() + " = " + toString(asgn->getExpr());
                break;
            }

            case Kind::Print:
            {
                Print * prnt = reinterpret_cast<Print *>(node.get());
                return "print " + toString(prnt->getExpr());
                break;
            }

            case Kind::Add:
            {
                Add * tmp = reinterpret_cast<Add *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") + " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Sub:
            {
                Add * tmp = reinterpret_cast<Add *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") - " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Mult:
            {
                Mult * tmp = reinterpret_cast<Mult *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") * " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Div:
            {
                Mult * tmp = reinterpret_cast<Mult *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") / " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Pow:
            {
                Mult * tmp = reinterpret_cast<Mult *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") ** " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Mod:
            {
                Mult * tmp = reinterpret_cast<Mult *>(node.get());
                NodePtr value1 = tmp->getExpr1();
                NodePtr value2 = tmp->getExpr2();
                return "(" + toString(value1) + ") % " +
                    "(" + toString(value2) + ")";
            }

            case Kind::Number:
            {
                return std::to_string(reinterpret_cast<Number *>(node.get())->getValue());
            }

            case Kind::Ident:
                return reinterpret_cast<Ident *>(node.get())->getText();

            case Kind::IfStmt:
            {
                NodePtr cond  = reinterpret_cast<IfStmt *>(node.get())->getCondition();
                NodePtr t_block  = reinterpret_cast<IfStmt *>(node.get())->getTrueBlock();
                NodePtr f_block  = reinterpret_cast<IfStmt *>(node.get())->getFalseBlock();
                return "if " + toString(cond) + "{ \n" + toString(t_block) + "\n}\n" + 
                        "else { \n" + toString(f_block) + "\n}\n";
            }

            case Kind::WhileStmt:
            {
                NodePtr cond  = reinterpret_cast<WhileStmt *>(node.get())->getCond();
                NodePtr l_block  = reinterpret_cast<WhileStmt *>(node.get())->getLoopBlock();
                return "while " + toString(cond) + " {\n" + toString(l_block) + "\n}\n";
            }

            case Kind::Compare:
            {
                NodePtr expr1 = reinterpret_cast<Compare *>(node.get())->getExpr1();
                NodePtr expr2 = reinterpret_cast<Compare *>(node.get())->getExpr2();
                std::string RelOp = reinterpret_cast<Compare *>(node.get())->getRelOp();
                if (RelOp == "==") {
                    return toString(expr1) + " == " + toString(expr2);
                }
                else if (RelOp == "!=") {
                    return toString(expr1) + " != " + toString(expr2);
                }
                else if (RelOp == ">") {
                    return toString(expr1) + " > " + toString(expr2);
                }
                else if (RelOp == ">=") {
                    return toString(expr1) + " >= " + toString(expr2);
                }
                else if (RelOp == "<") {
                    return toString(expr1) + " < " + toString(expr2);
                }
                else if (RelOp == "<=") {
                    return toString(expr1) + " >= " + toString(expr2);
                }
                else {
                    std::cout << "Invalid operator.\n";
                    throw 1;
                }
            }

            case Kind::Return:
            {
                NodePtr var = reinterpret_cast<Return *>(node.get())->getVar();
                return "return " + toString(var);
            }

            case Kind::FuncDecl:
            {
                std::string arguments;
                
                for (const auto& a : reinterpret_cast<FuncDecl *>(node.get())->getArgs())
                {
                    arguments += (toString(a) + ",");
                }
                if (!arguments.empty())
                {
                    arguments.pop_back();
                }
                NodePtr sequence = reinterpret_cast<FuncDecl *>(node.get())->getSequence();
                return reinterpret_cast<FuncDecl *>(node.get())->getFunctionName() 
                        + "(" + arguments + "){\n" + toString(sequence) + "}";
            }

            case Kind::FuncCall:
            {
                std::string arguments;
                for (const auto& a : reinterpret_cast<FuncDecl *>(node.get())->getArgs())
                {
                    arguments += (toString(a) + ",");
                }
                if (!arguments.empty())
                {
                    arguments.pop_back();
                }
                return reinterpret_cast<FuncDecl *>(node.get())->getFunctionName()
                        + "(" + arguments + ")";
            }

            

            default:
                std::cout << "Invalid Kind. \n";
                throw 1;
        }
    }

    int eval (NodePtr node, Ast::SymbolTable& vars)
    {
        switch (node->getKind())
        {
        case Kind::Sequence:
        {
            for(NodePtr s: reinterpret_cast<Sequence *>(node.get())->getStatements())
            {
                eval(s, vars);
            }
            return 0;
        }
        
        case Kind::Assign:
        {
            Assign * asgn= reinterpret_cast<Assign *>(node.get());
            std::string varName = asgn->getVar();
            auto var = vars.find(varName);
            if (var == vars.end())
            {
                vars.emplace(varName, std::make_shared<Number>(eval(asgn->getExpr(), vars)));
            }
            else
            {
                var->second = std::make_shared<Number>(eval(asgn->getExpr(), vars));
            }
            return 0;
        }

        case Kind::Print:
        {
            NodePtr prnt = reinterpret_cast<Print *>(node.get())->getExpr();
            eval(prnt, vars);
            return 0;
        }

        case Kind::PrintArgs:
        {
            NodePtr expr1 = reinterpret_cast<PrintArgs *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<PrintArgs *>(node.get())->getExpr2();
            if (expr1->getKind() == Kind::Literal) {
                std::string val = reinterpret_cast<Literal *>(expr1.get())->getLiteralValue();
                if (val.size() > 1)
                {
                    val.pop_back();
                    val = val.substr(1);
                }
                std::cout << val;
            }
            else
                std::cout << eval(expr1, vars);
            if (expr2->getKind() == Kind::Literal) {
                std::string val = reinterpret_cast<Literal *>(expr2.get())->getLiteralValue();
                if (val.size() > 1)
                {
                    val.pop_back();
                    val = val.substr(1);
                }
                std::cout << val;
            }
            else
                std::cout << eval(expr2, vars);
            return 0;
        }

        case Kind::Add:
        {
            NodePtr expr1 = reinterpret_cast<Add *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Add *>(node.get())->getExpr2();
            return eval(expr1, vars) + eval(expr2 ,vars);
        }

        case Kind::Sub:
        {
            NodePtr expr1 = reinterpret_cast<Sub *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Sub *>(node.get())->getExpr2();
            return eval(expr1, vars) - eval(expr2 ,vars);
        }

        case Kind::Mult:
        {
            NodePtr expr1 = reinterpret_cast<Mult *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Mult *>(node.get())->getExpr2();
            return eval(expr1, vars) * eval(expr2 ,vars);
        }

        case Kind::Div:
        {
            NodePtr expr1 = reinterpret_cast<Mult *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Mult *>(node.get())->getExpr2();
            return eval(expr1, vars) / eval(expr2 ,vars);
        }

        case Kind::Pow:
        {
            NodePtr expr1 = reinterpret_cast<Mult *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Mult *>(node.get())->getExpr2();
            return pow(eval(expr1, vars), eval(expr2 ,vars));
        }

        case Kind::Mod:
        {
            NodePtr expr1 = reinterpret_cast<Mult *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Mult *>(node.get())->getExpr2();
            return eval(expr1, vars) % eval(expr2 ,vars);
        }

        case Kind::Input:
        {
            Input * input = reinterpret_cast<Input *>(node.get());
            std::string outMsg = input->getOutString();
            std::cout << outMsg;
            std::string value;
            std::cin >> value;
            return std::stoi(value);
        }

        case Kind::Number:
        {
            Number * num = reinterpret_cast<Number *>(node.get());
            return num->getValue();
        }

        case Kind::Ident:
        {
            Ident *id = reinterpret_cast<Ident *>(node.get());
            auto var = vars.find(id->getText());
            if (var == vars.end())
            {
                std::cout << "Variable not found on stack.\n";
                throw 1;
            }
            else
            {
                return reinterpret_cast<Number *>(var->second.get())->getValue();
            }
        }

        case Kind::IfStmt:
        {
            NodePtr cond = reinterpret_cast<IfStmt *>(node.get())->getCondition();
            NodePtr t_block = reinterpret_cast<IfStmt *>(node.get())->getTrueBlock();
            NodePtr f_block = reinterpret_cast<IfStmt *>(node.get())->getFalseBlock();
            if (eval(cond, vars))
                eval(t_block, vars);
            else
                eval(f_block, vars);
            return 0;
        }

        case Kind::WhileStmt:
        {
            NodePtr cond = reinterpret_cast<WhileStmt *>(node.get())->getCond();
            NodePtr l_block = reinterpret_cast<WhileStmt *>(node.get())->getLoopBlock();
            while (eval(cond, vars))
                eval(l_block, vars);
            return 0;
        }

        case Kind::Compare:
        {
            NodePtr expr1 = reinterpret_cast<Compare *>(node.get())->getExpr1();
            NodePtr expr2 = reinterpret_cast<Compare *>(node.get())->getExpr2();
            std::string RelOp = reinterpret_cast<Compare *>(node.get())->getRelOp();
            if (RelOp == "==") {
                return eval(expr1, vars) == eval(expr2, vars);
            }
            else if (RelOp == "!=") {
                return eval(expr1, vars) != eval(expr2, vars);
            }
            else if (RelOp == ">") {
                return eval(expr1, vars) > eval(expr2, vars);
            }
            else if (RelOp == ">=") {
                return eval(expr1, vars) >= eval(expr2, vars);
            }
            else if (RelOp == "<") {
                return eval(expr1, vars) < eval(expr2, vars);
            }
            else if (RelOp == "<=") {
                return eval(expr1, vars) <= eval(expr2, vars);
            }
            else {
                std::cout << "Invalid operator.\n";
                throw 1;
            }

        }

        case Kind::Return:
        {

        }

        case Kind::FuncCall:
        {

        }

        case Kind::FuncDecl:
        {
            // std::string funcName = reinterpret_cast<FuncDecl *>(node.get())->getFunctionName();
            // auto var = funcs.find(funcName);
            // if (var == funcs.end())
            // {
            //     funcs.emplace(funcName, node.get());
            //     std::cout << "function\n";
            // }
            // else
            // {
            //     throw 1;
            // }
        }

        default:
            std::cout << "Invalid Kind. \n";
            throw 1;
        }
    }
}
