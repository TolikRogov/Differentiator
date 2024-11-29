#pragma once

#include "Variables.hpp"
#include "Operations.hpp"

Variable var_name_table[AMOUNT_OF_VARIABLES] = {
	[VAR_X] 	= 	{ .num = VAR_X,	.symbol = "x",	.value = 0,	.status = VAR_STATUS_DISUSING },
	[VAR_Y] 	= 	{ .num = VAR_Y, .symbol = "y", 	.value = 0, .status = VAR_STATUS_DISUSING },
	[VAR_Z] 	= 	{ .num = VAR_Z, .symbol = "z", 	.value = 0, .status = VAR_STATUS_DISUSING } };

Operation op_name_table[AMOUNT_OF_OPERATIONS] = {
	[ADD] 	= 	{ .num = ADD, 	.math_symbol = "+", 	.tex_symbol = "+", 			},
	[SUB] 	= 	{ .num = SUB, 	.math_symbol = "-", 	.tex_symbol = "-",			},
	[MUL] 	= 	{ .num = MUL, 	.math_symbol = "*", 	.tex_symbol = "\\cdot", 	},
	[DIV] 	= 	{ .num = DIV, 	.math_symbol = "/", 	.tex_symbol = "frac", 		},
	[SIN] 	= 	{ .num = SIN, 	.math_symbol = "sin", 	.tex_symbol = "\\sin", 		},
	[COS] 	= 	{ .num = COS, 	.math_symbol = "cos", 	.tex_symbol = "\\cos", 		},
	[SQRT]  = 	{ .num = SQRT, 	.math_symbol = "sqrt", 	.tex_symbol = "\\sqrt[]",	},
	[POW] 	= 	{ .num = POW, 	.math_symbol = "^", 	.tex_symbol = "^", 			},
	[LOG] 	= 	{ .num = LOG,	.math_symbol = "log",	.tex_symbol = "log_",		},
	[LN]	= 	{ .num = LN,	.math_symbol = "ln",	.tex_symbol = "\\ln",		},
	[EXP]	=	{ .num = EXP,	.math_symbol = "exp",	.tex_symbol = "e^",			} };
