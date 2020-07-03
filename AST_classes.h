#ifndef _AST_CLASSES
#define _AST_CLASSES
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

namespace Ast {

    class Node;
    using NodePtr = std::shared_ptr<Node>;
    using SymbolTable = std::unordered_map<std::string, int>;

    //RTTI
    enum class Kind {
        Sequence,
        Assign,
        Print,
        PrintArgs,
        Add,
        Sub,
        Mult,
        Div,
        Pow,
        Mod,
        Number,
        Ident,
        IfStmt,
        WhileStmt,
        Compare,
        FuncDecl,
        Return,
        FuncCall,
        Literal,
    };
    
    class Node {
    public:
        virtual ~Node() {}
        virtual Kind getKind() const = 0;
        virtual int getValue() const = 0;
    };

    std::string toString(const NodePtr& node);
    int eval (NodePtr node, SymbolTable& vars);

    class Add: public Node {
    public:
        Add(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Add; }
        int getValue() const override
        {

            int a = first->getValue();
            int b = second->getValue();
            return a + b;
        }
        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };

    class Sub: public Node {
    public:
        Sub(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Sub; }
        int getValue() const override
        {

            int a = first->getValue();
            int b = second->getValue();
            return a - b;
        }
        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };

    class Mult: public Node {
    public:
        Mult(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Mult; }
        int getValue() const override
        {
            int a = first->getValue();
            int b = second->getValue();
            return a * b;
        }

        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };

    class Div: public Node {
    public:
        Div(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Div; }
        int getValue() const override
        {
            int a = first->getValue();
            int b = second->getValue();
            return a / b;
        }

        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };

    class Pow: public Node {
    public:
        Pow(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Pow; }
        int getValue() const override
        {
            int a = first->getValue();
            int b = second->getValue();
            return pow(a, b);
        }

        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };
    
    class Mod: public Node {
    public:
        Mod(NodePtr f, NodePtr s): first(f), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Mod; }
        int getValue() const override
        {
            int a = first->getValue();
            int b = second->getValue();
            return a % b;
        }

        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

