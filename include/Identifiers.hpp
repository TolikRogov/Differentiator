#pragma once

#include "Tree.hpp"

#define ID_NAME_TABLE_CTOR(id_name_table) {			 	 \
	tree_status = IdNameTableCtor(id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define ID_NAME_TABLE_DTOR(id_name_table) {			 	 \
	tree_status = IdNameTableDtor(id_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define ID_NAME_TABLE_REALLOC(id_name_table) {		 	 \
	tree_status = IdNameTableRealloc(id_name_table);	\
	TREE_ERROR_CHECK(tree_status);						\
}

const size_t DEFAULT_ID_NAME_TABLE_CAPACITY = 1;
const size_t MAX_OPERATION_NAME_SIZE = 6;

enum IdType {
	ID_UNW = -1,
	ID_VAR,
	ID_FUNCTION,
};

struct Identifier  {
	int num;
	IdType type;
	char* string;
	size_t length;
	Number_t value;
};

struct IdNameTable {
	Identifier* data;
	size_t capacity;
	size_t size;
};

BinaryTreeStatusCode IdNameTableRealloc(IdNameTable* var_name_table);
BinaryTreeStatusCode IdNameTableCtor(IdNameTable* var_name_table);
BinaryTreeStatusCode IdNameTableDtor(IdNameTable* var_name_table);

BinaryTreeStatusCode ResetVariables(IdNameTable* id_name_table);
BinaryTreeStatusCode PrintIdNameTable(IdNameTable* id_name_table);

size_t NumberOfVariablesInSubtree(Node_t* node);
int IdNameTableGetDiffVarNumber(IdNameTable* var_name_table, int old_var_ret);
int IdNameTableGetIdNumber(IdNameTable* id_name_table, const char* string, size_t length);
size_t IdNameTableGetMaxIdLength(IdNameTable* id_name_table);
const char* IdNameTableGetIdType(IdNameTable* id_name_table, IdType type);
