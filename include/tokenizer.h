#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
} Token;

// Function prototypes
Token* tokenize(const char* code, size_t* token_count);
void free_tokens(Token* tokens, size_t token_count);

#endif
