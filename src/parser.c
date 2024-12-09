#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to create an AST node
ASTNode* create_ast_node(ASTNodeType type, const char* value) {
    printf("Creating AST Node: Type=%d, Value=%s\n", type, value ? value : "NULL");

    ASTNode* node = malloc(sizeof(ASTNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed for AST node.\n");
        return NULL;
    }

    node->type = type;
    node->value = value ? strdup(value) : NULL;
    node->left = NULL;
    node->right = NULL;
    node->init = NULL;
    node->condition = NULL;
    node->increment = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->body = NULL;

    return node;
}

// Free memory used by AST nodes
void free_ast(ASTNode* root) {
    if (!root) return;

    printf("Freeing AST Node: Type=%d, Value=%s\n", root->type, root->value ? root->value : "(null)");

    free_ast(root->left);
    free_ast(root->right);
    free_ast(root->init);
    free_ast(root->condition);
    free_ast(root->increment);
    free_ast(root->body);

    // Free children (for statements with multiple children)
    for (size_t i = 0; i < root->child_count; ++i) {
        free_ast(root->children[i]);
    }
    free(root->children);

    free(root->value);
    free(root);
}

// Print AST for debugging
void print_ast(ASTNode* node, int level) {
    if (!node) return;

    for (int i = 0; i < level; ++i) printf("  ");
    printf("Node Type: %d, Value: %s\n", node->type, node->value ? node->value : "(null)");

    if (node->left) {
        print_ast(node->left, level + 1);
    }

    if (node->right) {
        print_ast(node->right, level + 1);
    }

    if (node->init) {
        print_ast(node->init, level + 1);
    }
    if (node->condition) {
        print_ast(node->condition, level + 1);
    }
    if (node->increment) {
        print_ast(node->increment, level + 1);
    }
    if (node->body) {
        print_ast(node->body, level + 1);
    }
    for (size_t i = 0; i < node->child_count; ++i) {
        print_ast(node->children[i], level + 1);
    }
}

// Parse primary expressions (numbers or variables)
ASTNode* parse_primary(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count) return NULL;

    Token* token = &(*tokens)[*index];
    if (token->type == TOKEN_NUMBER) {
        (*index)++;
        return create_ast_node(AST_LITERAL, token->value);
    } else if (token->type == TOKEN_IDENTIFIER) {
        (*index)++;
        return create_ast_node(AST_VARIABLE, token->value);
    }

    fprintf(stderr, "Unexpected token in primary expression: Type=%d, Value=%s\n", token->type, token->value);
    return NULL;
}

// Parse multiplication and division
ASTNode* parse_mul_div(Token** tokens, size_t* index, size_t token_count) {
    ASTNode* left = parse_primary(tokens, index, token_count);
    if (!left) return NULL;

    while (*index < token_count) {
        Token* token = &(*tokens)[*index];
        if (token->type == TOKEN_OPERATOR && (strcmp(token->value, "*") == 0 || strcmp(token->value, "/") == 0)) {
            (*index)++;
            ASTNode* op_node = create_ast_node(AST_BINARY_OP, token->value);
            op_node->left = left;
            op_node->right = parse_primary(tokens, index, token_count);
            if (!op_node->right) {
                free_ast(op_node);
                return NULL; // Error: Missing right operand
            }
            left = op_node;
        } else {
            break;
        }
    }

    return left;
}

// Parse addition and subtraction
ASTNode* parse_add_sub(Token** tokens, size_t* index, size_t token_count) {
    ASTNode* left = parse_mul_div(tokens, index, token_count);
    if (!left) return NULL;

    while (*index < token_count) {
        Token* token = &(*tokens)[*index];
        if (token->type == TOKEN_OPERATOR && (strcmp(token->value, "+") == 0 || strcmp(token->value, "-") == 0)) {
            (*index)++;
            ASTNode* op_node = create_ast_node(AST_BINARY_OP, token->value);
            op_node->left = left;
            op_node->right = parse_mul_div(tokens, index, token_count);
            if (!op_node->right) {
                free_ast(op_node);
                return NULL; // Error: Missing right operand
            }
            left = op_node;
        } else {
            break;
        }
    }

    return left;
}

