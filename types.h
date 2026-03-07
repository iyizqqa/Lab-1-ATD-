#pragma once

#include <stddef.h>

typedef struct _complex {
	double re;
	double im;
} Complex;

typedef struct _field_info {
	size_t elem_size;
	const char* type_name;
	void(*add)(const void* a, const void* b, void* res);
	void(*multiply)(const void* a, const void* b, void* res);
	void(*scalar_multiply)(const void* a, const void* s, void* res);
	void(*set_zero)(void* a);
	void(*print)(const void* a);
	void(*read)(void* a);	
} FieldInfo;



const FieldInfo* get_int_info();
const FieldInfo* get_complex_info();



