// #include "vm.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// // Initialize the VM
// void vm_init(VM* vm) {
//     vm->symbol_count = 0;
// }

// // Look up a variable in the symbol table
// int vm_lookup_symbol(VM* vm, const char* name) {
//     for (int i = 0; i < vm->symbol_count; i++) {
//         if (strcmp(vm->symbol_table[i].name, name) == 0) {
//             return vm->symbol_table[i].value;
//         }
//     }
//     fprintf(stderr, "Error: Undefined variable '%s'\n", name);
//     exit(EXIT_FAILURE);
// }

// // Set a variable's value in the symbol table
// void vm_set_symbol(VM* vm, const char* name, int value) {
//     for (int i = 0; i < vm->symbol_count; i++) {
//         if (strcmp(vm->symbol_table[i].name, name) == 0) {
//             vm->symbol_table[i].value = value;
//             return;
//         }
//     }
//     if (vm->symbol_count >= MAX_SYMBOLS) {
//         fprintf(stderr, "Error: Symbol table full.\n");
//         exit(EXIT_FAILURE);
//     }
//     vm->symbol_table[vm->symbol_count].name = strdup(name);
//     vm->symbol_table[vm->symbol_count].value = value;
//     vm->symbol_count++;
// }

// // Evaluate the AST recursively
// int vm_evaluate(VM* vm, ASTNode* node) {
//     if (!node) return 0;

//     switch (node->type) {
//         case AST_PROGRAM: {
//             for (size_t i = 0; i < node->child_count; i++) {
//                 vm_evaluate(vm, node->children[i]);
//             }
//             return 0;
//         }
//         case AST_LITERAL:
//             return atoi(node->value);

//         case AST_VARIABLE:
//             return vm_lookup_symbol(vm, node->value);

//         case AST_ASSIGNMENT: {
//             int value = vm_evaluate(vm, node->right);
//             vm_set_symbol(vm, node->left->value, value);
//             printf("Assignment: %s = %d\n", node->left->value, value); // Debugging print
//             return value;
//         }
//         case AST_BINARY_OP: {
//             int left = vm_evaluate(vm, node->left);
//             int right = vm_evaluate(vm, node->right);
//             if (strcmp(node->value, "+") == 0) return left + right;
//             if (strcmp(node->value, "-") == 0) return left - right;
//             if (strcmp(node->value, "*") == 0) return left * right;
//             if (strcmp(node->value, "/") == 0) {
//                 if (right == 0) {
//                     fprintf(stderr, "Error: Division by zero.\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 return left / right;
//             }
//             fprintf(stderr, "Error: Unsupported operator '%s'\n", node->value);
//             exit(EXIT_FAILURE);
//         }
//         case AST_BLOCK: {
//             for (size_t i = 0; i < node->child_count; i++) {
//                 vm_evaluate(vm, node->children[i]);
//             }
//             return 0; 
//         }
//         case AST_FOR_LOOP: {
//             // Evaluate the range expression (start, end, step)
//             if (node->condition->type == AST_IN_KEYWORD && node->condition->right->type == AST_RANGE) {
//                 ASTNode* range_node = node->condition->right;

//                 int start = vm_evaluate(vm, range_node->children[0]);
//                 int end;
//                 int step = 1;