// Parse expressions, handling function calls like range()
ASTNode* parse_expression(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count) return NULL;

    Token* token = &(*tokens)[*index];

    // Handle function calls (specifically range for now)
    if (token->type == TOKEN_RANGE) {
        return parse_range_call(tokens, index, token_count);
    }

    // Default to parsing additive/subtractive expressions
    return parse_add_sub(tokens, index, token_count);
}

// Parse assignment expressions
ASTNode* parse_assignment(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_IDENTIFIER) return NULL;

    Token* token = &(*tokens)[*index];
    ASTNode* var_node = create_ast_node(AST_VARIABLE, token->value);
    (*index)++;

    if (*index < token_count && (*tokens)[*index].type == TOKEN_OPERATOR &&
        strcmp((*tokens)[*index].value, "=") == 0) {
        (*index)++;
        ASTNode* assign_node = create_ast_node(AST_ASSIGNMENT, "=");
        assign_node->left = var_node;
        assign_node->right = parse_expression(tokens, index, token_count);
        if (!assign_node->right) {
            fprintf(stderr, "Expected expression after '=' in assignment.\n");
            free_ast(var_node);
            free_ast(assign_node);
            return NULL;
        }
        return assign_node;
    }

    // If it's not an assignment, put the identifier token back and try parsing it as an expression
    (*index)--;
    free_ast(var_node);
    return NULL;
}

// Parse the range() function call
ASTNode* parse_range_call(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_RANGE) return NULL;
    
    // Create AST node for "range"
    ASTNode* range_node = create_ast_node(AST_RANGE, "range");
    (*index)++; // Consume "range"

    // Create AST node for the opening parenthesis
    ASTNode* open_paren = create_ast_node(AST_PUNCTUATION, "(");
    range_node->left = open_paren;

    if (*index >= token_count || (*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens)[*index].value, "(") != 0) {
        fprintf(stderr, "Expected '(' after 'range'.\n");
        free_ast(range_node);
        return NULL;
    }
    (*index)++; // Consume "("

    range_node->children = malloc(3 * sizeof(ASTNode*)); // Max 3 arguments for range
    range_node->child_count = 0;

    // Parse arguments until closing parenthesis
    while (*index < token_count && ((*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens)[*index].value, ")") != 0)) {
        ASTNode* arg = parse_expression(tokens, index, token_count);
        if (!arg) {
            fprintf(stderr, "Expected expression as argument in 'range'.\n");
            free_ast(range_node);
            return NULL;
        }
        range_node->children[range_node->child_count++] = arg;

        if (*index < token_count && (*tokens)[*index].type == TOKEN_COMMA) {
            ASTNode* comma_node = create_ast_node(AST_PUNCTUATION, ",");
            (*index)++; // Consume comma
        }
    }

    // Create AST node for the closing parenthesis
    ASTNode* close_paren = create_ast_node(AST_PUNCTUATION, ")");
    range_node->right = close_paren;

    if (*index >= token_count || (*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens)[*index].value, ")") != 0) {
        fprintf(stderr, "Expected ')' after 'range' arguments.\n");
        free_ast(range_node);
        return NULL;
    }
    (*index)++; // Consume ")"

    return range_node;
}

