#include "Differentiator.hpp"

BinaryTreeStatusCode Calculator(Tree* tree, VariableNameTable* var_name_table) {

	printf("-------------------------------------------------------------------------------------------------\n");
	printf("\t\t\t" BLUE("CALCULATOR")"\n");
	printf("-------------------------------------------------------------------------------------------------\n");
	printf(BLUE("Do you want to calculate value of expression? [y/n]:")" ");
	int check = 0;
	while ((check = getchar()) != EOF) {
		getchar();
		if (check == 'y')
			break;
		if (check == 'n')
			return TREE_NO_ERROR;
		printf(RED("Error! Try again")"\n");
		printf(BLUE("Do you want to calculate value of expression? [y/n]:")" ");
	}

	if (NumberOfVarStatusUsingVariables(var_name_table)) {
		printf("-------------------------------------\n");
		printf(BLUE("Set using variables values:")"\n");
	}
	for (size_t i = 0; i < var_name_table->size; i++) {
		if (var_name_table->data[i].status == VAR_STATUS_USING) {
			printf("\t" YELLOW("%s")" = ", var_name_table->data[i].symbol);
			scanf("%lg", &var_name_table->data[i].value);
		}
	}
	printf("-------------------------------------\n");
	printf(GREEN("Eval result: %lg\n"), Eval(tree->root, var_name_table));

	return TREE_NO_ERROR;
}

Number_t Eval(Node_t* node, VariableNameTable* var_name_table) {

	if (!node)
		TREE_ERROR_MESSAGE(TREE_NULL_POINTER);

	switch (node->type) {
		case NUM: return node->data.val_num;
		case VAR: return VarNameTableGetValue(var_name_table, node->data.val_var);
		case OP: {
			switch (node->data.val_op) {
				case ADD: 	return eL + eR;
				case SUB: 	return eL - eR;
				case MUL: 	return eL * eR;
				case DIV: 	{
								Number_t denominator = eR;
								if (!DiffCompareDouble(denominator, 0))
									return eL / denominator;
								else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case SIN: 	return sin(eL);
				case COS: 	return cos(eL);
				case SQRT:  return sqrt(eL);
				case POW:	return pow(eL, eR);
				case LOG:	{
								Number_t arg1 = eL;
								Number_t arg2 = eR;
								Number_t denominator = log(arg1);
								if (arg1 > 0 && arg2 > 0 && !DiffCompareDouble(denominator, 0))
									return log(arg2) / denominator;
							  	else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case LN:	{
								Number_t arg = eL;
								if (arg > 0)
									return log(arg);
							  	else { TREE_ERROR_MESSAGE(TREE_DEFINITION_AREA_ERROR); return 0; }
							}
				case EXP:   return exp(eL);
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
				case EXP:	return _EXP(cL);

				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return NULL;
			}
		}
		case UNW:
		default: return NULL;
	}
}

Node_t* doDifferentiation(Node_t* node, VariableNameTable* var_name_table) {

	if (!node)
		return NULL;

	Node_t* new_node = NULL;

	switch (node->type) {
		case NUM: return _NUM(0);
		case VAR: {
			if (var_name_table->data[node->data.val_var].state == VAR_DIFF_STATUS_VAR)
				return _NUM(1);
			else
				return _NUM(0);
		}
		case OP: {
			switch (node->data.val_op) {
				case ADD: 	{ new_node = _ADD(dL, dR); break; }
				case SUB: 	{ new_node = _SUB(dL, dR); break; }
				case MUL: 	{ new_node = _ADD(_MUL(dL, cR), _MUL(cL, dR)); break; }
				case DIV: 	{ new_node = _DIV(_SUB(_MUL(dL, cR), _MUL(cL, dR)), _POW(cR, _NUM(2))); break; }
				case POW:
				{
					size_t left_subtree = NumberOfVariablesInSubtree(node->left);
					size_t right_subtree = NumberOfVariablesInSubtree(node->right);

					if (left_subtree && !right_subtree) { new_node = _MUL(_MUL(cR, _POW(cL, _SUB(cR, _NUM(1)))), dL); break; }
					if (right_subtree && !left_subtree) { new_node = _MUL(_MUL(cP, _LN(cL)), dR); break; }
					if (left_subtree && right_subtree)  { new_node = _MUL(cP, _ADD(_MUL(dR, _LN(cL)), _MUL(cR, _DIV(dL, cL)))); break; }
				}
				case SIN: 	{ new_node = _MUL(_COS(cL), dL); break; }
				case COS: 	{ new_node = _MUL(_MUL( _NUM(-1), _SIN(cL)), dL); break; }
				case SQRT:	{ new_node = _MUL(_DIV(_NUM(1), _MUL(_NUM(2), _SQRT(cL))), dL); break; }
				case LOG:	{ new_node = _DIV(_SUB(_MUL(_DIV(dR, cR), _LN(cL)), _MUL(_LN(cR), _DIV(dL, cL))), _POW(_LN(cL), _NUM(2))); break; }
				case LN:	{ new_node = _MUL(_DIV(_NUM(1), cL), dL); break; }
				case EXP:	{ new_node = _MUL(cP, dL); break; }

				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return NULL;
			}
			Simplification(new_node, var_name_table);
			LaTexSubtreeDifferential(node, new_node, var_name_table);
			return new_node;
		}
		case UNW:
		default: return NULL;
	}
}

BinaryTreeStatusCode Simplification(Node_t* subtree_root, VariableNameTable* var_name_table) {

	size_t count_of_changes = 0;

	do {
		count_of_changes = 0;
		ConvolutionConstant(subtree_root, &count_of_changes, var_name_table);
		TrivialTransformations(subtree_root, &count_of_changes);
	} while (count_of_changes);

	return TREE_NO_ERROR;
}

int TrivialTransformations(Node_t* node, size_t* count_of_changes) {

	if (!node)
		return SIMPLIFY_IMPOSSIBLE;

	if (node->left) TrivialTransformations(node->left, count_of_changes);
	if (node->right) TrivialTransformations(node->right, count_of_changes);

#define REBINDING(from, to, event, new_data) {									 				 \
	if (node->from->type == NUM && DiffCompareDouble(node->from->data.val_num, event)) {		\
		node->type = node->to->type;															\
		SetNodeValue(node, new_data);															\
		Node_t* left = doCopySubtree(node->to->left);											\
		Node_t* right = doCopySubtree(node->to->right);											\
		TreeDtor(node->left); TreeDtor(node->right);											\
		node->left = left;	  node->right = right;												\
		(*count_of_changes)++;																	\
		return SIMPLIFY_ACCESS;																	\
	}																							\
}

#define NUMBER_AS_RESULT(event_pos, event, result) {									 				 	 \
	if ((node->event_pos->type == NUM && DiffCompareDouble(node->event_pos->data.val_num, event))) {		\
		node->type = NUM;																					\
		node->data.val_num = result;																		\
		TreeDtor(node->left); TreeDtor(node->right);														\
		node->left = node->right = NULL;																	\
		(*count_of_changes)++;																				\
		return SIMPLIFY_ACCESS;																				\
	}																										\
}

#define CHANGE_OPERATION(event_pos, event, new_op, new_data) {												 \
	if ((node->event_pos->type == NUM && DiffCompareDouble(node->event_pos->data.val_num, event))) {		\
		node->data.val_op = new_op;																			\
		node->event_pos->data.val_num = new_data;															\
		(*count_of_changes)++;																				\
		return SIMPLIFY_ACCESS;																				\
	}																										\
}

	switch (node->type) {
		case OP: {
			switch (node->data.val_op) {
				case ADD: {
					REBINDING(left, right, 0, node->right->data);
					REBINDING(right, left, 0, node->left->data);
					break;
				}
				case SUB: {
					REBINDING(right, left, 0, node->left->data);
					CHANGE_OPERATION(left, 0, MUL, -1);
					break;
				}
				case DIV: {
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					break;
				}
				case MUL: {
					REBINDING(left, right, 1, node->right->data);
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(left, 0, 0);
					NUMBER_AS_RESULT(right, 0, 0);
					break;
				}
				case POW: {
					REBINDING(left, right, 1, node->right->data);
					REBINDING(right, left, 1, node->left->data);
					NUMBER_AS_RESULT(right, 0, 1);
					NUMBER_AS_RESULT(left, 1, 1);
					break;
				}
				case SIN:
				case COS:
				case LOG:
				case LN:
				case SQRT:
				case EXP:
				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return SIMPLIFY_IMPOSSIBLE;
			}
		}
		case NUM:
		case VAR:
		case UNW:
		default: return SIMPLIFY_IMPOSSIBLE;
	}
