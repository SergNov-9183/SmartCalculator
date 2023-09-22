#include "Equation_bucket.h"
#include "Calculating.h"
#include <string.h>
#include <stdio.h>
#include <locale.h>

int main()
{
    {//Tutorial
        /*FILE* f = fopen("Equations.txt", "rt");
        const char zeroStr[str_len] = { 0 };
        const Equation zeroEquation[table_width] = { 0 };
        char str[str_len] = { 0 };
        Equation equation[table_width] = { 0 };
        while (fgets(str, str_len, f)) {
            if (!(str[0] == '#' || str[0] == ' ' || str[0] == '\t' || str[0] == '\n')) {
                CreateEquation(equation, str);
                PrintEqution(equation);
                printf("\n");
                memcpy(str, zeroStr, str_len);
                memcpy(equation, zeroEquation, table_width * sizeof(Equation) / sizeof(char));
            }
            else
                ClearData();
        }*/
        //char str[8][str_len] = { /*"1.92+2.33*(7+3*cos(ln(sin(e)*2.89)-pow(17.06,2)/2.17))"*/
        //                         "x = exp(-j * PI * alpha * len / v0 + pow(len * 7.92,s1))",
        //                         "alpha = exp(PI * len / (s1 + s2))",
        //                         "s1 = exp(exp(S11) * exp(S22))",
        //                         "s2 = (exp(S11 * S22)) ^ 2",
        //                         "len = 191.76",
        //                         "v0 = 299.77",
        //                         "S11 = -0.01-0.92j",
        //                         "S22 = 0.06 + 0.65j", };
        //Equation equation[8][table_width] = { 0,0 };
        //for (int i = 0; i < 8; i++) {
        //    CreateEquation(equation[i], str[i]);
        //    PrintEqution(equation[i]);
        //    printf("\n");
        //}
        //_Dcomplex x = CalculateEquation(equation[6]);
        //printf("\n%lf\n", creal(x));
        //printf("%.2lf", creal(x));
        //if (cimag(x) < 0)
        //    printf("%.2lfj", cimag(x));
        //else
        //    printf("+%.2lfj", cimag(x));
        ////strcpy(str, "alpha = sqrt(PI*len/(s1+s2))\0");
        ////CreateEquation(equation2, str);
        ////PrintEqution(equation);
        ////Equation* a = GetEquation(&(equation2[1]));
        ////printf("\n");
        ////PrintEqution(equation2);
        ////printf("\n%d %d\n", EquationSize(equation), EquationSize(equation2));
        ////_Dcomplex a = { 1.00, 1.00 };
        ////EquationSize(equation);
        ////PasteNumber(&(equation[EquationSize(equation)]), a);/*вставляет "a" в 1-ую пустую ячейку*/
        ////PrintEqution(equation);
        ////_Dcomplex x = GetNumber(&(a[0])); // показывает PI
        ////char y = GetSign(&(a[1])); // показывает exp (если точнее EXP = 5)
        ////char* st = GetVariable(&(a[2])); // показывает "alpha"
        ////printf("");
    }




    //setlocale(LC_ALL, "Russian");
    int status = 0, num_tasks = 0;
    char string[table_width] = { 0 };
    _Dcomplex results[RES_SIZE] = { 0 };
    int result_index = 0;
    FILE* fTasks = fopen("Equations.txt", "rt");
    while (!feof(fTasks))
    {
        fgets(string, table_width, fTasks);
        if (string[0] == '#')
            ++num_tasks;
    }
    printf("How to solve everything? \nJust insert a number (0 to exit, 1-%d to choose equation):\n", num_tasks);
    while (True)
    {
        scanf("%d", &status);
        switch (status)
        {
        case(0):
            system("cls");
            printf("Shutting down...\n");
            return 0;
            break;
        default:
            if (status > 0 && status <= num_tasks)
            {
                fseek(fTasks, 0, SEEK_SET);
                Calculate(status, fTasks, results, &result_index);
                for (short counter = 0; counter <= result_index; ++counter)
                {
                     //results[counter] = complexValue(0);
                }
                result_index = 0;
                printf("\nAnother problem? OK! Enter number (1-%d) or 0 to exit:\n", num_tasks);
            }
            else
                printf("No tasks to calculate((\nPlease, enter valid number (1-%d) or 0 to exit:\n", num_tasks);
            break;
        }
    }
    fclose(fTasks);

    return 0;
}