// Parse for loop
ASTNode* parse_for_loop(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_FOR) return NULL;
    
    // Create FOR node
    ASTNode* for_node = create_ast_node(AST_FOR_LOOP, "for");
    (*index)++; // Consume "for"

    // Parse loop variable
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "Expected identifier after 'for'.\n");
        free_ast(for_node);
        return NULL;
    }
    for_node->init = create_ast_node(AST_VARIABLE, (*tokens)[*index].value);
    (*index)++; // Consume identifier

    // Create IN node
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_IN) {
        fprintf(stderr, "Expected 'in' in for loop.\n");
        free_ast(for_node);
        return NULL;
    }
    ASTNode* in_node = create_ast_node(AST_IN_KEYWORD, "in");
    (*index)++; // Consume "in"

    // Parse range or other expression
    for_node->condition = parse_expression(tokens, index, token_count);

    if (!for_node->condition) {
        fprintf(stderr, "Expected expression after 'in' in for loop.\n");
        free_ast(for_node);
        return NULL;
    }

    // Add the IN keyword node between loop variable and range
    in_node->left = for_node->init;
    in_node->right = for_node->condition;
    for_node->condition = in_node;

    // Expect colon
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_COLON) {
        fprintf(stderr, "Expected ':' in for loop.\n");
        free_ast(for_node);
        return NULL;
    }
    ASTNode* colon_node = create_ast_node(AST_PUNCTUATION, ":");
    (*index)++; // Consume ":"

    // Parse loop body
    for_node->body = parse_statements(tokens, index, token_count);

    return for_node;
}

// Parse while loop
ASTNode* parse_while_loop(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_WHILE) return NULL;
    (*index)++; // Consume "while"

    ASTNode* while_node = create_ast_node(AST_WHILE_LOOP, "while");
    while_node->condition = parse_expression(tokens, index, token_count);

    if (!while_node->condition) {
        fprintf(stderr, "Expected condition after 'while'.\n");
        free_ast(while_node);
        return NULL;
    }

    if (*index >= token_count || (*tokens)[*index].type != TOKEN_COLON) {
        fprintf(stderr, "Expected ':' in while loop.\n");
        free_ast(while_node);
        return NULL;
    }
    ASTNode* colon_node = create_ast_node(AST_PUNCTUATION, ":");
    (*index)++; // Consume ":"

    while_node->body = parse_statements(tokens, index, token_count);

    return while_node;
}

ASTNode* parse_print_function(Token** tokens, size_t* index, size_t token_count){
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_IDENTIFIER || strcmp((*tokens)[*index].value, "print") != 0){
        return NULL; // Not a print function
    }
    
    // Create print function node
    ASTNode* print_node = create_ast_node(AST_FUNCTION_CALL, "print");
    (*index)++; //Consume "print"

    // Add opening parenthesis node
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens)[*index].value, "(") != 0){
        fprintf(stderr, "Expected '(' after 'print'.\n");
        free_ast(print_node);
        return NULL;
    }
    ASTNode* open_paren = create_ast_node(AST_PUNCTUATION, "(");
    print_node->left = open_paren;
    (*index)++; //Consume "("

    print_node->children = malloc(sizeof(ASTNode*)); //Allow for one argument initially
    print_node->child_count = 0;

    //Parse argument (simplified to handle only string literals for now)
    // if (*index < token_count && (*tokens)[*index].type == TOKEN_STRING){
    //     print_node->children[0] = create_ast_node(AST_LITERAL, (*tokens)[*index].value);
    //     print_node->child_count = 1;
    //     (*index)++;
    // } else {
    //     fprintf(stderr, "Expected string literal as argument in 'print'.\n");
    //     free_ast(print_node);
    //     return NULL;
    // }

    if (*index < token_count && (*tokens)[*index].type == TOKEN_STRING){
        print_node->children[0] = create_ast_node(AST_STRING, (*tokens)[*index].value);
        print_node->child_count = 1;
        (*index)++;
    } else {
        fprintf(stderr, "Expected string literal as argument in 'print'.\n");
        free_ast(print_node);
        return NULL;
    }

    // Add closing parenthesis node
    // if (*index >= token_count || (*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens
    // Finish the print function parsing
    if (*index >= token_count || (*tokens)[*index].type != TOKEN_PARENTHESIS || strcmp((*tokens)[*index].value, ")") != 0){
        fprintf(stderr, "Expected ')' after print argument.\n");
        free_ast(print_node);
        return NULL;
    }
    ASTNode* close_paren = create_ast_node(AST_PUNCTUATION, ")");
    print_node->right = close_paren;
    (*index)++; //Consume ")"

    return print_node;
}

