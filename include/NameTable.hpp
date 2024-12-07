#pragma once

#include "Operations.hpp"

Operation op_name_table[AMOUNT_OF_OPERATIONS] = {
	[ADD] 			= 	{ .num = ADD, 			.math_symbol = "+", 	.tex_symbol = "+", 			},
	[SUB] 			= 	{ .num = SUB, 			.math_symbol = "-", 	.tex_symbol = "-",			},
	[MUL] 			= 	{ .num = MUL, 			.math_symbol = "*", 	.tex_symbol = "\\cdot", 	},
	[DIV] 			= 	{ .num = DIV, 			.math_symbol = "/", 	.tex_symbol = "frac", 		},
	[SIN] 			= 	{ .num = SIN, 			.math_symbol = "sin", 	.tex_symbol = "\\sin", 		},
	[COS] 			= 	{ .num = COS, 			.math_symbol = "cos", 	.tex_symbol = "\\cos", 		},
	[SQRT]  		= 	{ .num = SQRT, 			.math_symbol = "sqrt", 	.tex_symbol = "\\sqrt[]",	},
	[POW] 			= 	{ .num = POW, 			.math_symbol = "^", 	.tex_symbol = "^", 			},
	[LOG] 			= 	{ .num = LOG,			.math_symbol = "log",	.tex_symbol = "log_",		},
	[LN]			= 	{ .num = LN,			.math_symbol = "ln",	.tex_symbol = "\\ln",		},
	[EXP]			=	{ .num = EXP,			.math_symbol = "exp",	.tex_symbol = "e^",			},
	[OPEN_BRACKET] 	=	{ .num = OPEN_BRACKET,	.math_symbol = "(",		.tex_symbol = "(",			},
	[CLOSE_BRACKET] =	{ .num = CLOSE_BRACKET,	.math_symbol = ")",		.tex_symbol = ")",			},
	[EOP]			=	{ .num = EOP,			.math_symbol = "$",		.tex_symbol = "$",			}};
