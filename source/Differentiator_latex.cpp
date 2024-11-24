#include "Differentiator_latex.hpp"

BinaryTreeStatusCode LaTexPrintTree(Tree* tree) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	fprintf(tex_file, "$");
	PrintExpressionTree(tree->root, tex_file);
	fprintf(tex_file, "$\n");

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file) {

	#include "Operations"

	if (!node)
		return TREE_NO_ERROR;

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case AMOUNT_OF_OPERATIONS: break;
				case DIV: {
					fprintf(tex_file, "(\\%s{", OpNameTableGetTexSymbol(node->data.val_op));
					if (node->left) PrintExpressionTree(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					fprintf(tex_file, "}{");
					if (node->right) PrintExpressionTree(node->right, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					fprintf(tex_file, "})");

					break;
				}
				case POW:
				case ADD:
				case MUL:
				case SUB: {
					fprintf(tex_file, "({");
					if (node->left) PrintExpressionTree(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					fprintf(tex_file, "}%s{", OpNameTableGetTexSymbol(node->data.val_op));
					if (node->right) PrintExpressionTree(node->right, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					fprintf(tex_file, "})");

					break;
				}
				case COS:
				case SQRT:
				case SIN: {
					fprintf(tex_file, "(\\%s{", OpNameTableGetTexSymbol(node->data.val_op));
					if (node->left) PrintExpressionTree(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					fprintf(tex_file, "})");

					break;
				}
				case INVALID_OPERATION:
				default: TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);
			}
			break;
		}
		case NUM: {
			fprintf(tex_file, "(%lg)", node->data.val_num);
			break;
		}
		case VAR: {
			fprintf(tex_file, "(%s)", VarNameTableGetSymbol(node->data.val_var));
			break;
		}
		case UNW: return TREE_NO_ERROR;
		default:  return TREE_INVALID_TYPE;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LatexDumpStart() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree_status = LaTexCreateDir();
	TREE_ERROR_CHECK(tree_status);

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "w");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__);

	TEX_PRINTF("\\documentclass[12pt, letterpaper]{article}\n");
	TEX_PRINTF("\\title{The best laboratory work!}\n");
	TEX_PRINTF("\\author{Rogov Anatoliy}\n");
	TEX_PRINTF("\\date{\\today}\n");
	TEX_PRINTF("\\begin{document}\n");
	TEX_PRINTF("\\maketitle\n");
	TEX_PRINTF("\\newpage\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexDumpFinish() {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__);

	TEX_PRINTF("\\end{document}\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	system("pdflatex " "-output-directory=" DIFF_LATEX_DIR_ " " DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_ "  -halt-on-error | grep '!.*'");

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexCreateDir() {

	DIR* latex_dir = opendir(DIFF_LATEX_DIR_);
	if (latex_dir) {
		if (closedir(latex_dir))
			TREE_ERROR_CHECK(TREE_DIR_CLOSE_ERROR);
		system("rm -rf " DIFF_LATEX_DIR_);
	}
	system("mkdir " DIFF_LATEX_DIR_);

	return TREE_NO_ERROR;
}
