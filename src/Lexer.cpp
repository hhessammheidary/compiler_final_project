#include "Lexer.h"

namespace charinfo {
    bool isWhitespace(char c) {
        return c == ' ' || c == '\t' || c == '\f' || c == '\v' || c == '\r' || c == '\n';
    }

    bool isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    bool isLetter(char c) {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }
}

void Lexer::next(Token& token) {
    while (*BufferPtr && charinfo::isWhitespace(*BufferPtr)) {
        ++BufferPtr;
    }

    if (!*BufferPtr) {
        token.Kind = Token::eoi;
        return;
    }

    if (charinfo::isLetter(*BufferPtr)) {
        const char* end = BufferPtr + 1;
        while (charinfo::isLetter(*end)) {
            ++end;
        }

        llvm::StringRef Name(BufferPtr, end - BufferPtr);
        Token::TokenKind kind = Token::ident;

        if (Name == "type") {
            kind = Token::KW_type;
        }
        else if (Name == "int") {
            kind = Token::KW_int;
        }

        formToken(token, end, kind);
        return;
    }
    else if (charinfo::isDigit(*BufferPtr)) {
        const char* end = BufferPtr + 1;
        while (charinfo::isDigit(*end)) {
            ++end;
        }

        formToken(token, end, Token::number);
        return;
    }
    else {
        switch (*BufferPtr) {
            case '+':
                formToken(token, BufferPtr + 1, Token::plus);
                break;
            case '-':
                formToken(token, BufferPtr + 1, Token::minus);
                break;
            case '*':
                formToken(token, BufferPtr + 1, Token::star);
                break;
            case '/':
                formToken(token, BufferPtr + 1, Token::slash);
                break;
            case '=':
                formToken(token, BufferPtr + 1, Token::equal);
                break;
            case '(':
                formToken(token, BufferPtr + 1, Token::l_paren);
                break;
            case ')':
                formToken(token, BufferPtr + 1, Token::r_paren);
                break;
            case ';':
                formToken(token, BufferPtr + 1, Token::semi_colon);
                break;
            case ',':
                formToken(token, BufferPtr + 1, Token::comma);
                break;
            default:
                formToken(token, BufferPtr + 1, Token::unknown);
        }

        return;
    }
}

void Lexer::formToken(Token& token, const char* tokEnd, Token::TokenKind kind) {
    token.Kind = kind;
    token.Text = llvm::StringRef(BufferPtr, tokEnd - BufferPtr);
    BufferPtr = tokEnd;
}


/*This code defines the implementation of a lexer that scans the
input string and produces a stream of tokens. 
The next function consumes the input string character by character 
and produces the next token in the stream. The formToken function is a 
helper function that sets the token kind and text based on the input 
parameters. The charinfo namespace contains helper functions that 
are used to classify characters as whitespace, digits, or letters.*/