#include "test_decimal.h"

START_TEST(negate) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num1, 1);
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num1, 1);

  s21_negate(num1, &num2);

  ck_assert_int_ne(get_sign(num1), get_sign(num2));
}
END_TEST

START_TEST(truncate) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 123456;
  set_scale(&num1, 3);
  set_sign(&num1, 1);

  s21_truncate(num1, &num2);

  ck_assert_int_eq(num2.bits[0], 123);
  ck_assert_int_eq(get_scale(num2), 0);
  ck_assert_int_eq(get_sign(num2), 1);
}
END_TEST

START_TEST(truncate_big) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[1] = 1;
  set_scale(&num1, 3);
  set_sign(&num1, 1);

  s21_truncate(num1, &num2);

  ck_assert_int_eq(num2.bits[0], 0b00000000010000011000100100110111);
  ck_assert_int_eq(num2.bits[1], 0);
  ck_assert_int_eq(num2.bits[2], 0);
  ck_assert_int_eq(get_scale(num2), 0);
  ck_assert_int_eq(get_sign(num2), 1);
}
END_TEST

START_TEST(truncate1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 123456;
  set_scale(&num1, 3);
  set_sign(&num1, 0);

  s21_truncate(num1, &num2);

  ck_assert_int_eq(num2.bits[0], 123);
  ck_assert_int_eq(get_scale(num2), 0);
  ck_assert_int_eq(get_sign(num2), 0);
}
END_TEST

START_TEST(truncate2) {
  s21_decimal num1 = {0}, num2 = {0};
  char orig[32] = "-545454512454545,35265454545645",
       exp[32] = "-545454512454545", res[32] = {0};
  string_to_decimal(orig, &num1);

  s21_truncate(num1, &num2);

  s21_big_decimal u = decimal_to_big(num2);
  s21_big_decimal_to_string(u, res);

  ck_assert_str_eq(exp, res);
}
END_TEST

START_TEST(round_) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 123356789;
  set_scale(&num1, 6);
  num2.bits[0] = 123756;
  set_scale(&num2, 3);

  s21_round(num1, &num1);
  s21_round(num2, &num2);

  ck_assert_int_eq(num1.bits[0], 123);
  ck_assert_int_eq(num2.bits[0], 124);
}
END_TEST

START_TEST(floor_) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 123789;
  set_scale(&num1, 3);
  num2.bits[0] = 123756;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  s21_floor(num1, &num1);
  s21_floor(num2, &num2);

  ck_assert_int_eq(num1.bits[0], 123);
  ck_assert_int_eq(get_sign(num1), 0);
  ck_assert_int_eq(num2.bits[0], 124);
  ck_assert_int_eq(get_sign(num2), 1);
}
END_TEST

Suite *test_other(void) {
  Suite *s1 = suite_create("\033[45m S21_OTHER \033[0m");
  TCase *tc1_1 = tcase_create("other_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, negate);
  tcase_add_test(tc1_1, truncate);
  tcase_add_test(tc1_1, truncate1);
  tcase_add_test(tc1_1, truncate_big);
  tcase_add_test(tc1_1, truncate2);
  tcase_add_test(tc1_1, round_);
  tcase_add_test(tc1_1, floor_);
  suite_add_tcase(s1, tc1_1);
  return s1;
}
