#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,        // Python keywords like if, else, def
    TOKEN_IDENTIFIER,     // Variable or function names
    TOKEN_NUMBER,         // Numeric literals
    TOKEN_STRING,         // String literals
    TOKEN_OPERATOR,       // Operators like +, -, *, /, >, <, ==
    TOKEN_PARENTHESIS,    // '(' or ')'
    TOKEN_COLON,          // ':'
    TOKEN_COMMA,          // ','
    TOKEN_COMMENT,        // Python comments (starting with #)
    TOKEN_NEWLINE,        // Newlines to handle indentation levels
    TOKEN_INDENT,         // Python indentation
    TOKEN_DEDENT,         // Python dedentation
    TOKEN_FOR,            // Python 'for' loop keyword
    TOKEN_WHILE,          // Python 'while' loop keyword
    TOKEN_IN,             // Python 'in' keyword
    TOKEN_RANGE,          // Python 'range' keyword
    TOKEN_FUNCTION_CALL,  // Function calls
    TOKEN_UNKNOWN         // Unknown or unsupported tokens
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// Function prototypes
Token* tokenize(const char* code, size_t* token_count);
void free_tokens(Token* tokens, size_t token_count);

#endif // TOKENIZER_H