//                 if (range_node->child_count == 2) {
//                     end = vm_evaluate(vm, range_node->children[1]);
//                 } else if (range_node->child_count == 3) {
//                     end = vm_evaluate(vm, range_node->children[1]);
//                     step = vm_evaluate(vm, range_node->children[2]);
//                 } else {
//                     // Default to end if only one argument is provided
//                     end = start;
//                     start = 0; 
//                 }
//                 // Iterate and execute the body
//                 for (int i = start; i < end; i += step) {
//                     vm_set_symbol(vm, node->init->value, i); // Set loop variable
//                     vm_evaluate(vm, node->body);
//                 }
//             } else {
//                 fprintf(stderr, "Error: 'for' loop expects 'range' after 'in'.\n");
//                 exit(EXIT_FAILURE);
//             }
//             return 0;
//         }
//         case AST_WHILE_LOOP: {
//             while (vm_evaluate(vm, node->condition)) {
//                 vm_evaluate(vm, node->body);
//             }
//             return 0;
//         }
//         case AST_FUNCTION_CALL: {
//             if (strcmp(node->value, "print") == 0) {
//                 if (node->child_count != 1) {
//                     fprintf(stderr, "Error: 'print' expects 1 argument.\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 int arg = vm_evaluate(vm, node->children[0]);
//                 printf("%d\n", arg);
//                 return 0;
//             } else {
//                 fprintf(stderr, "Error: Unsupported function call '%s'\n", node->value);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         default:
//             fprintf(stderr, "Error: Unsupported AST node type %d\n", node->type);
//             exit(EXIT_FAILURE);
//     }
// }


// #include "vm.h"
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>

// // Initialize the VM
// void vm_init(VM* vm) {
//     vm->symbol_count = 0;
// }

// // Look up a variable in the symbol table
// int vm_lookup_symbol(VM* vm, const char* name) {
//     printf("[DEBUG] Looking up symbol: %s\n", name);
//     for (int i = 0; i < vm->symbol_count; i++) {
//         if (strcmp(vm->symbol_table[i].name, name) == 0) {
//             printf("[DEBUG] Symbol found: %s = %d\n", name, vm->symbol_table[i].value);
//             return vm->symbol_table[i].value;
//         }
//     }
//     fprintf(stderr, "Error: Undefined variable '%s'\n", name);
//     exit(EXIT_FAILURE);
// }

// // Set a variable's value in the symbol table
// void vm_set_symbol(VM* vm, const char* name, int value) {
//     printf("[DEBUG] Setting symbol: %s = %d\n", name, value);
//     for (int i = 0; i < vm->symbol_count; i++) {
//         if (strcmp(vm->symbol_table[i].name, name) == 0) {
//             vm->symbol_table[i].value = value;
//             printf("[DEBUG] Symbol updated: %s = %d\n", name, value);
//             return;
//         }
//     }
//     if (vm->symbol_count >= MAX_SYMBOLS) {
//         fprintf(stderr, "Error: Symbol table full.\n");
//         exit(EXIT_FAILURE);
//     }
//     vm->symbol_table[vm->symbol_count].name = strdup(name);
//     vm->symbol_table[vm->symbol_count].value = value;
//     vm->symbol_count++;
//     printf("[DEBUG] Symbol added: %s = %d\n", name, value);
// }

// // Evaluate the AST recursively
// int vm_evaluate(VM* vm, ASTNode* node) {
//     if (!node) {
//         printf("[DEBUG] Node is NULL, returning 0\n");
//         return 0;
//     }

//     printf("[DEBUG] Evaluating node: Type=%d, Value=%s\n", node->type, node->value ? node->value : "NULL");

