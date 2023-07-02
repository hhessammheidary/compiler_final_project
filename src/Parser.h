#ifndef PARSER_H
#define PARSER_H

#include "AST.h"
#include "Lexer.h"
#include "llvm/Support/raw_ostream.h"

class Parser {
    Lexer& Lex;
    Token Tok;
    bool HasError;

    void error() {
        llvm::errs() << "Unexpected: " << Tok.getText() << "\n";
        HasError = true;
    }

    void advance() {
        Lex.next(Tok);
    }

    bool expect(Token::TokenKind Kind) {
        if (!Tok.is(Kind)) {
            error();
            return true;
        }
        return false;
    }

    bool consume(Token::TokenKind Kind) {
        if (expect(Kind))
            return true;
        advance();
        return false;
    }

    AST* parseGoal();
    Statement* parseStatement();
    Expr* parseExpr();
    Expr* parseTerm();
    Expr* parseFactor();

public:
    Parser(Lexer& Lex) : Lex(Lex), HasError(false) {
        advance();
    }
    AST* parse();
    bool hasError() {
        return HasError;
    }
};

#endif



/*This code defines the header file for a parser that takes a stream
 of tokens produced by the lexer and constructs an abstract 
 syntax tree (AST) representing the parsed code. 
 The Parser class contains member functions to implement the parsing 
 logic, including functions to parse the goal, statements, expressions, 
 terms, and factors. The advance function is used to advance the 
 token stream by one token, and the expect function checks if the 
 current token is of the expected kind and reports an error if it is not. 
 The consume function consumes the current token if it is of the expected 
 kind, and otherwise reports an error. 
 The error function reports a syntax error. 
 The hasError function returns true if a syntax error has been 
 encountered during parsing.*/