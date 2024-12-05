#pragma once

#include "Tree.hpp"

#define VAR_NAME_TABLE_CTOR(var_name_table) {			 \
	tree_status = VarNameTableCtor(var_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define VAR_NAME_TABLE_DTOR(var_name_table) {			 \
	tree_status = VarNameTableDtor(var_name_table);		\
	TREE_ERROR_CHECK(tree_status);						\
}

#define VAR_NAME_TABLE_REALLOC(var_name_table) {		 \
	tree_status = VarNameTableRealloc(var_name_table);	\
	TREE_ERROR_CHECK(tree_status);						\
}

const size_t DEFAULT_VAR_NAME_TABLE_CAPACITY = 1;
const size_t MAX_OPERATION_NAME_SIZE = 6;

enum VarStatus {
	VAR_STATUS_USING	= 1,
	VAR_STATUS_DISUSING	= 0,
};

enum VarDiffStatus {
	VAR_DIFF_STATUS_NUM = 0,
	VAR_DIFF_STATUS_VAR = 1,
};

struct Variable {
	size_t num;
	char symbol[MAX_OPERATION_NAME_SIZE];
	Number_t value;
	VarStatus status;
	VarDiffStatus state;
};

struct VariableNameTable {
	Variable* data;
	size_t capacity;
	size_t size;
};

size_t NumberOfVariablesInSubtree(Node_t* node);
size_t NumberOfVarStatusUsingVariables(VariableNameTable* var_name_table);
BinaryTreeStatusCode VarNameTableSetDiff(VariableNameTable* var_name_table);
BinaryTreeStatusCode VarNameTableRealloc(VariableNameTable* var_name_table);
BinaryTreeStatusCode VarNameTableCtor(VariableNameTable* var_name_table);
BinaryTreeStatusCode VarNameTableDtor(VariableNameTable* var_name_table);
const char* VarNameTableGetSymbol(VariableNameTable* var_name_table, size_t VarNumber);
int VarNameTableFindVariable(VariableNameTable* var_name_table, const char* string);
Number_t VarNameTableGetValue(VariableNameTable* var_name_table, size_t VarNumber);
VarStatus VarNameTableGetStatus(VariableNameTable* var_name_table, size_t VarNumber);
const char* VarNameTableGetStateVariable(VariableNameTable* var_name_table);
int VarNameTableGetStateVariableNumber(VariableNameTable* var_name_table);
BinaryTreeStatusCode ResetVariables(VariableNameTable* var_name_table);
