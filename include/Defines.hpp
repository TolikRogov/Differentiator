#pragma once

#define _NUM(val) CreateNode(NUM, val, NULL, NULL, NULL)
#define _VAR(let) CreateNode(VAR, let, NULL, NULL, NULL)

enum OpNames {
	ADD,
	SUB,
	MUL,
	DIV,
	SIN,
	COS,
	SQRT,
	POW,
	AMOUNT_OF_OPERATIONS
};
