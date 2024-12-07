#pragma once

#include "Differentiator_lexer.hpp"

#define DIFF_DATA_DIR_			"data/"
#define DIFF_EXPRESSION_FILE_	DIFF_DATA_DIR_ "expression.txt"

#define READ_EXPRESSION(tree, id_name_table, lexer) {		     \
	tree_status = OriginalFunction(tree, id_name_table, lexer);	\
	TREE_ERROR_CHECK(tree_status);							    \
}

BinaryTreeStatusCode ReadExpression(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
BinaryTreeStatusCode OriginalFunction(Tree* tree, IdNameTable* id_name_table, Lexer* lexer);
BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth, IdNameTable* id_name_table);
