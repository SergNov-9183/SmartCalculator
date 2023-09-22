#include "CalcImpl.h"

#include <stdio.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define NUMBER 'v'
#define UNARY_MINUS 'u'

typedef char BOOL;

typedef struct {
    char operand;
    _Dcomplex value;
} StackOperand;

typedef struct {
    unsigned size;
    unsigned capacity;
    StackOperand elements[table_width];
} OperandStack;

BOOL isZero(double value) {
    double precision = 1000000000;
    return llround(value * precision) == 0 ? TRUE : FALSE;
}

_Dcomplex complexValue(double r, double j) {
    _Dcomplex result;
    result._Val[0] = r;
    result._Val[1] = j;
    return result;
}

_Dcomplex divideComplex(_Dcomplex value1, _Dcomplex value2) {
    double a = creal(value1);
    double b = cimag(value1);
    double c = creal(value2);
    double d = cimag(value2);
    double denominator = c * c + d * d;
    //if (isZero(denominator)) {
    //    return complexValue(0, 0);
    //}
    _Dcomplex numerator = complexValue(a * c + b * d, b * c - a * d);
    return complexValue(creal(numerator) / denominator, cimag(numerator) / denominator);
}

_Dcomplex multiplyComplex(_Dcomplex value1, _Dcomplex value2) {
    double a = creal(value1);
    double b = cimag(value1);
    double c = creal(value2);
    double d = cimag(value2);
    return complexValue(a * c - b * d, b * c + a * d);
}

_Dcomplex minusComplex(_Dcomplex value1, _Dcomplex value2) {
    return complexValue(creal(value1) - creal(value2), cimag(value1) - cimag(value2));
}

_Dcomplex plusComplex(_Dcomplex value1, _Dcomplex value2) {
    return complexValue(creal(value1) + creal(value2), cimag(value1) + cimag(value2));
}

_Dcomplex unaryMinusComplex(_Dcomplex value) {
    return complexValue(-creal(value), -cimag(value));
}

_Dcomplex calcComplexFunction(char function, _Dcomplex arg1, _Dcomplex arg2) {
    switch (function) {
    case COS: return ccos(arg1);
    case SIN: return csin(arg1);
    case TG: return ctan(arg1);
    case LOG: return clog10(arg1);
    case LN: return clog(arg1);
    case SQRT: return csqrt(arg1);
    case POW: return cpow(arg1, arg2);
    case ABS: return complexValue(cabs(arg1), 0);
    case MAG: return complexValue(cabs(arg1), 0);
    case EXP: return cexp(arg1);
    case REAL: return complexValue(creal(arg1), 0);
    case IMAG: return complexValue(0, cimag(arg1));
    case PHASE: return complexValue(carg(arg1), 0);
    default:
        printf("\nError: unsupported complex function: %c\n", function);
        return complexValue(0, 0);
    }
}

BOOL canPushStack(OperandStack* stack, char operand) {
    if (stack->size == 0) {
        return operand == NUMBER ? TRUE : FALSE;
    }
    if (stack->size < stack->capacity) {
        return operand == NUMBER
            ? stack->elements[stack->size - 1].operand != NUMBER
            : stack->elements[stack->size - 1].operand == NUMBER;
    }
    return FALSE;
}

BOOL pushStack(OperandStack* stack, char operand, _Dcomplex value) {
    if (canPushStack(stack, operand)) {
        stack->elements[stack->size].operand = operand;
        stack->elements[stack->size].value = value;
        stack->size++;
        return TRUE;
    }
    return FALSE;
}

BOOL canCalcTopStack(OperandStack* stack) {
    return stack->size > 1 && (stack->elements[stack->size - 1].operand == '/' || stack->elements[stack->size - 1].operand == '*');
}

BOOL calcTopStack(OperandStack* stack, _Dcomplex value) {
    if (canCalcTopStack(stack)) {
        stack->elements[stack->size - 2].value = stack->elements[stack->size - 1].operand == '/'
            ? divideComplex(stack->elements[stack->size - 2].value, value)
            : multiplyComplex(stack->elements[stack->size - 2].value, value);
        stack->elements[stack->size - 1].operand = '\0';
        stack->size--;
        return TRUE;
    }
    return FALSE;
}

BOOL pushNumberToStack(OperandStack* stack, _Dcomplex value, char* unaryMinus) {
    if (calcTopStack(stack, *unaryMinus ? unaryMinusComplex(value) : value) ||
        pushStack(stack, NUMBER, *unaryMinus ? unaryMinusComplex(value) : value)) {
        *unaryMinus = FALSE;
        return TRUE;
    }
    double a = creal(value);
    double b = cimag(value);
    printf("\nError: can not push number: %.5lf%s%.5lfj\n", a, b < 0 ? " \b" : "+", b);
    return FALSE;
}

