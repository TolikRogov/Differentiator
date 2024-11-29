#pragma once

#include "Differentiator.hpp"

#define DIFF_DATA_DIR_			"data/"
#define DIFF_EXPRESSION_FILE_	DIFF_DATA_DIR_ "expression.txt"

#define READ_EXPRESSION(tree) {			 	 \
	tree_status = OriginalFunction(tree);	\
	TREE_ERROR_CHECK(tree_status);			\
}

BinaryTreeStatusCode ReadExpression(Tree* tree);
BinaryTreeStatusCode OriginalFunction(Tree* tree);
BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth);
