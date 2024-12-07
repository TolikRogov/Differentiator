#pragma once

#define _NUM(val) 			CreateNode(NUM, {.val_num = val}, NULL, NULL, NULL)
#define _VAR(let) 			CreateNode(VAR, {.val_var = let}, NULL, NULL, NULL)

#define _ADD(left, right) 	CreateNode(OP,  {.val_op = ADD}, 	left, right, NULL)
#define _SUB(left, right) 	CreateNode(OP,  {.val_op = SUB}, 	left, right, NULL)
#define _MUL(left, right) 	CreateNode(OP,  {.val_op = MUL}, 	left, right, NULL)
#define _DIV(left, right) 	CreateNode(OP,  {.val_op = DIV}, 	left, right, NULL)
#define _POW(left, right) 	CreateNode(OP,  {.val_op = POW}, 	left, right, NULL)
#define _SIN(left) 			CreateNode(OP,  {.val_op = SIN}, 	left, NULL, NULL)
#define _COS(left) 			CreateNode(OP,  {.val_op = COS}, 	left, NULL, NULL)
#define _SQRT(left) 		CreateNode(OP,  {.val_op = SQRT},	left, NULL, NULL)
#define _LOG(left, right)	CreateNode(OP,  {.val_op = LOG}, 	left, right, NULL)
#define _LN(left)			CreateNode(OP,	{.val_op = LN},		left, NULL, NULL)
#define _EXP(left)			CreateNode(OP,	{.val_op = EXP},	left, NULL, NULL)

#define dL doDifferentiation(node->left, id_name_table, IdNumber)
#define dR doDifferentiation(node->right, id_name_table, IdNumber)

#define cL doCopySubtree(node->left)
#define cR doCopySubtree(node->right)
#define cP doCopySubtree(node)

#define eR Eval(node->right, id_name_table)
#define eL Eval(node->left, id_name_table)
