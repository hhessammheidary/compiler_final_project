#pragma once

#include <vector>
#include <string>
#include "llvm/ADT/StringRef.h"

class AST;
class Statement;
class Goal;
class Expr;
class Factor;
class BinaryOp;
class TypeDecl;

class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Goal&) = 0;
    virtual void visit(Factor&) = 0;
    virtual void visit(BinaryOp&) = 0;
    virtual void visit(TypeDecl&) = 0;
    virtual void visit(AST&) = 0;
    virtual void visit(Expr&) = 0;
};

class AST {
public:
    virtual ~AST() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

class Statement : public AST {
public:
    Statement() = default;
};

class Goal : public AST {
public:
    using StatementVector = std::vector<Statement*>;
private:
    StatementVector statements;
public:
    explicit Goal(const StatementVector& statements) : statements(statements) {}
    auto begin() const { return statements.begin(); }
    auto end() const { return statements.end(); }
    virtual void accept(ASTVisitor& visitor) override;
};

class Expr : public Statement {
public:
    Expr() = default;
};

class Factor : public Expr {
public:
    enum class ValueKind {
        Ident,
        Number
    };

    explicit Factor(ValueKind kind, llvm::StringRef val)
        : kind(kind), val(val) {}

    ValueKind getKind() const { return kind; }
    llvm::StringRef getVal() const { return val; }
    virtual void accept(ASTVisitor& visitor) override;
private:
    ValueKind kind;
    llvm::StringRef val;
};

class BinaryOp : public Expr {
public:
    enum class Operator {
        Plus,
        Minus,
        Mul,
        Div,
        Equal
    };

    explicit BinaryOp(Operator op, Expr* left, Expr* right)
        : op(op), left(left), right(right) {}

    Expr* getLeft() const { return left; }
    Expr* getRight() const { return right; }
    Operator getOperator() const { return op; }
    virtual void accept(ASTVisitor& visitor) override;
private:
    Operator op;
    Expr* left;
    Expr* right;
};

class TypeDecl : public Statement {
public:
    using VarVector = std::vector<llvm::StringRef>;
private:
    VarVector vars;
public:
    explicit TypeDecl(const VarVector& vars) : vars(vars) {}
    auto begin() const { return vars.begin(); }
    auto end() const { return vars.end(); }
    virtual void accept(ASTVisitor& visitor) override;
};

inline void Goal::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

inline void Factor::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

inline void BinaryOp::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

inline void TypeDecl::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}






/*This code defines an abstract syntax tree (AST) for a 
language with statements, expressions, and declarations. 
It also defines an ASTVisitor class that can be used to 
traverse the AST and perform operations on it. 
The code uses standard C++ containers and avoids using forward 
declarations wherever possible.*/