#undef REBINDING
#undef NUMBER_AS_RESULT
#undef CHANGE_OPERATION
}

int ConvolutionConstant(Node_t* node, size_t* count_of_changes, VariableNameTable* var_name_table) {

	if (!node)
		return SIMPLIFY_IMPOSSIBLE;

	switch (node->type) {
		case NUM:	return SIMPLIFY_ACCESS;
		case VAR:	return SIMPLIFY_IMPOSSIBLE;
		case OP:
		{
			switch (node->data.val_op) {
				case ADD:
				case SUB:
				case MUL:
				case POW:
				case DIV:
				case EXP:
				case SQRT: { break; }

				case LOG:
				case LN:
				case SIN:
				case COS:
				case AMOUNT_OF_OPERATIONS:
				case INVALID_OPERATION:
				default: return SIMPLIFY_IMPOSSIBLE;
			}

			int left = ConvolutionConstant(node->left, count_of_changes, var_name_table);
			int right = ConvolutionConstant(node->right, count_of_changes, var_name_table);
			if ((left && right) || (left && !node->right)) {
				Number_t result = Eval(node, var_name_table);
				node->type = NUM;
				SetNodeValue(node, {.val_num = result});
				if (node->right) { TreeDtor(node->right); node->right = NULL; }
				if (node->left)  { TreeDtor(node->left); node->left = NULL;  }
				(*count_of_changes)++;
				return SIMPLIFY_ACCESS;
			}
		}
		case UNW:
		default: return SIMPLIFY_IMPOSSIBLE;
	}
}
