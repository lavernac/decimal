#include "../include/converters.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/helpers.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return 1;
  null_decimal_s(dst);
  if (src < 0) {
    set_sign(dst, 1);
    dst->bits[0] = -src;
  } else
    dst->bits[0] = src;
  return 0;
}

void float_scale(char *src, char *copy_src, float num, int *scale,
                 int *sign_scale, int scale_diff) {
  for (int i = 0; i < 32; i++) copy_src[i] = 0;
  sprintf(src, "%.*E", 6 - scale_diff, fabsf(num));
  for (int i = 0, j = 0; i < 8 - scale_diff; i++) {
    if (i != 1) {
      copy_src[j] = src[i];
      j++;
    }
  }
  (*scale) = 0;
  for (size_t i = strlen(src) - 3; i < strlen(src); i++) {
    if (src[i] != '+' && src[i] != '-') {
      (*scale) += src[i] - '0';
      if (i != strlen(src) - 1) (*scale) *= 10;
    } else
      (*sign_scale) = src[i];
  }
}

int s21_from_float_to_decimal(float num, s21_decimal *dst) {
  if (!dst) return 1;
  null_decimal_s(dst);
  if (num == 0.0) return 0;
  if (isnan(num) || isinf(num) ||
      fabs(num) > 7.9228162514264337593543950335e28 || fabs(num) < 1e-28)
    return 1;  // INFINITE / NAN / MAXMINFLOAT

  char src[32] = {0};
  char copy_src[32] = {0};
  int scale = 0, sign_scale = 0, copy_scale = 0;
  float_scale(src, copy_src, num, &scale, &sign_scale, 0);
  copy_scale = scale;
  if (scale >= 23 && sign_scale == '-') {
    float_scale(src, copy_src, num, &scale, &sign_scale, (scale - 22));
    if (copy_scale == 28 && scale < 28)
      scale -= (scale - 22) + 1;
    else
      scale -= (scale - 22);
  }

  copy_scale = scale;
  int flag = 0;
  if (scale > 6)
    scale -= 6;
  else {
    if (sign_scale == '+') copy_scale = -scale;
    flag = 1;
  }

  string_to_decimal(copy_src, dst);
  s21_decimal dec10 = {{10, 0, 0, 0}};
  if (sign_scale == '+' && !flag) {
    for (int i = 0; i < scale; i++) s21_mul(dec10, *dst, dst);
  } else {
    for (int i = 0; i < copy_scale + 6; i++) s21_div(*dst, dec10, dst);
  }

  if (num < 0.0) set_sign(dst, 1);
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst || check_helper_bit(src)) return 1;
  s21_decimal src_copy = src;
  s21_truncate(src, &src_copy);
  int ret_status = 0;
  if (src_copy.bits[0] <= INT_MAX ||
      ((src_copy.bits[0] <= 2147483648) && get_sign(src))) {
    *dst = src_copy.bits[0];
    for (int i = 1; i < 3; i++)
      if (src_copy.bits[i]) ret_status = 1;
    if (get_sign(src)) *dst *= -1;
  } else
    ret_status = 1;
  return ret_status;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst || check_helper_bit(src)) return 1;

  double result = 0;
  for (int i = 0; i < 96; i++) result += pow(2, i) * get_bit(src, i);

  if (get_scale(src) > 0) result /= pow(10, get_scale(src));

  if (get_sign(src) == 1) result = -result;

  *dst = result;
  return 0;
}

s21_big_decimal decimal_to_big(s21_decimal value) {
  s21_big_decimal big_value = {0};
  for (int i = 0; i < 3; i++) big_value.bits[i] = value.bits[i];
  big_value.bits[7] = value.bits[3];
  return big_value;
}

s21_decimal big_to_decimal(s21_big_decimal big_value) {
  s21_decimal value = {0};
  for (int i = 0; i < 3; i++) value.bits[i] = big_value.bits[i];
  value.bits[3] = big_value.bits[7];
  return value;
}

