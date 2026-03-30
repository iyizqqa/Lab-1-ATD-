#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix.h"
#include "types.h"
#include "test_algebra.h"

static void clear_test_matrix(Matrix *m)
{
	if (!m || !m->data)
	{
		return;
	}

	for (size_t i = 0; i < m->size; i++)
	{
		for (size_t j = 0; j < m->size; j++)
		{
			test_expr_clear((TestExpr *)get_element(m, i, j));
		}
	}

	clear_matrix(m);
}

static int fill_matrix(Matrix *m, char prefix)
{
	for (size_t i = 0; i < m->size; i++)
	{
		for (size_t j = 0; j < m->size; j++)
		{
			char *name = test_expr_make_name(prefix, i, j);
			if (!name)
			{
				return 0;
			}

			TestExpr *elem = (TestExpr *)get_element(m, i, j);
			test_expr_set_text(elem, name);
			free(name);
		}
	}

	return 1;
}

static int check_add(const Matrix *res)
{
	for (size_t i = 0; i < res->size; i++)
	{
		for (size_t j = 0; j < res->size; j++)
		{
			char *left = test_expr_make_name('a', i, j);
			char *right = test_expr_make_name('b', i, j);

			if (!left || !right)
			{
				free(left);
				free(right);
				return 0;
			}

			char *expected = test_expr_make_op("+", left, right);
			free(left);
			free(right);

			if (!expected)
			{
				return 0;
			}

			TestExpr *elem = (TestExpr *)get_element((Matrix *)res, i, j);
			if (!elem || !elem->text || strcmp(elem->text, expected) != 0)
			{
				free(expected);
				return 0;
			}

			free(expected);
		}
	}

	return 1;
}

static int check_scalar(const Matrix *res)
{
	for (size_t i = 0; i < res->size; i++)
	{
		for (size_t j = 0; j < res->size; j++)
		{
			char *left = test_expr_make_name('a', i, j);
			if (!left)
			{
				return 0;
			}

			char *expected = test_expr_make_op("*", left, "s");
			free(left);

			if (!expected)
			{
				return 0;
			}

			TestExpr *elem = (TestExpr *)get_element((Matrix *)res, i, j);
			if (!elem || !elem->text || strcmp(elem->text, expected) != 0)
			{
				free(expected);
				return 0;
			}

			free(expected);
		}
	}

	return 1;
}

static int check_multiply(const Matrix *res)
{
	for (size_t i = 0; i < res->size; i++)
	{
		for (size_t j = 0; j < res->size; j++)
		{
			char *sum = NULL;

			for (size_t k = 0; k < res->size; k++)
			{
				char *left = test_expr_make_name('a', i, k);
				char *right = test_expr_make_name('b', k, j);

				if (!left || !right)
				{
					free(left);
					free(right);
					free(sum);
					return 0;
				}

				char *term = test_expr_make_op("*", left, right);
				free(left);
				free(right);

				if (!term)
				{
					free(sum);
					return 0;
				}

				if (!sum)
				{
					sum = term;
				}
				else
				{
					char *new_sum = test_expr_make_op("+", sum, term);
					free(sum);
					free(term);

					if (!new_sum)
					{
						return 0;
					}

					sum = new_sum;
				}
			}

			TestExpr *elem = (TestExpr *)get_element((Matrix *)res, i, j);
			if (!sum || !elem || !elem->text || strcmp(elem->text, sum) != 0)
			{
				free(sum);
				return 0;
			}

			free(sum);
		}
	}

	return 1;
}

static int test_add(size_t n)
{
	Matrix a = {0, NULL, NULL};
	Matrix b = {0, NULL, NULL};
	Matrix res = {0, NULL, NULL};

	if (!init_matrix(&a, n, get_test_expr_info()))
	{
		return 0;
	}

	if (!init_matrix(&b, n, get_test_expr_info()))
	{
		clear_test_matrix(&a);
		return 0;
	}

	if (!fill_matrix(&a, 'a') || !fill_matrix(&b, 'b'))
	{
		clear_test_matrix(&a);
		clear_test_matrix(&b);
		return 0;
	}

	res = add_matrices(&a, &b);
	int ok = res.data && check_add(&res);

	clear_test_matrix(&a);
	clear_test_matrix(&b);
	clear_test_matrix(&res);

	return ok;
}

static int test_scalar_multiply(size_t n)
{
	Matrix a = {0, NULL, NULL};
	Matrix res = {0, NULL, NULL};
	TestExpr scalar = {NULL};

	if (!init_matrix(&a, n, get_test_expr_info()))
	{
		return 0;
	}

	if (!fill_matrix(&a, 'a'))
	{
		clear_test_matrix(&a);
		return 0;
	}

	test_expr_set_text(&scalar, "s");

	res = scalar_multiply_matrix(&a, &scalar);
	int ok = res.data && check_scalar(&res);

	test_expr_clear(&scalar);
	clear_test_matrix(&a);
	clear_test_matrix(&res);

	return ok;
}

static int test_multiply(size_t n)
{
	Matrix a = {0, NULL, NULL};
	Matrix b = {0, NULL, NULL};
	Matrix res = {0, NULL, NULL};

	if (!init_matrix(&a, n, get_test_expr_info()))
	{
		return 0;
	}

	if (!init_matrix(&b, n, get_test_expr_info()))
	{
		clear_test_matrix(&a);
		return 0;
	}

	if (!fill_matrix(&a, 'a') || !fill_matrix(&b, 'b'))
	{
		clear_test_matrix(&a);
		clear_test_matrix(&b);
		return 0;
	}

	res = multiply_matrices(&a, &b);
	int ok = res.data && check_multiply(&res);

	clear_test_matrix(&a);
	clear_test_matrix(&b);
	clear_test_matrix(&res);

	return ok;
}

static int test_different_types(void)
{
	Matrix a = {0, NULL, NULL};
	Matrix b = {0, NULL, NULL};

	if (!init_matrix(&a, 2, get_test_expr_info()))
	{
		return 0;
	}

	if (!init_matrix(&b, 2, get_int_info()))
	{
		clear_test_matrix(&a);
		return 0;
	}

	Matrix res = add_matrices(&a, &b);
	int ok = (res.data == NULL && res.size == 0 && res.info == NULL);

	clear_test_matrix(&a);
	clear_matrix(&b);

	return ok;
}

static void print_result(const char *name, int ok, int *all_ok)
{
	if (ok)
	{
		printf("%s OK\n", name);
	}
	else
	{
		printf("%s FAIL\n", name);
		*all_ok = 0;
	}
}

int main(void)
{
	size_t sizes[] = {1, 2, 3, 7};
	int all_ok = 1;
	char name[64];

	for (size_t i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++)
	{
		snprintf(name, sizeof(name), "test_add(%zu)", sizes[i]);
		print_result(name, test_add(sizes[i]), &all_ok);

		snprintf(name, sizeof(name), "test_multiply(%zu)", sizes[i]);
		print_result(name, test_multiply(sizes[i]), &all_ok);

		snprintf(name, sizeof(name), "test_scalar_multiply(%zu)", sizes[i]);
		print_result(name, test_scalar_multiply(sizes[i]), &all_ok);
	}

	print_result("test_different_types", test_different_types(), &all_ok);

	if (all_ok)
	{
		printf("ALL TESTS PASSED\n");
		return 0;
	}

	printf("SOME TESTS FAILED\n");
	return 1;
}



