#include "Identifiers.hpp"

BinaryTreeStatusCode IdNameTableRealloc(IdNameTable* id_name_table) {

	if (id_name_table->capacity == id_name_table->size) {
		size_t old_capacity = id_name_table->capacity;
		id_name_table->data = (Identifier*)realloc(id_name_table->data, (id_name_table->capacity *= 2) * sizeof(Identifier));
		if (!id_name_table->data)
			return TREE_ALLOC_ERROR;

		for (size_t i = old_capacity; i < id_name_table->capacity; i++) {
			id_name_table->data[i].length = 0;
			id_name_table->data[i].num = 0;
			id_name_table->data[i].string = NULL;
			id_name_table->data[i].type = ID_UNW;
			id_name_table->data[i].value = 0;
		}
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode IdNameTableCtor(IdNameTable* id_name_table) {

	id_name_table->capacity = DEFAULT_ID_NAME_TABLE_CAPACITY;
	id_name_table->data = (Identifier*)calloc(id_name_table->capacity, sizeof(Identifier));
	if (!id_name_table->data)
		return TREE_ALLOC_ERROR;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode IdNameTableDtor(IdNameTable* id_name_table) {

	id_name_table->capacity = 0;
	id_name_table->size = 0;

	if (id_name_table->data) {
		free(id_name_table->data);
		id_name_table->data = NULL;
	}

	return TREE_NO_ERROR;
}

int IdNameTableGetIdNumber(IdNameTable* id_name_table, const char* string, size_t length) {

	for (size_t i = 0; i < id_name_table->size; i++) {
		int equal = 1;
		for (size_t j = 0; j < length && id_name_table->data[i].string[j] != '\0'; j++) {
			if (*(string + j) != id_name_table->data[i].string[j]) {
				equal = 0;
				break;
			}
		}
		if (equal)
			return id_name_table->data[i].num;
	}

	return -1;
}

BinaryTreeStatusCode ResetVariables(IdNameTable* id_name_table) {

	for (size_t i = 0; i < id_name_table->size; i++) {
		if (id_name_table->data[i].type == ID_VAR)
			id_name_table->data[i].value = 0;
	}

	return TREE_NO_ERROR;
}

size_t NumberOfVariablesInSubtree(Node_t* node) {
	return (node->type == VAR ? 1 : 0) + (node->left ? NumberOfVariablesInSubtree(node->left) : 0) + (node->right ? NumberOfVariablesInSubtree(node->right) : 0);
}

size_t IdNameTableGetMaxIdLength(IdNameTable* id_name_table) {

	size_t max_size = 0;

	for (size_t i = 0; i < id_name_table->size; i++) {
		if (max_size < id_name_table->data[i].length)
			max_size = id_name_table->data[i].length;
	}

	return max_size;
}

int IdNameTableGetDiffVarNumber(IdNameTable* id_name_table, int old_var_ret) {

	static int id_number = -1;
	if (old_var_ret && id_number != -1)
		return id_number;

	if (id_name_table->size == 1)
		id_number = id_name_table->data[id_name_table->size - 1].num;

	if (id_name_table->size > 1) {
		char* identifier = (char*)calloc((size_t)(IdNameTableGetMaxIdLength(id_name_table) + 1), sizeof(char));
		if (!identifier)
			TREE_ERROR_CHECK(TREE_ALLOC_ERROR);

		printf(BLUE("Using variables"));
		for (size_t i = 0; i < id_name_table->size; i++) {
			if (id_name_table->data[i].type == ID_VAR) {
				printf(" - ");
				PrintNString(stdout, id_name_table->data[i].string, id_name_table->data[i].length);
			}
		}

		int id_length = 0;
		printf(":\n");
		do {
			printf(YELLOW("Enter the variable by which to differentiate:")" ");
			scanf("%s%n", identifier, &id_length);
			getchar();
		} while ((id_number = IdNameTableGetIdNumber(id_name_table, identifier, (size_t)id_length)) == -1 || id_name_table->data[id_number].type != ID_VAR);

		if (identifier) {
			free(identifier);
			identifier = NULL;
		}
	}

	return id_number;
}

const char* IdNameTableGetIdType(IdNameTable* id_name_table, IdType type) {
	switch (type) {
		case ID_FUNCTION: return RET_STRING(ID_FUNCTION);
		case ID_VAR: return RET_STRING(ID_VAR);
		case ID_UNW:
		default: return RET_STRING(ID_UNW);
	}
}

BinaryTreeStatusCode PrintIdNameTable(IdNameTable* id_name_table) {

	printf("\n");
	printf(BLUE("Struct address:") " " GREEN("%p") "\n", id_name_table);
	printf(BLUE("Table capacity:") " " GREEN("%zu") "\n", id_name_table->capacity);
	printf(BLUE("Table size:") 	   " " GREEN("%zu") "\n", id_name_table->size);
	printf(BLUE("data address:")   " " GREEN("%p") "\n", id_name_table->data);

	for (size_t i = 0; i < id_name_table->capacity; i++) {
		printf(BLUE("Identifier[%zu]:") " number - " GREEN("%d") " type - " GREEN("%s") " value - " GREEN("%lg") " length - " GREEN("%zu") " string - ",
			   i, id_name_table->data[i].num, IdNameTableGetIdType(id_name_table, id_name_table->data[i].type),  id_name_table->data[i].value, id_name_table->data[i].length);
		for (size_t j = 0; j < id_name_table->data[i].length; j++)
			printf(GREEN("%c"), id_name_table->data[i].string[j]);
		printf("\n");
	}

	return TREE_NO_ERROR;
}
