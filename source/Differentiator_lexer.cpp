#include "Differentiator_lexer.hpp"

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

	if (lexer->buffer) {
		free(lexer->buffer);
		lexer->buffer = NULL;
	}

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
		size_t old_capacity = lexer->capacity;
		lexer->tokens = (Token*)realloc(lexer->tokens, (lexer->capacity *= 2) * sizeof(Token));
		if (!lexer->tokens)
			return TREE_ALLOC_ERROR;

		for (size_t i = old_capacity; i < lexer->capacity; i++) {
			lexer->tokens[i].data.val_num = 0;
			lexer->tokens[i].index = 0;
			lexer->tokens[i].type = UNW;
		}
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

static BinaryTreeStatusCode PrintTokenValue(Token* token, IdNameTable* id_name_table) {

	printf(" value - ");

	switch (token->type) {
		case NUM: { printf(GREEN("%lg"), token->data.val_num); break; }
		case VAR: {
			PrintNString(stdout, id_name_table->data[token->data.val_var].string, id_name_table->data[token->data.val_var].length);
			break;
		}
		case OP:  { printf(GREEN("%s"), OpNameTableGetMathSymbol(token->data.val_op)); break; }
		case UNW:
		default:  { printf(GREEN("%s"), RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintTokenValueGrammar(Token* token) {
	switch (token->type) {
		case NUM: { printf(GREEN("%lg"), token->data.val_num); break; }
		case VAR: { printf(GREEN("%zu"), token->data.val_var); break; }
		case OP:  { printf(GREEN("%s"), OpNameTableGetMathSymbol(token->data.val_op)); break; }
		case UNW:
		default:  { printf(GREEN("%s"), RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintLexer(Lexer* lexer, IdNameTable* id_name_table) {

	if (!lexer)
		return TREE_NULL_POINTER;

	printf("\n");
	printf(BLUE("Lexer address:") " " GREEN("%p") "\n", lexer);
	printf(BLUE("Lexer capacity:") " " GREEN("%zu") "\n", lexer->capacity);
	printf(BLUE("Lexer size:") " " GREEN("%zu") "\n", lexer->size);

	for (size_t i = 0; i < lexer->capacity; i++) {
		printf(BLUE("Lexer token[%zu]:") " type - " GREEN("%s") " index - " GREEN("%zu"), i, GetTokenStringType(lexer->tokens[i].type), lexer->tokens[i].index);
		PrintTokenValue(&lexer->tokens[i], id_name_table);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexicalAnalysis(char* buffer, Lexer* lexer, IdNameTable* id_name_table, size_t size) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	size_t token_start = 0;
	char* token_end_pointer = NULL;
	size_t token_end = 1;

	while (true) {
		LEXER_REALLOC(lexer);

#ifdef PRINT_DEBUG
		PrintLexer(lexer, id_name_table);
		PrintIdNameTable(id_name_table);
#endif

		if (lexer && lexer->size && lexer->tokens[lexer->size - 1].type == OP && lexer->tokens[lexer->size - 1].data.val_op == EOP)
			break;

		SKIP_EXTRA(buffer, &token_start);

		token_end = token_start + 1;

		while ((isalpha(buffer[token_end])) && (isalpha(buffer[token_end - 1]))) { token_end++; }

		int op_find = 0;
		for (size_t i = 0; i < AMOUNT_OF_OPERATIONS; i++) {
			op_find = 1;
			size_t j = 0;
			for (; token_start + j != token_end && *(op_name_table[i].math_symbol + j) != '\0'; j++) {
				if (buffer[token_start + j] != *(op_name_table[i].math_symbol + j)) {
					op_find = 0;
					break;
				}
			}
			if (op_find && token_start + j == token_end && *(op_name_table[i].math_symbol + j) == '\0') {
				lexer->tokens[lexer->size].type = OP;
				lexer->tokens[lexer->size].data.val_op = op_name_table[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end;
				op_find = 2;
				break;
			}
		}

		if (op_find == 2)
			continue;

		Number_t number = strtod(buffer + token_start, &token_end_pointer);
		if (!DiffCompareDouble(number, 0)) {
			lexer->tokens[lexer->size].type = NUM;
			lexer->tokens[lexer->size].data.val_num = number;
			lexer->tokens[lexer->size].index = token_start;
			lexer->size++;
			token_start = (size_t)(token_end_pointer - buffer);
			continue;
		}

		while ((isalpha(buffer[token_end]) || isdigit(buffer[token_end]) || buffer[token_end] == '_') &&
			   (isalpha(buffer[token_end - 1]) || isdigit(buffer[token_end - 1]) || buffer[token_end - 1] == '_')) { token_end++; }

		int var_find = 0;
		for (size_t i = 0; i < id_name_table->size; i++) {
			var_find = 1;
			size_t j = 0;
			for (; token_start + j != token_end && j != id_name_table->data[i].length; j++) {
				if (buffer[token_start + j] != *(id_name_table->data[i].string + j)) {
					var_find = 0;
					break;
				}
			}
			if (var_find && token_start + j == token_end && j == id_name_table->data[i].length) {
				lexer->tokens[lexer->size].type = VAR;
				lexer->tokens[lexer->size].data.val_var = (size_t)id_name_table->data[i].num;
				lexer->tokens[lexer->size].index = token_start;
				lexer->size++;
				token_start = token_end;
				var_find = 2;
				break;
			}
		}

		if (var_find == 2)
			continue;

		ID_NAME_TABLE_REALLOC(id_name_table);

		id_name_table->data[id_name_table->size].num = (int)id_name_table->size;
		id_name_table->data[id_name_table->size].value = 0;
		id_name_table->data[id_name_table->size].length = (size_t)(token_end - token_start);
		id_name_table->data[id_name_table->size].string = buffer + token_start;
		id_name_table->data[id_name_table->size].type = ID_VAR;

		lexer->tokens[lexer->size].type = VAR;
		lexer->tokens[lexer->size].data.val_var = (size_t)id_name_table->data[id_name_table->size++].num;
		lexer->tokens[lexer->size].index = token_start;
		lexer->size++;
		token_start = token_end;
	}

	return TREE_NO_ERROR;
}