void reverse_str(char *str) {
  int len = strlen(str);
  for (int i = 0; i < len / 2; i++) {
    char temp = str[i];
    str[i] = str[len - 1 - i];
    str[len - 1 - i] = temp;
  }
}

unsigned int divide_by_10(unsigned int num[6]) {
  unsigned long long remainder = 0;
  for (int i = 6; i >= 0; i--) {
    unsigned long long temp = (remainder << 32) | num[i];
    num[i] = (unsigned int)(temp / 10);
    remainder = temp % 10;
  }
  return (unsigned int)remainder;
}

void uint96_to_str(unsigned int num[7], char *str) {
  unsigned int copy[7] = {num[0], num[1], num[2], num[3],
                          num[4], num[5], num[6]};
  char buffer[128] = {0};
  int index = 0;

  while (!(copy[0] == 0 && copy[1] == 0 && copy[2] == 0 && copy[3] == 0 &&
           copy[4] == 0 && copy[5] == 0 && copy[6] == 0)) {
    buffer[index++] = divide_by_10(copy) + '0';
  }

  if (index == 0) {
    strcpy(str, "0");
  } else {
    reverse_str(buffer);
    strcpy(str, buffer);
  }
}

void string_to_decimal(char *str, s21_decimal *num) {
  if (str && num) {
    s21_decimal str_num = {0};
    s21_decimal dec_10 = {0};
    char *str1 = calloc(60, sizeof(char)), *str2 = calloc(60, sizeof(char));
    int i = 0, sign = 0, len_str2 = 0;
    if (str[0] == '-') {
      i++;
      sign = 1;
    }

    for (int j = 0; str[i] != ',' && str[i] != '\0'; i++, j++) str1[j] = str[i];
    for (; str[i] != '\0'; i++, len_str2++) str2[len_str2] = str[i + 1];

    char *copy_not_scale = calloc(120, sizeof(char));
    sprintf(copy_not_scale, "%s%s", str1, str2);
    for (int len = strlen(copy_not_scale) - 1, i = 0; len >= 0; len--, i++) {
      null_decimal_s(&str_num);
      dec_10.bits[0] = 10;
      s21_from_int_to_decimal(copy_not_scale[i] - '0', &str_num);
      for (int num_dec = 0; num_dec < len; num_dec++)
        s21_mul(str_num, dec_10, &str_num);
      s21_add(str_num, *num, num);
    }
    set_scale(num, len_str2 - 1);
    set_sign(num, sign);
    if (str1) free(str1);
    if (str2) free(str2);
    if (copy_not_scale) free(copy_not_scale);
  }
}

void s21_big_decimal_to_string(s21_big_decimal decimal, char *str) {
  unsigned int num[7] = {0};
  for (int i = 0; i < 7; i++) num[i] = decimal.bits[i];
  char mantissa_str[128] = {0};
  uint96_to_str(num, mantissa_str);

  unsigned int exponent = big_get_scale(decimal);
  int sign = big_get_sign(decimal);
  int len = strlen(mantissa_str);
  int point_pos = len - exponent;

  char result[258] = {0};

  if (exponent == 0) {
    if (!sign)
      strcpy(result, mantissa_str);
    else
      sprintf(result, "-%s", mantissa_str);
  } else if (point_pos > 0) {
    if (!sign)
      snprintf(result, sizeof(result), "%.*s,%.*s", point_pos, mantissa_str,
               len - point_pos, mantissa_str + point_pos);
    else
      snprintf(result, sizeof(result), "-%.*s,%.*s", point_pos, mantissa_str,
               len - point_pos, mantissa_str + point_pos);
  } else if (exponent <= 127) {
    char buff[128] = {0};
    for (int i = 0; i < -point_pos; i++) buff[i] = '0';
    if (!sign)
      snprintf(result, sizeof(result), "0,%s%s", buff, mantissa_str);
    else
      snprintf(result, sizeof(result), "-0,%s%s", buff, mantissa_str);
  }
  strcpy(str, result);
}
