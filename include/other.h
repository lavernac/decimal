#pragma once
#include <stdlib.h>

#include "../s21_decimal.h"

/* Другие функции

Возвращаемое значение — код ошибки:
0 — OK;
1 — ошибка вычисления. */

// Округляет указанное Decimal число до ближайшего целого числа в сторону
// отрицательной бесконечности.
int s21_floor(s21_decimal value, s21_decimal *result);

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result);

// Возвращает целые цифры указанного Decimal числа; любые дробные цифры
// отбрасываются, включая конечные нули.
int s21_truncate(s21_decimal value, s21_decimal *result);

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result);

void big_truncate(s21_big_decimal *result);

int is_correct(s21_decimal value);

float get_rand_float(float left, float right);

int s21_big_round(s21_big_decimal big_value, s21_big_decimal *result);