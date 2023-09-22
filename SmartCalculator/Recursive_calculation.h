#ifndef RECURSIVE_CALCULATION_H
#define RECURSIVE_CALCULATION_H

#include "CalcImpl.h"
#include "Equation_bucket.h"
#include <stdio.h>
#include <string.h>

#define VARS_NUM 50
#define VAR_LEN 100
#define RES_SIZE 100

void RecCalc(Equation* Equation, char vars[][str_len], _Dcomplex results[RES_SIZE], unsigned* result_index);


#endif //RECURSIVE_CALCULATION