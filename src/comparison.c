#include "../include/comparison.h"

#include "../include/arithmetic.h"
#include "../include/helpers.h"

int s21_is_less(s21_decimal a, s21_decimal b) { return s21_is_greater(b, a); }

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return (s21_is_less(a, b) || s21_is_equal(a, b));
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  int result = 0, flag = 0;
  s21_big_decimal tmp1 = decimal_to_big(a), tmp2 = decimal_to_big(b), res = {0};
  if (!check_helper_bit(a) && !check_helper_bit(b)) {
    normalize(&tmp1, &tmp2, &res);
    int sign_a = big_get_sign(tmp1), sign_b = big_get_sign(tmp2);
    if (sign_a < sign_b && (!is_zero_s(a) || !is_zero_s(b))) {
      result = 1;
    } else if (sign_a == sign_b) {
      for (int i = 6; i >= 0 && !flag; i--) {
        if (tmp1.bits[i] > tmp2.bits[i]) {
          result = 1;
          flag = 1;
        } else if (tmp1.bits[i] < tmp2.bits[i])
          flag = 1;
      }
    }
    if (sign_a && sign_b && !s21_is_equal(a, b)) result = !result;
  } else
    result = 0;
  return result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return (s21_is_greater(a, b) || s21_is_equal(a, b));
}

int s21_is_equal(s21_decimal a, s21_decimal b) {
  int result = 1;
  if (!check_helper_bit(a) && !check_helper_bit(b) &&
      get_sign(a) == get_sign(b)) {
    s21_big_decimal big_a = decimal_to_big(a), big_b = decimal_to_big(b),
                    tmp = {0};
    normalize(&big_a, &big_b, &tmp);
    for (int i = 0; i < 6 && result; i++) {
      if (big_a.bits[i] != big_b.bits[i]) {
        result = 0;
      }
    }
  } else
    result = 0;
  if (is_zero_s(a) && is_zero_s(b)) result = 1;
  return result;
}

int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}