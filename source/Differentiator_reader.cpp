#include "Differentiator_reader.hpp"

BinaryTreeStatusCode InfixReader(Tree* tree) {

	FILE* exp_file = fopen(DIFF_EXPRESSION_FILE_, "r");
	if (!exp_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

	struct stat file_info = {};
	stat(DIFF_EXPRESSION_FILE_, &file_info);

	size_t size = (size_t)file_info.st_size;
	char* buffer = (char*)calloc(size, sizeof(char));
	if (!buffer)
		TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

	size_t read_check = fread(buffer, size, sizeof(char), exp_file);
	if (!read_check)
		TREE_ERROR_CHECK(TREE_READ_ERROR);

	if (fclose(exp_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

#ifdef PRINT_DEBUG
	for (size_t i = 0; i < size; i++) {
		printf("%c", buffer[i]);
	}
#endif

	int new_row_index = 0;
	for (; new_row_index < (int)size;) {
		if (*(buffer + new_row_index) == '#')
			while (*(buffer + new_row_index++) != '\n') {}
		else
			break;
	}
	if (new_row_index - (int)size >= 0)
		return TREE_NOTHING_TO_READ;

	ResetVariables();

	RecursionReadExpression(buffer + new_row_index, tree->root, 0);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth) {

	if (!buffer)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	static size_t index = 0;
	static int count_brackets = 0;

	static size_t op_index = 0;
	static char operation[MAX_OPERATION_NAME_SIZE] = {};

	if (count_brackets < 0)
		TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);

	switch (buffer[index]) {
		case '\n': {
			if (count_brackets) TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);
			return TREE_NO_ERROR;
		}
		case '(': {
			memset(operation, 0, MAX_OPERATION_NAME_SIZE);
			op_index = 0; count_brackets++;

			if (prev_node_depth == 0) {
				index++;
				RecursionReadExpression(buffer, node, count_brackets);
				break;
			}

			Node_t* new_node = CreateNode(UNW, {.val_num = UNKNOWN_WHAT}, NULL, NULL, node);
			if (!node->left) node->left = new_node;
			else if (!node->right) node->right = new_node;

#ifdef PRINT_DEBUG
			INIT_TREE(tree);
			tree.root = FindTreeRoot(node);
			BinaryTreeStatusCode tree_status = TREE_NO_ERROR;
			BINARY_TREE_GRAPH_DUMP(&tree, "RecursionReadExpression", node);
#endif

			index++;
			RecursionReadExpression(buffer, new_node, prev_node_depth);

			break;
		}
		case ')': { memset(operation, 0, MAX_OPERATION_NAME_SIZE); op_index = 0; count_brackets--; return TREE_NO_ERROR; }
		default: {
			operation[op_index++] = buffer[index];
			if (buffer[index + 1] != ')' && buffer[index + 1] != '(')
				break;

#ifdef PRINT_DEBUG
			printf("\n'%6s': current - %d, prev - %d\n", operation, count_brackets, prev_node_depth);
#endif
			Data_t node_data = {};
			if (sscanf(operation, "%lg", &node_data.val_num)) {
#ifdef PRINT_DEBUG
				printf("It is number: %lg\n", node_data.val_num);
#endif
				node->type = NUM;
				node->data.val_num = node_data.val_num;

				index++;
				RecursionReadExpression(buffer, node, count_brackets);
				return TREE_NO_ERROR;
			}

			if ((node_data.val_op = OpNameTableFindOperation(operation)) != INVALID_OPERATION) {
#ifdef PRINT_DEBUG
				printf("It is operation: %s\n", operation);
#endif
				node->type = OP;
				node->data.val_op = node_data.val_op;

				index++;
				RecursionReadExpression(buffer, node, count_brackets);
				return TREE_NO_ERROR;
			}

			if ((node_data.val_var = VarNameTableFindVariable(operation)) != INVALID_VARIABLE) {
#ifdef PRINT_DEBUG
				printf("It is variable: %s\n", operation);
#endif
				node->type = VAR;
				node->data.val_var = node_data.val_var;
				var_name_table[node->data.val_var].status = VAR_STATUS_USING;

				index++;
				RecursionReadExpression(buffer, node, count_brackets);
				return TREE_NO_ERROR;
			}

			TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);
		}
	}

	index++;
	RecursionReadExpression(buffer, node, prev_node_depth);

	return TREE_NO_ERROR;
}