// Parse different types of statements
ASTNode* parse_statement(Token** tokens, size_t* index, size_t token_count) {
    ASTNode* statement = NULL;

    // Try parsing different statement types in order
    statement = parse_assignment(tokens, index, token_count);
    if (statement) return statement;

    statement = parse_for_loop(tokens, index, token_count);
    if (statement) return statement;

    statement = parse_while_loop(tokens, index, token_count);
    if (statement) return statement;

    statement = parse_print_function(tokens, index, token_count);
    if (statement) return statement;

    // If no specific statement type matches, try parsing as an expression
    statement = parse_expression(tokens, index, token_count);
    if (statement) return statement;

    return NULL;
}

// Parse multiple statements (used for function bodies, loop bodies, etc.)
ASTNode* parse_statements(Token** tokens, size_t* index, size_t token_count) {
    if (*index >= token_count) return NULL;

    // Create a block node to hold multiple statements
    ASTNode* block_node = create_ast_node(AST_BLOCK, "block");
    size_t initial_size = 2; // Start with a smaller initial size
    block_node->children = malloc(sizeof(ASTNode*) * initial_size); 
    if (!block_node->children) {
        fprintf(stderr, "Memory allocation failed for block node children.\n");
        free_ast(block_node);
        return NULL;
    }
    block_node->child_count = 0;
    size_t allocated_size = initial_size;

    // Parse statements until indentation changes or we run out of tokens
    while (*index < token_count) {
        // Check for end of block (simplified - would need more sophisticated indentation tracking in a real parser)
        if ((*tokens)[*index].type == TOKEN_DEDENT) {
            (*index)++; // Consume dedent
            break;
        }

        ASTNode* statement = parse_statement(tokens, index, token_count);
        if (!statement) {
            // If we can't parse a statement, break the parsing
            break;
        }

        // Dynamically resize children array if needed
        if (block_node->child_count >= allocated_size) {
            allocated_size *= 2;
            ASTNode** new_children = realloc(block_node->children, sizeof(ASTNode*) * allocated_size);
            if (!new_children) {
                fprintf(stderr, "Memory reallocation failed for block node children.\n");
                free_ast(block_node); // Free previously allocated memory
                return NULL;
            }
            block_node->children = new_children;
        }

        // Add statement to block
        block_node->children[block_node->child_count++] = statement;
    }

    // If no statements were parsed, free the block node
    if (block_node->child_count == 0) {
        free(block_node->children);
        free(block_node);
        return NULL;
    }

    return block_node;
}

// Top-level parsing function
ASTNode* parse(Token* tokens, size_t token_count) {
    size_t index = 0;
    
    // Create root node for the entire program
    ASTNode* program_node = create_ast_node(AST_PROGRAM, "program");
    size_t initial_size = 10; // Start with a smaller initial size
    program_node->children = malloc(sizeof(ASTNode*) * initial_size); 
    if (!program_node->children) {
        fprintf(stderr, "Memory allocation failed for program node children.\n");
        free_ast(program_node);
        return NULL;
    }
    program_node->child_count = 0;
    size_t allocated_size = initial_size;

    // Parse statements until we run out of tokens
    while (index < token_count) {
        ASTNode* statement = parse_statement(&tokens, &index, token_count);
        if (!statement) {
            fprintf(stderr, "Parsing failed at token %zu\n", index);
            free_ast(program_node);
            return NULL;
        }
        
        // Dynamically resize children array if needed
        if (program_node->child_count >= allocated_size) {
            allocated_size *= 2;
            ASTNode** new_children = realloc(program_node->children, sizeof(ASTNode*) * allocated_size);
            if (!new_children) {
                fprintf(stderr, "Memory reallocation failed for program node children.\n");
                free_ast(program_node); // Free previously allocated memory
                return NULL;
            }
            program_node->children = new_children;
        }

        // Add statement to program node
        program_node->children[program_node->child_count++] = statement;
    }

    return program_node;
}