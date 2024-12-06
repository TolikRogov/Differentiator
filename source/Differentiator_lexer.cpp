#include "Differentiator_lexer.hpp"

#define EOP '$'
#define TOC '#'

BinaryTreeStatusCode SkipExtra(const char* buffer, size_t* token_start) {

	while (isspace(buffer[*token_start]))
		(*token_start)++;

	if (buffer[*token_start] == TOC) {
		while (buffer[++(*token_start)] != TOC) {
			if (buffer[*token_start] == EOP)
				return TREE_EXPRESSION_COMMENTS_ERROR;
		}
		(*token_start)++;
	}

	while (isspace(buffer[*token_start]))
		(*token_start)++;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerCtor(Lexer* lexer) {

	if (!lexer)
		return TREE_NULL_POINTER;

	lexer->capacity = LEXER_DEFAULT_CAPACITY;

	lexer->tokens = (Token*)calloc(lexer->capacity, sizeof(Token));
	if (!lexer->tokens)
		return TREE_ALLOC_ERROR;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerDtor(Lexer* lexer) {

	if (!lexer)
		return TREE_NO_ERROR;

	lexer->capacity = 0;
	lexer->size = 0;

	if (lexer->tokens) {
		free(lexer->tokens);
		lexer->tokens = NULL;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexerRealloc(Lexer* lexer) {

	if (lexer->capacity == lexer->size) {
		lexer->tokens = (Token*)realloc(lexer->tokens, (lexer->capacity *= 2) * sizeof(Token));
		if (!lexer->tokens)
			return TREE_ALLOC_ERROR;
	}

	return TREE_NO_ERROR;
}

static const char* GetTokenStringType(NodeType type) {
	switch (type) {
		case NUM: return RET_STRING(NUM);
		case VAR: return RET_STRING(VAR);
		case OP:  return RET_STRING(OP);
		case UNW:
		default:  return RET_STRING(UNW);
	}
}

static BinaryTreeStatusCode PrintTokenValue(Token* token, VariableNameTable* var_name_table) {

	printf(" value - ");

	switch (token->type) {
		case NUM: { printf(GREEN("%lg"), token->data.val_num); break; }
		case VAR: { printf(GREEN("%s"), VarNameTableGetSymbol(var_name_table, token->data.val_var)); break; }
		case OP:  { printf(GREEN("%s"), OpNameTableGetMathSymbol(token->data.val_op)); break; }
		case UNW:
		default:  { printf(GREEN("%s"), RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintLexer(Lexer* lexer, VariableNameTable* var_name_table) {

	if (!lexer)
		return TREE_NULL_POINTER;

	printf("\n");
	printf(BLUE("Lexer address:") " " GREEN("%p") "\n", lexer);
	printf(BLUE("Lexer capacity:") " " GREEN("%zu") "\n", lexer->capacity);
	printf(BLUE("Lexer size:") " " GREEN("%zu") "\n", lexer->size);

	for (size_t i = 0; i < lexer->size; i++) {
		printf(BLUE("Lexer token[%zu]:") " type - " GREEN("%s") " index - " GREEN("%zu"), i, GetTokenStringType(lexer->tokens[i].type), lexer->tokens[i].index);
		PrintTokenValue(&lexer->tokens[i], var_name_table);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexicalAnalysis(const char* buffer, Lexer* lexer, VariableNameTable* var_name_table) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	size_t token_start = 0;
	char* token_end_pointer = NULL;
	size_t token_end = 0;

	do {

		SKIP_EXTRA(buffer, &token_start);

		LEXER_REALLOC(lexer);

		Number_t number = strtod(buffer + token_start, &token_end_pointer);
		if (!DiffCompareDouble(number, 0)) {
			lexer->tokens[lexer->size].type = NUM;
			lexer->tokens[lexer->size].data.val_num = number;
			lexer->tokens[lexer->size].index = token_start;
			lexer->size++;
			token_start = (size_t)(token_end_pointer - buffer);
			continue;
		}

		token_end = token_start;
		while (isalpha(buffer[token_end])) { token_end++; }

		int op_find = 0;
		for (size_t i = 0; i < AMOUNT_OF_OPERATIONS; i++) {
			op_find = 1;
			for (size_t j = 0; j != token_end && *(op_name_table[i].math_symbol + j) != '\0'; j++) {
				if (buffer[token_start + j] != *(op_name_table[i].math_symbol + j)) {
					op_find = 0;
					break;
				}
			}
			if (op_find) {
				lexer->tokens[lexer->size].type = OP;
				lexer->tokens[lexer->size].data.val_op = op_name_table[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end + 1;
				break;
			}
		}

		if (op_find)
			continue;

		int var_find = 0;
		for (size_t i = 0; i < var_name_table->size; i++) {
			var_find = 1;
			for (size_t j = 0; j != token_end && *(var_name_table->data[i].symbol + j) != '\0'; j++) {
				if (buffer[token_start + j] != *(var_name_table->data[i].symbol + j)) {
					var_find = 0;
					break;
				}
			}
			if (var_find) {
				lexer->tokens[lexer->size].type = VAR;
				lexer->tokens[lexer->size].data.val_var = var_name_table->data[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end + 1;
				break;
			}
		}

		if (var_find)
			continue;

		VAR_NAME_TABLE_REALLOC(var_name_table);

		var_name_table->data[var_name_table->size].num = var_name_table->size;
		var_name_table->data[var_name_table->size].status = VAR_STATUS_USING;
		var_name_table->data[var_name_table->size].state = VAR_DIFF_STATUS_NUM;
		var_name_table->data[var_name_table->size].value = 0;

		var_name_table->data[var_name_table->size].symbol = (char*)calloc((size_t)(token_end - token_start + 1), sizeof(char));
		if (!var_name_table->data[var_name_table->size].symbol)
			TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

		for (size_t i = 0; i < token_end - token_start; i++)
			var_name_table->data[var_name_table->size].symbol[i] = buffer[token_start + i];
		var_name_table->data[var_name_table->size].symbol[token_end - token_start] = '\0';

		lexer->tokens[lexer->size].type = VAR;
		lexer->tokens[lexer->size].data.val_var = var_name_table->data[var_name_table->size++].num;
		lexer->tokens[lexer->size].index = token_start;
		lexer->size++;
		token_start = token_end + 1;
	} while (buffer[token_start] != EOP);

#ifdef PRINT_DEBUG
	LEXER_PRINT(lexer, var_name_table);
	PrintVarNameTable(var_name_table);
#endif

	return TREE_NO_ERROR;
}
