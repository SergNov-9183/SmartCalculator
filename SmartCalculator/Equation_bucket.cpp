#include "Equation_bucket.h"
#include <stdio.h>
#include <tgmath.h>
#include <math.h>
#include <string.h>

_Dcomplex num[table_width] = { 0 };
int num_size = 0;
int variables_size = 0;
char variables[table_width][str_len] = { 0 };
int operations_size = 0;
char operations[table_width] = { 0 };
Equation Equat[table_width][table_width] = { 0, 0 };
int equat_size = -1;
int signs[table_width] = { 0 };
int signs_size = 0;
/*num[table_width], variables[table_width][str_len], brackets[table_width], operations[table_width] - массивы для хранения данных разных
типов на их элементы потом будет ссылаться void* bucket в структуре,
так как сами массивы больше своей заполненной части переменные с _size показывают их реальный размер,
чтобы через, например, num[num_size] прийти к первому незаполненному элементу*/
int operandFlag[op_quan] = { 0 }; /* нужен для проверки бинарных оппераций, чтобы вставить операцию
								  с двумя операндами после первого операнда*/

void ClearData() {
	const char zeroStr[str_len] = { 0 };
	const Equation zeroEquation[table_width] = { 0 };
	num_size = 0;
	variables_size = 0;
	operations_size = 0;
	equat_size = -1;
	signs_size = 0;
	for (int i = 0; i < table_width; ++i) {
		num[i]._Val[0] = 0;
		num[i]._Val[1] = 0;
		memcpy(variables[i], zeroStr, str_len);
		operations[i] = 0;
		memcpy(Equat[i], zeroEquation, table_width * sizeof(Equation) / sizeof(char));
		signs[i] = 0;
	}
	return;
}

void GetStr(char dest[], char str[], unsigned short* i) {
	int j = 0;
	while (str[*i] && (str[*i] != '+') && (str[*i] != '-') && (str[*i] != '*') && (str[*i] != '/') && 
					  (str[*i] != '^') && (str[*i] != '(') && (str[*i] != ')') && (str[*i] != ',') &&
					  (str[*i] != ' ') && (str[*i] != '\t') && (str[*i] != '\n')) {
		dest[j++] = str[(*i)++];
	}
	return;
}

void PushSign(Equation* equation, char Sign, int* equation_size) {
	equation->type = sign;
	signs[signs_size] = Sign;
	equation->bucket = &(signs[signs_size]);
	++signs_size;
	++*equation_size;
	return;
}
void PushOperation(Equation* equation, char Operation, int* equation_size) {
	equation->type = operation;
	operations[operations_size] = Operation;
	equation->bucket = &(operations[operations_size]);
	++operations_size;
	++*equation_size;
	return;
}
void PushNumber(Equation* equation, int* equation_size) {
	equation->type = number;
	equation->bucket = &(num[num_size]);
	++num_size;
	++*equation_size;
	return;
}
void PushEquation(Equation* equation, Equation e[], int* equation_size) {
	equation->type = equat;
	memcpy(&(Equat[equat_size]), e, table_width);
	++*equation_size;
	return;
}

char GetOperation(Equation* equation) {
	return *(char*)(equation->bucket);
}
_Dcomplex GetNumber(Equation* equation) {
	return *(_Dcomplex*)(equation->bucket);
}
char* GetVariable(Equation* equation) {
	return (char*)(equation->bucket);
}
char GetSign(Equation* equation) {
	return *(char*)(equation->bucket);
}
Equation* GetEquation(Equation* equation) {
	return (Equation*)(equation->bucket);
}

