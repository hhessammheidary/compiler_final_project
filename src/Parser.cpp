#include "Parser.h"

AST* Parser::parse() {
    AST* Res = parseGoal();
    expect(Token::TokenKind::eoi);
    return Res;
}

AST* Parser::parseGoal() {
    llvm::SmallVector<Statement*, 8> Statements;
    Statement* statement;
    while (!Tok.is(Token::TokenKind::eoi) && (statement = parseStatement())) {
        Statements.push_back(statement);
    }

    return new Goal(Statements);
}

Statement* Parser::parseStatement() {
    if (Tok.is(Token::TokenKind::KW_type)) {
        llvm::SmallVector<llvm::StringRef, 8> Vars;
        advance();
        if (expect(Token::TokenKind::KW_int)) {
            goto _error;
        }

        advance();
        if (expect(Token::TokenKind::ident))
            goto _error;
        Vars.push_back(Tok.getText());

        advance();
        while (Tok.is(Token::TokenKind::comma)) {
            advance();
            if (expect(Token::TokenKind::ident))
                goto _error;
            Vars.push_back(Tok.getText());
            advance();
        }

        if (consume(Token::TokenKind::semi_colon))
            goto _error;

        return new TypeDecl(Vars);
    }

    if (Tok.is(Token::TokenKind::ident)) {

        Expr* Left = new Factor(Factor::Ident, Tok.getText());

        advance();
        if (expect(Token::TokenKind::equal))
            goto _error;

        BinaryOp::Operator Op = BinaryOp::Equal;
        Expr* E;

        advance();
        E = parseExpr();

        if (consume(Token::TokenKind::semi_colon))
            goto _error;

        Left = new BinaryOp(Op, Left, E);
        return Left;
    }

_error:
    while (Tok.getKind() != Token::TokenKind::eoi)
        advance();
    return nullptr;
}

Expr* Parser::parseExpr() {
    Expr* Left = parseTerm();
    while (Tok.isOneOf(Token::TokenKind::plus, Token::TokenKind::minus)) {
        BinaryOp::Operator Op = Tok.is(Token::TokenKind::plus)
                                    ? BinaryOp::Plus
                                    : BinaryOp::Minus;
        advance();
        Expr* Right = parseTerm();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr* Parser::parseTerm() {
    Expr* Left = parseFactor();
    while (Tok.isOneOf(Token::TokenKind::star, Token::TokenKind::slash)) {
        BinaryOp::Operator Op =
            Tok.is(Token::TokenKind::star) ? BinaryOp::Mul : BinaryOp::Div;
        advance();
        Expr* Right = parseFactor();
        Left = new BinaryOp(Op, Left, Right);
    }
    return Left;
}

Expr* Parser::parseFactor() {
    Expr* Res = nullptr;
    switch (Tok.getKind()) {
        case Token::TokenKind::number:
            Res = new Factor(Factor::Number, Tok.getText());
            advance();
            break;
        case Token::TokenKind::ident:
            Res = new Factor(Factor::Ident, Tok.getText());
            advance();
            break;
        case Token::TokenKind::l_paren:
            advance();
            Res = parseExpr();
            if (!consume(Token::TokenKind::r_paren))
                break;
        default:
            if (!Res)
                error();
            while (!Tok.isOneOf(Token::TokenKind::r_paren, Token::TokenKind::star,
                                Token::TokenKind::plus, Token::TokenKind::minus,
                                Token::TokenKind::slash, Token::TokenKind::equal, Token::TokenKind::eoi))
                advance();
    }
    return Res;
}






/*This code defines the implementation of a parser that takes 
a stream of tokens produced by the lexer and constructs an abstract 
syntax tree (AST) representing the parsed code. 
The parse function is the entry point for the parser, 
and it calls parseGoal to parse the input code and constructs an AST. 
The parseGoal function parses a sequence of statements and returns 
a Goal node representing the parsed code. 
The parseStatement function parses a single statement and returns 
a Statement node representing the parsed statement. 
The parseExpr, parseTerm, and parseFactor functions parse expressions, 
terms, and factors, respectively. 
The expect and consume functions are helper functions used to match 
and consume tokens of a particular kind. The error function is used 
to report a syntax error.*/