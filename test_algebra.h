#pragma once 

#include "types.h"

typedef struct _test_expr {
	char *text;
} TestExpr;

const FieldInfo *get_test_expr_info(void);

void test_expr_set_text(TestExpr *expr, const char *text);
void test_expr_clear(TestExpr *expr);

char *test_expr_make_name(char prefix, size_t row, size_t col);
char *test_expr_make_op(const char *op, const char *left, const char *right);
