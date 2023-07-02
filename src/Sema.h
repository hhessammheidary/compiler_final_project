#ifndef SEMA_H
#define SEMA_H

#include "AST.h"

class Sema {
public:
    bool semantic(AST* Tree);
};

#endif



/*This code defines the header file for a semantic analyzer that performs
 semantic analysis on an abstract syntax tree (AST) produced by a parser. 
 The Sema class contains a single member function semantic which takes 
 a pointer to the root of the AST and returns a boolean indicating whether 
 the semantic analysis was successful or not. 
 The AST.h header file is included to define the AST class used in 
 the function signature.*/