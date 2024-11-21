#include "Differentiator_dump.hpp"

int main() {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	INIT_TREE(tree);
	TREE_CTOR(&tree);

	*tree.root = {OP, DIV, CreateNode(OP, ADD, _NUM(30), _NUM(50), tree.root), CreateNode(OP, SUB, _NUM(12), _VAR('x'), tree.root)};

	TREE_DTOR(&tree);
	OPEN_HTML_FILE;

	return 0;
}
