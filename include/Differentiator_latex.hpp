#pragma once

#include "Differentiator.hpp"

#define DIFF_LATEX_DIR_			"LaTex/"
#define DIFF_LATEX_FILE_		DIFF_LATEX_DIR_ "laboratory_work.tex"

#define LATEX_DUMP_START() {			 \
	tree_status = LatexDumpStart();		\
	TREE_ERROR_CHECK(tree_status);		\
}

#define LATEX_DUMP_FINISH() {			 \
	tree_status = LaTexDumpFinish();	\
	TREE_ERROR_CHECK(tree_status);		\
}

#define LATEX_PRINT_TREE(tree) {		 \
	tree_status = LaTexPrintTree(tree);	\
	TREE_ERROR_CHECK(tree_status);		\
}

BinaryTreeStatusCode LatexDumpStart();
BinaryTreeStatusCode LaTexDumpFinish();
BinaryTreeStatusCode LaTexCreateDir();

BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file);
BinaryTreeStatusCode LaTexPrintTree(Tree* tree);
