#include "tokenizer.h"
#include "parser.h"
#include "vm.h"
#include <stdio.h>

int main() {
    // Example code to tokenize, parse, and execute
    const char* code = "for i in range(1,10): print('Hello')";
    // const char* code = "3*2 + 6"; 

    size_t token_count = 0;


    // Tokenize the code
    Token* tokens = tokenize(code, &token_count);
    if (!tokens) {
        fprintf(stderr, "Tokenization failed.\n");
        return 1;
    }

    printf("Tokens:\n");
    for (size_t i = 0; i < token_count; ++i) {
        printf("Token: %s, Type: %d\n", tokens[i].value, tokens[i].type);
    }

    // Parse the tokens into an AST
    ASTNode* ast = parse(tokens, token_count);
    if (!ast) {
        fprintf(stderr, "Parsing failed.\n");
        free_tokens(tokens, token_count);
        return 1;
    }

    // Print the AST for debugging
    printf("\nAbstract Syntax Tree:\n");
    print_ast(ast, 1);

    // Initialize the VM
    VM vm;
    vm_init(&vm);

    // Evaluate the AST
    printf("\nExecution Output:\n");
    vm_evaluate(&vm, ast);

    // Debugging: Print the contents of the symbol table
    printf("\nSymbol Table:\n");
    for (int i = 0; i < vm.symbol_count; ++i) {
        printf("Variable: %s, Value: %d\n", vm.symbol_table[i].name, vm.symbol_table[i].value);
    }

    // Free resources
    free_ast(ast);
    free_tokens(tokens, token_count);

    return 0;
}
