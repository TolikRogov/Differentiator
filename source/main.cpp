#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"
#include "Differentiator_reader.hpp"

int main() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	INIT_TREE(tree);
	TREE_CTOR(&tree);

	InfixReader(&tree);

	// tree.root = _MUL(_DIV(_SIN(_ADD(_NUM(1), _VAR('x'))), _POW(_NUM(12), _VAR('x'))), _SQRT(_SUB(_VAR('y'), _VAR('x'))));
	// LATEX_PRINT_TREE(&tree);

	TREE_DTOR(&tree);
	OPEN_HTML_FILE();

	return 0;
}