    private:
        NodePtr first;
        NodePtr second;
    };

    class Compare: public Node {
    public:
        Compare(NodePtr f, std::string ROp, NodePtr s): 
        first(f), RelOp(ROp), second(s) 
        {
        }

        Kind getKind() const override { return Kind::Compare; }
        int getValue() const override
        {
            int a = first->getValue();
            int b = second->getValue();
            return a == b;
        }

        const NodePtr& getExpr1() const
        {
            return first;
        }

        const NodePtr& getExpr2() const
        {
            return second;
        }

        const std::string& getRelOp() const
        {
            return RelOp;
        }

    private:
        NodePtr first;
        std::string RelOp;
        NodePtr second;
    };

    class Number: public Node {
    public:
        Number(int number): num(number) 
        {
        }

        Kind getKind() const override { return Kind::Number; }
        int getValue() const override { return num; }

    private:
        int num;
    };

    class Sequence: public Node {
    public:
        Sequence(std::vector<NodePtr>& stmts) 
        : statements(std::move(stmts))
        {
        }

        Kind getKind() const override 
        { 
            return Kind::Sequence;
        }

        int getValue() const override {
            std::cout << "Invalid call. (Sequence)\n";
            throw 1;
            return 0;
        }

        const std::vector<NodePtr>& getStatements() const
        {
            return statements;
        }

    private:
        std::vector<NodePtr> statements;
    };

    class Assign: public Node {
    public:
        Assign(const std::string& v, NodePtr rhs): var(v), expr(rhs) 
        {
        }

        std::string getVar() { return var; }
        const NodePtr& getExpr() const { return expr; }
        Kind getKind() const override 
        { 
            return Kind::Assign;
        }

        int getValue() const override {
            std::cout << "Invalid call. (Assign)\n";
            throw 1;
            return 0;
        }

    private:
        std::string var;
        NodePtr expr;
    };

    class Print: public Node {
    public:
        Print(NodePtr expr)
        : expr(expr) 
        {
        }

        Kind getKind() const override 
        { 
            return Kind::Print;
        }
        
        const NodePtr& getExpr() const
        {
            return expr;
        }
        
        int getValue() const override {
            std::cout << "Invalid call. (Print)\n";
            throw 1;
            return 0;
        }


    private:
        NodePtr expr;
    };

    class PrintArgs: public Node {
    public:
        PrintArgs(NodePtr expr1, NodePtr expr2)
        : expr1(expr1), expr2(expr2) 
        {
        }

        Kind getKind() const override 
        { 
            return Kind::PrintArgs;
        }
        
        const NodePtr& getExpr1() const
        {
            return expr1;
        }

        const NodePtr& getExpr2() const
        {
            return expr2;
        }
        
        int getValue() const override {
            std::cout << "Invalid call. (PrintArgs)\n";
            throw 1;
            return 0;
        }


    private:
        NodePtr expr1;
        NodePtr expr2;
    };

    class Ident: public Node {
    public:
        Ident(std::string identText)
        : text(identText)
        {
        }

        Kind getKind() const override 
        { 
            return Kind::Ident;
        }

        int getValue() const override {
            std::cout << "Invalid call. (Ident)\n";
            throw 1;
            return 0;
        }

        const std::string& getText() const { return text; }

    private:
        std::string text;
    };

    class IfStmt: public Node {
    public:
        IfStmt(NodePtr cond, NodePtr t_block, NodePtr f_block)
        : cond(cond), true_block(t_block), false_block(f_block)
        {
        }

        Kind getKind() const override 
        { 
            return Kind::IfStmt;
        }

        int getValue() const override {
            std::cout << "Invalid call. (IfStmt)\n";
            throw 1;
            return 0;
        }

        const NodePtr& getCondition() const { return cond; }
        const NodePtr& getTrueBlock() const { return true_block; }
        const NodePtr& getFalseBlock() const { return false_block; }

    private:
        NodePtr cond;
        NodePtr true_block;
        NodePtr false_block;
    };

    class WhileStmt: public Node {
    public:
        WhileStmt(NodePtr cond, NodePtr l_block)
        : cond(cond), loop_block(l_block)
        {
        }

        Kind getKind() const override 
        { 
            return Kind::WhileStmt;
        }

        int getValue() const override {
            std::cout << "Invalid call. (WhileStmt)\n";
            throw 1;
        }

        const NodePtr& getCond() const { return cond; }
        const NodePtr& getLoopBlock() const { return loop_block; }

    private:
        NodePtr cond;
        NodePtr loop_block;
    };

    class Return: public Node {
    public:
        Return(NodePtr var): 
        retVar(var)
        {
        }

        Kind getKind() const override { return Kind::Return; }
        int getValue() const override
        {
            std::cout << "Invalid call. (Return)\n";
            throw 1;
            return 0;   
        }

        const NodePtr& getVar() const
        {
            return retVar;
        }

    private:
        NodePtr retVar;
    };

    class FuncDecl: public Node {
    public:
        FuncDecl(std::string name , std::vector<Ast::NodePtr>& arguments, NodePtr s): 
        funcName(name), args(std::move(arguments)), seq(s)
        {
        }

        Kind getKind() const override { return Kind::FuncDecl; }
        int getValue() const override
        {
            std::cout << "Invalid call. (FuncDecl)\n";
            throw 1;  
        }

        const std::string getFunctionName() const { return funcName; }

        const std::vector<Ast::NodePtr>& getArgs() const
        {
            return args;
        }

        const NodePtr& getSequence() const
        {
            return seq;
        }

    private:
        std::string funcName;
        std::vector<Ast::NodePtr> args;
        NodePtr seq;
    };

    class FuncCall: public Node {
    public:
        FuncCall(std::string name , std::vector<Ast::NodePtr>& arguments): 
        funcName(name), args(std::move(arguments))
        {
        }

        Kind getKind() const override { return Kind::FuncCall; }
        int getValue() const override
        {
            std::cout << "Invalid call. (FuncCall)\n";
            throw 1;
            return 0;   
        }

        const std::string getFunctionName() const { return funcName; }

        const std::vector<Ast::NodePtr>& getArgs() const
        {
            return args;
        }

    private:
        std::string funcName;
        std::vector<Ast::NodePtr> args;
    };

    class Literal: public Node {
    public:
        Literal(std::string literal): 
        literalValue(literal)
        {
        }

        Kind getKind() const override { return Kind::Literal; }
        int getValue() const override
        {
            std::cout << "Invalid call. (Literal)\n";
            throw 1; 
        }

        const std::string& getLiteralValue() const { return literalValue; }

    private:
        std::string literalValue;
    };
}

#endif