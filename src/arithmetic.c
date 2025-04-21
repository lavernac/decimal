#include "../include/arithmetic.h"

#include <stdio.h>

int check_helper_bit(s21_decimal num) {
  int bit_mask = 0b01111111000000001111111111111111;
  int scale = get_scale(num);
  return (bit_mask & num.bits[3]) || scale >= 29;
}

int big_check_helper_bit(s21_big_decimal num) {
  int bit_mask = 0b01111111000000001111111111111111;
  int scale = big_get_scale(num);
  return (bit_mask & num.bits[7]) || scale >= 29;
}

void mul_10(s21_big_decimal *value, s21_big_decimal *res, int scale, int diff) {
  s21_big_decimal clone = {0};
  for (int i = 0; i < diff; i++) {
    clone = clone_big(*value);
    shift_left(value, 3);
    shift_left(&clone, 1);
    bitwise_addition(*value, clone, value);
    null_decimal(&clone);
  }
  big_set_scale(res, scale);
}

void post_normalization(s21_big_decimal *big_result, int *ret_status,
                        int mul_flag) {
  int res_tmp = check_res(*big_result);
  if (res_tmp) {
    int sign = big_get_sign(*big_result);
    big_set_sign(big_result, 0);
    char str_decimal[128] = {0}, str_scale[128] = {0};
    s21_big_decimal_to_string(*big_result, str_decimal);
    if (strchr(str_decimal, ',')) {
      int len_touch = strlen(strchr(str_decimal, ',')) - 1,
          len = strlen(str_decimal) - 1;
      int lun = 29 - (len - len_touch), flag = 0,
          touch = len - len_touch + 1 + lun, i = len - len_touch + 1;
      if (lun == 0) {
        touch = len + (len_touch - lun - 1);
        i = len - len_touch - 1;
      }
      for (int j = 0; i <= len; i++, j++) {
        if (i == touch && !flag) {
          i--;
          str_scale[j] = ',';
          flag = 1;
        } else if (str_decimal[i] != ',')
          str_scale[j] = str_decimal[i];
        else
          j--;
      }
      s21_decimal f_part = {0};
      s21_big_decimal big_f_part = {0}, tmp = {0};
      int num = strcmp(str_scale, "79228162514264337593543950335");
      if ((num > 0 && lun == 0 && strlen(str_scale) >= 28) ||
          (mul_flag && strlen(str_scale) >= 28)) {
        str_scale[28] = '\0';
        len_touch--;
      }
      string_to_decimal(str_scale, &f_part);
      big_f_part = decimal_to_big(f_part);
      if (lun == 0) big_set_scale(&big_f_part, len_touch);
      s21_big_round(big_f_part, &big_f_part);
      big_set_scale(&big_f_part, lun);
      if (lun == 0) {
        big_set_scale(big_result, big_get_scale(*big_result) + 1);
        big_truncate(big_result);
        mul_10(big_result, &tmp, 0, 1);
      } else
        big_truncate(big_result);
      normalize(big_result, &big_f_part, &tmp);
      bitwise_addition(*big_result, big_f_part, big_result);
      big_set_scale(big_result, lun);
    }
    big_set_sign(big_result, sign);
  }
  if (res_tmp == 3 && (check_res(*big_result) || is_zero(*big_result))) {
    *ret_status = 2;
  } else
    *ret_status = check_res(*big_result);
}

void bitwise_addition(s21_big_decimal value_1, s21_big_decimal value_2,
                      s21_big_decimal *result) {
  unsigned memory = 0;
  int size = (int)(sizeof(s21_big_decimal) / (sizeof(unsigned) - 1) * 32);

  for (int i = 0; i < size; i++) {
    int result_bit = big_get_bit(value_1, i) + big_get_bit(value_2, i) + memory;
    memory = result_bit / 2;
    result_bit %= 2;
    big_set_bit(result, i, result_bit);
  }
}