void GetBucket(Equation equation[], char str[], int* i, int* equation_size) {  
		if (str[*i] == ' ')
			++(*i);
		switch (str[*i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
		//case '^':
			if (str[*i] == '-' && ((!(*i) || (str[(*i) - 1] == ' ' || str[*i - 1] == '('))))
				str[*i] = 'u';
			PushSign(&(equation[*equation_size]), str[*i], equation_size);
			++(*i);
			break;

		case '^':
			if (equation[*equation_size - 1].type == number) {
				++equat_size;
				Equat[equat_size][0].type = number;
				Equat[equat_size][0].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 1].type = equat;
				equation[*equation_size - 1].bucket = Equat[equat_size];
			}
			else if (equation[*equation_size - 1].type == variable) {
				++equat_size;
				Equat[equat_size][0].type = variable;
				Equat[equat_size][0].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 1].type = equat;
				equation[*equation_size - 1].bucket = Equat[equat_size];
			}
			else if (equation[*equation_size - 2].type == operation) {
				++equat_size;
				Equat[equat_size][0].type = operation;
				Equat[equat_size][0].bucket = equation[*equation_size - 2].bucket;
				Equat[equat_size][1].type = equat;
				Equat[equat_size][1].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 2].type = equat;
				equation[*equation_size - 2].bucket = Equat[equat_size];
				equation[*equation_size - 1].type = 0;
				equation[*equation_size - 1].bucket = 0;
				--* equation_size;
			}
			equation[*equation_size] = equation[*equation_size - 1];
			PushOperation(&(equation[*equation_size - 1]), POW, equation_size);
			++(*i);
			GetBucket(equation, str, i, equation_size);
			if (equation[*equation_size - 1].type == number) {
				++equat_size;
				Equat[equat_size][0].type = number;
				Equat[equat_size][0].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 1].type = equat;
				equation[*equation_size - 1].bucket = Equat[equat_size];
			}
			else if (equation[*equation_size - 1].type == variable) {
				++equat_size;
				Equat[equat_size][0].type = variable;
				Equat[equat_size][0].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 1].type = equat;
				equation[*equation_size - 1].bucket = Equat[equat_size];
			}
			else if (equation[*equation_size - 1].type == operation) {
				GetBucket(equation, str, i, equation_size);
				++equat_size;
				Equat[equat_size][0].type = operation;
				Equat[equat_size][0].bucket = equation[*equation_size - 2].bucket;
				Equat[equat_size][1].type = equat;
				Equat[equat_size][1].bucket = equation[*equation_size - 1].bucket;
				equation[*equation_size - 2].type = equat;
				equation[*equation_size - 2].bucket = Equat[equat_size];
				equation[*equation_size - 1].type = 0;
				equation[*equation_size - 1].bucket = 0;
				--* equation_size;
			}
			break;
		case '(':
			if (operandFlag[POW]) {
				++equat_size;
				--operandFlag[POW];
				equation[*equation_size].bucket = &(Equat[equat_size]);
				++(*i);
				int size = 0;
				while (str[*i] != ',') {
					GetBucket(Equat[equat_size], str, i, &size);
				}
				PushEquation(&(equation[*equation_size]), Equat[equat_size], equation_size);
				//PushSign(&(equation[*equation_size]), '^', equation_size);
			}
			++equat_size;
			int x = equat_size;
			equation[*equation_size].bucket = &(Equat[equat_size]);
			++(*i);
			int size = 0;
			while (str[*i] != ')') {
				GetBucket(Equat[x], str, i, &size);
			}		
			PushEquation(&(equation[*equation_size]), Equat[equat_size], equation_size);
			++(*i);
			break;
		case ')':
			++(*i);
			break;
		default:
			if (str[*i] >= '0' && str[*i] <= '9') {
				double t = 0;
				sscanf(&(str[*i]), "%lf", &t);
				while (str[*i] >= '0' && str[*i] <= '9')
					++(*i);
				if (str[*i] == '.') {
					++(*i);
					while (str[*i] >= '0' && str[*i] <= '9')
						++(*i);
				}
				if (str[*i] == 'j') {
					num[num_size]._Val[1] = t;
					PushNumber(&(equation[*equation_size]), equation_size);
					++(*i);
				}
				else {
					num[num_size]._Val[0] = t;
					PushNumber(&(equation[*equation_size]), equation_size);
				}
			}
			else {
				char temp[str_len] = { 0 };
				GetStr(temp, str, i);
				//const
				if (!strcmp(temp, "PI")) {
					if (str[*i] == '*' && str[(*i) + 1] == 'j') {
						(*i) += 2;/*если константа умножена на j сразу записываем PI*j как
								  0.00+3.14j часть "*j" в строке пропускаем через (*i) += 2*/
						num[num_size]._Val[1] = M_PI;
						PushNumber(&(equation[*equation_size]), equation_size);
					}
					else {
						num[num_size]._Val[0] = M_PI;
						PushNumber(&(equation[*equation_size]), equation_size);
					}
				}
				else if (!strcmp(temp, "e")) {
					if (str[*i] == '*' && str[(*i) + 1] == 'j') {
						(*i) += 2;
						num[num_size]._Val[1] = M_E;
						PushNumber(&(equation[*equation_size]), equation_size);
					}
					else {
						num[num_size]._Val[0] = M_E;
						PushNumber(&(equation[*equation_size]), equation_size);
					}
				}
				else if (!strcmp(temp, "j")) {
					num[num_size]._Val[1] = 1;
					PushNumber(&(equation[*equation_size]), equation_size);
				}
				//trigonometry
				// '1' - SIN, '2' - COS, '3' - TG
				else if (!strcmp(temp, "sin"))
					PushOperation(&(equation[*equation_size]), SIN, equation_size);
				else if (!strcmp(temp, "cos"))
					PushOperation(&(equation[*equation_size]), COS, equation_size);
				else if (!strcmp(temp, "tg"))
					PushOperation(&(equation[*equation_size]), TG, equation_size);
				// LOG / EXP
				// '4' - LOG(2)(x), '5' - LN, '6' - EXP
				else if (!strcmp(temp, "log"))
					PushOperation(&(equation[*equation_size]), LOG, equation_size);
				else if (!strcmp(temp, "ln"))
					PushOperation(&(equation[*equation_size]), LN, equation_size);
				else if (!strcmp(temp, "exp"))
					PushOperation(&(equation[*equation_size]), EXP, equation_size);
				//POW / SQRT / ABS
				// '^' - POW, '7' - SQRT, '8' - ABS
				else if (!strcmp(temp, "sqrt"))
					PushOperation(&(equation[*equation_size]), SQRT, equation_size);
				else if (!strcmp(temp, "pow")) {
					PushOperation(&(equation[*equation_size]), POW, equation_size);
					++operandFlag[POW];
				}
				else if (!strcmp(temp, "abs"))
					PushOperation(&(equation[*equation_size]), ABS, equation_size);
				// complex operation
				// '9' - REAL, 'A' - IMAG, 'B' - MAG, 'C' - PHASE
				else if (!strcmp(temp, "real"))
					PushOperation(&(equation[*equation_size]), REAL, equation_size);
				else if (!strcmp(temp, "imag"))
					PushOperation(&(equation[*equation_size]), IMAG, equation_size);
				else if (!strcmp(temp, "mag"))
					PushOperation(&(equation[*equation_size]), MAG, equation_size);
				else if (!strcmp(temp, "phase"))
					PushOperation(&(equation[*equation_size]), PHASE, equation_size);
				else {
					strcpy(variables[variables_size], temp);
					equation[*equation_size].type = variable;
					equation[*equation_size].bucket = variables[variables_size];
					++variables_size;
					++*equation_size;
				}
			}
			break;
		}
	return;
}

