#pragma once

#include "Differentiator.hpp"

#define DIFF_DATA_DIR_			"data/"
#define DIFF_EXPRESSION_FILE_	DIFF_DATA_DIR_ "expression.txt"

BinaryTreeStatusCode ReadFromFileToBuffer();
BinaryTreeStatusCode RecursionReadExpression(char** buffer);
