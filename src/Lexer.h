#ifndef LEXER_H
#define LEXER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/MemoryBuffer.h"

class Lexer;

class Token {
    friend class Lexer;
public:
    enum class TokenKind : unsigned short {
        eoi,
        unknown,
        ident,
        number,
        comma,
        semi_colon,
        plus,
        minus,
        star,
        slash,
        equal,
        l_paren,
        r_paren,
        KW_type,
        KW_int
    };

private:
    TokenKind Kind;
    llvm::StringRef Text;

public:
    TokenKind getKind() const {
        return Kind;
    }

    llvm::StringRef getText() const {
        return Text;
    }

    bool is(TokenKind K) const {
        return Kind == K;
    }

    bool isOneOf(TokenKind K1, TokenKind K2) const {
        return is(K1) || is(K2);
    }

    template <typename... Ts>
    bool isOneOf(TokenKind K1, TokenKind K2, Ts... Ks) const {
        return is(K1) || isOneOf(K2, Ks...);
    }
};

class Lexer {
    const char* BufferStart;
    const char* BufferPtr;

public:
    Lexer(const llvm::StringRef& Buffer) {
        BufferStart = Buffer.begin();
        BufferPtr = BufferStart;
    }

    void next(Token& token);

private:
    void formToken(Token& Result, const char* TokEnd, Token::TokenKind Kind);
};

#endif



/*This code defines the header file for a lexer that scans
 the input string and produces a stream of tokens.
  The Token class defines the different kinds of tokens that 
  the lexer can produce, along with functions to retrieve the token
  kind and text. The Lexer class defines the interface for the lexer,
  including a constructor to initialize the buffer and a next 
  function to obtain the next token from the buffer. 
  The formToken function is a helper function used by the next 
  function to set the token kind and text.*/