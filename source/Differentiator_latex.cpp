#include "Differentiator_latex.hpp"

BinaryTreeStatusCode LaTexTaylorExpansion(Tree* tree, IdNameTable* id_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	if (id_name_table->size != 1) {

		INIT_TREE(diff_tree);
		diff_tree.root = tree->root;
		diff_tree.diff_number = tree->diff_number;
		DIFFERENTIATION(tree, &diff_tree, id_name_table, 0);

		return TREE_NO_ERROR;
	}

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	printf("-------------------------------------------------------------------------------------------------\n");
	printf("\t\t\t" BLUE("TAYLOR EXPANSION")"\n");
	printf("-------------------------------------------------------------------------------------------------\n");

	Number_t point = 0;
	printf(BLUE("Enter the value of the point at which you want to obtain the Taylor expansion:")" ");

	while (!scanf("%lg", &point) && getchar()) { printf(RED("ERROR - Try again!")"\n"); printf(YELLOW("Double number:")" "); }
	getchar();

	int max_degree = 0;
	printf(BLUE("Enter maximum degree of Taylor decomposition:")" ");

	while (!scanf("%d", &max_degree) && getchar()) { printf(RED("ERROR - Try again!")"\n"); printf(YELLOW("Integer number:")" "); }
	getchar();

	if (max_degree < 0)
		TREE_ERROR_CHECK(TREE_WRONG_TAYLOR_DEGREE);

	Identifier used_variable = id_name_table->data[id_name_table->size - 1];
	TEX_PRINTF("\\chapter{Разложение по формуле Тейлора до \\\\ $o((");
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF(" - ");
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF("_0)^{%d})$ в точке $", max_degree);
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF("_0 = %lg$}\n", point);
	id_name_table->data[used_variable.num].value = point;

	TEX_PRINTF("\\begin{center} $f^{0}(");
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF(") = ");
	PrintExpressionTree(tree->root, tex_file, id_name_table);
	TEX_PRINTF("$ \\end{center}\n");

	Number_t* members = (Number_t*)calloc((size_t)(max_degree + 1), sizeof(Number_t));
	if (!members)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	members[0] = Eval(tree->root, id_name_table);
	TEX_PRINTF("\\begin{center} $f^{0}(%lg) = %lg$ \\end{center}\n", point, members[0]);

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	INIT_TREE(diff_tree);
	diff_tree.root = tree->root;
	diff_tree.diff_number = tree->diff_number;
	for (size_t i = 0; i < (size_t)max_degree; i++) {
		DIFFERENTIATION(&diff_tree, &diff_tree, id_name_table, 1);

		tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
		if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

		members[i + 1] = Eval(diff_tree.root, id_name_table);
		TEX_PRINTF("\\begin{center} $f^{%zu}(%lg) = %lg$ \\end{center}\n", diff_tree.diff_number, point, members[i + 1]);

		if (fclose(tex_file))
			TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);
	}

	tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	TEX_PRINTF("\\section{Ответ}\n");
	TEX_PRINTF("\\begin{center} $f(");
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF(") = %lg", members[0]);
	for (size_t i = 0; i < (size_t)max_degree; i++) {
		TEX_PRINTF(" + \\frac{%lg}{%zu!}\\cdot(", members[i + 1], i + 1);
		PrintNString(tex_file, used_variable.string, used_variable.length);
		TEX_PRINTF(" - %lg)^{%zu}", point, i + 1);
	}

	TEX_PRINTF(" + o((");
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF(" - %lg)^{%d})$ \\end{center}\n", point, max_degree);

	TEX_PRINTF("\\section{График членов Тейлора}\n");

	TEX_PRINTF("\\pgfplotsset{\n"
			   "\tcolormap={bright}{\n");
	for (size_t i = 0; i < (size_t)(max_degree + 1); i++) {
		TEX_PRINTF("\trgb255=(%d,%d,%d)", rand()%256, rand()%256, rand()%256);
		if (i != (size_t)max_degree)
			TEX_PRINTF(",\n");
		else
			TEX_PRINTF("\n");
	}
	TEX_PRINTF("}}\n");

	TEX_PRINTF("\\begin{figure}[h]\n"
			   "\\centering\n"
			   "\\begin{tikzpicture}\n"
			   "\\begin{axis} [\n"
			   "\tlegend pos = north west,\n"
			   "\txlabel = {$");
	PrintNString(tex_file, id_name_table->data[id_name_table->size - 1].string, id_name_table->data[id_name_table->size - 1].length);
	TEX_PRINTF("$},\n"
			   "\tylabel = {$f(");
	PrintNString(tex_file, id_name_table->data[id_name_table->size - 1].string, id_name_table->data[id_name_table->size - 1].length);
	TEX_PRINTF(")$},\n"
			   "\twidth = 300,\n"
			   "\theight = 300,\n"
			   "\tgrid = major,\n"
			   "\tenlargelimits=true,\n"
			   "\tcolormap name=bright,\n"
			   "\tcycle list={[of colormap]},\n"
			   "\tevery axis plot/.append style={mark=none,ultra thick}\n"
			   "]\n");

	for (size_t i = 0; i < (size_t)(max_degree + 1); i++) {
		TEX_PRINTF("\\addplot+ [samples=750]{%lg", members[0]);
		for (size_t j = 0; j < i; j++) {
			TEX_PRINTF(" + (%lg/%d) * (", members[j + 1], Factorial(int(j + 1)));
			PrintNString(tex_file, "x", 1);
			TEX_PRINTF(" - %lg) ^ %zu", point, j + 1);
		}
		TEX_PRINTF("};\n");
	}

	TEX_PRINTF("\\legend{\n");
	for (size_t i = 0; i < diff_tree.diff_number + 1; i++) {
		TEX_PRINTF("\t$o({");
		PrintNString(tex_file, id_name_table->data[id_name_table->size - 1].string, id_name_table->data[id_name_table->size - 1].length);
		TEX_PRINTF("^{%zu}})$", i);
		if (i != diff_tree.diff_number)
			TEX_PRINTF(",\n");
		else
			TEX_PRINTF("\n");
	}
	TEX_PRINTF("}\n");

	TEX_PRINTF("\\end{axis}\n"
			   "\\end{tikzpicture}\n"
			   "\\caption{График членов разложения}\n"
			   "\\end{figure}\n");

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#undef TEX_PRINTF

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexDifferentiation(Tree* function_tree, Tree* diff_tree, IdNameTable* id_name_table, int old_var_dif) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	diff_tree->diff_number = function_tree->diff_number + 1;

	Identifier used_variable = id_name_table->data[IdNameTableGetDiffVarNumber(id_name_table, old_var_dif)];
	TEX_PRINTF("\\section{Производная $f^{(%zu)}$ по переменной \"$", diff_tree->diff_number);
	PrintNString(tex_file, used_variable.string, used_variable.length);
	TEX_PRINTF("$\"}\n");
	TEX_PRINTF("\\renewcommand{\\thesubsection}{\\arabic{subsection}}\n"
			   "\\titleformat{\\subsection}{\\normalfont\\bfseries}{}{0em}{#1\\ \\thesubsection}\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	diff_tree->root = doDifferentiation(function_tree->root, id_name_table, used_variable.num);
	BINARY_TREE_GRAPH_DUMP(diff_tree, "ExpressionDifferentiation", diff_tree->root, id_name_table);

	LATEX_PRINT_TREE(diff_tree, id_name_table);

#ifdef DRAW_PLOT
	tree_status = DrawGraph(diff_tree);
	TREE_ERROR_CHECK(tree_status);
#endif

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexSubtreeDifferential(Node_t* subtree_root, Node_t* diff_subtree_root, IdNameTable* id_name_table) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\subsection{Шаг}\n"
			   "\\begin{center} $");
	PrintExpressionTree(subtree_root, tex_file, id_name_table);
	TEX_PRINTF("^{\\prime} = ");
	PrintExpressionTree(diff_subtree_root, tex_file, id_name_table);
	TEX_PRINTF("$ \\end{center} \n\n");

