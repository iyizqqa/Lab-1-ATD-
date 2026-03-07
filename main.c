#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "types.h"
#include "tests.h"

int main() {
	run_all_tests();

	Matrix* A = NULL;
	Matrix* B = NULL;
	Matrix* Res = NULL;
	int command;
	int off_prog = 0;

	printf("\n===Polymorphic Square Matrix (Lab option 21)===\n");
    	printf("1.Create matrix A\n");
    	printf("2.Create matrix B\n");
   	printf("3.Add matrices (A + B)\n");
    	printf("4.Multiply matrices (A * B)\n");
    	printf("5.Multiply A by scalar\n");
    	printf("6.Show matrix\n");
    	printf("0.Exit\n");

    	printf("Enter command: ");
    	scanf("%d", &command);

	while (off_prog != 1) {
		switch (command) {
			case 1:
			case 2: {
				size_t n;
				int type_number;
				printf("Enter type (1=int, 2=complex): ");
				scanf("%d", &type_number);
				printf("Enter size N of matrix(NxN): ");
				scanf("%zu", &n);	
				
				const FieldInfo* info = (type_number == 1) ? get_int_info() : get_complex_info();
				Matrix** target = (command == 1) ? &A : &B;

				destroy_matrix(target);
				*target = create_matrix(n, info);

				if (*target) {
					input_matrix(*target);
					printf("Matrix %c created successfully\n", (command == 1) ? 'A' : 'B');
				} else {
					printf("Error: couldn't create the matrix\n");
				}
				
				break;
			}

			case 3:
				destroy_matrix(&Res);
				Res = add_matrices(A, B);
				if (Res) {
					printf("Addition completed successfully\n");
				} else {
					printf("Error: couldn't add\n");
				}
				
				break;

			case 4:
				destroy_matrix(&Res);
				Res = multiply_matrices(A, B);
				if (Res) {
					printf("Multiplication completed successfully\n");
				} else {
					printf("Error: couldn't multiply\n");
				}

				break;

			case 5: {
				if (!A || !B) {
					printf("Error: create matrix first\n");
					break;
				}

				printf("Enter scalar type (1=int, 2=complex): ");
				int scalar_type;
				scanf("%d", &scalar_type);

				int matrix_type = (A->info == get_int_info()) ? 1 : 2;
				if (scalar_type != matrix_type) {
					printf("Error: scalar type match matrix type\n");
					break;
				}
				
				//можно малоком но тогда освобождать надо както не хочу в куче хранить буду в стеке
				void* scalar = NULL;
				int int_scalar;
				Complex complex_scalar;

				if (scalar_type == 1) {
					printf("Enter scalar value (int) : ");
					scanf("%d", &int_scalar);
					scalar = &int_scalar;  // адрес стековой переменной
				} else {
					printf("Enter scalar value (complex re im): ");
					scanf("%lf %lf", &complex_scalar.re, &complex_scalar.im);
					scalar = &complex_scalar;
				}

				destroy_matrix(&Res);
				Res = scalar_multiply_matrix(A, scalar);

				if (Res) {
					printf("Scalar multiplication completed successfully\n");
				} else {
					printf("Error: couldn't multiply\n");
				}
				break;
			}

			case 6: {
				size_t show_which;
				printf("Which matrix to show (1=A, 2=B, 3=Result): ");
				scanf("%zu", &show_which);
				const Matrix* matrix_to_show = (show_which == 1) ? A : (show_which == 2) ? B : Res;
				if (matrix_to_show) {
					output_matrix(matrix_to_show);
				} else {
					printf("Error\n");
				}
				break;
			}


			case 0:
				off_prog = 1;
				break;

			default:
				printf("Error: invalid command\n");
				break;
		}
		
		if (off_prog == 0) {
			printf("Enter command: ");
			scanf("%d", &command);
		}
	}

	destroy_matrix(&A);
	destroy_matrix(&B);
	destroy_matrix(&Res);

	return 0;

}

	
