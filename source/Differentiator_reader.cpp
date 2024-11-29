#include "Differentiator_reader.hpp"
#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"

const char* s = "8000/2*(5-5)$";
int p = 0;

int GetG();
int GetP();
int GetN();
int GetT();
int GetE();

int GetN() {
	int old_p = p;
	int val = 0;
	while ('0' <= s[p] && s[p] <= '9') {
		val = val * 10 + (s[p] - '0');
		p++;
	}
	if (old_p == p) exit(0);
	return val;
}

int GetP() {
	if (s[p] == '(') {
		p++;
		int val = GetE();
		if (s[p] != ')')
			exit(0);
		p++;
		return val;
	}
	else
		return GetN();
}

int GetT() {
	int val = GetP();
	while (s[p] == '*' || s[p] == '/') {
		int op = s[p];
		p++;
		int val2 = GetP();
		if (op == '*')
			val *= val2;
		else
			val /= val2;
	}
	return val;
}

int GetE() {
	int val = GetT();
	while (s[p] == '+' || s[p] == '-') {
		int op = s[p];
		p++;
		int val2 = GetT();
		if (op == '+')
			val += val2;
		else
			val -= val2;
	}
	return val;
}

int GetG() {
	int val = GetE();
	if (s[p] != '$')
		exit(0);
	p++;
	return val;
}

BinaryTreeStatusCode OriginalFunction(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree_status = ReadExpression(tree);
	TREE_ERROR_CHECK(tree_status);

	NameTablePrint();
	BINARY_TREE_GRAPH_DUMP(tree, "ExpressionReader", NULL);

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\section{Исходная функция}\n");
	TEX_PRINTF("\\centering\n");
	TEX_PRINTF("$f(");
	for (size_t i = 0, j = 0; i < AMOUNT_OF_VARIABLES; i++) {
		if (var_name_table[i].status == VAR_STATUS_USING) {
			j++;
			if (j == 1)
				TEX_PRINTF("%s", var_name_table[i].symbol);
			else
				TEX_PRINTF(", %s", var_name_table[i].symbol);
		}
	}
	TEX_PRINTF(") = ");

	PrintExpressionTree(tree->root, tex_file);
	TEX_PRINTF("$\\\\\n");

	DrawGraph(tree, tex_file);

	TEX_PRINTF("\\newpage\n");
	TEX_PRINTF("\\section{Дифференцирование}\n");

	if(fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReadExpression(Tree* tree) {

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

	if (buffer) {
		free(buffer);
		buffer = NULL;
	}

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