//     switch (node->type) {
//         case AST_PROGRAM: {
//             printf("[DEBUG] Entering AST_PROGRAM\n");
//             int result = 0;
//             for (size_t i = 0; i < node->child_count; i++) {
//                 printf("[DEBUG] Evaluating child %zu of AST_PROGRAM\n", i);
//                 result = vm_evaluate(vm, node->children[i]);
//                 printf("[DEBUG] Result of child %zu evaluation: %d\n", i, result);
//             }
//             printf("[DEBUG] Exiting AST_PROGRAM, returning %d\n", result);
//             return result;
//         }
//         case AST_LITERAL: {
//             int value = atoi(node->value);
//             printf("[DEBUG] AST_LITERAL: Value=%d\n", value);
//             return value;
//         }
//         case AST_VARIABLE: {
//             int value = vm_lookup_symbol(vm, node->value);
//             printf("[DEBUG] AST_VARIABLE: %s = %d\n", node->value, value);
//             return value;
//         }
//         case AST_ASSIGNMENT: {
//             printf("[DEBUG] Entering AST_ASSIGNMENT\n");
//             int value = vm_evaluate(vm, node->right);
//             vm_set_symbol(vm, node->left->value, value);
//             printf("[DEBUG] AST_ASSIGNMENT: %s = %d\n", node->left->value, value);
//             return value;
//         }
//         case AST_BINARY_OP: {
//             printf("[DEBUG] Entering AST_BINARY_OP\n");
//             int left = vm_evaluate(vm, node->left);
//             int right = vm_evaluate(vm, node->right);
//             int result;
//             if (strcmp(node->value, "+") == 0) {
//                 result = left + right;
//             } else if (strcmp(node->value, "-") == 0) {
//                 result = left - right;
//             } else if (strcmp(node->value, "*") == 0) {
//                 result = left * right;
//             } else if (strcmp(node->value, "/") == 0) {
//                 if (right == 0) {
//                     fprintf(stderr, "Error: Division by zero.\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 result = left / right;
//             } else {
//                 fprintf(stderr, "Error: Unsupported operator '%s'\n", node->value);
//                 exit(EXIT_FAILURE);
//             }
//             printf("[DEBUG] AST_BINARY_OP: %d %s %d = %d\n", left, node->value, right, result);
//             return result;
//         }
//         case AST_BLOCK: {
//             printf("[DEBUG] Entering AST_BLOCK\n");
//             int result = 0;
//             for (size_t i = 0; i < node->child_count; i++) {
//                 printf("[DEBUG] Evaluating child %zu of AST_BLOCK\n", i);
//                 result = vm_evaluate(vm, node->children[i]);
//                 printf("[DEBUG] Result of child %zu evaluation: %d\n", i, result);
//             }
//             printf("[DEBUG] Exiting AST_BLOCK, returning %d\n", result);
//             return result;
//         }
//         case AST_FOR_LOOP: {
//             printf("[DEBUG] Entering AST_FOR_LOOP\n");
//             if (node->condition->type == AST_IN_KEYWORD && node->condition->right->type == AST_RANGE) {
//                 ASTNode* range_node = node->condition->right;
//                 int start = vm_evaluate(vm, range_node->children[0]);
//                 int end;
//                 int step = 1;
//                 if (range_node->child_count == 2) {
//                     end = vm_evaluate(vm, range_node->children[1]);
//                 } else if (range_node->child_count == 3) {
//                     end = vm_evaluate(vm, range_node->children[1]);
//                     step = vm_evaluate(vm, range_node->children[2]);
//                 } else {
//                     end = start;
//                     start = 0;
//                 }
//                 printf("[DEBUG] AST_FOR_LOOP: range(%d, %d, %d)\n", start, end, step);
//                 for (int i = start; i < end; i += step) {
//                     vm_set_symbol(vm, node->init->value, i);
//                     vm_evaluate(vm, node->body);
//                 }
//             } else {
//                 fprintf(stderr, "Error: 'for' loop expects 'range' after 'in'.\n");
//                 exit(EXIT_FAILURE);
//             }
//             printf("[DEBUG] Exiting AST_FOR_LOOP\n");
//             return 0;
//         }
//         case AST_WHILE_LOOP: {
//             printf("[DEBUG] Entering AST_WHILE_LOOP\n");
//             while (vm_evaluate(vm, node->condition)) {
//                 vm_evaluate(vm, node->body);
//             }
//             printf("[DEBUG] Exiting AST_WHILE_LOOP\n");
//             return 0;
//         }
//         case AST_FUNCTION_CALL: {
//             printf("[DEBUG] Entering AST_FUNCTION_CALL\n");
//             if (strcmp(node->value, "print") == 0) {
//                 if (node->child_count != 1) {
//                     fprintf(stderr, "Error: 'print' expects 1 argument.\n");
//                     exit(EXIT_FAILURE);
//                 }
//                 int arg = vm_evaluate(vm, node->children[0]);
//                 printf("[DEBUG] AST_FUNCTION_CALL: print(%d)\n", arg);
//                 printf("%d\n", arg);
//                 return 0;
//             } else {
//                 fprintf(stderr, "Error: Unsupported function call '%s'\n", node->value);
//                 exit(EXIT_FAILURE);
//             }
//         }
//         default:
//             fprintf(stderr, "Error: Unsupported AST node type %d\n", node->type);
//             exit(EXIT_FAILURE);
//     }
// }



