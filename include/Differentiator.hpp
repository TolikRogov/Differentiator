#pragma once

#include "Tree.hpp"

#define BINARY_TREE_GRAPH_DUMP(tree, func_name, point) {	 																\
	tree_status = BinaryTreeGraphDump(tree, {.file = __FILE__, .line = __LINE__, .func = func_name, .pointer = point});		\
	TREE_ERROR_CHECK(tree_status);																	 					   \
}

#define COPY_SUBTREE(subtree_root, destination) {								 \
	(destination)->root = doCopySubtree(subtree_root);							\
	BINARY_TREE_GRAPH_DUMP(destination, "CopySubtree", (destination)->root);	\
}

#define LATEX_PRINT_TREE(tree) {		 \
	tree_status = LaTexPrintTree(tree);	\
	TREE_ERROR_CHECK(tree_status);		\
}

#define DIFFERENTIATION(function_tree, diff_tree) {				 	 \
	tree_status = LaTexDifferentiation(function_tree, diff_tree);	\
	TREE_ERROR_CHECK(tree_status);									\
}

enum SimplifierStatus {
	SIMPLIFY_IMPOSSIBLE = 0,
	SIMPLIFY_ACCESS		= 1,
};

//TODO:
// struct LexerItem {
// 	NodeType type;
// 	union Data_t data;
// };
//
// LexerItem array[] = {};

struct DumpLogInfo {
	const char* file;
	const size_t line;
	const char* func;
	const Node_t* pointer;
};

const size_t MAX_OPERATION_NAME_SIZE = 6;

BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info);

size_t NumberOfVarStatusUsingVariables();
BinaryTreeStatusCode VarNameTableSetDiff();

BinaryTreeStatusCode Calculator(Tree* tree);
Number_t Eval(Node_t* node);

Node_t* doCopySubtree(Node_t* node);
Node_t* doDifferentiation(Node_t* node);
size_t NumberOfVariablesInSubtree(Node_t* node);

BinaryTreeStatusCode Simplification(Node_t* node);
int ConvolutionConstant(Node_t* node, size_t* count_of_changes);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);

BinaryTreeStatusCode LaTexPrintTree(Tree* tree);
BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file);
BinaryTreeStatusCode LaTexSubtreeDifferential(Node_t* subtree_root, Node_t* diff_subtree_root);
BinaryTreeStatusCode LaTexDifferentiation(Tree* function_tree, Tree* diff_tree);
