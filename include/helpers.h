#pragma once
#include <string.h>

#include "../s21_decimal.h"

int is_zero_s(s21_decimal num);
int is_zero(s21_big_decimal num);  // bits[0] = bits[1] = bits[2] = 0
void null_decimal(s21_big_decimal *num);
void null_decimal_s(s21_decimal *num);

int big_get_sign(s21_big_decimal num);
void big_set_sign(s21_big_decimal *num, int sign_val);
int big_get_scale(s21_big_decimal num);
void big_set_scale(s21_big_decimal *num, int scale_val);
int big_get_bit(s21_big_decimal num, int bit);  // 0 -
int big_set_bit(s21_big_decimal *num, int bit, unsigned value);

int get_sign(s21_decimal num);
void set_sign(s21_decimal *num, int sign_val);
int get_scale(s21_decimal num);
void set_scale(s21_decimal *num, int scale_val);
int get_bit(s21_decimal num, int bit);  // 0 - 95
int set_bit(s21_decimal *num, int bit, unsigned value);

void shift_left(s21_big_decimal *num, int shift_value);
s21_big_decimal clone_big(s21_big_decimal value);
void to_binary(unsigned a);
void print_full(s21_big_decimal num);
void print_s(s21_decimal num);

// void string_to_decimal(char *str, s21_decimal *num);
int check_res(s21_big_decimal num);

int check_helper_bit(s21_decimal num);
int big_check_helper_bit(s21_big_decimal num);