#include "vm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Initialize the VM
void vm_init(VM* vm) {
    vm->symbol_count = 0;
}

// Look up a variable in the symbol table
int vm_lookup_symbol(VM* vm, const char* name) {
    printf("[DEBUG] Looking up symbol: %s\n", name);
    for (int i = 0; i < vm->symbol_count; i++) {
        if (strcmp(vm->symbol_table[i].name, name) == 0) {
            printf("[DEBUG] Symbol found: %s = %d\n", name, vm->symbol_table[i].value);
            return vm->symbol_table[i].value;
        }
    }
    fprintf(stderr, "Error: Undefined variable '%s'\n", name);
    exit(EXIT_FAILURE);
}

// Set a variable's value in the symbol table
void vm_set_symbol(VM* vm, const char* name, int value) {
    printf("[DEBUG] Setting symbol: %s = %d\n", name, value);
    for (int i = 0; i < vm->symbol_count; i++) {
        if (strcmp(vm->symbol_table[i].name, name) == 0) {
            vm->symbol_table[i].value = value;
            printf("[DEBUG] Symbol updated: %s = %d\n", name, value);
            return;
        }
    }
    if (vm->symbol_count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table full.\n");
        exit(EXIT_FAILURE);
    }
    vm->symbol_table[vm->symbol_count].name = strdup(name);
    vm->symbol_table[vm->symbol_count].value = value;
    vm->symbol_count++;
    printf("[DEBUG] Symbol added: %s = %d\n", name, value);
}

