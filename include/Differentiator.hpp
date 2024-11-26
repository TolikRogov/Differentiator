#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include "Differentiator_errors.hpp"
#include "Defines.hpp"

typedef double Number_t;

#include "Variables.hpp"
#include "Operations.hpp"

#define INIT_TREE(tree) Tree tree = {.info = {.name = #tree, .file_name = __FILE__, .line = __LINE__}};

#define BINARY_TREE_GRAPH_DUMP(tree, func_name, point) {	 																\
	tree_status = BinaryTreeGraphDump(tree, {.file = __FILE__, .line = __LINE__, .func = func_name, .pointer = point});		\
	TREE_ERROR_CHECK(tree_status);																	 					   \
}

#define TREE_HTML_DUMP_START() {				 \
	tree_status = BinaryTreeHtmlDumpStart();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#define TREE_HTML_DUMP_FINISH() {				 \
	tree_status = BinaryTreeHtmlDumpFinish();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#define TREE_CTOR(tree) {						 		 \
	tree_status = TreeCtor(tree);						\
	TREE_ERROR_CHECK(tree_status);						\
	BINARY_TREE_GRAPH_DUMP(tree, "TreeCtor", NULL);		\
}

#define TREE_DTOR(tree) {						 	 	 \
	TREE_HTML_DUMP_FINISH();							\
	tree_status = TreeDtor((tree)->root);				\
	TREE_ERROR_CHECK(tree_status);						\
}

#define COPY_SUBTREE(subtree_root, destination) {								 \
	(destination)->root = doCopySubtree(subtree_root);							\
	BINARY_TREE_GRAPH_DUMP(destination, "CopySubtree", (destination)->root);	\
}

#define EXPRESSION_DIFFERENTIATION(expression_tree, diff_tree) {						 \
	(diff_tree)->root = Differentiation((expression_tree)->root);						\
	BINARY_TREE_GRAPH_DUMP(diff_tree, "ExpressionDifferentiation", (diff_tree)->root);	\
	Simplification(diff_tree);													 		 \
	BINARY_TREE_GRAPH_DUMP(diff_tree, "Simplification", (diff_tree)->root);				 \
	LATEX_PRINT_TREE(diff_tree);														\
}

enum TreeDumpCheck {
	TREE_FIRST_RUN,
	TREE_SECOND_RUN,
	TREE_ALREADY_ON,
};

enum NodeType {
	UNW,
	NUM,
	VAR,
	OP
};

union Data_t {
	Number_t val_num;
	OpNum val_op;
	VarNum val_var;
};

struct Node_t {
	NodeType type;
	union Data_t data;
	Node_t* left;
	Node_t* right;
	Node_t* parent;
};

struct TreeLogInfo {
	const char* name;
	const char* file_name;
	const size_t line;
};

struct DumpLogInfo {
	const char* file;
	const size_t line;
	const char* func;
	const Node_t* pointer;
};

struct Tree {
	Node_t* root;
	TreeLogInfo info;
};

const Number_t UNKNOWN_WHAT = -666;
const Number_t NONE 		  = -666;
const size_t MAX_OPERATION_NAME_SIZE = 6;

BinaryTreeStatusCode TreeCtor(Tree* tree);
BinaryTreeStatusCode TreeDtor(Node_t* node);

BinaryTreeStatusCode BinaryTreeHtmlDumpStart();
BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info);
BinaryTreeStatusCode BinaryTreeHtmlDumpFinish();

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root);
Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent);
Node_t* FindTreeRoot(Node_t* node);
BinaryTreeStatusCode NodePrintData(Node_t* node);
BinaryTreeStatusCode ReplaceUnknownWhat(Node_t* node, Data_t data, NodeType type);

const char* OpNameTableGetMathSymbol(OpNum op_number);
const char* OpNameTableGetTexSymbol(OpNum op_number);
OpNum OpNameTableFindOperation(const char* string);

const char* VarNameTableGetSymbol(VarNum number);
VarNum VarNameTableFindVariable(const char* string);
Number_t VarNameTableGetValue(VarNum number);
BinaryTreeStatusCode ResetVariables();

BinaryTreeStatusCode Calculator(Tree* tree);
Number_t Eval(Node_t* node);

Node_t* doCopySubtree(Node_t* node);
Node_t* Differentiation(Node_t* node);
size_t NumberOfVariablesInSubtree(Node_t* node);

BinaryTreeStatusCode Simplification(Tree* tree);
int ConvolutionConstant(Node_t* node, size_t* count_of_changes);
int TrivialTransformations(Node_t* node, size_t* count_of_changes);
