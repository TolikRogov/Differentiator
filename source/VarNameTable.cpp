#include "Variables.hpp"

BinaryTreeStatusCode VarNameTableRealloc(VariableNameTable* var_name_table) {

	if (var_name_table->capacity == var_name_table->size) {
		var_name_table->data = (Variable*)realloc(var_name_table->data, (var_name_table->capacity *= 2) * sizeof(Variable));
		if (!var_name_table->data)
			return TREE_ALLOC_ERROR;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode VarNameTableCtor(VariableNameTable* var_name_table) {

	var_name_table->capacity = DEFAULT_VAR_NAME_TABLE_CAPACITY;
	var_name_table->data = (Variable*)calloc(var_name_table->capacity, sizeof(Variable));
	if (!var_name_table->data)
		return TREE_ALLOC_ERROR;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode VarNameTableDtor(VariableNameTable* var_name_table) {

	for (size_t i = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].symbol) {
			free(var_name_table->data[i].symbol);
			var_name_table->data[i].symbol = NULL;
		}
	}

	var_name_table->capacity = 0;
	var_name_table->size = 0;

	if (var_name_table->data) {
		free(var_name_table->data);
		var_name_table->data = NULL;
	}

	return TREE_NO_ERROR;
}

int VarNameTableFindVariable(VariableNameTable* var_name_table, const char* string) {

	for (size_t i = 0; i < var_name_table->size; i++) {
		if (StrCmp(string, var_name_table->data[i].symbol) == 0)
			return (int)var_name_table->data[i].num;
	}

	return -1;
}

const char* VarNameTableGetSymbol(VariableNameTable* var_name_table, size_t VarNumber) {
	return var_name_table->data[VarNumber].symbol;
}

Number_t VarNameTableGetValue(VariableNameTable* var_name_table, size_t VarNumber) {
	return var_name_table->data[VarNumber].value;
}

VarStatus VarNameTableGetStatus(VariableNameTable* var_name_table, size_t VarNumber) {
	return var_name_table->data[VarNumber].status;
}

const char* VarNameTableGetStateVariable(VariableNameTable* var_name_table) {

	for (size_t i = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].state == VAR_DIFF_STATUS_VAR)
			return var_name_table->data[i].symbol;
	}

	return NULL;
}

int VarNameTableGetStateVariableNumber(VariableNameTable* var_name_table) {

	for (size_t i = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].state == VAR_DIFF_STATUS_VAR)
			return (int)var_name_table->data[i].num;
	}

	return -1;
}

BinaryTreeStatusCode ResetVariables(VariableNameTable* var_name_table) {

	for (size_t i = 0; i < var_name_table->size; i++) {
		var_name_table->data[i].value = 0;
		var_name_table->data[i].status = VAR_STATUS_DISUSING;
	}

	return TREE_NO_ERROR;
}

size_t NumberOfVariablesInSubtree(Node_t* node) {
	return (node->type == VAR ? 1 : 0) + (node->left ? NumberOfVariablesInSubtree(node->left) : 0) + (node->right ? NumberOfVariablesInSubtree(node->right) : 0);
}

size_t NumberOfVarStatusUsingVariables(VariableNameTable* var_name_table) {

	size_t cnt = 0;
	for (size_t i = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].status == VAR_STATUS_USING)
			cnt++;
	}

	return cnt;
}

BinaryTreeStatusCode VarNameTableSetDiff(VariableNameTable* var_name_table) {

	size_t cnt_of_using_variables = NumberOfVarStatusUsingVariables(var_name_table);
	if (cnt_of_using_variables == 1) {
		for (size_t i = 0; i < var_name_table->size; i++) {
			if (var_name_table->data[i].status == VAR_STATUS_USING) {
				var_name_table->data[i].state = VAR_DIFF_STATUS_VAR;
				break;
			}
		}
	}
	else if (cnt_of_using_variables > 1) {
		char* variable = (char*)calloc((size_t)(GetMaxSizeOfVariable(var_name_table) + 1), sizeof(char));
		if (!variable)
			TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

		int var_number = -1;

		printf(BLUE("Using variables"));
		for (size_t i = 0; i < var_name_table->size; i++) {
			if (var_name_table->data[i].status == VAR_STATUS_USING)
				printf(" - %s", var_name_table->data[i].symbol);
		}
		printf(":\n");
		do {
			printf(YELLOW("Enter the variable by which to differentiate:")" ");
			scanf("%s", variable);
			getchar();
		} while ((var_number = VarNameTableFindVariable(var_name_table, variable)) == -1 || VarNameTableGetStatus(var_name_table, (size_t)var_number) == VAR_STATUS_DISUSING);

		var_name_table->data[var_number].state = VAR_DIFF_STATUS_VAR;
		if (variable) {
			free(variable);
			variable = NULL;
		}
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode PrintVarNameTable(VariableNameTable* var_name_table) {

	printf("\n");
	printf(BLUE("Struct address:") " " GREEN("%p") "\n", var_name_table);
	printf(BLUE("Table capacity:") " " GREEN("%zu") "\n", var_name_table->capacity);
	printf(BLUE("Table size:") 	   " " GREEN("%zu") "\n", var_name_table->size);
	printf(BLUE("data address:")   " " GREEN("%p") "\n", var_name_table->data);

	for (size_t i = 0; i < var_name_table->size; i++) {
		printf(BLUE("Variable[%zu]:") " number - " GREEN("%zu") " state - " GREEN("%s") " status - " GREEN("%s") " symbol - " GREEN("%s") " value - " GREEN("%lg") "\n",
			   i, var_name_table->data[i].num, (var_name_table->data[i].state == VAR_DIFF_STATUS_NUM ? RET_STRING(VAR_DIFF_STATUS_NUM) : RET_STRING(VAR_DIFF_STATUS_VAR)),
			   (var_name_table->data[i].status == VAR_STATUS_USING ? RET_STRING(VAR_STATUS_USING) : RET_STRING(VAR_STATUS_DISUSING)), var_name_table->data[i].symbol,
			   var_name_table->data[i].value);
	}

	return TREE_NO_ERROR;
}

int GetMaxSizeOfVariable(VariableNameTable* var_name_table) {

	int max_size = 0;

	for (size_t i = 0; i < var_name_table->size; i++) {
		int cur_size = StrLen(var_name_table->data[i].symbol);
		if (max_size < cur_size)
			max_size = cur_size;
	}

	return max_size;
}
