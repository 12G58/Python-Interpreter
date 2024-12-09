#include "tokenizer.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Helper function to create a token
Token create_token(TokenType type, const char* value) {
    Token token;
    token.type = type;
    token.value = malloc(strlen(value) + 1);
    if (token.value != NULL) {
        strcpy(token.value, value);
    }
    return token;
}

// Free tokens
void free_tokens(Token* tokens, size_t token_count) {
    for (size_t i = 0; i < token_count; ++i) {
        free(tokens[i].value);
    }
    free(tokens);
}

// Check if a string is a Python keyword
int is_keyword(const char* token) {
    const char* keywords[] = {
        "if", "else", "while", "for", "def", "return", "class",
        "try", "except", "finally", "with", "as", "import",
        "in", "range", NULL
    };

    for (int i = 0; keywords[i] != NULL; i++) {
        if (strcmp(token, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Tokenizer function
Token* tokenize(const char* code, size_t* token_count) {
    *token_count = 0;
    size_t capacity = 10;
    Token* tokens = malloc(capacity * sizeof(Token));
    if (tokens == NULL) {
        fprintf(stderr, "Failed to allocate memory for tokens.\n");
        return NULL;
    }

    size_t i = 0, current_indent = 0, line_start_indent = 0;
    while (code[i] != '\0') {
        // Skip whitespace and handle indentation
        if (isspace(code[i])) {
            if (code[i] == '\n') { // Handle newline
                tokens[(*token_count)++] = create_token(TOKEN_NEWLINE, "\\n");
                if (*token_count >= capacity) {
                    capacity *= 2;
                    tokens = realloc(tokens, capacity * sizeof(Token));
                }

                i++;
                line_start_indent = 0;
                while (isspace(code[i]) && code[i] != '\n') {
                    line_start_indent += (code[i] == '\t') ? 4 : 1;
                    i++;
                }

                if (line_start_indent > current_indent) {
                    tokens[(*token_count)++] = create_token(TOKEN_INDENT, "<INDENT>");
                } else if (line_start_indent < current_indent) {
                    tokens[(*token_count)++] = create_token(TOKEN_DEDENT, "<DEDENT>");
                }
                current_indent = line_start_indent;
                continue;
            }
            i++;
            continue;
        }

        // Handle single-character tokens
        if (strchr("()+-*/:,.", code[i])) {
            char op[2] = {code[i], '\0'};
            TokenType type = (code[i] == '(' || code[i] == ')') ? TOKEN_PARENTHESIS :
                             (code[i] == ':') ? TOKEN_COLON :
                             (code[i] == ',') ? TOKEN_COMMA : TOKEN_OPERATOR;
            tokens[(*token_count)++] = create_token(type, op);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            i++;
            continue;
        }

        // Handle multi-character operators
        if (strchr("><=!", code[i])) {
            char op[3] = {code[i], '\0', '\0'};
            if (code[i + 1] == '=' || (code[i] == '!' && code[i + 1] == '=')) {
                op[1] = code[++i];
            }
            tokens[(*token_count)++] = create_token(TOKEN_OPERATOR, op);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            i++;
            continue;
        }

        // Handle comments
        if (code[i] == '#') {
            size_t start = i;
            while (code[i] != '\n' && code[i] != '\0') i++;
            size_t length = i - start;
            char* comment = strndup(&code[start], length);
            tokens[(*token_count)++] = create_token(TOKEN_COMMENT, comment);
            free(comment);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            continue;
        }

        // Handle string literals
        if (code[i] == '\'' || code[i] == '"') {
            char quote = code[i++];
            size_t start = i;
            while (code[i] != quote && code[i] != '\0') i++;
            if (code[i] == quote) i++;
            size_t length = i - start;
            char* string = strndup(&code[start], length);
            tokens[(*token_count)++] = create_token(TOKEN_STRING, string);
            free(string);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            continue;
        }

        // Handle identifiers and keywords
        if (isalpha(code[i])) {
            size_t start = i;
            while (isalnum(code[i]) || code[i] == '_') i++;
            size_t length = i - start;
            char* identifier = strndup(&code[start], length);
            TokenType type = is_keyword(identifier) ? 
                             (strcmp(identifier, "for") == 0 ? TOKEN_FOR : 
                              strcmp(identifier, "while") == 0 ? TOKEN_WHILE : 
                              strcmp(identifier, "in") == 0 ? TOKEN_IN : 
                              strcmp(identifier, "range") == 0 ? TOKEN_RANGE : TOKEN_KEYWORD) 
                             : TOKEN_IDENTIFIER;
            tokens[(*token_count)++] = create_token(type, identifier);
            free(identifier);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            continue;
        }

        // Handle numeric literals
        if (isdigit(code[i])) {
            size_t start = i;
            while (isdigit(code[i])) i++;
            size_t length = i - start;
            char* number = strndup(&code[start], length);
            tokens[(*token_count)++] = create_token(TOKEN_NUMBER, number);
            free(number);
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(Token));
            }
            continue;
        }

        // Handle unknown tokens
        char unknown[2] = {code[i], '\0'};
        tokens[(*token_count)++] = create_token(TOKEN_UNKNOWN, unknown);
        if (*token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
        }
        i++;
    }

    return tokens;
}
