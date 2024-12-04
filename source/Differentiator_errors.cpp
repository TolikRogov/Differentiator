#include "Differentiator_errors.hpp"

const char* BinaryTreeErrorsMessenger(BinaryTreeStatusCode status) {
	switch(status) {
		case TREE_NO_ERROR:					return "TREE ERROR - NO ERROR";
		case TREE_ALLOC_ERROR:				return "TREE ERROR - MEMORY ALLOCATION ERROR";
		case TREE_NULL_POINTER:				return "TREE ERROR - POINTER IS NULL";
		case TREE_READ_ERROR:				return "TREE ERROR - ERROR READ FROM FILE";
		case TREE_INVALID_TYPE:				return "TREE ERROR - INVALID TYPE OF INPUT ELEMENT";
		case TREE_BASE_SYNTAX_ERROR:		return "TREE ERROR - INVALID SYNTAX IN DATA BASE";
		case TREE_LATEX_SYNTAX_ERROR:		return "TREE ERROR - LATEX SYNTAX IS UNVALID";
		case TREE_EXPRESSION_SYNTAX_ERROR:	return "TREE ERROR - INVALID ELEMENT OF MATH EXPRESSION";
		case TREE_DEFINITION_AREA_ERROR:	return "TREE ERROR - DISSATISFACTION WITH DEFINITION AREA";
		case TREE_TAYLOR_CONDITION_ERROR:	return "TREE ERROR - WRONG CONDITION FOR GET TAYLOR";
		case TREE_ROOT_IS_UNKNOWN:			return "TREE ERROR - ROOT IS UNKOWN";
		case TREE_WRONG_TAYLOR_DEGREE:		return "TREE ERROR - WRONG TAYLOR DEGREE";
		case TREE_NODE_NON_EXISTENT:		return "TREE ERROR - NODE IS NON EXISTENT";
		case TREE_NODE_ALREADY_EXISTENT:	return "TREE ERROR - NODE HAS ALREADY BEEN IN TREE";
		case TREE_NOTHING_TO_READ:			return "TREE ERROR - EXPRESSION FILE HASN'T READABLE INFORMATION";
		case TREE_FILE_OPEN_ERROR:			return "TREE ERROR - FILE WAS NOT OPENED";
		case TREE_FILE_CLOSE_ERROR:			return "TREE ERROR - FILE WAS NOT CLOSED";
		case TREE_DIR_CLOSE_ERROR:			return "TREE ERROR - DIRECTORY WAS NOT CLOSED";
		case TREE_UNDEFINED_ERROR:
		default: 							return "UNDEFINED ERROR";
	}
}

int StrCmp(const char* str1, const char* str2) {

	if (!str1 || !str2)
		return -1;

	size_t i = 0;
	for (; *(str1 + i) != '\0' && *(str2 + i) != '\0'; i++) {
		if (*(str1 + i) != *(str2 + i))
			break;
	}
	return (*(str1 + i) - *(str2 + i));
}

int DiffCompareDouble(double var1, double var2) {
	return (fabs(var1 - var2) <= DIFF_EPS);
}

int Factorial(int n) {
	if (n == 1)
		return n;
	return n * Factorial(n - 1);
}
