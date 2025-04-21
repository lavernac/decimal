#pragma once
#include "../s21_decimal.h"

/* Преобразователи

Возвращаемое значение — код ошибки:
0 — OK;
1 — ошибка конвертации. */
int s21_from_int_to_decimal(int src, s21_decimal *dst);      // Из int
int s21_from_float_to_decimal(float src, s21_decimal *dst);  // Из float
int s21_from_decimal_to_int(s21_decimal src, int *dst);      // В int
int s21_from_decimal_to_float(s21_decimal src, float *dst);  // В float

s21_big_decimal decimal_to_big(s21_decimal value);
s21_decimal big_to_decimal(s21_big_decimal value);
void string_to_decimal(char *str, s21_decimal *num);
void s21_big_decimal_to_string(s21_big_decimal decimal, char *str);