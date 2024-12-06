#include "Differentiator_reader.hpp"
#include "Differentiator_dump.hpp"
#include "Differentiator_latex.hpp"
#include "Differentiator_lexer.hpp"
#include "Variables.hpp"

Node_t* GetGrammar(Lexer* lexer, size_t* pc);
Node_t* GetExpression(Lexer* lexer, size_t* pc);
Node_t* GetTerminator(Lexer* lexer, size_t* pc);
Node_t* GetPriority(Lexer* lexer, size_t* pc);
Node_t* GetNumber(Lexer* lexer, size_t* pc);

Node_t* GetNumber(Lexer* lexer, size_t* pc) {
	printf("Number %zu\n", *pc);
	if (lexer->tokens[*pc].type == NUM)
		return _NUM(lexer->tokens[(*pc)++].data.val_num);
	else
		return NULL;

}

Node_t* GetPriority(Lexer* lexer, size_t* pc) {
	printf("Priority %zu\n", *pc);
	Node_t* node = NULL;

	if (lexer->tokens[*pc].type == OP && lexer->tokens[*pc].data.val_op == OPEN_BRACKET) {
		(*pc)++;
		node = GetExpression(lexer, pc);
		if (!node) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (lexer->tokens[*pc].type == OP && lexer->tokens[*pc].data.val_op != CLOSE_BRACKET) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		(*pc)++;
		return node;
	}
	else
		return GetNumber(lexer, pc);
}

Node_t* GetTerminator(Lexer* lexer, size_t* pc) {
	printf("Terminator %zu\n", *pc);
	Node_t* node1 = GetPriority(lexer, pc);
	if (!node1) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}
	printf("%d\n", lexer->tokens[*pc].type);
	if (lexer->tokens[*pc].type != OP)
		return node1;

	while (lexer->tokens[*pc].data.val_op == MUL || lexer->tokens[*pc].data.val_op == DIV) {
		OpNum op = lexer->tokens[(*pc)++].data.val_op;
		Node_t* node2 = GetPriority(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (op == MUL)
			node1 = _MUL(node1, node2);
		else
			node1 = _DIV(node1, node2);
	}

	return node1;
}

Node_t* GetExpression(Lexer* lexer, size_t* pc) {
	printf("Expression %zu\n", *pc);
	Node_t* node1 = GetTerminator(lexer, pc);
	if (!node1) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	if (lexer->tokens[*pc].type != OP)
		return node1;

	while (lexer->tokens[*pc].data.val_op == ADD || lexer->tokens[*pc].data.val_op == SUB) {
		OpNum op = lexer->tokens[(*pc)++].data.val_op;
		Node_t* node2 = GetTerminator(lexer, pc);
		if (!node2) {
			TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
			return NULL;
		}

		if (op == ADD)
			node1 = _ADD(node1, node2);
		else
			node1 = _SUB(node1, node2);
	}

	return node1;
}

Node_t* GetGrammar(Lexer* lexer, size_t* pc) {
	printf("Grammar %zu\n", *pc);
	Node_t* node = GetExpression(lexer, pc);
	if (!node) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	if (lexer->tokens[*pc].type == OP && lexer->tokens[(*pc)++].data.val_op != EOP) {
		TREE_ERROR_MESSAGE(TREE_EXPRESSION_SYNTAX_ERROR);
		return NULL;
	}

	return node;
}

BinaryTreeStatusCode OriginalFunction(Tree* tree, VariableNameTable* var_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree_status = ReadExpression(tree, var_name_table);
	TREE_ERROR_CHECK(tree_status);

	VarNameTableSetDiff(var_name_table);
	NameTablePrint(var_name_table);
	BINARY_TREE_GRAPH_DUMP(tree, "ExpressionReader", NULL, var_name_table);

	FILE* tex_file = fopen(DIFF_LATEX_FILE_ DIFF_TEX_EXTENSION_, "a");
	if (!tex_file)
		TREE_ERROR_CHECK(TREE_FILE_OPEN_ERROR);

#define TEX_PRINTF(...) fprintf(tex_file, __VA_ARGS__)

	TEX_PRINTF("\\chapter{Исходная функция}\n");
	TEX_PRINTF("\\hfil $f(");
	for (size_t i = 0, j = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].status == VAR_STATUS_USING) {
			j++;
			if (j == 1)
				TEX_PRINTF("%s", var_name_table->data[i].symbol);
			else
				TEX_PRINTF(", %s", var_name_table->data[i].symbol);
		}
	}
	TEX_PRINTF(") = ");

	PrintExpressionTree(tree->root, tex_file, var_name_table);
	TEX_PRINTF("$\\\\\n");

	if(fclose(tex_file))
		TREE_ERROR_CHECK(TREE_FILE_CLOSE_ERROR);

	tree_status = DrawGraph(tree, var_name_table);
	TREE_ERROR_CHECK(tree_status);

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReadExpression(Tree* tree, VariableNameTable* var_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

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

	ResetVariables(var_name_table);

	Lexer lexer = {};
	LEXER_CTOR(&lexer);

	LEXICAL_ANALYSIS(buffer, &lexer, var_name_table);

	size_t pc = 0;
	tree->root = GetGrammar(&lexer, &pc);
	if (!tree->root)
		TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);

	if (buffer) {
		free(buffer);
		buffer = NULL;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode RecursionReadExpression(char* buffer, Node_t* node, int prev_node_depth, VariableNameTable* var_name_table) {

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
				RecursionReadExpression(buffer, node, count_brackets, var_name_table);
				break;
			}

			Node_t* new_node = CreateNode(UNW, {.val_num = UNKNOWN_WHAT}, NULL, NULL, node);
			if (!node->left) node->left = new_node;
			else if (!node->right) node->right = new_node;

#ifdef PRINT_DEBUG
			INIT_TREE(tree);
			tree.root = FindTreeRoot(node);
			BinaryTreeStatusCode tree_status = TREE_NO_ERROR;
			BINARY_TREE_GRAPH_DUMP(&tree, "RecursionReadExpression", node, var_name_table);
#endif

			index++;
			RecursionReadExpression(buffer, new_node, prev_node_depth, var_name_table);

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
				RecursionReadExpression(buffer, node, count_brackets, var_name_table);
				return TREE_NO_ERROR;
			}

			if ((node_data.val_op = OpNameTableFindOperation(operation)) != INVALID_OPERATION) {
#ifdef PRINT_DEBUG
				printf("It is operation: %s\n", operation);
#endif
				node->type = OP;
				node->data.val_op = node_data.val_op;

				index++;
				RecursionReadExpression(buffer, node, count_brackets, var_name_table);
				return TREE_NO_ERROR;
			}

			int var_find = VarNameTableFindVariable(var_name_table, operation);
			if ((var_find != -1) && (node_data.val_var = (size_t)var_find)) {
#ifdef PRINT_DEBUG
				printf("It is variable: %s\n", operation);
#endif
				node->type = VAR;
				node->data.val_var = node_data.val_var;
				var_name_table->data[node->data.val_var].status = VAR_STATUS_USING;

				index++;
				RecursionReadExpression(buffer, node, count_brackets, var_name_table);
				return TREE_NO_ERROR;
			}

			TREE_ERROR_CHECK(TREE_EXPRESSION_SYNTAX_ERROR);
		}
	}

	index++;
	RecursionReadExpression(buffer, node, prev_node_depth, var_name_table);

	return TREE_NO_ERROR;
}