#undef TEX_PRINTF

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LaTexPrintTree(Tree* tree, IdNameTable* id_name_table) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\titleformat{\\subsection}{\\normalfont\\bfseries}{}{0em}{\\thesubsection \\ #1\\ }\n"
			   "\\subsection{Результат}\n");

	TEX_PRINTF("\\begin{center} $f^{(%zu)}(", tree->diff_number);
	for (size_t i = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].type == ID_VAR) {
			PrintNString(tex_file, id_name_table->data[i].string, id_name_table->data[i].length);
			break;
		}
	}
	TEX_PRINTF(") = ");
	PrintExpressionTree(tree->root, tex_file, id_name_table);
	TEX_PRINTF("$ \\end{center} \n");

	if (fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#undef TEX_PRINTF

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintExpressionTree(Node_t* node, FILE* tex_file, IdNameTable* id_name_table) {

	if (!node)
		return TREE_NO_ERROR;

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case AMOUNT_OF_OPERATIONS: break;
				case LOG:
				case DIV: {
					fprintf(tex_file, "(\\%s{", OpNameTableGetTexSymbol(node->data.val_op));
					if (node->left) PrintExpressionTree(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					fprintf(tex_file, "}{");
					if (node->right) PrintExpressionTree(node->right, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					fprintf(tex_file, "})");

					break;
				}
				case POW:
				case ADD:
				case MUL:
				case SUB: {
					fprintf(tex_file, "({");
					if (node->left) PrintExpressionTree(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					fprintf(tex_file, "}%s{", OpNameTableGetTexSymbol(node->data.val_op));
					if (node->right) PrintExpressionTree(node->right, tex_file, id_name_table);
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
					if (node->left) PrintExpressionTree(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					fprintf(tex_file, "})");

					break;
				}
				case OPEN_BRACKET:
				case CLOSE_BRACKET:
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
			PrintNString(tex_file, id_name_table->data[node->data.val_var].string, id_name_table->data[node->data.val_var].length);
			break;
		}
		case UNW: return TREE_NO_ERROR;
		default:  return TREE_INVALID_TYPE;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintMathExpression(Node_t* node, FILE* tex_file, IdNameTable* id_name_table) {

	if (!node)
		return TREE_NO_ERROR;

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__);

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case AMOUNT_OF_OPERATIONS: break;
				case LOG: {
					TEX_PRINTF("(ln(");
					if (node->right) PrintMathExpression(node->right, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					TEX_PRINTF(")/ln(");
					if (node->left) PrintMathExpression(node->left, tex_file, id_name_table);
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
					if (node->left) PrintMathExpression(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);

					TEX_PRINTF(")%s(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->right) PrintMathExpression(node->right, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF("))");

					break;
				}
				case LN:
				case EXP:
				case SQRT: {
					TEX_PRINTF("(%s(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->left) PrintMathExpression(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF("))");

					break;
				}
				case COS:
				case SIN: {
					TEX_PRINTF("(%s(deg(", OpNameTableGetMathSymbol(node->data.val_op));
					if (node->left) PrintMathExpression(node->left, tex_file, id_name_table);
					else TREE_ERROR_CHECK(TREE_LATEX_SYNTAX_ERROR);
					TEX_PRINTF(")))");

					break;
				}
				case OPEN_BRACKET:
				case CLOSE_BRACKET:
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
			TEX_PRINTF("(x)");
			break;
		}
		case UNW: return TREE_NO_ERROR;
		default:  return TREE_INVALID_TYPE;
	}

#undef TEX_PRINTF

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode DrawGraph(Tree* tree, IdNameTable* id_name_table) {

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	for (size_t i = 0, j = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].type == ID_VAR)
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
			   "\txlabel = {$");
	PrintNString(tex_file, id_name_table->data[id_name_table->size - 1].string, id_name_table->data[id_name_table->size - 1].length);
	TEX_PRINTF("$},\n"
			   "\tylabel = {$f(");
	PrintNString(tex_file, id_name_table->data[id_name_table->size - 1].string, id_name_table->data[id_name_table->size - 1].length);
	TEX_PRINTF(")$},\n"
			   "\twidth = 300,\n"
			   "\theight = 300,\n"
			   "\trestrict y to domain=-30:30,\n"
    		   "\tgrid = major,\n"
			   "\tenlargelimits=true,\n"
			   "]\n"
			   "\\legend{\n"
			   "\t$f^{(%zu)}$\n}\n", tree->diff_number);
	TEX_PRINTF("\\addplot[blue, samples=750]{");
	PrintMathExpression(tree->root, tex_file, id_name_table);
	TEX_PRINTF("};\n"
			   "\\end{axis}\n"
			   "\\end{tikzpicture}\n"
			   "\\caption{График функции}\n"
			   "\\end{figure}\n");

#undef TEX_PRINTF

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

	TEX_PRINTF("\\documentclass[12pt, letterpaper]{report}\n"
			   "\\usepackage{amsthm,amssymb}\n"
			   "\\usepackage{mathtext}\n"
			   "\\usepackage[T1]{fontenc}\n"
			   "\\usepackage[utf8]{inputenc}\n"
			   "\\usepackage{pgfplots}\n"
			   "\\usepgfplotslibrary{colormaps}\n"
			   "\\usepackage{sectsty}\n"
			   "\\usepackage[explicit]{titlesec}\n"
			   "\\pgfplotsset{compat=1.9}\n"
			   "\\usepackage[english,russian]{babel}\n"
			   "\\makeatletter\n"
			   "\\renewcommand{\\@chapapp}{Пункт}\n"
			   "\\makeatother\n"
			   "\\chaptertitlefont{\\Large}\n"
			   "\\begin{document}\n"
			   "\\begin{titlepage}\n"
			   "\\begin{center}\n"
			   "\\vspace*{1cm}\n"
			   "\\textbf{МОСКОВСКИЙ ФИЗИКО-ТЕХНИЧЕСКИЙ ИНСТИТУТ (НАЦИОНАЛЬНЫЙ ИССЛЕДОВАТЕЛЬСКИЙ УНИВЕРСИТЕТ)}\\\\\n"
			   "\\vspace{0.5cm} Физтех-школа Радиотехники и компьютерных технологий\\\\\n"
			   "\\vspace{5cm} \\LARGE{Лабораторная работа %d.%d.%d\\\\\nИсследование функции}\n", 1 + rand()%10, 1 + rand()%10, 1 + rand()%10);
	TEX_PRINTF("\\vfill\n"
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
