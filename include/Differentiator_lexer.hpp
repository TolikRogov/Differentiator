#pragma once

#include "Differentiator.hpp"

#define LEXICAL_ANALYSIS(buffer, lexer, var_name_table) {				 \
	tree_status = LexicalAnalysis(buffer, lexer, var_name_table);		\
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

#define LEXER_PRINT(lexer, var_name_table) {				 \
	tree_status = PrintLexer(lexer, var_name_table);		\
	TREE_ERROR_CHECK(tree_status);							\
}

struct Token {
	NodeType type;
	union Data_t data;
	size_t index;
};

struct Lexer {
	Token* tokens;
	size_t capacity;
	size_t size;
};

const size_t LEXER_DEFAULT_CAPACITY = 4;

BinaryTreeStatusCode LexerCtor(Lexer* lexer);
BinaryTreeStatusCode LexerDtor(Lexer* lexer);
BinaryTreeStatusCode LexerRealloc(Lexer* lexer);
BinaryTreeStatusCode PrintLexer(Lexer* lexer, VariableNameTable* var_name_table);
BinaryTreeStatusCode SkipExtra(const char* buffer, size_t* token_start);
BinaryTreeStatusCode LexicalAnalysis(const char* buffer, Lexer* lexer, VariableNameTable* var_name_table);
