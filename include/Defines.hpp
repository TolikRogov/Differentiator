#pragma once

#define _NUM(val) CreateNode(NUM, val, NULL, NULL, NULL)
#define _VAR(let) CreateNode(VAR, let, NULL, NULL, NULL)

enum OpNames: int {
	DIV = '/',
	ADD = '+',
	SUB = '-',
	MUL = '*',
};
