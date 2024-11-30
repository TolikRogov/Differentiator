#include "Differentiator_latex.hpp"

BinaryTreeStatusCode LaTexDifferentiation(Tree* function_tree, Tree* diff_tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	diff_tree->diff_number = function_tree->diff_number + 1;

	TEX_PRINTF("\\section{Производная (%zu) по переменной \"%s\"}\n", diff_tree->diff_number, VarNameTableGetStateVariable());
	TEX_PRINTF("\\renewcommand{\\thesubsection}{\\arabic{subsection}}\n"
			   "\\titleformat{\\subsection}{\\normalfont\\bfseries}{}{0em}{#1\\ \\thesubsection}\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	diff_tree->root = doDifferentiation(function_tree->root);
	BINARY_TREE_GRAPH_DUMP(diff_tree, "ExpressionDifferentiation", diff_tree->root);

	LATEX_PRINT_TREE(diff_tree);

	tree_status = DrawGraph(diff_tree);
	TREE_ERROR_CHECK(tree_status);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexSubtreeDifferential(Node_t* subtree_root, Node_t* diff_subtree_root) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\subsection{Шаг}\n"
			   "\\hfil $");
	PrintExpressionTree(subtree_root, tex_file);
	TEX_PRINTF("^{\\prime} = ");
	PrintExpressionTree(diff_subtree_root, tex_file);
	TEX_PRINTF("$\n\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexPrintTree(Tree* tree) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\titleformat{\\subsection}{\\normalfont\\bfseries}{}{0em}{\\thesubsection \\ #1\\ }\n"
			   "\\subsection{Результат}\n");

	TEX_PRINTF("\\hfil $f^{(%zu)}(", tree->diff_number);
	for (size_t i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		if (var_name_table[i].state == VAR_DIFF_STATUS_VAR) {
				TEX_PRINTF("%s", var_name_table[i].symbol);
				break;
		}
	}
	TEX_PRINTF(") = ");
	PrintExpressionTree(tree->root, tex_file);
	TEX_PRINTF("$\n");

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#undef TEX_PRINTF

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
				case EXP:
				case SIN: {
					fprintf(tex_file, "(%s{", OpNameTableGetTexSymbol(node->data.val_op));
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
				case EXP:
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

	TEX_PRINTF("\\begin{figure}[h]\n"
			   "\\centering\n"
			   "\\begin{tikzpicture}\n"
			   "\\begin{axis} [\n"
    		   "\tlegend pos = north west,\n"
			   "\txlabel = {$x$},\n"
			   "\tylabel = {$f$},\n"
			   "\twidth = 300,\n"
			   "\theight = 300,\n"
			   "\trestrict y to domain=-30:30,\n"
    		   "\tgrid = major,\n"
			   "\tenlargelimits=true,\n"
			   "]\n"
			   "\\legend{\n"
			   "\t$f^{(%zu)}$\n}\n", tree->diff_number);
	TEX_PRINTF("\\addplot[blue, samples=750]{");
	PrintMathExpression(tree->root, tex_file);
	TEX_PRINTF("};\n"
			   "\\end{axis}\n"
			   "\\end{tikzpicture}\n"
			   "\\caption{График функции}\n"
			   "\\end{figure}\n");

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

	TEX_PRINTF("\\documentclass[12pt, letterpaper]{article}\n"
				"\\usepackage{amsthm,amssymb}\n"
				"\\usepackage{mathtext}\n"
				"\\usepackage[T1]{fontenc}\n"
				"\\usepackage[utf8]{inputenc}\n"
				"\\usepackage{pgfplots}\n"
				"\\usepackage[explicit]{titlesec}\n"
				"\\pgfplotsset{compat=1.9}\n"
				"\\usepackage[english,russian]{babel}\n"
				"\\begin{document}\n"
				"\\begin{titlepage}\n"
   				"\\begin{center}\n"
				"\\vspace*{1cm}\n"
				"\\textbf{МОСКОВСКИЙ ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ (НАЦИОНАЛЬНЫЙ ИССЛЕДОВАТЕЛЬСКИЙ УНИВЕРСИТЕТ)}\\\\\n"
				"\\vspace{0.5cm} Физтех-школа Радиотехники и компьютерных технологий\\\\\n"
				"\\vspace{5cm} \\LARGE{Исследование функции}\n"
				"\\vfill\n"
				"\\large{\\textbf{Рогов Анатолий Б01-406}} \\\\\n"
				"\\large \\today\n"
				"\\vspace{0.8cm}\n"
				"\\end{center}\n"
				"\\end{titlepage}\n");

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
