#include "Recursive_calculation.h"
#include "Equation_bucket.h"


int FindStrWVar(char target[], char Vars[][str_len])
{
    short i = 0;
    char var[VAR_LEN] = { 0 };
    do
        sscanf(Vars[i++], "%s", var);
    while (strcmp(var, target) && i < VARS_NUM);

    return i - 1;
}


void RecCalc(Equation* Main_Equation, char vars[][str_len], _Dcomplex results[RES_SIZE], int* result_index)
{
    short equat_quantity = 0, vars_quantity = 0;
    unsigned short counter = 0;
    int current_index = 0;

    while (Main_Equation[current_index].type)
    {
        while (Main_Equation[current_index].type && Main_Equation[current_index].type != variable && Main_Equation[current_index].type != equat)
            ++current_index;

        if (Main_Equation[current_index].type)
        {
            if (Main_Equation[current_index].type == equat)
            {
                RecCalc(GetEquation(&(Main_Equation[current_index])), vars, results, result_index);
                if (*result_index)
                {
                    Main_Equation[current_index].bucket = (void*)(&(results[(*result_index) - 1]));
                    Main_Equation[current_index].type = number;
                }
            }
            else
            {
                Equation Temp_Equation[table_width] = { 0,0 };
                CreateEquation(Temp_Equation, vars[FindStrWVar(GetVariable(&(Main_Equation[current_index])), vars)]);
                RecCalc(Temp_Equation, vars, results, result_index);

                if (*result_index)
                {
                    Main_Equation[current_index].bucket = (void*)(&(results[(*result_index) - 1]));
                    Main_Equation[current_index].type = number;
                }
            }
            continue;

        }
        else
        {
            --current_index;
            //PrintEqution(Main_Equation);
            results[*result_index] = calculate(Main_Equation);
            ++(*result_index);
            return;
        }
    }
    return;
}
