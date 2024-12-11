#pragma once

#include "Differentiator.hpp"

#define INIT_LEXER(lexer) Lexer lexer = {};

#define LEXICAL_ANALYSIS(buffer, lexer, id_name_table, size) {			 \
	tree_status = LexicalAnalysis(buffer, lexer, id_name_table, size);	\
	TREE_ERROR_CHECK(tree_status);										\
}

#define SKIP_EXTRA(buffer, token_start) {					 \
	tree_status = SkipExtra(buffer, token_start);			\
	TREE_ERROR_CHECK(tree_status);							\
}

#define LEXER_CTOR(lexer) {					 \
	tree_status = LexerCtor(lexer);			\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_DTOR(lexer) {					 \
	tree_status = LexerDtor(lexer);			\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_REALLOC(lexer) {				 \
	tree_status = LexerRealloc(lexer);		\
	TREE_ERROR_CHECK(tree_status);			\
}

#define LEXER_PRINT(lexer, id_name_table) {				 \
	tree_status = PrintLexer(lexer, id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

struct Token {
	NodeType type;
	union Data_t data;
	size_t index;
};

struct Lexer {
	Token* tokens;
	char* buffer;
	size_t capacity;
	size_t size;
};

const size_t LEXER_DEFAULT_CAPACITY = 4;

BinaryTreeStatusCode LexerCtor(Lexer* lexer);
BinaryTreeStatusCode LexerDtor(Lexer* lexer);
BinaryTreeStatusCode LexerRealloc(Lexer* lexer);
BinaryTreeStatusCode PrintLexer(Lexer* lexer, IdNameTable* id_name_table);
BinaryTreeStatusCode SkipExtra(const char* buffer, size_t* token_start);
BinaryTreeStatusCode LexicalAnalysis(char* buffer, Lexer* lexer, IdNameTable* id_name_table, size_t size);
BinaryTreeStatusCode PrintTokenValueGrammar(Token* token);