BOOL pushSignToStack(OperandStack* stack, char sign, char* unaryMinus) {
    if (sign == UNARY_MINUS) {
        *unaryMinus = TRUE;
        return TRUE;
    }
    if (pushStack(stack, sign, complexValue(0, 0)) == TRUE) {
        return TRUE;
    }
    printf("\nError: can not push sign: %c\n", sign);
    return FALSE;
}

_Dcomplex calcStack(OperandStack* stack) {
    _Dcomplex result = complexValue(0, 0);
    if (stack->size > 0) {
        StackOperand stackOperand = stack->elements[0];
        result = stackOperand.value;
        char currentOperation = '\0';

        for (unsigned i = 1; i < stack->size; ++i) {
            stackOperand = stack->elements[i];
            if (stackOperand.operand == '+' || stackOperand.operand == '-') {
                currentOperation = stackOperand.operand;
            }
            else {
                result = currentOperation == '+' ? plusComplex(result, stackOperand.value) : minusComplex(result, stackOperand.value);
            }
        }
    }
    return result;
}

_Dcomplex getValue(Equation* equation, unsigned* index) {
    ++(*index);
    switch (equation[*index].type) {
    case number:
        return *(_Dcomplex*)(equation[*index].bucket);
    case equat:
        return calculate((Equation*)(equation[*index].bucket));
    default:
        printf("\nError: can not get number: %c\n", equation[*index].type);
        return complexValue(0, 0);
    }
}

_Dcomplex calcFunction(Equation* equation, unsigned* index, char function) {
    _Dcomplex value1 = getValue(equation, index);
    _Dcomplex value2 = complexValue(0, 0);
    if (function == POW) {
        value2 = getValue(equation, index);
    }
    return calcComplexFunction(function, value1, value2);
}

_Dcomplex calculate(Equation* equation) {
    OperandStack stack = { 0 };
    stack.capacity = table_width;
    BOOL unaryMinus = FALSE;
    unsigned index = 0;
    while (equation[index].type) {
        switch (equation[index].type) {
        case number:
            if (!pushNumberToStack(&stack, *(_Dcomplex*)(equation[index].bucket), &unaryMinus)) {
                return complexValue(0, 0);
            }
            break;
        case sign:
            if (!pushSignToStack(&stack, *(char*)(equation[index].bucket), &unaryMinus)) {
                return complexValue(0, 0);
            }
            break;
        case operation:
            if (!pushNumberToStack(&stack, calcFunction(equation, &index, *(char*)(equation[index].bucket)), &unaryMinus)) {
                printf("\nError: can not calculate function\n");
                return complexValue(0, 0);
            }
            break;
        case equat:
            if (!pushNumberToStack(&stack, calculate((Equation*)(equation[index].bucket)), &unaryMinus)) {
                printf("\nError: can not calculate subequation\n");
                return complexValue(0, 0);
            }
            break;
        default:
            printf("\nError: not supported type: %d\n", equation[index].type);
            return complexValue(0, 0);
        }
        ++index;
    }
    return calcStack(&stack);
}

void printComplexValue(_Dcomplex value, int decimal_places) {
    double a = creal(value);
    double b = cimag(value);
    double minimum = pow(0.1, decimal_places) * 0.5;
    /*BOOL aIsZero = isZero(a);
    BOOL bIsZero = isZero(b);
    if (aIsZero && bIsZero) {
        printf("\nresult = 0\n");
    }
    else if (aIsZero) {
        printf("\nresult = %.5lfj\n", b);
    }
    else if (bIsZero) {
        printf("\nresult = %.5lf\n", a);
    }
    else {
        printf("\nresult = %.5lf%s%.5lfj\n", a, b < 0 ? " \b" : "+", b);
    }*/
    char output[7] = "%._lf\0\0";
    if (decimal_places > 9) {
        output[2] = decimal_places / 10 + '0';
        output[3] = decimal_places % 10 + '0';
        output[4] = 'l';
        output[5] = 'f';
    }
    else
        output[2] = decimal_places + '0';
    if (fabs(a) < minimum && fabs(b) < minimum)
        printf("\nresult = 0\n");
    else {
        //printf("\nresult = ");
        if (fabs(a) >= minimum) {
            if (fabs(a - (int)a) < minimum)
                printf("%d", (int)a);
            else
                printf(output, a);
        }
        if (fabs(b) >= minimum) {
            if (fabs(a) >= minimum && b > 0)
                printf("+");
            if (fabs(b - (int)b) < minimum)
                printf("%dj", (int)b);
            else {
                printf(output, b);
                printf("j");
            }
        }
        printf("\n");
    }
}
