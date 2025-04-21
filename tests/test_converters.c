#include "test_decimal.h"

START_TEST(i_dec) {
  s21_decimal result = {0};
  int src = 12345;
  int status = s21_from_int_to_decimal(src, &result);
  int status_expected = 0;

  ck_assert_int_eq(status, status_expected);
  ck_assert_int_eq(result.bits[0], src);
}
END_TEST

START_TEST(i_dec1) {
  s21_decimal result = {0};
  int src = 0;
  int status = s21_from_int_to_decimal(src, &result);
  int status_expected = 0;

  ck_assert_int_eq(status, status_expected);
  ck_assert_int_eq(src, result.bits[0]);
}
END_TEST

START_TEST(f_dec) {
  s21_decimal result = {0};
  float src = 123.4567;
  int status = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1234567);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(f_dec1) {
  s21_decimal result = {0};
  float src = -123.00f;
  int status = s21_from_float_to_decimal(src, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 123);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(dec_i) {
  s21_decimal src = {0};
  int dst;
  src.bits[0] = 12345;
  set_scale(&src, 4);
  set_sign(&src, 1);

  int status = s21_from_decimal_to_int(src, &dst);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(dst, -1);
}
END_TEST

START_TEST(dec_i1) {
  s21_decimal src = {0};
  int dst;
  src.bits[0] = 123456789;
  set_scale(&src, 0);
  set_sign(&src, 0);

  int status = s21_from_decimal_to_int(src, &dst);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(dst, 123456789);
}
END_TEST

START_TEST(dec_f) {
  s21_decimal src = {0};
  float dst;
  src.bits[0] = 1234567;
  set_scale(&src, 5);
  set_sign(&src, 1);

  int status = s21_from_decimal_to_float(src, &dst);

  ck_assert_int_eq(status, 0);
  ck_assert_float_eq_tol(dst, -12.34567, 1e-6);
}
END_TEST

START_TEST(dec_f1) {
  s21_decimal src = {0};
  float dst;
  src.bits[0] = 1234567;
  set_scale(&src, 5);
  set_sign(&src, 0);

  int status = s21_from_decimal_to_float(src, &dst);

  ck_assert_int_eq(status, 0);
  ck_assert_float_eq_tol(dst, 12.34567, 1e-6);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal) {
  int a = 123456789;
  s21_decimal result = {{0, 0, 0, 0}};

  int res = s21_from_int_to_decimal(a, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123456789);
}
END_TEST

START_TEST(test_s21_from_float_to_decimal) {
  float a = 123.456f;
  s21_decimal result = {{0, 0, 0, 0}};

  int res = s21_from_float_to_decimal(a, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 123456);
  ck_assert_int_eq(result.bits[3] >> 16, 3);
}
END_TEST

START_TEST(test_s21_from_decimal_to_int) {
  s21_decimal a = {{123456789, 0, 0, 0}};
  int result = 0;

  int res = s21_from_decimal_to_int(a, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result, 123456789);
}
END_TEST

START_TEST(test_s21_from_decimal_to_float) {
  s21_decimal a = {{123456789, 0, 0, 0}};
  float result = 0.0f;

  int res = s21_from_decimal_to_float(a, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_float_eq(result, 123456789.0f);
}
END_TEST

START_TEST(from_float_to_decimal_regular) {
  s21_decimal dec = {0};
  int code;
  float flt = 10.0;
  for (int power = 0; power < 29; power++) {
    code = s21_from_float_to_decimal(pow(flt, power), &dec);

    ck_assert_int_eq(code, 0);
  }
  // free(dec);
}

START_TEST(s21_from_decimal_to_float_1) {
  s21_decimal var = {0};
  float value = 0;
  s21_from_float_to_decimal(5.0f, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(5.0f, value, 10e-5);
}

START_TEST(s21_from_decimal_to_float_2) {
  s21_decimal var = {0};
  for (int i = 0; i < 30; i++) {
    float n = get_rand_float(-8388608, 8388608);
    null_decimal_s(&var);
    float value = 0;
    s21_from_float_to_decimal(n, &var);
    s21_from_decimal_to_float(var, &value);
    ck_assert_float_eq_tol(n, value, 10);
  }
}

START_TEST(s21_from_decimal_to_float_3) {
  s21_decimal var = {0};
  for (int i = 0; i < 30; i++) {
    float n = get_rand_float(-1e-6 / 3, 1e-6 / 3);
    null_decimal_s(&var);
    float value = 0;
    s21_from_float_to_decimal(n, &var);
    s21_from_decimal_to_float(var, &value);
    ck_assert_float_eq_tol(n, value, 10);
  }
}

START_TEST(s21_from_decimal_to_float_4) {
  s21_decimal var = {0};
  int c = s21_from_decimal_to_float(var, NULL);
  ck_assert_int_eq(c, 1);
}

START_TEST(s21_from_decimal_to_float_5) {
  s21_decimal var = {{0, 0, 0, 0}};
  s21_negate(var, &var);
  float n = -0.0;
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}

START_TEST(s21_from_decimal_to_float_6) {
  s21_decimal a = {{0xffffffff, 0xffffffff, 0xffffffff, 0x80000000}};
  uint32_t u_fl = 0xEF800000;
  float toFloat = 0.0;
  float expectedFloat = 0.0;
  memcpy(&expectedFloat, &u_fl, sizeof(uint32_t));
  int s21_ret = s21_from_decimal_to_float(a, &toFloat);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_float_eq(expectedFloat, toFloat);
}

START_TEST(s21_from_decimal_to_float_7) {
  s21_decimal a = {{0x270ae101, 0x070acf57, 0xc4a2bb63, 0x80190000}};
  uint32_t u_fl = 0xC5BE2CA2;
  float toFloat = 0.0;
  float expectedFloat = 0.0;
  memcpy(&expectedFloat, &u_fl, sizeof(uint32_t));
  int s21_ret = s21_from_decimal_to_float(a, &toFloat);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_float_eq(expectedFloat, toFloat);
}

START_TEST(s21_from_decimal_to_float_8) {
  s21_decimal a = {{0x29b6f8a3, 0xff6dd0e9, 0x0effcd58, 0x80190000}};
  uint32_t u_fl = 0xC3E81A0F;
  float toFloat = 0.0;
  float expectedFloat = 0.0;
  memcpy(&expectedFloat, &u_fl, sizeof(uint32_t));
  int s21_ret = s21_from_decimal_to_float(a, &toFloat);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_float_eq(expectedFloat, toFloat);
}

START_TEST(s21_from_decimal_to_float_9) {
  s21_decimal a = {{0x00000000, 0x00000000, 0x00000000, 0x00000000}};
  uint32_t u_fl = 0x00000000;
  float toFloat = 0.0;
  float expectedFloat = 0.0;
  memcpy(&expectedFloat, &u_fl, sizeof(uint32_t));
  int s21_ret = s21_from_decimal_to_float(a, &toFloat);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_float_eq(expectedFloat, toFloat);
}

START_TEST(s21_from_float_to_decimal_0) {
  s21_decimal res = {0};
  float fromFloat = -7648904.0;
  int s21_ret = s21_from_float_to_decimal(fromFloat, &res);
  s21_decimal expected = {{7648904, 0, 0, 0x80000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_float_to_decimal_1) {
  s21_decimal res = {0};
  float fromFloat = 1234567.0;
  int s21_ret = s21_from_float_to_decimal(fromFloat, &res);
  s21_decimal expected = {{1234567, 0, 0, 0x00000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_float_to_decimal_2) {
  s21_decimal res = {0};
  float fromFloat = -1234567.0;
  int s21_ret = s21_from_float_to_decimal(fromFloat, &res);
  s21_decimal expected = {{1234567, 0x00000000, 0x00000000, 0x80000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_float_to_decimal_3) {
  s21_decimal res = {0};
  float fromFloat = 1.568993E-19;
  int s21_ret = s21_from_float_to_decimal(fromFloat, &res);
  s21_decimal expected = {{0x0017f0e1, 0x00000000, 0x00000000, 0x00190000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_float_to_decimal_4) {
  s21_decimal value_2 = {0};
  s21_decimal test = {0};
  float a = 0.8;
  float b = 0.0;
  set_scale(&test, 1);
  int res = s21_from_float_to_decimal(a, &value_2);
  s21_from_decimal_to_float(value_2, &b);
  ck_assert_uint_eq(res, 0);
  ck_assert_uint_eq(get_scale(value_2), get_scale(test));
  ck_assert_float_eq(a, b);
}

// 10.0 / 11.0 = 0.909090909090909090909090...
START_TEST(s21_from_float_to_decimal_5) {
  s21_decimal value_2 = {0};
  s21_decimal test = {0};
  float number = 10.0 / 11.0, b = 0;
  set_scale(&test, 7);
  int res = s21_from_float_to_decimal(number, &value_2);
  s21_from_decimal_to_float(value_2, &b);
  ck_assert_uint_eq(res, 0);
  ck_assert_uint_eq(get_scale(value_2), get_scale(test));
  ck_assert_float_eq_tol(number, b, 6);
}

START_TEST(from_int_to_decimal_pozitive) {
  s21_decimal dec = {0};
  int code, chislo;
  for (int i = 0; i < 30; i++) {
    chislo = rand();
    code = s21_from_int_to_decimal(chislo, &dec);
    ck_assert_int_eq(code, 0);
    ck_assert_int_eq(chislo, dec.bits[0]);
  }
}

START_TEST(from_int_to_decimal_negative) {
  s21_decimal dec = {0};
  int code, chislo;
  for (int i = 0; i < 30; i++) {
    chislo = rand() * (-1);
    code = s21_from_int_to_decimal(chislo, &dec);
    ck_assert_int_eq(code, 0);
    ck_assert_int_eq(chislo * (-1), dec.bits[0]);
    ck_assert_int_eq(1, get_sign(dec));
  }
}

START_TEST(from_int_to_decimal_error) {
  s21_decimal *dec = NULL;
  int code, chislo;
  chislo = 1;
  code = s21_from_int_to_decimal(chislo, dec);
  ck_assert_int_eq(code, 1);
}

START_TEST(from_decimal_to_int_int) {
  s21_decimal dec = {0};
  int code, chislo, new;
  for (int i = 0; i < 30; i++) {
    chislo = rand();
    null_decimal_s(&dec);
    s21_from_int_to_decimal(chislo, &dec);
    code = s21_from_decimal_to_int(dec, &new);
    ck_assert_int_eq(code, 0);
    ck_assert_int_eq(chislo, new);
  }
}

START_TEST(from_decimal_to_int_float) {
  s21_decimal dec = {0};
  int code, new;
  float chislo;
  for (int i = 0; i < 30; i++) {
    chislo = get_rand_float(-10000, 100000);
    null_decimal_s(&dec);
    s21_from_float_to_decimal(chislo, &dec);
    code = s21_from_decimal_to_int(dec, &new);
    ck_assert_int_eq(code, 0);
    ck_assert_int_eq((int)chislo, new);
  }
}

START_TEST(from_decimal_to_int_error) {
  s21_decimal dec = {0};
  int code = 0, new = 0;
  float chislo = 0;
  chislo = get_rand_float(1.0e+15F, 1.0e+20F);
  set_sign(&dec, 1);
  code = s21_from_float_to_decimal(chislo, &dec);
  code = s21_from_decimal_to_int(dec, &new);

  ck_assert_int_eq(code, 1);
}

START_TEST(from_decimal_to_int_error_NULL) {
  s21_decimal *dec = NULL;
  int code;
  float chislo;
  chislo = 13.0f;
  code = s21_from_float_to_decimal(chislo, dec);
  ck_assert_int_eq(code, 1);
}

START_TEST(s21_from_decimal_to_int_0) {
  s21_decimal a = {{0xba1eccc0, 0xc20df37b, 0x3ec10d97, 0x80130000}};
  int toInt = 0;
  int s21_ret = s21_from_decimal_to_int(a, &toInt);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_int_eq(toInt, -1942145748);
}

START_TEST(s21_from_decimal_to_int_1) {
  s21_decimal a = {{0xad95c559, 0xb3d99c8e, 0x24372227, 0x00130000}};
  int toInt = 0;
  int s21_ret = s21_from_decimal_to_int(a, &toInt);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_int_eq(toInt, 1120811256);
}

START_TEST(s21_from_decimal_to_int_2) {
  s21_decimal a = {{0x65ea2349, 0x429f839f, 0x2d9d5d09, 0x00130000}};
  int toInt = 0;
  int s21_ret = s21_from_decimal_to_int(a, &toInt);
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_int_eq(toInt, 1411706614);
}

START_TEST(s21_from_decimal_to_int_3) {
  s21_decimal a = {{0x00000000, 0x00000001, 0x00000001, 0x00000000}};
  int toInt = 0;
  int s21_ret = s21_from_decimal_to_int(a, &toInt);
  int expected_ret = 1;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_int_eq(toInt, 0);
}

START_TEST(s21_from_int_to_decimal_0) {
  s21_decimal res;
  int fromInt = 2147483647;
  int s21_ret = s21_from_int_to_decimal(fromInt, &res);
  s21_decimal expected = {{0x7fffffff, 0x00000000, 0x00000000, 0x00000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_int_to_decimal_1) {
  s21_decimal res;
  int fromInt = -489;
  int s21_ret = s21_from_int_to_decimal(fromInt, &res);
  s21_decimal expected = {{0x000001e9, 0x00000000, 0x00000000, 0x80000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

START_TEST(s21_from_int_to_decimal_2) {
  s21_decimal res;
  int fromInt = -419;
  int s21_ret = s21_from_int_to_decimal(fromInt, &res);
  s21_decimal expected = {{0x000001a3, 0x00000000, 0x00000000, 0x80000000}};
  int expected_ret = 0;
  ck_assert_int_eq(s21_ret, expected_ret);
  ck_assert_mem_eq(&expected, &res, 4 * sizeof(uint32_t));
}

Suite *test_converters(void) {
  Suite *s1 = suite_create("\033[45m S21_CONVERTERS \033[0m");
  TCase *tc1_1 = tcase_create("converters_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, i_dec);
  tcase_add_test(tc1_1, i_dec1);
  tcase_add_test(tc1_1, f_dec);
  tcase_add_test(tc1_1, f_dec1);
  tcase_add_test(tc1_1, dec_i);
  tcase_add_test(tc1_1, dec_i1);
  tcase_add_test(tc1_1, dec_f);
  tcase_add_test(tc1_1, dec_f1);

  tcase_add_test(tc1_1, test_s21_from_int_to_decimal);
  tcase_add_test(tc1_1, test_s21_from_float_to_decimal);
  tcase_add_test(tc1_1, test_s21_from_decimal_to_int);
  tcase_add_test(tc1_1, test_s21_from_decimal_to_float);

  tcase_add_test(tc1_1, from_float_to_decimal_regular);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_1);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_2);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_3);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_4);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_5);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_6);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_7);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_8);
  tcase_add_test(tc1_1, s21_from_decimal_to_float_9);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_0);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_1);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_2);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_3);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_4);
  tcase_add_test(tc1_1, s21_from_float_to_decimal_5);

  tcase_add_test(tc1_1, from_int_to_decimal_pozitive);
  tcase_add_test(tc1_1, from_int_to_decimal_negative);
  tcase_add_test(tc1_1, from_int_to_decimal_error);
  tcase_add_test(tc1_1, from_decimal_to_int_int);
  tcase_add_test(tc1_1, from_decimal_to_int_float);
  tcase_add_test(tc1_1,
                 from_decimal_to_int_error);  // Я НЕ ПОНИМАЮ КАК ПОЧЧНИТЬ
  tcase_add_test(tc1_1, from_decimal_to_int_error_NULL);
  tcase_add_test(tc1_1, s21_from_decimal_to_int_0);
  tcase_add_test(tc1_1, s21_from_decimal_to_int_1);
  tcase_add_test(tc1_1, s21_from_decimal_to_int_2);
  tcase_add_test(tc1_1, s21_from_decimal_to_int_3);
  tcase_add_test(tc1_1, s21_from_int_to_decimal_0);
  tcase_add_test(tc1_1, s21_from_int_to_decimal_1);
  tcase_add_test(tc1_1, s21_from_int_to_decimal_2);

  // suite_add_tcase(s1, tc1_1);
  return s1;
}
