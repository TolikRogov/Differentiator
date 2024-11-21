#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"

int main() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	LATEX_DUMP_START();

	INIT_TREE(tree);
	TREE_CTOR(&tree);

	*tree.root = {OP, DIV, CreateNode(OP, ADD, _NUM(20), _NUM(50), tree.root), CreateNode(OP, SUB, _NUM(12), _VAR('x'), tree.root)};
	//*tree.root = {OP, MUL, CreateNode(OP, ADD, _NUM(30), _NUM(50), tree.root), CreateNode(OP, SUB, _NUM(12), _VAR('x'), tree.root)};

	LATEX_PRINT_TREE(&tree);

	TREE_DTOR(&tree);

	LATEX_DUMP_FINISH();
	OPEN_LATEX_PDF();
	return 0;
}
