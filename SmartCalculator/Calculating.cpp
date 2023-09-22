#include "Recursive_calculation.h"
#include "Calculating.h"
#include "CalcImpl.h"

void Calculate(int number, FILE* fTask, _Dcomplex results[RES_SIZE], int* result_index)
{
	short counter = 0;
	char des_value[VAR_LEN] = { 0 };
	char string[str_len] = { 0 };
	char equation_str[str_len] = { 0 };
	char vars_table[VARS_NUM][str_len] = { 0 };
	Equation InitialEquation[table_width] = { 0,0 };
	Equation EquationToCount[table_width] = { 0,0 };
	printf("\nSpecify the precision of calculations (number of decimal places {0 - 12}):\n");
	int decimal_places = 0;
	scanf("%d", &decimal_places);
	system("cls");
	if (number == 1)
	{
		//fgets(string, str_len, fTask);
		fgets(string, str_len, fTask);
	}
	else
		while (counter != number - 1)
		{
			if (strlen(string) == 1)
				counter++;
			fgets(string, str_len, fTask);
		}

	fgets(equation_str, str_len, fTask);
	equation_str[strlen(equation_str) - 1] = 0;
	printf("\nYou want to calculate sample NUM %d:\n\n%s", number, equation_str);

	sscanf(equation_str, "%s", des_value);
	CreateEquation(InitialEquation, equation_str);
	//PrintEqution(InitialEquation);
	unsigned index = -1;
	do
	{
		++index;
		fgets(vars_table[index], str_len, fTask);
	} while (strlen(vars_table[index]) > 1);
	if (index > 1)
	{
		printf(",\twhere:\n\n");
		index = 0;
		while (strlen(vars_table[index])>1)
			printf("%s", vars_table[index++]);
	}
	else
		printf("\n");
	RecCalc(&InitialEquation, vars_table, results, result_index);

	printf("\nYour result:\n%s = ", des_value);
	printComplexValue(results[*result_index - 1], decimal_places);
	ClearData();
}