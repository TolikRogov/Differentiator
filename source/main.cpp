#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"
#include "Differentiator_reader.hpp"
#include "NameTable.hpp"

int main() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	LATEX_DUMP_START();

	INIT_TREE(tree);
	TREE_CTOR(&tree);

	INFIX_EXPRESSION_READER(&tree);

	INIT_TREE(diff_tree);
	EXPRESSION_DIFFERENTIATION(&tree, &diff_tree);

	TREE_DTOR(&tree);

	OPEN_LATEX_PDF();
	OPEN_HTML_FILE();

	return 0;
}
