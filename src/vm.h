// #ifndef VM_H
// #define VM_H

// #include "parser.h"

// // Maximum number of symbols (variables)
// #define MAX_SYMBOLS 128

// // Symbol Table Entry
// typedef struct {
//     char* name;  // Variable name
//     int value;   // Variable value
// } Symbol;

// // Virtual Machine Structure
// typedef struct {
//     Symbol symbol_table[MAX_SYMBOLS];
//     int symbol_count;
// } VM;

// // VM Functions
// void vm_init(VM* vm);
// int vm_evaluate(VM* vm, ASTNode* node);

// #endif // VM_H


#ifndef VM_H
#define VM_H

#include "parser.h"

#define MAX_SYMBOLS 100

// Symbol table entry
typedef struct {
    char* name;
    int value;
} Symbol;

// Virtual Machine structure
typedef struct {
    Symbol symbol_table[MAX_SYMBOLS];
    int symbol_count;
} VM;

// Initialize the VM
void vm_init(VM* vm);

// Look up a variable in the symbol table
int vm_lookup_symbol(VM* vm, const char* name);

// Set a variable's value in the symbol table
void vm_set_symbol(VM* vm, const char* name, int value);

// Evaluate the AST recursively
int vm_evaluate(VM* vm, ASTNode* node);

#endif