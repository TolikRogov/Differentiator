#pragma once

enum OpNum {
	INVALID_OPERATION = -1,
	ADD,
	SUB,
	MUL,
	DIV,
	SIN,
	COS,
	SQRT,
	POW,
	LOG,
	LN,
	EXP,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	EOP,
	AMOUNT_OF_OPERATIONS
};

struct Operation {
	OpNum num;
	const char* math_symbol;
	const char* tex_symbol;
};

extern Operation op_name_table[];
