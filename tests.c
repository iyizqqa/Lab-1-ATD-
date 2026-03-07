#include "tests.h"
#include "matrix.h"
#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int passed = 0;
static int total = 0;

void check(const char* name, int condition) {
	total++;
	if (condition) {
		printf("PASS: %s\n", name);
		passed++;
	} else {
		printf("FAIL: %s\n", name);
	}
}

#define EQ_DOUBLE(a,b) (fabs((a) - (b)) < 1e-9)


// create and destroy matrices

void test_create_destroy_int(void) {
	printf("\n=== Тест 1.1: create/destroy (int) ===\n");

	const FieldInfo* info = get_int_info();

	Matrix* m1 = create_matrix(1, info);
	check("create matrix(1,int) != NULL", m1 != NULL);
	check("size == 1", m1 && m1->size == 1);
	destroy_matrix(&m1);

	Matrix* m2 = create_matrix(3, info);
	check("create_matrix(3, int) != NULL", m2 != NULL);
	check("size == 3", m2 && m2->size == 3);
        destroy_matrix(&m2);
    
         Matrix* m3 = create_matrix(10, info);
         check("create_matrix(10, int) != NULL", m3 != NULL);
         destroy_matrix(&m3);
    
         check("create_matrix(0, int) == NULL", create_matrix(0, info) == NULL);
         check("create_matrix(2, NULL) == NULL", create_matrix(2, NULL) == NULL);
}


void test_create_destroy_complex(void) {
	printf("\n=== Тест 1.2: create/destroy (complex) ===\n");
    
        const FieldInfo* info = get_complex_info();
    
        Matrix* m1 = create_matrix(1, info);
        check("create_matrix(1, complex) != NULL", m1 != NULL);
        check("size == 1", m1 && m1->size == 1);
        check("type == complex", m1 && m1->info == info);
        destroy_matrix(&m1);
    
        Matrix* m2 = create_matrix(2, info);
        check("create_matrix(2, complex) != NULL", m2 != NULL);
        destroy_matrix(&m2);
    
        check("create_matrix(0, complex) == NULL", create_matrix(0, info) == NULL);
	check("create_matrix(2, NULL) == NULL", create_matrix(2, NULL) == NULL);
}


// get element

void test_get_element_int(void) {
	printf("\n=== Тест 2.1: get_element (int) ===\n");

        const FieldInfo* info = get_int_info();
        Matrix* m = create_matrix(3, info);

    
        for (size_t i = 0; i < 3; i++) {
        	for (size_t j = 0; j < 3; j++) {
                	int val = i * 3 + j + 1;  /* 1..9 */
                        *(int*)get_element(m, i, j) = val;
		}
	}
	
	check("elem[0,0] == 1", *(int*)get_element(m, 0, 0) == 1);
    	check("elem[0,1] == 2", *(int*)get_element(m, 0, 1) == 2);
    	check("elem[1,0] == 4", *(int*)get_element(m, 1, 0) == 4);
    	check("elem[2,2] == 9", *(int*)get_element(m, 2, 2) == 9);

    
    	check("get_element(3,0) == NULL", get_element(m, 3, 0) == NULL);
    	check("get_element(0,3) == NULL", get_element(m, 0, 3) == NULL);
    	check("get_element(NULL,0,0) == NULL", get_element(NULL, 0, 0) == NULL);

    	destroy_matrix(&m);
}



void test_get_element_complex(void) {
	printf("\n=== Тест 2.2: get_element (complex) ===\n");
    
    	const FieldInfo* info = get_complex_info();
	Matrix* m = create_matrix(2, info);
    
    	Complex c1 = {1.0, 2.0};
    	Complex c2 = {3.0, 4.0};
    
    	memcpy(get_element(m, 0, 0), &c1, sizeof(Complex));
    	memcpy(get_element(m, 1, 1), &c2, sizeof(Complex));
    
    	Complex r1, r2;
    	memcpy(&r1, get_element(m, 0, 0), sizeof(Complex));
    	memcpy(&r2, get_element(m, 1, 1), sizeof(Complex));
    
    	check("c1.re == 1.0", EQ_DOUBLE(r1.re, 1.0));
    	check("c1.im == 2.0", EQ_DOUBLE(r1.im, 2.0));
    	check("c2.re == 3.0", EQ_DOUBLE(r2.re, 3.0));
    	check("c2.im == 4.0", EQ_DOUBLE(r2.im, 4.0));
    
    	destroy_matrix(&m);
}


