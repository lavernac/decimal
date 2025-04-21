#pragma once
#include "../s21_decimal.h"

/* Арифметические операторы

Функции возвращают код ошибки:
0 — OK;
1 — число слишком велико или равно бесконечности;
2 — число слишком мало или равно отрицательной бесконечности;
3 — деление на 0. */

int s21_add(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Сложение +
int s21_sub(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Вычитание -
int s21_mul(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Умножение *
int s21_div(s21_decimal value_1, s21_decimal value_2,
            s21_decimal *result);  // Деление /

int normalize(s21_big_decimal *num1, s21_big_decimal *num2,
              s21_big_decimal *res);

int s21_is_greater_or_eq_big(s21_big_decimal tmp1, s21_big_decimal tmp2,
                             int must_eq, int);
void bitwise_addition(s21_big_decimal value_1, s21_big_decimal value_2,
                      s21_big_decimal *result);
void mul_10(s21_big_decimal *value, s21_big_decimal *res, int scale, int diff);