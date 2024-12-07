#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"
#include "Differentiator_reader.hpp"
#include "Differentiator_lexer.hpp"
#include "Identifiers.hpp"
#include "NameTable.hpp"

int main() {

	srand((unsigned int)time(NULL));

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	LATEX_DUMP_START();

	INIT_LEXER(lexer);
	LEXER_CTOR(&lexer);

	INIT_ID_NAME_TABLE(id_name_table);
	ID_NAME_TABLE_CTOR(&id_name_table);

	INIT_TREE(tree);
	TREE_CTOR(&tree, &id_name_table);

	READ_EXPRESSION(&tree, &id_name_table, &lexer);
	Calculator(&tree, &id_name_table);

	GET_TAYLOR(&tree, &id_name_table);

	LEXER_DTOR(&lexer);
	ID_NAME_TABLE_DTOR(&id_name_table);
	TREE_DTOR(&tree);

	OPEN_LATEX_PDF();
	OPEN_HTML_FILE();

	return 0;
}
