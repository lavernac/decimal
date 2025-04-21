#include "test_decimal.h"

START_TEST(greater_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_is_greater(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(greater_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_greater(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(greater_3) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_greater(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(greater_4) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num2, 1);
  num2.bits[0] = 12346;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_greater(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(greater_5) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12346;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_greater(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(greater_6) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  num1.bits[1] = 12345;
  num1.bits[2] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  num2.bits[1] = 12345;
  num2.bits[2] = 54321;
  set_scale(&num2, 3);

  int res = s21_is_greater(num1, num2);
  ck_assert_int_eq(res, 0);
}
END_TEST

START_TEST(eq_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_is_equal(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(eq_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(eq_3) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_scale(&num1, 1);

  int status = s21_is_equal(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(neq_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_is_not_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(neq_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_not_equal(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_3) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_4) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num2, 1);
  num2.bits[0] = 12346;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_5) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12346;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_6) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12346;
  set_scale(&num2, 3);

  int status = s21_is_less(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(less_eq_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_less_or_equal(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

START_TEST(less_eq_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num1, 1);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_less_or_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(less_eq_3) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12346;
  set_scale(&num2, 3);

  int status = s21_is_less_or_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(gr_eq_1) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_greater_or_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(gr_eq_2) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num2, 1);
  num2.bits[0] = 12345;
  set_scale(&num2, 3);
  set_sign(&num2, 1);

  int status = s21_is_greater_or_equal(num1, num2);

  ck_assert_int_eq(status, 1);
}
END_TEST

START_TEST(gr_eq_3) {
  s21_decimal num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12346;
  set_scale(&num2, 3);

  int status = s21_is_greater_or_equal(num1, num2);

  ck_assert_int_eq(status, 0);
}
END_TEST

Suite *test_cmp(void) {
  Suite *s1 = suite_create("\033[45m S21_COMPARISON \033[0m");
  TCase *tc1_1 = tcase_create("comparison_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, greater_1);
  tcase_add_test(tc1_1, greater_2);
  tcase_add_test(tc1_1, greater_3);
  tcase_add_test(tc1_1, greater_4);
  tcase_add_test(tc1_1, greater_5);
  tcase_add_test(tc1_1, greater_6);

  tcase_add_test(tc1_1, eq_1);
  tcase_add_test(tc1_1, eq_2);
  tcase_add_test(tc1_1, eq_3);

  tcase_add_test(tc1_1, neq_1);
  tcase_add_test(tc1_1, neq_2);

  tcase_add_test(tc1_1, less_1);
  tcase_add_test(tc1_1, less_2);
  tcase_add_test(tc1_1, less_3);
  tcase_add_test(tc1_1, less_4);
  tcase_add_test(tc1_1, less_5);
  tcase_add_test(tc1_1, less_6);

  tcase_add_test(tc1_1, less_eq_1);
  tcase_add_test(tc1_1, less_eq_2);
  tcase_add_test(tc1_1, less_eq_3);

  tcase_add_test(tc1_1, gr_eq_1);
  tcase_add_test(tc1_1, gr_eq_2);
  tcase_add_test(tc1_1, gr_eq_3);

  suite_add_tcase(s1, tc1_1);
  return s1;
}
