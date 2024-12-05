#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"
#include "Differentiator_reader.hpp"
#include "Variables.hpp"
#include "NameTable.hpp"

int main() {

	srand((unsigned int)time(NULL));

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	LATEX_DUMP_START();

	INIT_VAR_NAME_TABLE(var_name_table);
	VAR_NAME_TABLE_CTOR(&var_name_table);

	INIT_TREE(tree);
	TREE_CTOR(&tree);

	READ_EXPRESSION(&tree, &var_name_table);

// 	Calculator(&tree, &var_name_table);
//
// 	GET_TAYLOR(&tree, &var_name_table);

	VAR_NAME_TABLE_DTOR(&var_name_table);
	TREE_DTOR(&tree);

// 	OPEN_LATEX_PDF();
// 	OPEN_HTML_FILE();

	return 0;
}
