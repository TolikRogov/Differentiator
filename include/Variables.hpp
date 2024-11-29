#pragma once

enum VarNum {
	INVALID_VARIABLE = -1,
	VAR_X,
	VAR_Y,
	VAR_Z,
	AMOUNT_OF_VARIABLES
};

enum VarStatus {
	VAR_STATUS_USING	= 1,
	VAR_STATUS_DISUSING	= 0,
};

enum VarDiffStatus {
	VAR_DIFF_STATUS_NUM = 0,
	VAR_DIFF_STATUS_VAR = 1,
};

struct Variable {
	VarNum num;
	const char* symbol;
	Number_t value;
	VarStatus status;
	VarDiffStatus state;
};

extern Variable var_name_table[];
