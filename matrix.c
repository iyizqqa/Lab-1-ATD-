#include "matrix.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>


Matrix* create_matrix(size_t n, const FieldInfo* info) {
	if (n == 0 || info == NULL) return NULL;

	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
	if (!m) return NULL;

	m->size = n;
	m->info = info;

	m->data = calloc(n*n, info->elem_size);
	if (!m->data) {
		free(m);
		return NULL;
	}

	return m;
}

void destroy_matrix(Matrix** m) {
	if (m == NULL || *m == NULL) return;

	free((*m)->data);
	free(*m);

	*m = NULL;
}



void* get_element(const Matrix* m, size_t row, size_t col) {
	if (!m || row >= m->size || col >= m->size) return NULL;

	return (char*)m->data + (row * m->size + col) * m->info->elem_size;
}



Matrix* add_matrices(const Matrix* a, const Matrix* b) {
	if (!a || !b) return NULL;

	if (a->size != b->size) {
		fprintf(stderr, "Error: sizes of matrices don't match (%zu != %zu)\n", a->size, b->size);
		return NULL;
	}
	

	if (a->info != b->info) {
		fprintf(stderr, "Error: incompetible types (%s vs %s)\n", a->info->type_name, b->info->type_name);
		return NULL;
	}

	
	Matrix* res = create_matrix(a->size, a->info);
	if (!res) return NULL;

	for (size_t i = 0; i < a->size; i++) {
		for (size_t j = 0; j < a->size; j++) {
			void* res_elem = get_element(res, i, j);
			const void* a_elem = get_element(a, i, j);
			const void* b_elem = get_element(b, i, j);

			res->info->add(a_elem, b_elem, res_elem);
		}
	}
	
	return res;
}


Matrix* multiply_matrices(const Matrix* a, const Matrix* b) {
        if (!a || !b) return NULL;

        if(a->size != b->size) {
                fprintf(stderr, "Error: sizes of matrices don't match (%zu != %zu)\n", a->size, b->size);
                return NULL;
        }

        if (a->info != b->info) {
                fprintf(stderr, "Error: incompetible types (%s vs %s)\n", a->info->type_name, b->info->type_name);
                return NULL;
        }

        Matrix* res = create_matrix(a->size, a->info);
        if (!res) return NULL;

        for (size_t i = 0; i < res->size; i++) {
                for (size_t j = 0; j < res->size; j++) {
                        res->info->set_zero(get_element(res, i, j));
                }
        }

        // C[i][j] = sum(A[i][k] * B[k][j])
        for (size_t i = 0; i < a->size; i++) {  //строки резултата и матрицы а
                for (size_t j = 0; j < b->size; j++) {  //столбцы рез-та и матрицы б
                        for (size_t k = 0; k < a->size; k++) {   //итерация по сумме

                                const void* a_elem = get_element(a, i, k);
                                const void* b_elem = get_element(b, k, j);
                                void* res_elem = get_element(res, i, j);

                                //tmp = A[i][k] * B[k][j]
                                void* tmp = malloc(a->info->elem_size);
                                if (!tmp) {
                                        destroy_matrix(&res);
                                        return NULL;
                                }

                                a->info->multiply(a_elem, b_elem, tmp);

                                //res[i][j] = res[i][j] + tmp
                                a->info->add(res_elem, tmp, res_elem);

                                free(tmp);
                        }
                }
        }


	return res;
}



Matrix* scalar_multiply_matrix(const Matrix* a, const void* scalar) {
        if (!a || !scalar) return NULL;

        Matrix* res = create_matrix(a->size, a->info);
        if (!res) return NULL;

        for (size_t i = 0; i < a->size; i++) {
                for (size_t j = 0; j < a->size; j++) {
                        void* res_elem = get_element(res, i, j);
                        const void* a_elem = get_element(a, i, j);

                        res->info->scalar_multiply(a_elem, scalar, res_elem);
                }
        }

        return res;
}



void input_matrix(Matrix* m) {
	if (!m) return;

	printf("Input matrix %zux%zu (%s):\n", m->size, m->size, m->info->type_name);

	for (size_t i = 0; i < m->size; i++) {
		for (size_t j = 0; j < m->size; j++) {
			printf("Element [%zu][%zu]: ", i, j);
			void* elem = get_element(m, i, j);
			m->info->read(elem);
		}
	}
}

void output_matrix(const Matrix* m) {
	if (!m) {
		printf("Null matrix\n");
		return;
	}

	for (size_t i = 0; i < m->size; i++){
		printf("| ");
		for (size_t j = 0; j < m->size; j++) {
			const void* elem = get_element((Matrix*)m, i, j);
			m->info->print(elem);
			printf(" ");
		}
		printf("|\n");
	}
}


