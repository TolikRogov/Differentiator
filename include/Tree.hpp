#pragma once

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <time.h>
#include "Differentiator_errors.hpp"
#include "Defines.hpp"

typedef double Number_t;

#include "Operations.hpp"

#define INIT_TREE(tree) Tree tree = {.info = {.name = #tree, .file_name = __FILE__, .line = __LINE__}};
#define INIT_VAR_NAME_TABLE(var_name_table) VariableNameTable var_name_table = {};

#define TREE_HTML_DUMP_START() {				 \
	tree_status = BinaryTreeHtmlDumpStart();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#define TREE_HTML_DUMP_FINISH() {				 \
	tree_status = BinaryTreeHtmlDumpFinish();	\
	TREE_ERROR_CHECK(tree_status);				\
}

#ifdef PRINT_DEBUG
	#define TREE_CTOR(tree) {						 		 \
		tree_status = TreeCtor(tree);						\
		TREE_ERROR_CHECK(tree_status);						\
		BINARY_TREE_GRAPH_DUMP(tree, "TreeCtor", NULL);		\
	}
#else
	#define TREE_CTOR(tree) {						 		 \
		tree_status = TreeCtor(tree);						\
		TREE_ERROR_CHECK(tree_status);						\
	}
#endif

#define TREE_DTOR(tree) {						 	 	 \
	LATEX_DUMP_FINISH();								\
	TREE_HTML_DUMP_FINISH();							\
	tree_status = TreeDtor((tree)->root);				\
	TREE_ERROR_CHECK(tree_status);						\
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
	size_t val_var;
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

struct Tree {
	Node_t* root;
	TreeLogInfo info;
	size_t diff_number;
};

const Number_t UNKNOWN_WHAT = -666;

BinaryTreeStatusCode TreeCtor(Tree* tree);
BinaryTreeStatusCode TreeDtor(Node_t* node);

BinaryTreeStatusCode BinaryTreeHtmlDumpStart();
BinaryTreeStatusCode BinaryTreeHtmlDumpFinish();

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root);
Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent);
Node_t* FindTreeRoot(Node_t* node);
BinaryTreeStatusCode SetNodeValue(Node_t* node, Data_t data);
BinaryTreeStatusCode NodePrintData(Node_t* node);
BinaryTreeStatusCode ReplaceUnknownWhat(Node_t* node, Data_t data, NodeType type);

const char* OpNameTableGetMathSymbol(OpNum op_number);
const char* OpNameTableGetTexSymbol(OpNum op_number);
OpNum OpNameTableFindOperation(const char* string);
