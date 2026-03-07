#include "types.h"
#include <stdio.h>
#include <stdlib.h>

void int_add(const void* a, const void* b, void* res){
	*(int*)res = *(const int*)a + *(const int*)b;
}

void int_multiply(const void* a, const void* b, void* res) {
	*(int*)res = *(const int*)a * *(const int*)b;
}

void int_scalar_multiply(const void* a, const void* s, void* res) {
	*(int*)res = *(const int*)a * *(const int*)s;
}

void int_set_zero(void* a) {
	*(int*)a = 0;
}

void int_print(const void* a) {
	printf("%d", *(const int*)a);
}

void int_read(void *a) {
	scanf("%d", (int*)a);
}


const FieldInfo* get_int_info() {
	static FieldInfo* info = NULL;

	if (info == NULL) {
		info = (FieldInfo*)malloc(sizeof(FieldInfo));
		if (info == NULL) return NULL;

		info->elem_size = sizeof(int);
		info->type_name = "int";

		info->add = int_add;
		info->multiply = int_multiply;
		info->scalar_multiply = int_scalar_multiply;
		info->set_zero = int_set_zero;
		info->print = int_print;
		info->read = int_read;
	}

	return info;
}


void complex_add(const void* a, const void* b, void* res) {
	Complex* r = (Complex*)res;
	const Complex* va = (const Complex*)a;
	const Complex* vb = (const Complex*)b;

	r->re = va->re + vb->re;
	r->im = va->im + vb->im;
}

void complex_multiply(const void* a, const void* b, void* res){
	Complex* r = (Complex*)res;
	const Complex* va = (const Complex*)a;
	const Complex* vb = (const Complex*)b;

	// (a + bi)(c + di) = ac + adi + bci - bd = (ac - bd) + (ad + bc)i
	r->re = va->re * vb->re - va->im * vb->im;
	r->im = va->re * vb->im + va->im * vb->re;
}


void complex_scalar_multiply(const void* a, const void* s, void* res){
	Complex* r = (Complex*) res;
	const Complex* va = (const Complex*)a;
	const Complex* vs = (const Complex*)s;

	r->re = va->re * vs->re - va->im * vs->im;
	r->im = va->re * vs->im + va->im * vs->re;
}


void complex_set_zero(void* a){
	Complex* va = (Complex*)a;
	va->re = 0.0;
	va->im = 0.0;
}

void complex_print(const void* a){
	const Complex* va = (const Complex*)a;
	if (va->im >= 0) {
		printf("%.2f+%.2fi", va->re, va->im);
	} else {
		printf("%.2f%.2fi", va->re, va->im);
	}
}

void complex_read(void* a){
	Complex* va = (Complex*)a;
	printf("Действительная часть: ");
	scanf("%lf", &va->re);
	printf("Мнимая часть: ");
	scanf("%lf", &va->im);
}


const FieldInfo* get_complex_info() {
	static FieldInfo* info = NULL;
	
	if (info == NULL) {
		info = (FieldInfo*)malloc(sizeof(FieldInfo));
		if (info == NULL) return NULL;

		info->elem_size = sizeof(Complex);
		info->type_name = "complex";

		info->add = complex_add;
		info->multiply = complex_multiply;
		info->scalar_multiply = complex_scalar_multiply;
		info->set_zero = complex_set_zero;
		info->print = complex_print;
		info->read = complex_read;
	}

	return info;
}