void CreateEquation(Equation equation[], char str[]) {

	int equation_size = 0;
	if (str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == ' ' || str[strlen(str) - 1] == '\t') {
		str[strlen(str) - 1] = 0;
	}
	int i = 0;
	{
		while (str[i] && str[i++] != '=');
		if (!str[i])
			i = 0;
	}

	while (str[i]) {
		GetBucket(equation, str, &i, &equation_size);
	}
	equation_size = 0;
	return;
}

void PrintEqution(Equation *equation) {
	int i = 0;
	while (equation[i].type) {
		switch (equation[i].type)
		{
		case number:
			printf("%.2lf", creal(GetNumber(&(equation[i]))));
			if (cimag(GetNumber(&(equation[i]))) >= 0)
				printf("+");						
			printf("%.2lfj ", cimag(GetNumber(&(equation[i]))));
			break;
		case sign:
		case operation:
			printf("%c ", *(char*)(equation[i].bucket));
			break;
		case variable:
			printf("%s ", (char*)(equation[i].bucket));
			break;
		case equat:
			printf("(");
			//PrintEqution((Equation*)(equation[i].bucket));
			PrintEqution(GetEquation(&(equation[i])));
			printf("\b) ");
			break;
		default:
			printf("\nError\n");
			break;
		}
		++i;
	}
	return;
}

int EquationSize(Equation equation[]) {
	int i = 0;
	while (equation[i++].type);
	return i - 1;
}