int bitwise_substraction(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  unsigned memory = 0, ret = 0;
  int size = (int)(sizeof(s21_big_decimal) / (sizeof(unsigned) - 1) * 32);
  if (big_get_sign(value_1) && big_get_sign(value_2) &&
      s21_is_greater_or_eq_big(value_1, value_2, 0, 0))
    ret = 1;
  if (s21_is_greater_or_eq_big(value_2, value_1, 0, 0)) {
    s21_big_decimal tmp = value_1;
    value_1 = value_2;
    value_2 = tmp;
    if (!(big_get_sign(value_1) && big_get_sign(value_2))) ret = 1;
  }
  for (int i = 0; i < size; i++) {
    int result_bit = big_get_bit(value_1, i) - big_get_bit(value_2, i) - memory;
    // Если результат отрицательный, занимаем 1 из следующего бита
    if (result_bit < 0) {
      result_bit += 2;  // Добавляем 2, так как работаем с битами (0 или 1)
      memory = 1;       // Устанавливаем заем
    } else {
      memory = 0;  // Заем не нужен
    }
    big_set_bit(result, i, result_bit);
  }
  return ret;
}

int normalize(s21_big_decimal *num1, s21_big_decimal *num2,
              s21_big_decimal *res) {
  int scale1 = big_get_scale(*num1), scale2 = big_get_scale(*num2),
      diff = (scale1 > scale2) ? (scale1 - scale2) : (scale2 - scale1);
  if (scale1 > scale2)
    mul_10(num2, res, scale1, diff);
  else
    mul_10(num1, res, scale2, diff);
  return diff;
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_1 = decimal_to_big(value_1),
                  big_value_2 = decimal_to_big(value_2), big_result = {0};
  int ret_status = 0;
  if (!check_helper_bit(value_1) && !check_helper_bit(value_2) && result) {
    normalize(&big_value_1, &big_value_2, &big_result);
    if (get_sign(value_1) == get_sign(value_2)) {
      bitwise_addition(big_value_1, big_value_2, &big_result);
      if (get_sign(value_1) == 1) big_set_sign(&big_result, 1);
    } else {
      if (get_sign(value_1) == 0)
        big_set_sign(&big_result, bitwise_substraction(big_value_1, big_value_2,
                                                       &big_result));
      else
        big_set_sign(&big_result, !bitwise_substraction(
                                      big_value_1, big_value_2, &big_result));
    }
    if (is_zero(big_result)) null_decimal(&big_result);
    post_normalization(&big_result, &ret_status, 0);
    *result = big_to_decimal(big_result);
  } else
    ret_status = 1;
  return ret_status;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_1 = decimal_to_big(value_1),
                  big_value_2 = decimal_to_big(value_2), big_result = {0};
  int ret_status = 0;
  if (!check_helper_bit(value_1) && !check_helper_bit(value_2) && result) {
    normalize(&big_value_1, &big_value_2, &big_result);
    if (get_sign(value_1) == get_sign(value_2)) {
      big_set_sign(&big_result,
                   bitwise_substraction(big_value_1, big_value_2, &big_result));
    } else {
      bitwise_addition(big_value_1, big_value_2, &big_result);
      if (get_sign(value_1)) big_set_sign(&big_result, 1);
    }
    if (is_zero(big_result)) null_decimal(&big_result);
    post_normalization(&big_result, &ret_status, 0);
    *result = big_to_decimal(big_result);
  } else
    ret_status = 1;
  return ret_status;
}

int get_biggest_bit(s21_big_decimal num) {
  int res = 0;
  for (int i = 3; i >= 1 && !res; i--) {
    for (int j = 32; j >= 1 && !res; j--) {
      if ((num.bits[i - 1] >> (j - 1)) & 1) res = i * j + ((32 - j) * (i - 1));
    }
  }
  return res;
}

void big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
             s21_big_decimal *big_result) {
  for (int i = get_biggest_bit(big_value_2); i >= 0; i--) {
    if (big_get_bit(big_value_2, i) == 1) {
      bitwise_addition(big_value_1, *big_result, big_result);
    }
    if (i != 0) shift_left(big_result, 1);
  }
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_1 = decimal_to_big(value_1),
                  big_value_2 = decimal_to_big(value_2), big_result = {0};
  int ret_status = 0;
  if (!check_helper_bit(value_1) && !check_helper_bit(value_2) && result) {
    if (is_zero_s(value_1) || is_zero_s(value_2))
      null_decimal_s(result);
    else {
      big_mul(big_value_1, big_value_2, &big_result);
      big_set_sign(&big_result, get_sign(value_1) != get_sign(value_2));
      big_set_scale(&big_result, get_scale(value_1) + get_scale(value_2));
      if (is_zero(big_result)) null_decimal(&big_result);
      post_normalization(&big_result, &ret_status, 1);
      *result = big_to_decimal(big_result);
      set_sign(result, get_sign(value_1) != get_sign(value_2));
      set_scale(result, big_get_scale(big_result));
    }
  } else
    ret_status = 1;
  return ret_status;
}

