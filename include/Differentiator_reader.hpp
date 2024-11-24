#pragma once

#include "Differentiator.hpp"

#define DIFF_DATA_DIR_			"data/"
#define DIFF_EXPRESSION_FILE_	DIFF_DATA_DIR_ "expression.txt"

#define INFIX_EXPRESSION_READER(tree) {			 		 		 \
	tree_status = InfixReader(tree);					 		 \
	TREE_ERROR_CHECK(tree_status);								\
	BINARY_TREE_GRAPH_DUMP(tree, "ExpressionReader", NULL); 	\
}

BinaryTreeStatusCode InfixReader(Tree* tree);
BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth);
