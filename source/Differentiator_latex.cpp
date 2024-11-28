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

	if (!node)
		return TREE_NO_ERROR;

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case AMOUNT_OF_OPERATIONS: break;
				case LOG:
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
				case LN:
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
			fprintf(tex_file, "%lg", node->data.val_num);
			break;
		}
		case VAR: {
			fprintf(tex_file, "%s", VarNameTableGetSymbol(node->data.val_var));
			break;
		}
		case UNW: return TREE_NO_ERROR;
		default:  return TREE_INVALID_TYPE;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintMathExpression(Node_t* node, FILE* tex_file) {

	if (!node)
		return TREE_NO_ERROR;

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__);

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case AMOUNT_OF_OPERATIONS: break;
				case LOG: {
					TEX_PRINTF("(ln(");
					if (node->right) PrintMathExpression(node->right, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					TEX_PRINTF(")/ln(");
					if (node->left) PrintMathExpression(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF("))");

					break;
				}
				case POW:
				case ADD:
				case MUL:
				case SUB:
				case DIV: {
					TEX_PRINTF("((");
					if (node->left) PrintMathExpression(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					TEX_PRINTF(")%s(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->right) PrintMathExpression(node->right, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF("))");

					break;
				}
				case LN:
				case SQRT: {
					TEX_PRINTF("(%s(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->left) PrintMathExpression(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF("))");

					break;
				}
				case COS:
				case SIN: {
					TEX_PRINTF("(%s(deg(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->left) PrintMathExpression(node->left, tex_file);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF(")))");

					break;
				}
				case INVALID_OPERATION:
				default: TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);
			}
			break;
		}
		case NUM: {
			TEX_PRINTF("(%lg)", node->data.val_num);
			break;
		}
		case VAR: {
			TEX_PRINTF("(%s)", VarNameTableGetSymbol(node->data.val_var));
			break;
		}
		case UNW: return TREE_NO_ERROR;
		default:  return TREE_INVALID_TYPE;
	}

#undef TEX_PRINTF

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode DrawGraph(Tree* tree) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	for (size_t i = 0, j = 0; i < AMOUNT_OF_VARIABLES; i++) {
		if (var_name_table[i].status == VAR_STATUS_USING)
			j++;
		if (j > 1)
			return TREE_NO_ERROR;
	}

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__);
#define ABS_X 6
#define ABS_Y 1

	TEX_PRINTF("\\begin{figure}[h]\n");
	TEX_PRINTF("\\centering\n");
	TEX_PRINTF("\\begin{tikzpicture}\n");
	TEX_PRINTF("\\begin{axis} [\n");
    TEX_PRINTF("\tlegend pos = north west,\n");
	TEX_PRINTF("\txlabel = {$x$},\n");
	TEX_PRINTF("\tylabel = {$f$},\n");
	TEX_PRINTF("\txmin = %d,\n", (-1) * ABS_X);
	TEX_PRINTF("\txmax = %d,\n", ABS_X);
	TEX_PRINTF("\trestrict y to domain=-30:30,\n");
    TEX_PRINTF("\tgrid = major,\n");
	TEX_PRINTF("\tenlargelimits=true,\n");
	TEX_PRINTF("]\n");
	TEX_PRINTF("\\legend{\n");
	TEX_PRINTF("\t$");
	PrintExpressionTree(tree->root, tex_file);
	TEX_PRINTF("$\n}\n");
	TEX_PRINTF("\\addplot[blue, samples=750]{");
	PrintMathExpression(tree->root, tex_file);
	TEX_PRINTF("};\n");
	TEX_PRINTF("\\end{axis}\n");
	TEX_PRINTF("\\end{tikzpicture}\n");
	TEX_PRINTF("\\caption{График функции}\n");
	TEX_PRINTF("\\end{figure}\n");
	TEX_PRINTF("\\newpage\n");

#undef TEX_PRINTF
#undef ABS_X

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

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
	TEX_PRINTF("\\usepackage{amsthm,amssymb}\n");
	TEX_PRINTF("\\usepackage{mathtext}\n");
	TEX_PRINTF("\\usepackage[T1]{fontenc}\n");
	TEX_PRINTF("\\usepackage[utf8]{inputenc}\n");
	TEX_PRINTF("\\usepackage{pgfplots}\n");
	TEX_PRINTF("\\pgfplotsset{compat=1.9}\n");
	TEX_PRINTF("\\usepackage[english,russian]{babel}\n");
	TEX_PRINTF("\\title{Лабораторная работа минимум на отл 10!}\n");
	TEX_PRINTF("\\author{Рогов Анатолий}\n");
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
