#include <stdio.h>

#include "matrix.h"
#include "types.h"

static int input_command(int *command)
{
	char next_char;
	int result;
	int done = 0;

	do
	{
		printf("Input command: ");
		result = scanf("%d%c", command, &next_char);
		if (result == EOF)
		{
			printf("error\n");
			done = 1;
		}
		else if (result == 2)
		{
			if (next_char != '\n')
			{
				printf("Incorrect Input\n");
				scanf("%*[^\n]");
			}
			else
			{
				if ((*command >= 0) && (*command <= 6))
				{
					done = 1;
				}
				else
				{
					printf("Choose command 0-6\n");
				}
			}
		}
		else
		{
			printf("Incorrect input\n");
			scanf("%*[^\n]");
		}
	} while (!done);

	return result;
}

static int input_type_number(int *type_number)
{
	char next_char;
	int result;
	int done = 0;

	do
	{
		printf("Enter type (1=int, 2=complex): ");
		result = scanf("%d%c", type_number, &next_char);

		if (result == EOF)
		{
			printf("Input finished\n");
			done = 1;
		}
		else if (result == 2)
		{
			if (next_char != '\n')
			{
				printf("Incorrect input\n");
				scanf("%*[^\n]");
				scanf("%*c");
			}
			else
			{
				if (*type_number == 1 || *type_number == 2)
				{
					done = 1;
				}
				else
				{
					printf("Choose type 1 or 2\n");
				}
			}
		}
		else
		{
			printf("Incorrect input\n");
			scanf("%*[^\n]");
			scanf("%*c");
		}
	} while (!done);

	return result;
}

static int input_size_value(size_t *n)
{
	char next_char;
	int result;
	int done = 0;

	do
	{
		printf("Enter size N of matrix (NxN): ");
		result = scanf("%zu%c", n, &next_char);

		if (result == EOF)
		{
			printf("Input finished\n");
			done = 1;
		}
		else if (result == 2)
		{
			if (next_char != '\n')
			{
				printf("Incorrect input\n");
				scanf("%*[^\n]");
				scanf("%*c");
			}
			else if (*n == 0)
			{
				printf("Size must be > 0\n");
			}
			else
			{
				done = 1;
			}
		}
		else
		{
			printf("Incorrect input\n");
			scanf("%*[^\n]");
			scanf("%*c");
		}
	} while (!done);

	return result;
}

static int input_int_value(int *value)
{
	char next_char;
	int result;
	int done = 0;

	do
	{
		printf("Enter int scalar: ");
		result = scanf("%d%c", value, &next_char);

		if (result == EOF)
		{
			printf("Input finished\n");
			done = 1;
		}
		else if (result == 2)
		{
			if (next_char != '\n')
			{
				printf("Incorrect input\n");
				scanf("%*[^\n]");
				scanf("%*c");
			}
			else
			{
				done = 1;
			}
		}
		else
		{
			printf("Incorrect input\n");
			scanf("%*[^\n]");
			scanf("%*c");
		}
	} while (!done);

	return result;
}

static int input_double_value(const char *prompt, double *value)
{
	char next_char;
	int result;
	int done = 0;

	do
	{
		printf("%s", prompt);
		result = scanf("%lf%c", value, &next_char);

		if (result == EOF)
		{
			printf("Input finished\n");
			done = 1;
		}
		else if (result == 2)
		{
			if (next_char != '\n')
			{
				printf("Incorrect input\n");
				scanf("%*[^\n]");
				scanf("%*c");
			}
			else
			{
				done = 1;
			}
		}
		else
		{
			printf("Incorrect input\n");
			scanf("%*[^\n]");
			scanf("%*c");
		}
	} while (!done);

	return result;
}

int main()
{

	Matrix A = {0, NULL, NULL};
	Matrix B = {0, NULL, NULL};
	Matrix Res = {0, NULL, NULL};

	int command;
	int type_number;
	size_t n;
	const FieldInfo *info;

	do
	{
		printf("\n===Polymorphic Square Matrix (Lab option 21)===\n");
		printf("1.Create matrix A\n");
		printf("2.Create matrix B\n");
		printf("3.Add matrices (A + B)\n");
		printf("4.Multiply matrices (A * B)\n");
		printf("5.Multiply A by scalar\n");
		printf("6.Show matrix\n");
		printf("0.Exit\n");

		if (input_command(&command) == EOF)
		{
			break;
		}
		switch (command)
		{
		case 1:
		case 2:
			if (input_type_number(&type_number) == EOF)
			{
				command = 0;
				break;
			}

			if (input_size_value(&n) == EOF)
			{
				command = 0;
				break;
			}

			if (type_number == 1)
			{
				info = get_int_info();
			}
			else
			{
				info = get_complex_info();
			}

			if (command == 1)
			{
				clear_matrix(&A);

				if (!init_matrix(&A, n, info))
				{
					printf("Error creating matrix A\n");
					break;
				}

				printf("Enter matrix A:\n");
				input_matrix(&A);
			}
			else
			{
				clear_matrix(&B);

				if (!init_matrix(&B, n, info))
				{
					printf("Error creating matrix B\n");
					break;
				}

				printf("Enter matrix B:\n");
				input_matrix(&B);
			}
			break;

		case 3:
			if (!A.data || !B.data)
			{
				printf("Create matrices A and B first\n");
				break;
			}

			clear_matrix(&Res);
			Res = add_matrices(&A, &B);

			if (!Res.data)
			{
				printf("Addition error\n");
			}
			else
			{
				printf("Result of A + B:\n");
				output_matrix(&Res);
			}
			break;

		case 4:
			if (!A.data || !B.data)
			{
				printf("Create matrices A and B first\n");
				break;
			}

			clear_matrix(&Res);
			Res = multiply_matrices(&A, &B);

			if (!Res.data)
			{
				printf("Multiplication error\n");
			}
			else
			{
				printf("Result of A * B:\n");
				output_matrix(&Res);
			}
			break;

		case 5:
			if (!A.data)
			{
				printf("Create matrix A first\n");
				break;
			}

			clear_matrix(&Res);

			if (A.info == get_int_info())
			{
				int scalar;

				if (input_int_value(&scalar) == EOF)
				{
					command = 0;
					break;
				}

				Res = scalar_multiply_matrix(&A, &scalar);
			}
			else if (A.info == get_complex_info())
			{
				Complex scalar;

				if (input_double_value("Real part: ", &scalar.re) == EOF)
				{
					command = 0;
					break;
				}

				if (input_double_value("Imaginary part: ", &scalar.im) == EOF)
				{
					command = 0;
					break;
				}

				Res = scalar_multiply_matrix(&A, &scalar);
			}

			if (Res.data == NULL)
			{
				printf("Scalar multiplication error\n");
			}
			else
			{
				printf("Result of scalar multiplication:\n");
				output_matrix(&Res);
			}
			break;

		case 6:
			printf("Matrix A:\n");
			output_matrix(&A);

			printf("Matrix B:\n");
			output_matrix(&B);

			printf("Result matrix:\n");
			output_matrix(&Res);
			break;

		case 0:
			break;

		default:
			printf("Wrong command\n");
			break;
		}

	} while (command != 0);

	clear_matrix(&A);
	clear_matrix(&B);
	clear_matrix(&Res);

	return 0;
}
