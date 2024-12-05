#pragma once

#include "Differentiator.hpp"

#define DIFF_DATA_DIR_			"data/"
#define DIFF_EXPRESSION_FILE_	DIFF_DATA_DIR_ "expression.txt"

#define READ_EXPRESSION(tree, var_name_table) {			 	 \
	tree_status = OriginalFunction(tree, var_name_table);	\
	TREE_ERROR_CHECK(tree_status);							\
}

BinaryTreeStatusCode ReadExpression(Tree* tree, VariableNameTable* var_name_table);
BinaryTreeStatusCode OriginalFunction(Tree* tree, VariableNameTable* var_name_table);
BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth, VariableNameTable* var_name_table);
