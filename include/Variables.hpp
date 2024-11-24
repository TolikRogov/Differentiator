#pragma once

enum VarNum {
	INVALID_VARIABLE = -1,
	VAR_X,
	VAR_Y,
	VAR_Z,
	AMOUNT_OF_VARIABLES
};

enum VarStatus {
	VAR_STATUS_USING,
	VAR_STATUS_DISUSING
};

struct Variable {
	VarNum num;
	const char* symbol;
	Number_t value;
	VarStatus status;
};

extern Variable var_name_table[];
