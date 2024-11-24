#include "Differentiator.hpp"

char TREE_DUMP = TREE_FIRST_RUN;

BinaryTreeStatusCode TreeCtor(Tree* tree) {

	BinaryTreeStatusCode tree_status = TREE_NO_ERROR;

	tree->root = CreateNode(UNW, {.val_num = UNKNOWN_WHAT}, NULL, NULL, NULL);

	if (TREE_DUMP == TREE_ALREADY_ON)
		return TREE_NO_ERROR;

	TREE_HTML_DUMP_START();
	TREE_DUMP = TREE_ALREADY_ON;

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode TreeDtor(Node_t* node) {

	if (node->left)  TreeDtor(node->left);
	if (node->right) TreeDtor(node->right);

	if (node) {
		free(node);
		node = NULL;
	}

	return TREE_NO_ERROR;
}

Node_t* CreateNode(NodeType type, Data_t data, Node_t* left, Node_t* right, Node_t* parent) {

	Node_t* node = (Node_t*)calloc(1, sizeof(*node));
	node->type = type;

	switch (type) {
		case UNW:
		case NUM: { node->data.val_num = data.val_num; break; }
		case OP:  { node->data.val_op  = data.val_op;  break; }
		case VAR: { node->data.val_var = data.val_var; break; }
	}

	if (left)   { node->left = left;   left->parent = node;  }
	if (right)  { node->right = right; right->parent = node; }
	if (parent) { node->parent = parent; }

	return node;
}

BinaryTreeStatusCode IsRootUnknownWhat(Node_t* root) {

	if (!root)
		return TREE_NO_ERROR;

	if (root->left == NULL && \
		root->right == NULL && \
		root->type == UNW && \
		DiffCompareDouble(root->data.val_num, UNKNOWN_WHAT))
		return TREE_ROOT_IS_UNKNOWN;

	return TREE_NO_ERROR;
}

Node_t* FindTreeRoot(Node_t* node) {

	if (!node->parent)
		return node;

	return FindTreeRoot(node->parent);
}

BinaryTreeStatusCode NodePrintData(Node_t* node) {

	if (!node)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	switch (node->type) {
		case OP:  { printf("%s\n", op_name_table[node->data.val_op].math_symbol); 	 break; }
		case NUM: { printf("%lg\n", node->data.val_num); break; }
		case VAR: { printf("%s\n", var_name_table[node->data.val_var].symbol);  break; }
		case UNW: { printf("%s\n", "UNKOWN WHAT"); break; }
		default: TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode ReplaceUnknownWhat(Node_t* node, Data_t data, NodeType type) {

	switch (type) {
		case OP:  { node->type = OP;  node->data.val_op  = data.val_op;  break; }
		case NUM: { node->type = NUM; node->data.val_num = data.val_num; break; }
		case VAR: { node->type = VAR; node->data.val_num = data.val_var; break; }
		case UNW:
		default: TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}

	return TREE_NO_ERROR;
}

const char* OpNameTableGetTexSymbol(OpNum op_number) {
	return	op_name_table[op_number].tex_symbol;
}

const char* OpNameTableGetMathSymbol(OpNum op_number) {
	return	op_name_table[op_number].math_symbol;
}

OpNum OpNameTableFindOperation(const char* string) {

	for (size_t i = 0; i < AMOUNT_OF_OPERATIONS; i++) {
		if (StrCmp(string, op_name_table[i].math_symbol) == 0)
			return op_name_table[i].num;
	}

	return INVALID_OPERATION;
}

VarNum VarNameTableFindVariable(const char* string) {

	for (size_t i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		if (StrCmp(string, var_name_table[i].symbol) == 0)
			return var_name_table[i].num;
	}

	return INVALID_VARIABLE;
}

const char* VarNameTableGetSymbol(VarNum number) {
	return var_name_table[number].symbol;
}

Number_t VarNameTableGetValue(VarNum number) {
	return var_name_table[number].value;
}

BinaryTreeStatusCode ResetVariables() {

	for (size_t i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		var_name_table[i].value = 0;
		var_name_table[i].status = VAR_STATUS_DISUSING;
	}

	return TREE_NO_ERROR;
}

BinaryTreeStatusCode Calculator(Tree* tree) {

	printf("-------------------------------------\n");
	printf(BLUE("Set using variables values:")"\n");
	for (size_t i = 0; i < AMOUNT_OF_VARIABLES; i++) {
		if (var_name_table[i].status == VAR_STATUS_USING) {
			printf("\t" YELLOW("%s:")" ", var_name_table[i].symbol);
			scanf("%lg", &var_name_table[i].value);
		}
	}
	printf("-------------------------------------\n");
	printf(GREEN("Eval result: %lg\n"), Eval(tree->root));

	return TREE_NO_ERROR;
}

Number_t Eval(Node_t* node) {
	if (!node)
		TREE_ERROR_CHECK(TREE_NULL_POINTER);

	switch (node->type) {
		case NUM: return node->data.val_num;
		case VAR: return VarNameTableGetValue(node->data.val_var);
		case OP: {
			switch (node->data.val_op) {
				case ADD: 					return Eval(node->left) + Eval(node->right);
				case SUB: 					return Eval(node->left) - Eval(node->right);
				case MUL: 					return Eval(node->left) * Eval(node->right);
				case DIV: 					return Eval(node->left) / Eval(node->right);
				case SIN: 					return sin(Eval(node->left));
				case COS: 					return cos(Eval(node->left));
				case SQRT:  				return sqrt(Eval(node->left));
				case POW:					return pow(Eval(node->left), Eval(node->right));
				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default:					TREE_ERROR_CHECK(TREE_INVALID_TYPE);
			}
		}
		case UNW:
		default:  	TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}
}
