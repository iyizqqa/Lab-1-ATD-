#include "test_algebra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_text(const char *text)
{
	size_t len;
	char *res;

	if (!text)
	{
		return NULL;
	}

	len = strlen(text);
	res = malloc(len + 1);
	if (!res)
	{
		return NULL;
	}

	strcpy(res, text);
	return res;
}

char *test_expr_make_name(char prefix, size_t row, size_t col)
{
	int len;
	char *name;

	len = snprintf(NULL, 0, "%c%zu_%zu", prefix, row, col);
	if (len < 0)
	{
		return NULL;
	}

	name = malloc((size_t)len + 1);
	if (!name)
	{
		return NULL;
	}

	snprintf(name, (size_t)len + 1, "%c%zu_%zu", prefix, row, col);
	return name;
}

char *test_expr_make_op(const char *op, const char *left, const char *right)
{
	int len;
	char *expr;

	len = snprintf(NULL, 0, "(%s %s %s)", op, left, right);
	if (len < 0)
	{
		return NULL;
	}

	expr = malloc((size_t)len + 1);
	if (!expr)
	{
		return NULL;
	}

	snprintf(expr, (size_t)len + 1, "(%s %s %s)", op, left, right);
	return expr;
}

void test_expr_set_text(TestExpr *expr, const char *text)
{
	if (!expr)
	{
		return;
	}

	free(expr->text);
	expr->text = copy_text(text);
}

void test_expr_clear(TestExpr *expr)
{
	if (!expr)
	{
		return;
	}

	free(expr->text);
	expr->text = NULL;
}

static int is_zero(const TestExpr *expr)
{
	return expr && expr->text && strcmp(expr->text, "0") == 0;
}

static void test_expr_add(const void *a, const void *b, void *res)
{
	const TestExpr *left = (const TestExpr *)a;
	const TestExpr *right = (const TestExpr *)b;
	TestExpr *result = (TestExpr *)res;
	char *text;

	if (is_zero(left))
	{
		test_expr_set_text(result, right->text);
		return;
	}

	if (is_zero(right))
	{
		test_expr_set_text(result, left->text);
		return;
	}

	text = test_expr_make_op("+", left->text, right->text);
	free(result->text);
	result->text = text;
}

static void test_expr_multiply(const void *a, const void *b, void *res)
{
	const TestExpr *left = (const TestExpr *)a;
	const TestExpr *right = (const TestExpr *)b;
	TestExpr *result = (TestExpr *)res;
	char *text;

	text = test_expr_make_op("*", left->text, right->text);
	free(result->text);
	result->text = text;
}

static void test_expr_set_zero(void *a)
{
	TestExpr *expr = (TestExpr *)a;

	free(expr->text);
	expr->text = copy_text("0");
}

static void test_expr_print(const void *a)
{
	const TestExpr *expr = (const TestExpr *)a;

	if (!expr || !expr->text)
	{
		printf("null");
		return;
	}

	printf("%s", expr->text);
}

static void test_expr_read(void *a)
{
	TestExpr *expr = (TestExpr *)a;
	char buffer[256];

	if (!expr)
	{
		return;
	}

	scanf("%255s", buffer);
	test_expr_set_text(expr, buffer);
}

const FieldInfo *get_test_expr_info(void)
{
	static const FieldInfo info = {
		sizeof(TestExpr),
		"test_expr",
		test_expr_add,
		test_expr_multiply,
		test_expr_set_zero,
		test_expr_print,
		test_expr_read
	};

	return &info;
}

















