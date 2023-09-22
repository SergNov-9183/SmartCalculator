#include "Vars_To_Eqs.h"
#define VAR_LEN 100

int FindStrWVar(char target[], char VarsProblem[][str_len])
{
    short i = 0;
    char var[VAR_LEN] = { 0 };
    do
        sscanf(VarsProblem[i++], "%s", var);
    while (strcmp(var, target) && i < VARS_NUM);

    return i - 1;
}

void CopyEquation(Equation dest[], Equation source[], unsigned short terminatingIndex)
{
    unsigned short i = 0;
    while (source[i].type && i < terminatingIndex)
    {
        dest[i].bucket = source[i].bucket;
        dest[i].type = source[i].type;
        ++i;
    }

    return;
}

void ClearEquation(Equation equation[])
{
    unsigned short i = 0;
    while (equation[i].type)
    {
        equation[i].bucket = NULL;
        equation[i].type = 0;
        ++i;
    }

    return;
}


void RecursiveTransfer(Equation Main_Equation[], Equation VarEquation[], Equation FinalEquation[], unsigned short* index_main, unsigned short* index_final, char VarsProblem[][str_len])
{
    *index_main = 0;
    Equation As_Equation[table_width] = { 0, 0 };
    char str_required[str_len] = { 0 };
    unsigned short memory_index = 0;
    unsigned short num_str_required = 0;
    while (Main_Equation[*index_main].type)
    {
        if (Main_Equation[*index_main].type == variable)
        {
            if (EquationSize(VarEquation))
                ClearEquation(VarEquation);
            strcpy(str_required, VarsProblem[FindStrWVar(GetVariable(&(Main_Equation[*index_main])), VarsProblem)]);
            CreateEquation(VarEquation, str_required);
            memory_index = *index_main;

            RecursiveTransfer(VarEquation, As_Equation, FinalEquation, index_main, index_final, VarsProblem);

            *index_main = memory_index + 1;
            
        }
        else
        {
            FinalEquation[*index_final].bucket = Main_Equation[*index_main].bucket;
            FinalEquation[*index_final].type =   Main_Equation[*index_main].type;
            ++(*index_final);
            ++(*index_main);
        }
    }

    return;
}

void VariablesToEquation(Equation Main_Equation[], Equation Final_Equation[], char VarsProblem[][str_len])
{
    unsigned short     index = 0;
    unsigned short new_index = 0;

    Equation VarEquation  [table_width] = { 0, 0 };

    RecursiveTransfer(Main_Equation, VarEquation, Final_Equation, &index, &new_index, VarsProblem);

    return;
}
