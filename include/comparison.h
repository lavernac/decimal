#pragma once
#include "../s21_decimal.h"

/* Операторы сравнения

Возвращаемое значение:
0 — FALSE;
1 — TRUE. */

int s21_is_less(s21_decimal, s21_decimal);              // Меньше
int s21_is_less_or_equal(s21_decimal, s21_decimal);     // Меньше или равно <=
int s21_is_greater(s21_decimal, s21_decimal);           // Больше >
int s21_is_greater_or_equal(s21_decimal, s21_decimal);  // Больше или равно >=
int s21_is_equal(s21_decimal, s21_decimal);             // Равно ==
int s21_is_not_equal(s21_decimal, s21_decimal);         // Не равно !=