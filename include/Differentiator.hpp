#pragma once

#include "Identifiers.hpp"

#define BINARY_TREE_GRAPH_DUMP(tree, func_name, point, id_name_table) {	 																	\
	tree_status = BinaryTreeGraphDump(tree, {.file = __FILE__, .line = __LINE__, .func = func_name, .pointer = point}, id_name_table);		\
	TREE_ERROR_CHECK(tree_status);																	 					   				   \
}

#define COPY_SUBTREE(subtree_root, destination) {												 \
	(destination)->root = doCopySubtree(subtree_root);											\
	BINARY_TREE_GRAPH_DUMP(destination, "CopySubtree", (destination)->root, id_name_table);		\
}

#define LATEX_PRINT_TREE(tree, id_name_table) {			 \
	tree_status = LaTexPrintTree(tree, id_name_table);	\
	TREE_ERROR_CHECK(tree_status);						\
}

#define DIFFERENTIATION(function_tree, diff_tree, id_name_table, old_var_diff) {				 \
	tree_status = LaTexDifferentiation(function_tree, diff_tree, id_name_table, old_var_diff);	\
	TREE_ERROR_CHECK(tree_status);																\
}

enum SimplifierStatus {
	SIMPLIFY_IMPOSSIBLE = 0,
	SIMPLIFY_ACCESS		= 1,
};

struct DumpLogInfo {
	const char* file;
	const size_t line;
	const char* func;
	const Node_t* pointer;
};

BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info, IdNameTable* id_name_table);

BinaryTreeStatusCode Calculator(Tree* tree, IdNameTable* id_name_table);
Number_t Eval(Node_t* node, IdNameTable* id_name_table);

Node_t* doCopySubtree(Node_t* node);
Node_t* doDifferentiation(Node_t* node, IdNameTable* id_name_table, int old_var_diff);
size_t NumberOfVariablesInSubtree(Node_t* node);

BinaryTreeStatusCode Simplification(Node_t* node, IdNameTable* id_name_table);
int ConvolutionConstant(Node_t* node, size_t* count_of_changes, IdNameTable* id_name_table);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);

BinaryTreeStatusCode LaTexPrintTree(Tree* tree, IdNameTable* id_name_table);
BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file, IdNameTable* id_name_table);
BinaryTreeStatusCode LaTexSubtreeDifferential(Node_t* subtree_root, Node_t* diff_subtree_root, IdNameTable* id_name_table);
BinaryTreeStatusCode LaTexDifferentiation(Tree* function_tree, Tree* diff_tree, IdNameTable* id_name_table, int old_var_diff);
