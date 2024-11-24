#pragma once

#define _NUM(val) 			CreateNode(NUM, {.val_num = val}, NULL, NULL, NULL)
#define _VAR(let) 			CreateNode(VAR, {.val_var = let}, NULL, NULL, NULL)

#define _ADD(left, right) 	CreateNode(OP,  {.val_op = ADD}, left, right, NULL)
#define _SUB(left, right) 	CreateNode(OP,  {.val_op = SUB}, left, right, NULL)
#define _MUL(left, right) 	CreateNode(OP,  {.val_op = MUL}, left, right, NULL)
#define _DIV(left, right) 	CreateNode(OP,  {.val_op = DIV}, left, right, NULL)
#define _POW(left, right) 	CreateNode(OP,  {.val_op = POW}, left, right, NULL)
#define _SIN(left) 			CreateNode(OP,  {.val_op = SIN}, left, NULL, NULL)
#define _COS(left) 			CreateNode(OP,  {.val_op = COS}, left, NULL, NULL)
#define _SQRT(left) 		CreateNode(OP,  {.val_op = SQRT}, left, NULL, NULL)

#define dL Differentiation(node->left)
#define dR Differentiation(node->right)
#define cL doCopySubtree(node->left)
#define cR doCopySubtree(node->right)
