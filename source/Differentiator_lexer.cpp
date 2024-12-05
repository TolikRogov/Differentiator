#include "Differentiator_lexer.hpp"

#define EOP '$'
#define TOC '#'

BinaryTreeStatusCode SkipExtra(const char* buffer, size_t* token_start, size_t* token_end) {

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

	*token_end = *token_start;

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

static BinaryTreeStatusCode PrintTokenValue(Token* token) {

	printf(" value - ");

	switch (token->type) {
		case NUM: { printf("%lg", token->data.val_num); break; }
		case VAR: { printf("%s", VarNameTableGetSymbol(token->data.val_var)); break; }
		case OP:  { printf("%s", OpNameTableGetMathSymbol(token->data.val_op)); break; }
		case UNW:
		default:  { printf("%s", RET_STRING(UNW)); break; }
	}
	printf("\n");
	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintLexer(Lexer* lexer) {

	if (!lexer)
		return TREE_NULL_POINTER;

	printf(BLUE("Lexer address:") " " GREEN("%p") "\n", lexer);
	printf(BLUE("Lexer capacity:") " " GREEN("%zu") "\n", lexer->capacity);
	printf(BLUE("Lexer size:") " " GREEN("%zu") "\n", lexer->size);

	for (size_t i = 0; i < lexer->size; i++) {
		printf(BLUE("Lexer token[%zu]:") " type - " GREEN("%s") " index - " GREEN("%zu"), i, GetTokenStringType(lexer->tokens[i].type), lexer->tokens[i].index);
		PrintTokenValue(&lexer->tokens[i]);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode LexicalAnalysis(const char* buffer, Lexer* lexer) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	size_t token_start = 0;
	size_t token_end = 0;

	while (buffer[token_start] != EOP) {

		SKIP_EXTRA(buffer, &token_start, &token_end);

		LEXER_REALLOC(lexer);

		if (!isalpha(buffer[token_start]) && !isdigit(buffer[token_start])) {
			lexer->tokens[lexer->size].type = OP;
			char op[MAX_OPERATION_NAME_SIZE] = { buffer[token_start], '\0' };
			lexer->tokens[lexer->size].data.val_op = OpNameTableFindOperation(op);
			lexer->tokens[lexer->size].index = token_start;
			lexer->size++;
		}

		printf("%c", buffer[token_start]);
		token_start++;
	}
	printf("\n");
	LEXER_PRINT(lexer);
	return TREE_NO_ERROR;
}