// Evaluate the AST recursively
int vm_evaluate(VM* vm, ASTNode* node) {
    if (!node) {
        printf("[DEBUG] Node is NULL, returning 0\n");
        return 0;
    }

    printf("[DEBUG] Evaluating node: Type=%d, Value=%s\n", node->type, node->value ? node->value : "NULL");

    switch (node->type) {
        case AST_PROGRAM: {
            printf("[DEBUG] Entering AST_PROGRAM\n");
            int result = 0;
            for (size_t i = 0; i < node->child_count; i++) {
                printf("[DEBUG] Evaluating child %zu of AST_PROGRAM\n", i);
                result = vm_evaluate(vm, node->children[i]);
                printf("[DEBUG] Result of child %zu evaluation: %d\n", i, result);
            }
            printf("[DEBUG] Exiting AST_PROGRAM, returning %d\n", result);
            return result;
        }
        case AST_LITERAL: {
            int value = atoi(node->value);
            printf("[DEBUG] AST_LITERAL: Value=%d\n", value);
            return value;
        }
        case AST_VARIABLE: {
            int value = vm_lookup_symbol(vm, node->value);
            printf("[DEBUG] AST_VARIABLE: %s = %d\n", node->value, value);
            return value;
        }
        case AST_ASSIGNMENT: {
            printf("[DEBUG] Entering AST_ASSIGNMENT\n");
            int value = vm_evaluate(vm, node->right);
            vm_set_symbol(vm, node->left->value, value);
            printf("[DEBUG] AST_ASSIGNMENT: %s = %d\n", node->left->value, value);
            return value;
        }
        case AST_BINARY_OP: {
            printf("[DEBUG] Entering AST_BINARY_OP\n");
            int left = vm_evaluate(vm, node->left);
            int right = vm_evaluate(vm, node->right);
            int result;
            if (strcmp(node->value, "+") == 0) {
                result = left + right;
            } else if (strcmp(node->value, "-") == 0) {
                result = left - right;
            } else if (strcmp(node->value, "*") == 0) {
                result = left * right;
            } else if (strcmp(node->value, "/") == 0) {
                if (right == 0) {
                    fprintf(stderr, "Error: Division by zero.\n");
                    exit(EXIT_FAILURE);
                }
                result = left / right;
            } else {
                fprintf(stderr, "Error: Unsupported operator '%s'\n", node->value);
                exit(EXIT_FAILURE);
            }
            printf("[DEBUG] AST_BINARY_OP: %d %s %d = %d\n", left, node->value, right, result);
            return result;
        }
        case AST_BLOCK: {
            printf("[DEBUG] Entering AST_BLOCK\n");
            int result = 0;
            for (size_t i = 0; i < node->child_count; i++) {
                printf("[DEBUG] Evaluating child %zu of AST_BLOCK\n", i);
                result = vm_evaluate(vm, node->children[i]);
                printf("[DEBUG] Result of child %zu evaluation: %d\n", i, result);
            }
            printf("[DEBUG] Exiting AST_BLOCK, returning %d\n", result);
            return result;
        }
        case AST_FOR_LOOP: {
            printf("[DEBUG] Entering AST_FOR_LOOP\n");
            if (node->condition->type == AST_IN_KEYWORD && node->condition->right->type == AST_RANGE) {
                ASTNode* range_node = node->condition->right;
                int start = vm_evaluate(vm, range_node->children[0]);
                int end;
                int step = 1;
                if (range_node->child_count == 2) {
                    end = vm_evaluate(vm, range_node->children[1]);
                } else if (range_node->child_count == 3) {
                    end = vm_evaluate(vm, range_node->children[1]);
                    step = vm_evaluate(vm, range_node->children[2]);
                } else {
                    end = start;
                    start = 0;
                }
                printf("[DEBUG] AST_FOR_LOOP: range(%d, %d, %d)\n", start, end, step);
                for (int i = start; i < end; i += step) {
                    vm_set_symbol(vm, node->init->value, i);
                    vm_evaluate(vm, node->body);
                }
            } else {
                fprintf(stderr, "Error: 'for' loop expects 'range' after 'in'.\n");
                exit(EXIT_FAILURE);
            }
            printf("[DEBUG] Exiting AST_FOR_LOOP\n");
            return 0;
        }
        case AST_WHILE_LOOP: {
            printf("[DEBUG] Entering AST_WHILE_LOOP\n");
            while (vm_evaluate(vm, node->condition)) {
                vm_evaluate(vm, node->body);
            }
            printf("[DEBUG] Exiting AST_WHILE_LOOP\n");
            return 0;
        }
        case AST_FUNCTION_CALL: {
            printf("[DEBUG] Entering AST_FUNCTION_CALL\n");
            if (strcmp(node->value, "print") == 0) {
                printf("[DEBUG] AST_FUNCTION_CALL: print\n");
                if (node->child_count != 1) {
                    fprintf(stderr, "Error: 'print' expects 1 argument.\n");
                    exit(EXIT_FAILURE);
                }
                // Handle different argument types (string or number)
                if (node->children[0]->type == AST_STRING) {
                    printf("%s\n", node->children[0]->value);
                } else {
                    int arg = vm_evaluate(vm, node->children[0]);
                    printf("%d\n", arg);
                }
                return 0;
            } else {
                fprintf(stderr, "Error: Unsupported function call '%s'\n", node->value);
                exit(EXIT_FAILURE);
            }
        }
        default:
            fprintf(stderr, "Error: Unsupported AST node type %d\n", node->type);
            exit(EXIT_FAILURE);
    }
}