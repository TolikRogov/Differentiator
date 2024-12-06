#pragma once

#include "Tree.hpp"
#include "Variables.hpp"

#define BINARY_TREE_GRAPH_DUMP(tree, func_name, point, var_name_table) {	 																\
	tree_status = BinaryTreeGraphDump(tree, {.file = __FILE__, .line = __LINE__, .func = func_name, .pointer = point}, var_name_table);		\
	TREE_ERROR_CHECK(tree_status);																	 					   				   \
}

#define COPY_SUBTREE(subtree_root, destination) {												 \
	(destination)->root = doCopySubtree(subtree_root);											\
	BINARY_TREE_GRAPH_DUMP(destination, "CopySubtree", (destination)->root, var_name_table);	\
}

#define LATEX_PRINT_TREE(tree, var_name_table) {		 \
	tree_status = LaTexPrintTree(tree, var_name_table);	\
	TREE_ERROR_CHECK(tree_status);						\
}

#define DIFFERENTIATION(function_tree, diff_tree, var_name_table) {				 	 \
	tree_status = LaTexDifferentiation(function_tree, diff_tree, var_name_table);	\
	TREE_ERROR_CHECK(tree_status);													\
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

BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info, VariableNameTable* var_name_table);

BinaryTreeStatusCode Calculator(Tree* tree, VariableNameTable* var_name_table);
Number_t Eval(Node_t* node, VariableNameTable* var_name_table);

Node_t* doCopySubtree(Node_t* node);
Node_t* doDifferentiation(Node_t* node, VariableNameTable* var_name_table);
size_t NumberOfVariablesInSubtree(Node_t* node);

BinaryTreeStatusCode Simplification(Node_t* node, VariableNameTable* var_name_table);
int ConvolutionConstant(Node_t* node, size_t* count_of_changes, VariableNameTable* var_name_table);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);

BinaryTreeStatusCode LaTexPrintTree(Tree* tree, VariableNameTable* var_name_table);
BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file, VariableNameTable* var_name_table);
BinaryTreeStatusCode LaTexSubtreeDifferential(Node_t* subtree_root, Node_t* diff_subtree_root, VariableNameTable* var_name_table);
BinaryTreeStatusCode LaTexDifferentiation(Tree* function_tree, Tree* diff_tree, VariableNameTable* var_name_table);
