#ifndef Const_enum_H
#define Const_enum_H

#define _USE_MATH_DEFINES
#define table_width 100
#define str_len 256
#define RES_SIZE 100

typedef enum { sign = 1, operation, variable, number, equat } Type;
typedef enum {
	SIN=1, COS, TG, LOG, LN, EXP, POW, SQRT,
	ABS, REAL, IMAG, MAG, PHASE, op_quan = 14, 
	plus = '+', minus = '-', devide = '/',
	multiply = '*', power = '^'
} Operation;
//typedef enum {
//	SIN = 's', COS = 'c', TG = 't', LOG = 'l', LN = 'n', EXP = 'e', POW = 'p', SQRT = 'q',
//	ABS = 'a', REAL = 'r', IMAG = 'i', MAG = 'm', PHASE = 'h', op_quan = 256,
//	plus = '+', minus = '-', devide = '/',
//	multiply = '*', power = '^'
//} Operation;

#endif