void PasteNumber(Equation* equation, _Dcomplex n) {
	num[num_size] = n;
	equation->type = number;
	equation->bucket = &(num[num_size]);
	++num_size;
	return;
}


//_Dcomplex CalculateAll(char str[][str_len], Equation equation[]) {
//	Equation newEquation[table_width] = { 0,0 };
//	int i = 0;
//	while (equation[i].type) {
//		if (equation[i].type == variable) {
//			int j = 1;
//			while (str[j][0]) {
//				char temp[str_len] = { 0 };
//				int is = 0;
//				while (str[j][is] != ' ')
//					temp[is] = str[j][is++];
//				if (!strcmp(GetVariable(&(equation[i])), temp)) {
//					CreateEquation(newEquation, str[j]);
//					PasteNumber(&(equation[i]), CalculateAll(str, newEquation));
//				}
//				++j;
//			}
//		}
//		++i;
//	}
//	_Dcomplex a = { 1, 0 };
//	return a;
//	//return CalculateEquation(equation);
//}

//int GetPriority(Equation sign) {
//	switch (GetSign(&sign))
//	{
//	case '+':
//	case '-':
//		return 1;
//		break;
//	case '/':
//	case '*':
//		return 2;
//		break;
//	case '^':
//		return 3;
//		break;
//	default:
//		break;
//	}
//}
//
//void PushStack(Equation stack[], int* size, Equation a) {
//	stack[*size] = a;
//	++*size;
//	return;
//}
//void PopStack(Equation stack[], int* size) {
//	--*size;
//	stack[*size].type = 0;
//	stack[*size].bucket = 0;
//	return;
//}
//void UnloadStack(Equation dest[], int* dest_size, Equation source[], int* source_size) {
//	while (*source_size) {
//		--* source_size;
//		dest[*dest_size] = source[*source_size];
//		source[*source_size].type = 0;
//		source[*source_size].bucket = 0;
//		++* dest_size;
//	}
//	return;
//}
//
//void SearchOperations(Equation* equation) {
//	int i = 0;
//	while (equation[i].type) {
//		_Dcomplex result = { 0,0 };
//		//if (equation[i].type == sign && GetSign(&(equation[i])) == 'u') {
//		if (GetSign(&(equation[i])) == '-' && (i == 0 || equation[i - 1].type == sign)) {
//			++i;
//			if (equation[i].type == number) {
//				result._Val[0] = -(GetNumber(&equation[i])._Val[0]);
//				result._Val[1] = -(GetNumber(&equation[i])._Val[1]);
//			}
//			else {
//				result = CalculateEquation(GetEquation(&equation[i]));
//				result._Val[0] = -result._Val[0];
//				result._Val[1] = -result._Val[1];
//			}
//			PasteNumber(&(equation[i - 1]), result);
//			int j = i;
//			while (equation[j].type) {
//				equation[j].type = equation[j + 1].type;
//				equation[j].bucket = equation[j + 1].bucket;
//				++j;
//			}
//			--i;
//		}
//		if (equation[i].type == operation) {
//			++i;/* увеличиваю i, так как следующий элемент после операции (equation[i].type == operation) - 
//				это аргумент функции в формате (Equation*)*/
//			switch (GetOperation(&(equation[i-1])))
//			{
//			case SIN:
//				result = csin(CalculateEquation(GetEquation(&equation[i])));
//				// GetEquation возвращает equation[i].bucket в формате Equation* 
//				break;
//			case COS:
//				result = ccos(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case TG:
//				result = ctan(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case LOG:
//				result = clog10(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case LN:
//				result = clog(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case EXP:
//				result = cexp(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case SQRT:
//				result = csqrt(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case ABS:
//				result._Val[0] = cabs(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case REAL:
//				result._Val[0] = creal(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case IMAG:
//				result._Val[1] = cimag(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case MAG:
//				result._Val[0] = sqrt(pow(creal(CalculateEquation(GetEquation(&equation[i]))), 2) +
//									  pow(cimag(CalculateEquation(GetEquation(&equation[i]))), 2));
//				break;
//			case PHASE:
//				result._Val[0] = carg(CalculateEquation(GetEquation(&equation[i])));
//				break;
//			case POW:
//				result = cpow(CalculateEquation(GetEquation(&equation[i])), CalculateEquation(GetEquation(&equation[i + 1])));
//				int j = i + 1;
//				while (equation[j].type) { // так как в функции 2 операнда замещает 2-ой так же как 1-ый см. строка 443
//					equation[j].type = equation[j + 1].type;
//					equation[j].bucket = equation[j + 1].bucket;
//					++j;
//				}
//			default:
//				break;
//			}
//			PasteNumber(&(equation[i - 1]), result); //замещает операцию результирующим значением -> SIN(PI)->0(PI)
//			int j = i;
//			while (equation[j].type) { // цикл замещает операнд сдвигая массив после него влево -> 0(PI)+10->1+10
//				equation[j].type = equation[j + 1].type;
//				equation[j].bucket = equation[j + 1].bucket;
//				++j;
//			}
//			--i;
//			/*PrintEqution(equation);
//			printf("\n");*/
//		}
//		++i;
//	}
//}
//
//_Dcomplex CalculateEquation(Equation* equation) {
//	SearchOperations(equation);
//	Equation main_stack[table_width] = { 0, 0 };
//	Equation additional_stack[table_width] = { 0, 0 };
//	int main_stack_size = 0, additional_stack_size = 0;
//	int i = 0;
//	while (equation[i].type) {
//		switch (equation[i].type)
//		{
//		case equat:
//			PasteNumber(&(equation[i]), CalculateEquation(GetEquation(&(equation[i]))));
//		case number:
//			PushStack(main_stack, &main_stack_size, equation[i]);
//			break;
//		case sign:
//			if (additional_stack_size == 0)
//				PushStack(additional_stack, &additional_stack_size, equation[i]);
//			else {
//				while (additional_stack_size && GetPriority(equation[i]) <= GetPriority(additional_stack[additional_stack_size - 1])) {
//					PushStack(main_stack, &main_stack_size, additional_stack[additional_stack_size - 1]);
//					PopStack(additional_stack, &additional_stack_size);
//				}
//				PushStack(additional_stack, &additional_stack_size, equation[i]);
//			}
//			break;
//		default:
//			break;
//		}
//		++i;
//	}
//	UnloadStack(main_stack, &main_stack_size, additional_stack, &additional_stack_size);
//	/*PrintEqution(main_stack);
//	printf("\n");*/
//
//	i = 0;
//	_Dcomplex a = { 0,0 };
//	while (main_stack[i].type) {
//		if (main_stack[i].type == number)
//			PushStack(additional_stack, &additional_stack_size, main_stack[i]);
//		else {
//			switch (GetSign(&(main_stack[i])))
//			{
//			case '+':
//				a = GetNumber(&(additional_stack[additional_stack_size - 2]));
//				a._Val[0] += creal(GetNumber(&(additional_stack[additional_stack_size - 1])));
//				a._Val[1] += cimag(GetNumber(&(additional_stack[additional_stack_size - 1])));
//				break;
//			case '-':
//				a = GetNumber(&(additional_stack[additional_stack_size - 2]));
//				a._Val[0] -= creal(GetNumber(&(additional_stack[additional_stack_size - 1])));
//				a._Val[1] -= cimag(GetNumber(&(additional_stack[additional_stack_size - 1])));
//				break;
//			case '*':
//				a = _Cmulcc(GetNumber(&(additional_stack[additional_stack_size - 2])),
//					GetNumber(&(additional_stack[additional_stack_size - 1])));
//				break;
//			case '/':
//				a = _Cmulcr(_Cmulcc(GetNumber(&(additional_stack[additional_stack_size - 2])),
//					conj(GetNumber(&(additional_stack[additional_stack_size - 1])))),
//					1/(_Cmulcc(GetNumber(&(additional_stack[additional_stack_size - 1])),
//						conj(GetNumber(&(additional_stack[additional_stack_size - 1]))))._Val[0]));
//				break;
//			case '^':
//				a = cpow(GetNumber(&(additional_stack[additional_stack_size - 2])),
//					GetNumber(&(additional_stack[additional_stack_size - 1])));
//				break;
//			default:
//				break;
//			}
//			PasteNumber(&(additional_stack[additional_stack_size - 2]), a);
//			PopStack(additional_stack, &additional_stack_size);
//		}
//		++i;
//	}
//	return GetNumber(&(additional_stack[0]));
//}