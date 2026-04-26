#include "types.h"
#include <stdio.h>

void int_add(const void *a, const void *b, void *res)
{
	*(int *)res = *(const int *)a + *(const int *)b;
}

void int_multiply(const void *a, const void *b, void *res)
{
	*(int *)res = *(const int *)a * *(const int *)b;
}

void int_set_zero(void *a)
{
	*(int *)a = 0;
}

void int_print(const void *a)
{
	printf("%d", *(const int *)a);
}

void int_read(void *a)
{
	scanf("%d", (int *)a);
}

const FieldInfo *get_int_info(void)
{
	static const FieldInfo info = {
		sizeof(int),
		"int",
		int_add,
		int_multiply,
		int_set_zero,
		int_print,
		int_read,
	};

	return &info;
}

void complex_add(const void *a, const void *b, void *res)
{
	Complex *r = (Complex *)res;
	const Complex *va = (const Complex *)a;
	const Complex *vb = (const Complex *)b;

	r->re = va->re + vb->re;
	r->im = va->im + vb->im;
}

void complex_multiply(const void *a, const void *b, void *res)
{
	Complex *r = (Complex *)res;
	const Complex *va = (const Complex *)a;
	const Complex *vb = (const Complex *)b;

	// (a + bi)(c + di) = ac + adi + bci - bd = (ac - bd) + (ad + bc)i
	r->re = va->re * vb->re - va->im * vb->im;
	r->im = va->re * vb->im + va->im * vb->re;
}

void complex_set_zero(void *a)
{
	Complex *va = (Complex *)a;
	va->re = 0.0;
	va->im = 0.0;
}

void complex_print(const void *a)
{
	const Complex *va = (const Complex *)a;
	if (va->im >= 0)
	{
		printf("%.2f+%.2fi", va->re, va->im);
	}
	else
	{
		printf("%.2f%.2fi", va->re, va->im);
	}
}

void complex_read(void *a)
{
	Complex *va = (Complex *)a;
	printf("Действительная часть: ");
	scanf("%lf", &va->re);
	printf("Мнимая часть: ");
	scanf("%lf", &va->im);
}

const FieldInfo *get_complex_info()
{
	static const FieldInfo info = {
		sizeof(Complex),
		"complex",
		complex_add,
		complex_multiply,
		complex_set_zero,
		complex_print,
		complex_read,
	};

	return &info;
}
