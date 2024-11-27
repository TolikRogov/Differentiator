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
			printf("\t" YELLOW("%s")" = ", var_name_table[i].symbol);
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
				case ADD: 	return Eval(node->left) + Eval(node->right);
				case SUB: 	return Eval(node->left) - Eval(node->right);
				case MUL: 	return Eval(node->left) * Eval(node->right);
				case DIV: 	{
								Number_t denominator = Eval(node->right);
								if (!DiffCompareDouble(denominator, 0))
									return Eval(node->left) / denominator;
								else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case SIN: 	return sin(Eval(node->left));
				case COS: 	return cos(Eval(node->left));
				case SQRT:  return sqrt(Eval(node->left));
				case POW:	return pow(Eval(node->left), Eval(node->right));
				case LOG:	{
								Number_t arg1 = Eval(node->left);
								Number_t arg2 = Eval(node->right);
								Number_t denominator = log(arg1);
								if (arg1 > 0 && arg2 > 0 && !DiffCompareDouble(denominator, 0))
									return log(arg2) / denominator;
							  	else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case LN:	{
								Number_t arg = Eval(node->left);
								if (arg > 0)
									return log(arg);
							  	else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default:					TREE_ERROR_CHECK(TREE_INVALID_TYPE);
			}
		}
		case UNW:
		default:  	TREE_ERROR_CHECK(TREE_INVALID_TYPE);
	}
}

Node_t* doCopySubtree(Node_t* node) {

	if (!node)
		return NULL;

	switch (node->type) {
		case NUM: return _NUM(node->data.val_num);
		case VAR: return _VAR(node->data.val_var);
		case OP: {
			switch (node->data.val_op) {
				case ADD: 	return _ADD(cL, cR);
				case SUB: 	return _SUB(cL, cR);
				case MUL: 	return _MUL(cL, cR);
				case DIV: 	return _DIV(cL, cR);
				case POW: 	return _POW(cL, cR);
				case SIN: 	return _SIN(cL);
				case COS: 	return _COS(cL);
				case SQRT:	return _SQRT(cL);
				case LOG:	return _LOG(cL, cR);
				case LN:	return _LN(cL);

				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return NULL;
			}
		}
		case UNW:
		default: return NULL;
	}
}

size_t NumberOfVariablesInSubtree(Node_t* node) {
	return (node->type == VAR ? 1 : 0) + (node->left ? NumberOfVariablesInSubtree(node->left) : 0) + (node->right ? NumberOfVariablesInSubtree(node->right) : 0);
}

Node_t* Differentiation(Node_t* node) {

	if (!node)
		return NULL;

	switch (node->type) {
		case NUM: return _NUM(0);
		case VAR: return _NUM(1);
		case OP: {
			switch (node->data.val_op) {
				case ADD: 	return _ADD(dL, dR);
				case SUB: 	return _SUB(dL, dR);
				case MUL: 	return _ADD(_MUL(dL, cR), _MUL(cL, dR));
				case DIV: 	return _DIV(_SUB(_MUL(dL, cR), _MUL(cL, dR)), _POW(cR, _NUM(2)));
				case POW:
				{
					size_t left_subtree = NumberOfVariablesInSubtree(node->left);
					size_t right_subtree = NumberOfVariablesInSubtree(node->right);

					if (left_subtree && !right_subtree)
						return _MUL(_MUL(cR, _POW(cL, _SUB(cR, _NUM(1)))), dL);
					if (right_subtree && !left_subtree)
						return _MUL(_MUL(cP, _LN(cL)), dR);
					if (left_subtree && right_subtree)
						return _MUL(cP, _ADD(_MUL(dR, _LN(cL)), _MUL(cR, _DIV(dL, cL))));
				}
				case SIN: 	return _MUL(_COS(cL), dL);
				case COS: 	return _MUL(_MUL(_SIN(cL), _NUM(-1)), dL);
				case SQRT:	return _MUL(_DIV(_NUM(1), _MUL(_NUM(2), _SQRT(cL))), dL);
				case LOG:	return _MUL(_DIV(_NUM(1), _MUL(cR, _LN(cL))), dR);
				case LN:	return _MUL(_DIV(_NUM(1), cL), dL);

				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return NULL;
			}
		}
		case UNW:
		default: return NULL;
	}
}

BinaryTreeStatusCode Simplification(Tree* tree) {

	size_t count_of_changes = 0;

	do {
		count_of_changes = 0;
		ConvolutionConstant(tree->root, &count_of_changes);
		TrivialTransformations(tree->root, &count_of_changes);
	} while (count_of_changes);

	return TREE_NO_ERROR;
}

int TrivialTransformations(Node_t* node, size_t* count_of_changes) {

	if (!node)
		return 0;

#define REBINDING(from, to, event, new_data) {									 				 \
	if (node->from->type == NUM && DiffCompareDouble(node->from->data.val_num, event)) {		\
		node->type = node->to->type;															\
		node->data.val_num = new_data;															\
		Node_t* to = node->to;																	\
		Node_t* from = node->from;																\
		node->from = node->to->left;															\
		node->to = node->to->right;																\
		node->to->parent = node->from->parent = node;											\
		if (to) free(to);																		\
		if (from) free(from);																	\
		break;																					\
	}																							\
}

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case ADD: {
					REBINDING(left, right, 0, node->right->data.val_num);
					REBINDING(right, left, 0, node->left->data.val_num);
				}
				case SUB: REBINDING(right, left, 0, node->left->data.val_num);
				case DIV: REBINDING(right, left, 1, node->left->data.val_num);
				case MUL: {
					REBINDING(left, right, 1, node->right->data.val_num);
					REBINDING(right, left, 1, node->left->data.val_num);
					REBINDING(left, right, 0, 0);
					REBINDING(right, left, 0, 0);
				}
				case INVALID_OPERATION:
				default: return 0;
			}
			(*count_of_changes)++;
			return 0;
		}
		case NUM:
		case VAR:
		case UNW:
		default: return 0;
	}
#undef REBINDING
}

int ConvolutionConstant(Node_t* node, size_t* count_of_changes) {

	if (!node)
		return 0;

	switch (node->type) {
		case NUM:	return 1;
		case VAR:	return 0;
		case OP:
		{
			int left = ConvolutionConstant(node->left, count_of_changes);
			int right = ConvolutionConstant(node->right, count_of_changes);
			if ((left && right) || (left && !node->right)) {
				Number_t result = Eval(node);
				node->type = NUM;
				node->data.val_num = result;
				if (node->left)  free(node->left);
				if (node->right) free(node->right);
				node->left = node->right = NULL;
				(*count_of_changes)++;
			}
			return 0;
		}
		case UNW:
		default: return 0;
	}
}
