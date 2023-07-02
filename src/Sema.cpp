#include "Sema.h"
#include "llvm/ADT/StringSet.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>
using namespace std;

namespace {
    class DeclCheck : public ASTVisitor {
        llvm::StringSet<> Scope;
        bool HasError;

        enum class ErrorType {
            Twice,
            Not
        };

        void error(ErrorType ET, llvm::StringRef V) {
            llvm::errs() << "Variable " << V << " "
                         << (ET == ErrorType::Twice ? "already" : "not")
                         << " declared\n";
            HasError = true;
        }

    public:
        DeclCheck() : HasError(false) {}

        bool hasError() {
            return HasError;
        }

        virtual void visit(Factor& Node) override {
            if (Node.getKind() == Factor::Ident) {
                if (Scope.find(Node.getVal()) == Scope.end())
                    error(ErrorType::Not, Node.getVal());
            }
        }

        virtual void visit(BinaryOp& Node) override {
            if (Node.getLeft())
                Node.getLeft()->accept(*this);
            else
                HasError = true;
            if (Node.getRight())
                Node.getRight()->accept(*this);
            else
                HasError = true;
        }

        virtual void visit(TypeDecl& Node) override {
            for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
                if (!Scope.insert(*I).second)
                    error(ErrorType::Twice, *I);
            }
        }

        virtual void visit(Goal& Node) override {
            for (auto I = Node.begin(), E = Node.end(); I != E; ++I) {
                (*I)->accept(*this);
            }
        }
    };
}

bool Sema::semantic(AST* Tree) {
    if (!Tree)
        return false;
    DeclCheck Check;
    Tree->accept(Check);
    return Check.hasError();
}




/*This code defines the implementation of a semantic analyzer that 
checks the validity of the input code represented by the AST. 
The DeclCheck class is a visitor that traverses the AST and checks 
that all variables are declared before use and that no variable is 
declared twice in the same scope. 
The error function reports a semantic error. 
The hasError function returns true if a semantic error has been 
encountered during analysis. The visit functions in DeclCheck are 
overridden to implement the specific checks for each AST node type. 
The semantic function is the entry point for semantic analysis and calls 
the accept function on the AST with an instance of DeclCheck to 
perform the checks.*/