// add matrices


void test_add_int_2x2(void) {
	printf("\n=== Тест 3.1: add_matrices (int 2x2) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* B = create_matrix(2, info);

   
    	*(int*)get_element(A, 0, 0) = 1;  *(int*)get_element(A, 0, 1) = 2;
    	*(int*)get_element(A, 1, 0) = 3;  *(int*)get_element(A, 1, 1) = 4;
    	*(int*)get_element(B, 0, 0) = 10; *(int*)get_element(B, 0, 1) = 20;
    	*(int*)get_element(B, 1, 0) = 30; *(int*)get_element(B, 1, 1) = 40;

    	Matrix* C = add_matrices(A, B);

    	check("C != NULL", C != NULL);
    	check("C[0,0] == 11", C && *(int*)get_element(C, 0, 0) == 11);
    	check("C[0,1] == 22", C && *(int*)get_element(C, 0, 1) == 22);
    	check("C[1,0] == 33", C && *(int*)get_element(C, 1, 0) == 33);
    	check("C[1,1] == 44", C && *(int*)get_element(C, 1, 1) == 44);

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}


void test_add_int_1x1(void) {
	printf("\n=== Тест 3.2: add_matrices (int 1x1) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(1, info);
    	Matrix* B = create_matrix(1, info);

    	*(int*)get_element(A, 0, 0) = 5;
    	*(int*)get_element(B, 0, 0) = 7;

    	Matrix* C = add_matrices(A, B);

    	check("C != NULL", C != NULL);
    	check("C[0,0] == 12", C && *(int*)get_element(C, 0, 0) == 12);

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}

void test_add_complex_2x2(void) {
	printf("\n=== Тест 3.3: add_matrices (complex 2x2) ===\n");

    	const FieldInfo* info = get_complex_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* B = create_matrix(2, info);

    	Complex a00 = {1.0, 2.0}, a01 = {3.0, 4.0};
    	Complex a10 = {5.0, 6.0}, a11 = {7.0, 8.0};
    	Complex b00 = {0.5, 0.5}, b01 = {1.5, 1.5};
    	Complex b10 = {2.5, 2.5}, b11 = {3.5, 3.5};

    	memcpy(get_element(A, 0, 0), &a00, sizeof(Complex));
    	memcpy(get_element(A, 0, 1), &a01, sizeof(Complex));
    	memcpy(get_element(A, 1, 0), &a10, sizeof(Complex));
    	memcpy(get_element(A, 1, 1), &a11, sizeof(Complex));
    	memcpy(get_element(B, 0, 0), &b00, sizeof(Complex));
    	memcpy(get_element(B, 0, 1), &b01, sizeof(Complex));
    	memcpy(get_element(B, 1, 0), &b10, sizeof(Complex));
    	memcpy(get_element(B, 1, 1), &b11, sizeof(Complex));

    	Matrix* C = add_matrices(A, B);

    	Complex r00, r11;
    	memcpy(&r00, get_element(C, 0, 0), sizeof(Complex));
    	memcpy(&r11, get_element(C, 1, 1), sizeof(Complex));

    	check("C != NULL", C != NULL);
    	check("C[0,0].re == 1.5", C && EQ_DOUBLE(r00.re, 1.5));
    	check("C[0,0].im == 2.5", C && EQ_DOUBLE(r00.im, 2.5));
    	check("C[1,1].re == 10.5", C && EQ_DOUBLE(r11.re, 10.5));
    	check("C[1,1].im == 11.5", C && EQ_DOUBLE(r11.im, 11.5));

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}

void test_add_complex_1x1(void) {
	printf("\n=== Тест 3.4: add_matrices (complex 1x1) ===\n");

    	const FieldInfo* info = get_complex_info();
    	Matrix* A = create_matrix(1, info);
    	Matrix* B = create_matrix(1, info);

    	Complex ca = {1.0, 2.0}, cb = {3.0, 4.0};
    	memcpy(get_element(A, 0, 0), &ca, sizeof(Complex));
    	memcpy(get_element(B, 0, 0), &cb, sizeof(Complex));

    	Matrix* C = add_matrices(A, B);
    	Complex* res = (Complex*)get_element(C, 0, 0);

    	check("C != NULL", C != NULL);
    	check("res.re == 4.0", C && EQ_DOUBLE(res->re, 4.0));
    	check("res.im == 6.0", C && EQ_DOUBLE(res->im, 6.0));

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}

// multiply


void test_mul_int_2x2(void) {
	printf("\n=== Тест 4.1: mul_matrices (int 2x2) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* B = create_matrix(2, info);

    /* A = |1 2|, B = |5 6|, A*B = |19 22| */
    /*     |3 4|     |7 8|         |43 50| */
    	*(int*)get_element(A, 0, 0) = 1; *(int*)get_element(A, 0, 1) = 2;
    	*(int*)get_element(A, 1, 0) = 3; *(int*)get_element(A, 1, 1) = 4;
    	*(int*)get_element(B, 0, 0) = 5; *(int*)get_element(B, 0, 1) = 6;
    	*(int*)get_element(B, 1, 0) = 7; *(int*)get_element(B, 1, 1) = 8;

    	Matrix* C = multiply_matrices(A, B);

    	check("C != NULL", C != NULL);
    	check("C[0,0] == 19", C && *(int*)get_element(C, 0, 0) == 19);
    	check("C[0,1] == 22", C && *(int*)get_element(C, 0, 1) == 22);
    	check("C[1,0] == 43", C && *(int*)get_element(C, 1, 0) == 43);
    	check("C[1,1] == 50", C && *(int*)get_element(C, 1, 1) == 50);

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}


void test_mul_int_1x1(void) {
	printf("\n=== Тест 4.2: mul_matrices (int 1x1) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(1, info);
    	Matrix* B = create_matrix(1, info);

    	*(int*)get_element(A, 0, 0) = 3;
    	*(int*)get_element(B, 0, 0) = 7;

    	Matrix* C = multiply_matrices(A, B);

    	check("C != NULL", C != NULL);
    	check("C[0,0] == 21", C && *(int*)get_element(C, 0, 0) == 21);

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}


void test_mul_complex_2x2(void) {
	printf("\n=== Тест 4.3: mul_matrices (complex 2x2) ===\n");

    	const FieldInfo* info = get_complex_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* B = create_matrix(2, info);

    /* A = |1+0i  2+0i|, B = |5+0i  6+0i| */
    /*     |3+0i  4+0i|     |7+0i  8+0i| */
    	Complex a00 = {1.0, 0.0}, a01 = {2.0, 0.0};
    	Complex a10 = {3.0, 0.0}, a11 = {4.0, 0.0};
    	Complex b00 = {5.0, 0.0}, b01 = {6.0, 0.0};
    	Complex b10 = {7.0, 0.0}, b11 = {8.0, 0.0};

    	memcpy(get_element(A, 0, 0), &a00, sizeof(Complex));
    	memcpy(get_element(A, 0, 1), &a01, sizeof(Complex));
    	memcpy(get_element(A, 1, 0), &a10, sizeof(Complex));
    	memcpy(get_element(A, 1, 1), &a11, sizeof(Complex));
    	memcpy(get_element(B, 0, 0), &b00, sizeof(Complex));
    	memcpy(get_element(B, 0, 1), &b01, sizeof(Complex));
    	memcpy(get_element(B, 1, 0), &b10, sizeof(Complex));
    	memcpy(get_element(B, 1, 1), &b11, sizeof(Complex));

    	Matrix* C = multiply_matrices(A, B);

    	Complex r00;
    	memcpy(&r00, get_element(C, 0, 0), sizeof(Complex));

    	check("C != NULL", C != NULL);
    	check("C[0,0].re == 19.0", C && EQ_DOUBLE(r00.re, 19.0));
    	check("C[0,0].im == 0.0", C && EQ_DOUBLE(r00.im, 0.0));

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}

// scalar multiply

void test_scalar_mul_int(void) {
	printf("\n=== Тест 5.1: scalar_mul_matrix (int) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);

    	*(int*)get_element(A, 0, 0) = 1; *(int*)get_element(A, 0, 1) = 2;
    	*(int*)get_element(A, 1, 0) = 3; *(int*)get_element(A, 1, 1) = 4;

    	int k = 3;
    	Matrix* B = scalar_multiply_matrix(A, &k);

    	check("B != NULL", B != NULL);
    	check("B[0,0] == 3",  B && *(int*)get_element(B, 0, 0) == 3);
    	check("B[0,1] == 6",  B && *(int*)get_element(B, 0, 1) == 6);
    	check("B[1,0] == 9",  B && *(int*)get_element(B, 1, 0) == 9);
    	check("B[1,1] == 12", B && *(int*)get_element(B, 1, 1) == 12);

    	destroy_matrix(&A); destroy_matrix(&B);
}

void test_scalar_mul_int_negative(void) {
	printf("\n=== Тест 5.2: scalar_mul_matrix (int, отрицательный) ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(1, info);
    	*(int*)get_element(A, 0, 0) = 5;

    	int k = -2;
    	Matrix* B = scalar_multiply_matrix(A, &k);

    	check("B != NULL", B != NULL);
    	check("B[0,0] == -10", B && *(int*)get_element(B, 0, 0) == -10);

    	destroy_matrix(&A); destroy_matrix(&B);
}


void test_scalar_mul_complex(void) {
	printf("\n=== Тест 5.3: scalar_mul_matrix (complex) ===\n");

    	const FieldInfo* info = get_complex_info();
    	Matrix* A = create_matrix(2, info);

    	Complex c00 = {1.0, 2.0}, c01 = {3.0, 4.0};
    	Complex c10 = {5.0, 6.0}, c11 = {7.0, 8.0};
    	memcpy(get_element(A, 0, 0), &c00, sizeof(Complex));
    	memcpy(get_element(A, 0, 1), &c01, sizeof(Complex));
    	memcpy(get_element(A, 1, 0), &c10, sizeof(Complex));
    	memcpy(get_element(A, 1, 1), &c11, sizeof(Complex));

    	Complex s = {2.0, 0.0}; 
    	Matrix* B = scalar_multiply_matrix(A, &s);

    	Complex r;
    	memcpy(&r, get_element(B, 0, 0), sizeof(Complex));

    	check("B != NULL", B != NULL);
    	check("B[0,0].re == 2.0", B && EQ_DOUBLE(r.re, 2.0));
    	check("B[0,0].im == 4.0", B && EQ_DOUBLE(r.im, 4.0));

    	destroy_matrix(&A); destroy_matrix(&B);
}



// errors


void test_error_different_types(void) {
	printf("\n=== Тест 6.1: ошибка - разные типы ===\n");

    	Matrix* A = create_matrix(2, get_int_info());
    	Matrix* B = create_matrix(2, get_complex_info());

    	check("add(int, complex) == NULL", add_matrices(A, B) == NULL);
    	check("mul(int, complex) == NULL", multiply_matrices(A, B) == NULL);

    	destroy_matrix(&A); destroy_matrix(&B);
}

void test_error_different_sizes(void) {
	printf("\n=== Тест 6.2: ошибка - разные размеры ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* B = create_matrix(3, info);

    	check("add(2x2, 3x3) == NULL", add_matrices(A, B) == NULL);
    	check("mul(2x2, 3x3) == NULL", multiply_matrices(A, B) == NULL);

    	destroy_matrix(&A); destroy_matrix(&B);
}


void test_error_null_matrices(void) {
	printf("\n=== Тест 6.3: ошибка - NULL матрицы ===\n");

    	Matrix* A = create_matrix(2, get_int_info());
	
    	check("add(NULL, A) == NULL", add_matrices(NULL, A) == NULL);
    	check("add(A, NULL) == NULL", add_matrices(A, NULL) == NULL);
    	check("mul(NULL, A) == NULL", multiply_matrices(NULL, A) == NULL);
    	check("scalar_mul(NULL, &k) == NULL", scalar_multiply_matrix(NULL, A) == NULL);

    	destroy_matrix(&A);
}


void test_error_get_element_bounds(void) {
	printf("\n=== Тест 6.4: ошибка - выход за границы ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* m = create_matrix(2, info);

    	check("get_element(2,0) == NULL", get_element(m, 2, 0) == NULL);
    	check("get_element(0,2) == NULL", get_element(m, 0, 2) == NULL);
    	check("get_element(5,5) == NULL", get_element(m, 5, 5) == NULL);
    	check("get_element(NULL,0,0) == NULL", get_element(NULL, 0, 0) == NULL);

    	destroy_matrix(&m);
}

// граничные случаи 


void test_edge_zero_matrix(void) {
	printf("\n=== Тест 7.1: матрица с нулевыми элементами ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);  // calloc обнуляет память
    	Matrix* B = create_matrix(2, info);

    	*(int*)get_element(B, 0, 0) = 1; *(int*)get_element(B, 0, 1) = 2;
    	*(int*)get_element(B, 1, 0) = 3; *(int*)get_element(B, 1, 1) = 4;

    	Matrix* C = add_matrices(A, B);

    	check("C[0,0] == 1", C && *(int*)get_element(C, 0, 0) == 1);
    	check("C[1,1] == 4", C && *(int*)get_element(C, 1, 1) == 4);

    	destroy_matrix(&A); destroy_matrix(&B); destroy_matrix(&C);
}

void test_edge_identity_mul(void) {
	printf("\n=== Тест 7.2: умножение на единичную матрицу ===\n");

    	const FieldInfo* info = get_int_info();
    	Matrix* A = create_matrix(2, info);
    	Matrix* I = create_matrix(2, info);  // Единичная

    	*(int*)get_element(A, 0, 0) = 1; *(int*)get_element(A, 0, 1) = 2;
    	*(int*)get_element(A, 1, 0) = 3; *(int*)get_element(A, 1, 1) = 4;

    	*(int*)get_element(I, 0, 0) = 1; *(int*)get_element(I, 0, 1) = 0;
    	*(int*)get_element(I, 1, 0) = 0; *(int*)get_element(I, 1, 1) = 1;

    	Matrix* C = multiply_matrices(A, I);

    	check("A*I == A[0,0]", C && *(int*)get_element(C, 0, 0) == 1);
    	check("A*I == A[0,1]", C && *(int*)get_element(C, 0, 1) == 2);
    	check("A*I == A[1,0]", C && *(int*)get_element(C, 1, 0) == 3);
    	check("A*I == A[1,1]", C && *(int*)get_element(C, 1, 1) == 4);

    	destroy_matrix(&A); destroy_matrix(&I); destroy_matrix(&C);
}

// main func all tests


void run_all_tests(void) {
	printf("\nTESTING THE PROGRAMM");
	
	passed = 0;
    	total = 0;

	// Раздел 1: Создание/удаление 
    	test_create_destroy_int();
    	test_create_destroy_complex();

   	 // Раздел 2: Доступ к элементам 
   	 test_get_element_int();
   	 test_get_element_complex();

    	 // Раздел 3: Сложение 
    	test_add_int_2x2();
    	test_add_int_1x1();
    	test_add_complex_2x2();
    	test_add_complex_1x1();

    	// Раздел 4: Умножение матриц
    	test_mul_int_2x2();
    	test_mul_int_1x1();
    	test_mul_complex_2x2();

    	// Раздел 5: Умножение на скаляр 
    	test_scalar_mul_int();
    	test_scalar_mul_int_negative();
    	test_scalar_mul_complex();

	// Раздел 6: Обработка ошибок 
    	test_error_different_types();
    	test_error_different_sizes();
    	test_error_null_matrices();
    	test_error_get_element_bounds();

    	// Раздел 7: Граничные случаи 
    	test_edge_zero_matrix();
    	test_edge_identity_mul();

	printf("\n");
    	printf("╔══════════════════════════════════════════╗\n");
    	printf("║              РЕЗУЛЬТАТЫ                  ║\n");
    	printf("╠══════════════════════════════════════════╣\n");
    	printf("║  Пройдено: %3d / %3d тестов              ║\n", passed, total);
    	printf("╚══════════════════════════════════════════╝\n");

	if (passed == total) {
        printf("\n ВСЕ ТЕСТЫ ПРОЙДЕНЫ! Отличная работа!\n\n");
        } else {
		printf("\n ЕСТЬ ПРОВАЛЕННЫЕ ТЕСТЫ! Проверь код.\n\n");
        }
}