int s21_is_greater_or_eq_big(s21_big_decimal tmp1, s21_big_decimal tmp2,
                             int must_eq, int must_normalize) {
  int result = 0;
  int flag = 0;
  if (must_normalize) {
    s21_big_decimal trash = {0};
    normalize(&tmp1, &tmp2, &trash);
  }

  for (int i = 6; i >= 0 && !flag; i--) {
    if (tmp1.bits[i] != tmp2.bits[i]) {
      if (tmp1.bits[i] > tmp2.bits[i])
        result = 1;
      else
        result = 0;
      flag = 1;
    }
  }
  if (!result && must_eq) {
    result = 1;
    for (int i = 0; i < 7 && result; i++) {
      result = tmp1.bits[i] == tmp2.bits[i];
    }
  }
  return result;
}

void bitwise_sub_for_div(s21_big_decimal num1, s21_big_decimal num2,
                         s21_big_decimal *res_tmp, s21_big_decimal *res,
                         int shift) {
  for (int i = 0; i < shift; i++) shift_left(&num2, 1);
  if (s21_is_greater_or_eq_big(num1, num2, 1, 0)) {
    bitwise_substraction(num1, num2, res_tmp);
    big_set_bit(res, 0, 1);
  } else
    big_set_bit(res, 0, 0);
}

int normalize_div(s21_big_decimal *num1, s21_big_decimal *num2) {
  s21_big_decimal clone = {0};
  int scale_diff = 0;
  for (; s21_is_greater_or_eq_big(*num2, *num1, 0, 0); scale_diff++) {
    clone = clone_big(*num1);
    shift_left(num1, 3);
    shift_left(&clone, 1);
    bitwise_addition(*num1, clone, num1);
    null_decimal(&clone);
  }
  return scale_diff;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_2 = decimal_to_big(value_2), big_result = {0},
                  big_result_tmp = decimal_to_big(value_1), div_result = {0},
                  dec_10 = {0};
  dec_10.bits[0] = 10;
  s21_big_decimal big_value = {0};
  int num = 1, digit_part_len = 0, scale_num = 0, ret_status = 0;
  if ((get_scale(value_2) - get_scale(value_1)) > 1) scale_num = 1;
  if (check_helper_bit(value_1) || check_helper_bit(value_2))
    ret_status = 1;
  else if (!is_zero_s(value_2) && result) {
    int biggest_bit_pos_v2 = get_biggest_bit(big_value_2), scale_diff = 0;
    for (int i = 0;
         !is_zero(big_result_tmp) && i < 28 + scale_num - digit_part_len;
         i++, num++, null_decimal(&div_result)) {
      if (s21_is_greater_or_eq_big(big_value_2, big_result_tmp, 0, 0) &&
          !is_zero_s(value_1))
        scale_diff += normalize_div(&big_result_tmp, &big_value_2);
      for (int j = get_biggest_bit(big_result_tmp);
           j > 0 && (j - biggest_bit_pos_v2) >= 0; j--) {
        shift_left(&div_result, 1);
        bitwise_sub_for_div(big_result_tmp, big_value_2, &big_result_tmp,
                            &div_result, j - biggest_bit_pos_v2);
      }
      if (i == 0) {
        char digit_part[32] = {0};
        s21_big_decimal_to_string(div_result, digit_part);
        digit_part_len = strlen(digit_part) - 1;
      }
      bitwise_addition(div_result, big_value, &big_result);
      null_decimal(&big_value);
      big_mul(big_result, dec_10, &big_value);
    }
    int res_scale = get_scale(value_1) - get_scale(value_2) + scale_diff;
    if (res_scale > 0) {
      big_set_scale(&big_result, res_scale);
    } else {
      res_scale *= -1;
      for (int i = 0; i < res_scale; i++) {
        s21_big_decimal clone = clone_big(big_result);
        shift_left(&big_result, 3);
        shift_left(&clone, 1);
        bitwise_addition(big_result, clone, &big_result);
        null_decimal(&clone);
      }
    }
    if (is_zero(big_result)) null_decimal(&big_result);
    post_normalization(&big_result, &ret_status, 1);
    *result = big_to_decimal(big_result);
    set_sign(result, get_sign(value_1) != get_sign(value_2));
  } else
    ret_status = 3;
  return ret_status;
}