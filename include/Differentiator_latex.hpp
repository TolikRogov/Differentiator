#pragma once

#include "Differentiator.hpp"

#define DIFF_LATEX_DIR_			"LaTex/"
#define DIFF_LATEX_FILE_		DIFF_LATEX_DIR_ "laboratory_work"
#define DIFF_TEX_EXTENSION_ 	".tex"
#define DIFF_PDF_EXTENSION_		".pdf"

#define OPEN_LATEX_PDF() system("(pdflatex " "-output-directory=" DIFF_LATEX_DIR_ " " DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_ "  -halt-on-error" " && " \
								OPEN DIFF_LATEX_FILE_ DIFF_PDF_EXTENSION_ ") | grep '!.*'");

#define LATEX_DUMP_START() {			 \
	tree_status = LatexDumpStart();		\
	TREE_ERROR_CHECK(tree_status);		\
}

#define LATEX_DUMP_FINISH() {			 \
	tree_status = LaTexDumpFinish();	\
	TREE_ERROR_CHECK(tree_status);		\
}

#define GET_TAYLOR(tree, id_name_table) {			 			 \
	tree_status = LaTexTaylorExpansion(tree, id_name_table);	\
	TREE_ERROR_CHECK(tree_status);								\
}

BinaryTreeStatusCode LatexDumpStart();
BinaryTreeStatusCode LaTexDumpFinish();
BinaryTreeStatusCode LaTexCreateDir();

BinaryTreeStatusCode PrintMathExpression(Node_t* node, FILE* tex_file, IdNameTable* id_name_table);
BinaryTreeStatusCode LaTexTaylorExpansion(Tree* tree, IdNameTable* id_name_table);
BinaryTreeStatusCode DrawGraph(Tree* tree, IdNameTable* id_name_table);
