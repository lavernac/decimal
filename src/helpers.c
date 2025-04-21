#include "../include/helpers.h"

#include <stdio.h>  // ЕСЛИ ЧО ПОТОМ УДАЛИМ
#include <stdlib.h>

int is_zero_s(s21_decimal num) {
  int is_zero = 1;
  for (int i = 0; i < 3; i++)
    if (num.bits[i]) is_zero = 0;
  return is_zero;
}

int is_zero(s21_big_decimal num) {
  int is_zero = 1;
  for (int i = 0; i < 7; i++)
    if (num.bits[i]) is_zero = 0;
  return is_zero;
}

void null_decimal(s21_big_decimal *num) {
  for (int i = 0; i < 8; i++) num->bits[i] = 0;
}

void null_decimal_s(s21_decimal *num) {
  for (int i = 0; i < 4; i++) num->bits[i] = 0;
}

int big_get_sign(s21_big_decimal num) { return (num.bits[7] >> 31) & 1; }

void big_set_sign(s21_big_decimal *num, int sign_val) {
  if (sign_val == 1 || sign_val == '-')
    num->bits[7] |= (1 << 31);
  else if (sign_val == 0 || sign_val == '+')
    num->bits[7] &= ~(1 << 31);
}

int big_get_scale(s21_big_decimal num) {
  big_set_sign(&num, 0);
  return num.bits[7] >> 16;
}

void big_set_scale(s21_big_decimal *num, int scale_val) {
  if (scale_val >= 0) {
    scale_val <<= 16;
    num->bits[7] &= 1 << 31;
    num->bits[7] |= scale_val;
  }
}

int big_get_bit(s21_big_decimal num, int bit) {
  int res = -1;
  if (bit >= 0 && bit <= 223) res &= (num.bits[bit / 32] >> (bit % 32)) & 1;
  return res;
}

int big_set_bit(s21_big_decimal *num, int bit, unsigned value) {
  int flag = 0;
  if (bit >= 0 && bit <= 223 && (value == 0 || value == 1)) {
    if (value == 1)
      num->bits[bit / 32] |= (1 << (bit % 32));
    else
      num->bits[bit / 32] &= ~(1 << (bit % 32));
  } else
    flag = -1;
  return flag;
}

void convert_to_binary(unsigned a, int level) {
  if (level != 31) convert_to_binary(a / 2, level + 1);
  printf("%d", a % 2);
}

void to_binary(unsigned a) {
  convert_to_binary(a, 0);
  putchar('\n');
}

// void print_full(s21_big_decimal num) {
//   for (int i = 223; i >= 0; i--) {
//     printf("%d", big_get_bit(num, i));
//     if (!(i % 32)) printf("\n");
//   }
//   printf("        Вспомогательный\n");
//   to_binary(num.bits[7]);
// }

// void print_s(s21_decimal num) {
//   for (int i = 95; i >= 0; i--) {
//     printf("%d", get_bit(num, i));
//     if (!(i % 32)) printf("\n");
//   }
//   printf("        Вспомогательный\n");
//   to_binary(num.bits[3]);
// }

int get_sign(s21_decimal num) { return (num.bits[3] >> 31) & 1; }

void set_sign(s21_decimal *num, int sign_val) {
  if (sign_val == 1 || sign_val == '-')
    num->bits[3] |= (1 << 31);
  else if (sign_val == 0 || sign_val == '+')
    num->bits[3] &= ~(1 << 31);
}

int get_scale(s21_decimal num) {
  set_sign(&num, 0);
  return num.bits[3] >> 16;
}

void set_scale(s21_decimal *num, int scale_val) {
  if (scale_val >= 0 && scale_val <= 28) {
    scale_val <<= 16;
    num->bits[3] &= 1 << 31;
    num->bits[3] |= scale_val;
  }
}

int get_bit(s21_decimal num, int bit) {
  int res = -1;
  if (bit >= 0 && bit <= 95) res &= (num.bits[bit / 32] >> (bit % 32)) & 1;
  return res;
}

int set_bit(s21_decimal *num, int bit, unsigned value) {
  int flag = 0;
  if (bit >= 0 && bit <= 95 && (value == 0 || value == 1)) {
    if (value == 1)
      num->bits[bit / 32] |= (1 << (bit % 32));
    else
      num->bits[bit / 32] &= ~(1 << (bit % 32));
  } else
    flag = -1;
  return flag;
}

void shift_left(s21_big_decimal *num, int shift_value) {
  if (shift_value) {
    unsigned memory = 0;
    int num_int = (int)(sizeof(s21_big_decimal) / sizeof(unsigned) - 1);
    for (int i = 0; i < num_int; i++) {
      unsigned temp = num->bits[i];
      num->bits[i] <<= shift_value;
      num->bits[i] |= memory;
      memory = temp >> (32 - shift_value);
    }
  }
}

s21_big_decimal clone_big(s21_big_decimal value) {
  s21_big_decimal clone = {0};
  for (int i = 0; i < 8; i++) clone.bits[i] = value.bits[i];
  return clone;
}

int check_res(s21_big_decimal num) {
  int res = 0, sign = big_get_sign(num);
  for (int i = 3; i <= 6; i++) {
    if (num.bits[i] && !sign)
      res = 1;
    else if (num.bits[i] && sign)
      res = 2;
  }
  if (big_get_scale(num) > 28) {
    res = 3;
  }
  return res;
}