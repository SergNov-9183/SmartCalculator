#ifndef Equation_bucket_H
#define Equation_bucket_H
#define _CRT_SECURE_NO_WARNINGS

#include <complex.h>
#include "Const_enum.h"

typedef struct {
	Type type;
	void* bucket; // адрес элемента в одном из 4-ёх массивов (зависит от типа) для хранения данных
} Equation;

void CreateEquation(Equation equation[], char str[]);
int EquationSize(Equation equation[]);
void PrintEqution(Equation* equation);
void PasteNumber(Equation* equation, _Dcomplex n);
char GetOperation(Equation* equation);
_Dcomplex GetNumber(Equation* equation);
char* GetVariable(Equation* equation);
char GetSign(Equation* equation);
Equation* GetEquation(Equation* equation);
//_Dcomplex CalculateEquation(Equation* equation);
void ClearData();
#endif