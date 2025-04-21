#include "../include/other.h"

#include <math.h>

// Округляет Decimal до ближайшего целого числа.
int s21_round(s21_decimal value, s21_decimal *result) {
  char str[128] = {0};
  s21_big_decimal big_value = decimal_to_big(value);
  s21_big_decimal_to_string(big_value, str);
  int ret = 0;
  if (!result)
    ret = 1;
  else if (check_helper_bit(value))
    ret = 1;
  else if (strchr(str, ',')) {
    null_decimal_s(result);
    int sign = get_sign(value);
    set_sign(&value, 0);
    int len = strlen(str) - 1, len_touch = strlen(strchr(str, ',')) - 1,
        lent = len - len_touch + 2;
    int flag = 0;
    if (lent <= len && str[len - len_touch + 1] == '5') {
      for (int i = lent; i <= len && !flag; i++)
        if (str[i] != '0') flag = 1;
    }

    if (str[len - len_touch + 1] >= '5') {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_truncate(value, result);
      if ((str[len - len_touch - 1] - '0') % 2 == 0) {
        if (flag && str[len - len_touch + 1] == '5')
          s21_add(*result, one, result);
        else if (str[len - len_touch + 1] > '5')
          s21_add(*result, one, result);
      } else
        s21_add(*result, one, result);
    } else
      s21_truncate(value, result);
    set_sign(result, sign);
  } else
    *result = big_to_decimal(big_value);
  return ret;
}

int s21_big_round(s21_big_decimal big_value, s21_big_decimal *result) {
  char str[60] = {0};
  s21_big_decimal_to_string(big_value, str);
  int ret = 0;
  if (!result)
    ret = 1;
  else if (big_check_helper_bit(big_value))
    ret = 1;
  else if (strchr(str, ',')) {
    null_decimal(result);
    int sign = big_get_sign(big_value);
    big_set_sign(&big_value, 0);
    int len = strlen(str) - 1, len_touch = strlen(strchr(str, ',')) - 1,
        lent = len - len_touch + 2;
    int flag = 0;
    if (lent <= len && str[len - len_touch + 1] == '5') {
      for (int i = lent; i <= len && !flag; i++)
        if (str[i] != '0') flag = 1;
    }

    if (str[len - len_touch + 1] >= '5') {
      s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
      big_truncate(&big_value);
      *result = big_value;
      if ((str[len - len_touch - 1] - '0') % 2 == 0) {
        if (flag && str[len - len_touch + 1] == '5')
          bitwise_addition(*result, one, result);
        else if (str[len - len_touch + 1] > '5')
          bitwise_addition(*result, one, result);
      } else
        bitwise_addition(*result, one, result);
    } else {
      big_truncate(&big_value);
      *result = big_value;
    }

    big_set_sign(result, sign);
  } else
    *result = big_value;
  return ret;
}

// Делаем округление в меньшую сторону без знака, потом его возвращаем
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int ret_status = 0;
  if (result && !check_helper_bit(value)) {
    int sign = get_sign(value);
    set_sign(&value, 0);
    s21_floor(value, result);
    set_sign(result, sign);
  } else
    ret_status = 1;
  return ret_status;
}

int div_by_10(unsigned int bits[3]) {
  unsigned long long num = 0;
  for (int i = 2; i >= 0; i--) {
    num = (num << 32) | bits[i];
    bits[i] = num / 10;
    num %= 10;
  }
  return num;
}

int big_div_by_10(unsigned int bits[7]) {
  unsigned long long num = 0;
  for (int i = 6; i >= 0; i--) {
    num = (num << 32) | bits[i];
    bits[i] = num / 10;
    num %= 10;
  }
  return num;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  if (!result || check_helper_bit(value)) return 1;

  memcpy(result, &value, sizeof(s21_decimal));
  int scale = get_scale(value);
  int sign = get_sign(value);

  if (scale == 0) return 0;

  unsigned int bits[3] = {value.bits[0], value.bits[1], value.bits[2]};
  int has_remainder = 0;

  for (int i = 0; i < scale; i++) {
    has_remainder = div_by_10(bits) || has_remainder;
  }

  result->bits[0] = bits[0];
  result->bits[1] = bits[1];
  result->bits[2] = bits[2];
  set_scale(result, 0);
  if (sign && has_remainder) {
    unsigned int one[3] = {1, 0, 0};
    unsigned long long carry = 0;
    for (int i = 0; i < 3; i++) {
      carry += (unsigned long long)bits[i] + one[i];
      bits[i] = carry & 0xFFFFFFFF;
      carry >>= 32;
    }
    if (carry) return 1;
    memcpy(result->bits, bits, sizeof(bits));
  }

  return 0;
}

int big_floor(s21_big_decimal value, s21_big_decimal *result) {
  if (!result) return 1;

  memcpy(result, &value, sizeof(s21_big_decimal));
  int scale = big_get_scale(value);
  int sign = big_get_sign(value);

  if (scale == 0) return 0;

  unsigned int bits[7] = {0};
  for (int i = 0; i < 7; i++) bits[i] = value.bits[i];
  int has_remainder = 0;
  for (int i = 0; i < scale; i++) {
    has_remainder = big_div_by_10(bits) || has_remainder;
  }
  for (int i = 0; i < 7; i++) (*result).bits[i] = bits[i];
  big_set_scale(result, 0);

  if (sign && has_remainder) {
    unsigned int one[7] = {1, 0, 0, 0, 0, 0, 0};
    unsigned long long carry = 0;
    for (int i = 0; i < 7; i++) {
      carry += (unsigned long long)bits[i] + one[i];
      bits[i] = carry & 0xFFFFFFFF;
      carry >>= 32;
    }
    if (carry) return 1;
    memcpy(result->bits, bits, sizeof(bits));
  }

  return 0;
}

void big_truncate(s21_big_decimal *result) {
  s21_big_decimal copy = *result;
  int sign = big_get_sign(*result);
  big_set_sign(&copy, 0);
  big_floor(copy, result);
  big_set_sign(result, sign);
}

// Возвращает результат умножения указанного Decimal на -1.
int s21_negate(s21_decimal value, s21_decimal *result) {
  int ret_status = 0;
  if (result && !check_helper_bit(value)) {
    result->bits[0] = value.bits[0];
    result->bits[1] = value.bits[1];
    result->bits[2] = value.bits[2];
    set_scale(result, get_scale(value));
    set_sign(result, !get_sign(value));
  } else
    ret_status = 1;
  return ret_status;
}

// int is_correct(s21_decimal value) {
//   int error = 0;
//   for (int i = 96; i < 112 && !error; i++) error = get_bit(value, i);
//   for (int i = 120; i < 127 && !error; i++) error = get_bit(value, i);
//   return error;
// }

float get_rand_float(float left, float right) {
  if (left > right) {
    return get_rand_float(right, left);
  }
  return left + (right - left) * rand() / RAND_MAX;
}