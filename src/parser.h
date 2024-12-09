#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

// Expanded AST node types to support Python constructs
typedef enum {
    AST_LITERAL,         // Numeric or string literals
    AST_VARIABLE,        // Variables
    AST_BINARY_OP,       // Binary operations (e.g., x + y)
    AST_PROGRAM,         // Root node for entire program
    AST_ASSIGNMENT,      // Assignment statements (e.g., x = y)
    AST_BLOCK,           // Represents a block of statements (indented code)
    AST_FOR_LOOP,        // For 'for' loops with range or iterable
    AST_WHILE_LOOP,      // For 'while' loops
    AST_IN_KEYWORD,      // Python 'in' keyword for loops and membership tests
    AST_RANGE,           // Python range() function
    AST_FUNCTION_CALL,   // Function call (like print)
    AST_PUNCTUATION,     // Punctuation tokens like parentheses and commas
    AST_COMPARISON_OP,    // Comparison operations (e.g., x > y)
    AST_STRING           // String literal
} ASTNodeType;

// AST node structure with additional fields to support more complex parsing
typedef struct ASTNode {
    ASTNodeType type;
    char* value;            // For literals, variable names, or operators
    struct ASTNode* left;   // For binary operations or conditions
    struct ASTNode* right;  // For binary operations, assignments, or blocks
    struct ASTNode* init;   // For loop initialization
    struct ASTNode* condition;  // For loop or conditional conditions
    struct ASTNode* increment;  // For loop increment/iteration
    struct ASTNode** children;  // For nodes with multiple children (lists, function calls, blocks)
    size_t child_count;     // Number of children
    struct ASTNode* body;   // For loop bodies and other block structures
} ASTNode;

// Function prototypes for parsing
ASTNode* parse(Token* tokens, size_t token_count);
ASTNode* parse_primary(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_mul_div(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_add_sub(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_expression(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_assignment(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_statements(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_statement(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_for_loop(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_while_loop(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_print_function(Token** tokens, size_t* index, size_t token_count);
ASTNode* parse_range_call(Token** tokens, size_t* index, size_t token_count);

// Memory management and debugging
void free_ast(ASTNode* root);
void print_ast(ASTNode* node, int level);

// Node creation utility
ASTNode* create_ast_node(ASTNodeType type, const char* value);

#endif // PARSER_H