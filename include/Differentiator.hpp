#pragma once

#include <stdlib.h>
#include <dirent.h>
#include "Differentiator_errors.hpp"
#include "Defines.hpp"

typedef int Data_t;

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
	BINARY_TREE_GRAPH_DUMP(tree, "TreeDtor", NULL); 	\
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

struct Node_t {
	NodeType type;
	Data_t data;
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

const Data_t UNKNOWN_WHAT = -666;
const Data_t NONE 		  = -666;

BinaryTreeStatusCode TreeCtor(Tree* tree);
BinaryTreeStatusCode TreeDtor(Node_t* node);

BinaryTreeStatusCode BinaryTreeHtmlDumpStart();
BinaryTreeStatusCode BinaryTreeGraphDump(Tree* tree, DumpLogInfo dump_info);
BinaryTreeStatusCode BinaryTreeHtmlDumpFinish();

Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent);
Node_t* FindTreeRoot(Node_t* node);

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root);
