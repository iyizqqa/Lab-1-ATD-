#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>

int init_matrix(Matrix *m, size_t n, const FieldInfo *info)
{
	size_t size_of_matrix;

	if (!m || !info || n == 0)
	{
		return 0;
	}

	if (m->data)
	{
		return 0;
	}

	size_of_matrix = n * n;

	m->data = calloc(size_of_matrix, info->elem_size);
	if (!m->data)
	{
		return 0;
	}

	m->size = n;
	m->info = info;

	for (size_t i = 0; i < size_of_matrix; i++)
	{
		void *elem = (char *)m->data + i * info->elem_size;
		info->set_zero(elem);
	}

	return 1;
}

void clear_matrix(Matrix *m)
{
	if (!m)
	{
		return;
	}

	free(m->data);	// освобождаем память где лежали элементы
	m->data = NULL; // убираем старый указатель
	m->size = 0;	// сбрасываем размер
	m->info = NULL; // сбрасываем описание типа
}

void *get_element(Matrix *m, size_t row, size_t col)
{
	if (!m || !m->data || row >= m->size || col >= m->size)
	{
		return NULL;
	}
	return (char *)m->data + (row * m->size + col) * m->info->elem_size;
}

const void *get_element_const(const Matrix *m, size_t row, size_t col)
{
	if (!m || !m->data || row >= m->size || col >= m->size)
	{
		return NULL;
	}

	return (const char *)m->data + (row * m->size + col) * m->info->elem_size;
}

Matrix add_matrices(const Matrix *a, const Matrix *b)
{
	Matrix res = {0, NULL, NULL};

	if (!a || !b)
	{
		return res;
	}

	if (a->size != b->size)
	{
		fprintf(stderr, "Error: sizes of matrices don't match (%zu != %zu)\n", a->size, b->size);
		return res;
	}

	if (a->info != b->info)
	{
		fprintf(stderr, "Error: incompetible types (%s vs %s)\n", a->info->type_name, b->info->type_name);
		return res;
	}

	if (!init_matrix(&res, a->size, a->info))
	{
		return (Matrix){0, NULL, NULL};
	}

	for (size_t i = 0; i < a->size; i++)
	{
		for (size_t j = 0; j < a->size; j++)
		{
			const void *a_elem = get_element_const(a, i, j);
			const void *b_elem = get_element_const(b, i, j);
			void *res_elem = get_element(&res, i, j);

			res.info->add(a_elem, b_elem, res_elem);
		}
	}

	return res;
}

Matrix multiply_matrices(const Matrix *a, const Matrix *b)
{
	Matrix res = {0, NULL, NULL};
	void *tmp = NULL;

	if (!a || !b)
	{
		return res;
	}

	if (a->size != b->size)
	{
		fprintf(stderr, "Error: sizes of matrices don't match (%zu != %zu)\n", a->size, b->size);
		return res;
	}

	if (a->info != b->info)
	{
		fprintf(stderr, "Error: incompetible types (%s vs %s)\n", a->info->type_name, b->info->type_name);
		return res;
	}

	if (!init_matrix(&res, a->size, a->info))
	{
		return (Matrix){0, NULL, NULL};
	}

	// выделяем временный буфер один раз и переиспользуем

	tmp = malloc(a->info->elem_size);
	if (!tmp)
	{
		clear_matrix(&res);
		return (Matrix){0, NULL, NULL};
	}

	// C[i][j] = sum(A[i][k] * B[k][j])
	for (size_t i = 0; i < a->size; i++)
	{
		for (size_t j = 0; j < a->size; j++)
		{
			void *res_elem = get_element(&res, i, j); // получаем адрес элемента результата
			res.info->set_zero(res_elem);			  // зануляем накопитель суммы

			for (size_t k = 0; k < a->size; k++)
			{
				const void *a_elem = get_element_const(a, i, k);
				const void *b_elem = get_element_const(b, k, j);

				a->info->multiply(a_elem, b_elem, tmp);
				a->info->add(res_elem, tmp, res_elem);
			}
		}
	}

	free(tmp);
	return res;
}

Matrix scalar_multiply_matrix(const Matrix *a, const void *scalar)
{
	Matrix res = {0, NULL, NULL};

	if (!a || !scalar)
	{
		return res;
	}

	if (!init_matrix(&res, a->size, a->info))
	{
		return (Matrix){0, NULL, NULL};
	}

	for (size_t i = 0; i < a->size; i++)
	{
		for (size_t j = 0; j < a->size; j++)
		{
			const void *a_elem = get_element_const(a, i, j);
			void *res_elem = get_element(&res, i, j);

			res.info->scalar_multiply(a_elem, scalar, res_elem);
		}
	}

	return res;
}

void input_matrix(Matrix *m)
{
	if (!m || !m->data)
	{
		return;
	}

	printf("Input matrix %zux%zu (%s):\n", m->size, m->size, m->info->type_name);

	for (size_t i = 0; i < m->size; i++)
	{
		for (size_t j = 0; j < m->size; j++)
		{
			printf("Element [%zu][%zu]: ", i, j);
			void *elem = get_element(m, i, j);
			m->info->read(elem);
		}
	}
}

void output_matrix(const Matrix *m)
{
	if (!m || !m->data)
	{
		printf("Null matrix\n");
		return;
	}

	for (size_t i = 0; i < m->size; i++)
	{
		printf("| ");
		for (size_t j = 0; j < m->size; j++)
		{
			const void *elem = get_element_const(m, i, j);
			m->info->print(elem);
			printf(" ");
		}
		printf("|\n");
	}
}
