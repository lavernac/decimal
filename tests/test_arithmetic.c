#include "test_decimal.h"

#define MINUS 0x80000000

START_TEST(add) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 135795);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(add_diff) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);
  set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 111105);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(add_diff2) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 31;
  set_scale(&num1, 1);
  set_sign(&num1, 1);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 11);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(add_minus) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  set_sign(&num1, 1);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);
  set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 135795);
  ck_assert_int_eq(get_scale(result), 4);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(add_minus_minus) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  // set_scale(&num1, 3);
  set_sign(&num1, 1);
  num2.bits[0] = 5;
  // set_scale(&num2, 4);
  set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 6);
  ck_assert_int_eq(get_scale(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(add_minus_bigger) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 6;
  // set_scale(&num1, 3);
  set_sign(&num1, 1);
  num2.bits[0] = 5;
  // set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_scale(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(add_small) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  // set_scale(&num1, 3);
  // set_sign(&num1, 1);
  num2.bits[0] = 1;
  set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 10001);
  ck_assert_int_eq(get_scale(result), 4);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(add_minus_small) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  // set_scale(&num1, 3);
  set_sign(&num1, 1);
  num2.bits[0] = 5;
  // set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(get_scale(result), 0);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(add_plus_minus) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  // set_scale(&num1, 3);
  num2.bits[0] = 5;
  set_sign(&num2, 1);
  // set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(get_scale(result), 0);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(add_overflow) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  for (int i = 0; i < 3; i++) {
    num1.bits[i] = 0xffffffff;
  }
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_sign(&num2, 1);
  // set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_add(num1, num2, &result);

  ck_assert_int_eq(status, 1);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_scale(result), 0);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(sub) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 111105);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(sub_diff) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 12345;
  set_scale(&num1, 3);
  num2.bits[0] = 12345;
  set_scale(&num2, 4);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 135795);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(sub_lower) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 3;
  // set_scale(&num2, 4);
  // set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(sub_minus_lower) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 3;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(sub_minus_minus_eq) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 0);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(sub_minus_num2_lower) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 3;
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(sub_minus_add) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);
  // set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 4);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(sub_minus_add_lower) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  // set_scale(&num1, 3);
  num2.bits[0] = 3;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(sub_minus_minus_bigger) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(sub_minus_bigger) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);
  set_sign(&num1, 1);
  // set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(sub_minus_num2_bigger) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  set_sign(&num2, 1);

  int status = s21_sub(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 5);
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(mul_fffff) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 0xfffff;
  set_scale(&num1, 1);
  num2.bits[0] = 0xfffff;
  set_scale(&num2, 2);
  set_sign(&num2, 1);

  int status = s21_mul(num1, num2, &result);

  ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], 0xffe00001);
  ck_assert_int_eq(result.bits[1], 0xff);
  ck_assert_int_eq(get_scale(result), 3);
  ck_assert_int_eq(get_sign(result), 1);
}
END_TEST

START_TEST(test_add) {
  {
    int num1 = 0, num2 = 0, sum_int = 0, res_int = 0;
    s21_decimal a = {0}, b = {0}, res_dec = {0};

    num1 = 42;
    num2 = 30;
    sum_int = 72;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);
    ck_assert_int_eq(s21_add(a, b, &res_dec), 0);

    num1 = 1000;
    num2 = 2000;
    sum_int = 3000;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = 0;
    num2 = 0;
    sum_int = 0;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = -15;
    num2 = 5;
    sum_int = -10;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = 11115;
    num2 = 5;
    sum_int = 11120;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = -1;
    num2 = -10;
    sum_int = -11;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = -1234;
    num2 = 234;
    sum_int = -1000;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = -9403;
    num2 = 234;
    sum_int = -9169;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = 9403;
    num2 = 202;
    sum_int = 9605;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    num1 = 450;
    num2 = -50;
    sum_int = 400;
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, sum_int);

    float fnum1 = 7.25;
    float fnum2 = 9.5;
    float res_origin = 16.75;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);
    float res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    num1 = -1234;
    fnum2 = 1.234;
    res_origin = -1232.766;
    s21_from_int_to_decimal(num1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    num1 = -1234;
    fnum2 = -1.234;
    res_origin = -1235.234;
    s21_from_int_to_decimal(num1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    fnum1 = -94;
    fnum2 = 0.00234;
    res_origin = fnum1 + fnum2;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    fnum1 = -940.3;
    fnum2 = 0.000234;
    res_origin = -940.299766;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    fnum1 = -0.9403;
    fnum2 = 0.000234;
    res_origin = -0.940066;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    // fnum1 = -0.9403;
    // fnum2 = 2.0234;
    // res_origin = fnum1 + fnum2;  // 1,0831
    // s21_from_float_to_decimal(fnum1, &a);
    // s21_from_float_to_decimal(fnum2, &b);
    // printf("%d %d\n", a.bits[0], b.bits[0]);

    // res_float = 0.0;
    // s21_add(a, b, &res_dec);
    // printf("%d\n", res_dec.bits[0]);

    // s21_from_decimal_to_float(res_dec, &res_float);
    // ck_assert_float_eq_tol(res_float, res_origin, 1e-6);

    fnum1 = -0.9403;
    fnum2 = -112.0234;
    res_origin = -112.9637;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    fnum1 = -940.0;
    fnum2 = -112.0234;
    res_origin = -1052.0234;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    fnum1 = -0.94e03;
    fnum2 = -112.0e2;
    res_origin = -12140;
    s21_from_float_to_decimal(fnum1, &a);
    s21_from_float_to_decimal(fnum2, &b);

    res_float = 0.0;
    s21_add(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);

    int tmp1 = 100;
    int tmp2 = 99999;
    int res_s21 = 0;
    int res = 100099;
    s21_decimal res1;
    s21_from_int_to_decimal(tmp1, &a);
    s21_from_int_to_decimal(tmp2, &b);
    s21_add(a, b, &res1);
    s21_from_decimal_to_int(res1, &res_s21);
    ck_assert_int_eq(res_s21, res);

    tmp1 = -100;
    tmp2 = -99999;
    res_s21 = 0;
    res = -100099;

    s21_from_int_to_decimal(tmp1, &a);
    s21_from_int_to_decimal(tmp2, &b);
    s21_add(a, b, &res1);
    s21_from_decimal_to_int(res1, &res_s21);
    ck_assert_int_eq(res_s21, res);

    tmp1 = 2147483647;
    tmp2 = -2147483647;
    res_s21 = 0;
    res = 0;

    s21_from_int_to_decimal(tmp1, &a);
    s21_from_int_to_decimal(tmp2, &b);
    s21_add(a, b, &res1);
    s21_from_decimal_to_int(res1, &res_s21);
    ck_assert_int_eq(res_s21, res);

    a.bits[3] = 0;
    a.bits[2] = 0;
    a.bits[1] = 0xF;
    a.bits[0] = 0x67E4FEEF;
    b.bits[3] = 0;
    b.bits[2] = 0;
    b.bits[1] = 0xFF;
    b.bits[0] = 0x67E4FFFF;
    s21_decimal dec_res;
    dec_res.bits[3] = 0;
    dec_res.bits[2] = 0;
    dec_res.bits[1] = 0x10E;
    dec_res.bits[0] = 0xCFC9FEEE;

    s21_add(a, b, &res1);
    ck_assert_int_eq(dec_res.bits[3], res1.bits[3]);
    ck_assert_int_eq(dec_res.bits[2], res1.bits[2]);
    ck_assert_int_eq(dec_res.bits[1], res1.bits[1]);
    ck_assert_int_eq(dec_res.bits[0], res1.bits[0]);

    tmp1 = -2147483647;
    tmp2 = 2147483647;
    res_s21 = 0;
    res = 0;

    s21_from_int_to_decimal(tmp1, &a);
    s21_from_int_to_decimal(tmp2, &b);
    s21_add(a, b, &res1);
    s21_from_decimal_to_int(res1, &res_s21);
    ck_assert_int_eq(res_s21, res);

    a.bits[0] = UINT_MAX;
    a.bits[1] = UINT_MAX;
    a.bits[2] = UINT_MAX;
    a.bits[3] = 0;

    b.bits[0] = 1;
    b.bits[1] = 0;
    b.bits[2] = 0;
    b.bits[3] = 0;

    int ret_s21 = s21_add(a, b, &res1);
    ck_assert_int_eq(1, ret_s21);

    a.bits[0] = 0xffffffff;
    a.bits[1] = 0xffffffff;
    a.bits[2] = 0xffffffff;
    a.bits[3] = MINUS;

    b.bits[0] = 1;
    b.bits[1] = 0;
    b.bits[2] = 0;
    b.bits[3] = MINUS;

    ret_s21 = s21_add(a, b, &res1);
    ck_assert_int_eq(2, ret_s21);

    float float_a = -10758.218750;
    float float_b = 6268.843750;

    float float_res = float_a + float_b;

    s21_decimal expected = {0};
    s21_from_float_to_decimal(float_res, &expected);

    s21_decimal dec_a = {0};
    s21_from_float_to_decimal(float_a, &dec_a);
    s21_decimal dec_b = {0};
    s21_from_float_to_decimal(float_b, &dec_b);

    s21_decimal result = {0};
    int code = s21_add(dec_a, dec_b, &result);

    float got_float = 0;
    s21_from_decimal_to_float(result, &got_float);

    ck_assert_int_eq(code, 0);
    ck_assert_float_eq_tol(got_float, float_res, 1e06);

    a.bits[0] = UINT_MAX;
    a.bits[1] = UINT_MAX;
    a.bits[2] = UINT_MAX;
    a.bits[3] = 0;

    b.bits[0] = 0;
    b.bits[1] = 0;
    b.bits[2] = 0;
    b.bits[3] = 0;
    s21_from_float_to_decimal(0.01448, &b);

    s21_add(a, b, &res_dec);
    ck_assert_int_eq(s21_add(a, b, &res_dec), 0);

    a.bits[0] = UINT_MAX;
    a.bits[1] = UINT_MAX;
    a.bits[2] = UINT_MAX;
    a.bits[3] = ~(INT_MAX);

    b.bits[0] = 0;
    b.bits[1] = 0;
    b.bits[2] = 0;
    b.bits[3] = 0;
    s21_from_float_to_decimal(-0.01448, &b);

    s21_add(a, b, &res_dec);
    ck_assert_int_eq(s21_add(a, b, &res_dec), 0);
  }
  {
    s21_decimal val1 = {{15, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{2, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{15, 0, 0, 0}};
    s21_decimal val2 = {{15, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    s21_decimal val2 = {{1, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(1, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(2, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{8, 0, 0, 0}};
    s21_decimal val2 = {{2, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{2, 0, 0, 0}};
    s21_decimal val2 = {{8, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{2, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{8, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {0};
    s21_decimal val2 = {0};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{4, 0, 0, 0}};
    s21_decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {0};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{4, 0, 0, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(2, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    s21_decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{4, 0, 0, 0}};
    s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
    s21_decimal res = {0};
    ck_assert_int_eq(0, s21_add(val1, val2, &res));
  }
  {
    s21_decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
    s21_decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
    s21_decimal res = {0};
    ck_assert_int_eq(2, s21_add(val1, val2, &res));
  }
  {
    s21_decimal c = {{5, 0, 0, 0}};
    s21_decimal d = {{5, 0, 0, 0}};
    s21_decimal etalon = {{10, 0, 0, 0}};
    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{100, 0, 0, 0}};
    s21_decimal d = {{100, 0, 0, 0}};
    s21_decimal etalon = {{100, 0, 0, 0}};
    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;
    int add = s21_add(c, d, p_res);
    ;
    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(equal, 0);
  }
  {
    s21_decimal c = {{2147483647, 0, 0, 0}};
    s21_decimal d = {{2147483647, 0, 0, 0}};
    s21_decimal etalon = {{0xFFFFFFFE, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal* p_result = &result;

    int add = s21_add(c, d, p_result);
    ck_assert_int_eq(add, 0);
    ;
    int equal = s21_is_equal(result, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1000, 0, 0, 0}};
    s21_decimal d = {{1000, 0, 0, 0}};
    s21_decimal etalon = {{2000, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal* p_result = &result;

    int add = s21_add(c, d, p_result);
    ck_assert_int_eq(add, 0);
    ;
    int equal = s21_is_equal(result, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1000, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1000, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 0, 0}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal* p_result = &result;

    int add = s21_add(c, d, p_result);
    ck_assert_int_eq(add, 0);
    ;
    int equal = s21_is_equal(result, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal etalon = {{2, 0, 0, ~(INT_MAX)}};
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal* p_result = &result;

    int add = s21_add(c, d, p_result);
    ck_assert_int_eq(add, 0);
    ;
    int equal = s21_is_equal(*p_result, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0, 0, 0, 0}};
    s21_decimal d = {{0, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, 0}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{2, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1000, 0, 0, 0}};
    s21_decimal d = {{1000, 0, 0, 0}};
    s21_decimal etalon = {{2000, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{INT_MAX, 0, 0, 0}};
    s21_decimal d = {{INT_MAX, 0, 0, 0}};
    s21_decimal etalon = {{0xFFFFFFFE, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{INT_MAX, 0, 0, 0}};
    s21_decimal d = {{INT_MAX, 0, 0, 0}};
    s21_decimal etalon = {{UINT32_MAX - 1, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{UINT32_MAX, 0, 0, 0}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{0, 1, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[3], etalon.bits[3]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0, UINT_MAX - 1, 0, 0}};
    s21_decimal d = {{0, 1, 0, 0}};
    s21_decimal etalon = {{0, UINT_MAX, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0, 0, UINT_MAX - 1, 0}};
    s21_decimal d = {{0, 0, 1, 0}};
    s21_decimal etalon = {{0, 0, UINT_MAX, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }

  {
    s21_decimal c = {{0, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{0, 0, 0, ~(INT_MAX)}};
    s21_decimal etalon = {{0, 0, 0, 0}};
    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal etalon = {{2, 0, 0, ~(INT_MAX)}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[3], etalon.bits[3]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{11, 0, 0, 65536}};
    s21_decimal d = {{0, 0, 0, 0}};
    s21_decimal etalon = {{11, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{11, 0, 0, 65536}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{21, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{111, 0, 0, 65536}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{121, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{111, 0, 0, 196608}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{1111, 0, 0, 196608}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {0};
    c.bits[0] = 0b11111111111111111111111111111110;
    c.bits[1] = 0b00000000000000000000000000000000;
    c.bits[2] = 0b00000000000000000000000000000000;
    c.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {0};
    etalon.bits[0] = 0b11111111111111111111111111111111;
    etalon.bits[1] = 0b00000000000000000000000000000000;
    etalon.bits[2] = 0b00000000000000000000000000000000;
    etalon.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{UINT32_MAX - 1, UINT32_MAX, 0, 0}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{UINT32_MAX, UINT32_MAX, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0b00000000000000000001100101111001, 0, 0, 0}};
    s21_decimal d = {{0b00000000000000010010000110001001, 0, 0, 0}};
    s21_decimal etalon = {{0b00000000000000010011101100000010, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0b10001001111001111111111111111111,
                      0b10001010110001110010001100000100, 0, 0}};
    s21_decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
    s21_decimal etalon = {{0b10001001111010000000000000000000,
                           0b10001010110001110010001100000100, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{UINT_MAX, UINT_MAX, 0, 0}};
    s21_decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 1, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }

  {
    s21_decimal c = {{0b10000010111000100101101011101101,
                      0b11111001111010000010010110101101,
                      0b10110000001111101111000010010100,
                      0b10000000000011100000000000000000}};
    s21_decimal d = {{0b00000011010000001001011100101110, 0, 0, 0}};
    s21_decimal etalon = {{0b01110001001010101101101011101101,
                           0b00101110001111001110000111111000,
                           0b10110000001111101110111101101101,
                           0b10000000000011100000000000000000}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {0}, d = {0}, etalon = {0}, res = {0};
    string_to_decimal("-545454512454545,35265454545645", &c);
    string_to_decimal("-5,352654545456454545645464", &d);
    string_to_decimal("-545454512454550,70530909091290", &etalon);
    int add = s21_add(c, d, &res);
    // print_s(etalon);
    // print_s(res);
    // printf("%d %d\n", get_scale(res), get_scale(etalon));

    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal a, b, origin;
    a.bits[0] = 0b01001110111001000011100101110110;
    a.bits[1] = 0b01001011001101011010000111011001;
    a.bits[2] = 0b00011001101110010111010010111111;
    a.bits[3] = 0b00000000000011110000000000000000;
    b.bits[0] = 0b00000000000000000000000000000000;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000000000000000000000;
    origin.bits[0] = 0b01001110111001000011100101110110;
    origin.bits[1] = 0b01001011001101011010000111011001;
    origin.bits[2] = 0b00011001101110010111010010111111;
    origin.bits[3] = 0b00000000000011110000000000000000;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_decimal* p_res = &result;
    int add = s21_add(a, b, p_res);
    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(origin.bits[0], result.bits[0]);
    ck_assert_int_eq(origin.bits[1], result.bits[1]);
    ck_assert_int_eq(origin.bits[2], result.bits[2]);
    ck_assert_int_eq(origin.bits[3], result.bits[3]);

    int equal = s21_is_equal(result, origin);
    ck_assert_int_eq(equal, 1);
  }

  {
    s21_decimal c = {{0, 0, 0, 0}};
    s21_decimal d = {{0, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, 0}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{2, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1000, 0, 0, 0}};
    s21_decimal d = {{1000, 0, 0, 0}};
    s21_decimal etalon = {{2000, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;
    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);
    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{INT_MAX, 0, 0, 0}};
    s21_decimal d = {{INT_MAX, 0, 0, 0}};
    s21_decimal etalon = {{0xFFFFFFFE, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{INT_MAX, 0, 0, 0}};
    s21_decimal d = {{INT_MAX, 0, 0, 0}};
    s21_decimal etalon = {{UINT32_MAX - 1, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{UINT_MAX, 0, 0, 0}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{0, 1, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[3], etalon.bits[3]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0, UINT_MAX - 1, 0, 0}};
    s21_decimal d = {{0, 1, 0, 0}};
    s21_decimal etalon = {{0, UINT_MAX, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0, 0, UINT_MAX - 1, 0}};
    s21_decimal d = {{0, 0, 1, 0}};
    s21_decimal etalon = {{0, 0, UINT_MAX, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  // {
  //   s21_decimal c = {{0, 0, 0, ~(INT_MAX)}};
  //   s21_decimal d = {{0, 0, 0, ~(INT_MAX)}};
  //   s21_decimal etalon = {{0, 0, 0, ~(INT_MAX)}};

  //   s21_decimal res = {{0, 0, 0, 0}};
  //   s21_decimal* p_res = &res;

  //   int add = s21_add(c, d, p_res);
  //   ck_assert_int_eq(add, 0);
  //   int equal = s21_is_equal(res, etalon);
  //   ck_assert_int_eq(equal, 1);
  // }
  {
    s21_decimal c = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal d = {{1, 0, 0, ~(INT_MAX)}};
    s21_decimal etalon = {{2, 0, 0, ~(INT_MAX)}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;
    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{11, 0, 0, 65536}};
    s21_decimal d = {{0, 0, 0, 0}};
    s21_decimal etalon = {{11, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{11, 0, 0, 65536}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{21, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{111, 0, 0, 65536}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{121, 0, 0, 65536}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{111, 0, 0, 196608}};
    s21_decimal d = {{1, 0, 0, 0}};
    s21_decimal etalon = {{1111, 0, 0, 196608}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0b00000000000000000001100101111001, 0, 0, 0}};
    s21_decimal d = {{0b00000000000000010010000110001001, 0, 0, 0}};
    s21_decimal etalon = {{0b00000000000000010011101100000010, 0, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0b10001001111001111111111111111111,
                      0b10001010110001110010001100000100, 0, 0}};
    s21_decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
    s21_decimal etalon = {{0b10001001111010000000000000000000,
                           0b10001010110001110010001100000100, 0, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;
    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{UINT_MAX, UINT_MAX, 0, 0}};
    s21_decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
    s21_decimal etalon = {{0, 0, 1, 0}};

    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;

    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);

    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }
  {
    s21_decimal c = {{0b10000010111000100101101011101101,
                      0b11111001111010000010010110101101,
                      0b10110000001111101111000010010100,
                      0b10000000000011100000000000000000}};
    s21_decimal d = {{0b00000011010000001001011100101110, 0, 0, 0}};
    s21_decimal etalon = {{0b01110001001010101101101011101101,
                           0b00101110001111001110000111111000,
                           0b10110000001111101110111101101101,
                           0b10000000000011100000000000000000}};
    s21_decimal res = {{0, 0, 0, 0}};
    s21_decimal* p_res = &res;
    int add = s21_add(c, d, p_res);
    ck_assert_int_eq(add, 0);
    ck_assert_int_eq(res.bits[0], etalon.bits[0]);
    ck_assert_int_eq(res.bits[1], etalon.bits[1]);
    ck_assert_int_eq(res.bits[2], etalon.bits[2]);
    ck_assert_int_eq(res.bits[3], etalon.bits[3]);
    int equal = s21_is_equal(res, etalon);
    ck_assert_int_eq(equal, 1);
  }

  {
    s21_decimal a = {0};
    a.bits[0] = 0b11111111111111111111111111111111;
    a.bits[1] = 0b11111111111111111111111111111111;
    a.bits[2] = 0b11111111111111111111111111111111;
    a.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000000000000010;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000110000000000000000;
    s21_decimal s21_res = {0};
    ck_assert_int_eq(s21_add(a, b, &s21_res), 0);
  }
  {
    s21_decimal a = {0};
    a.bits[0] = 0b11111111111111111111111111111111;
    a.bits[1] = 0b11111111111111111111111111111111;
    a.bits[2] = 0b11111111111111111111111111111111;
    a.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000000000000101;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000010000000000000000;
    s21_decimal s21_res = {0};
    ck_assert_int_eq(s21_add(a, b, &s21_res), 1);
  }
  {
    s21_decimal a = {0};
    a.bits[0] = 0b11111111111111111111111111111111;
    a.bits[1] = 0b11111111111111111111111111111111;
    a.bits[2] = 0b11111111111111111111111111111111;
    a.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000000000000001;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal s21_res = {0};
    ck_assert_int_eq(s21_add(a, b, &s21_res), 1);
  }
  {
    s21_decimal src1 = {0};
    src1.bits[0] = 0xffffffff;
    src1.bits[1] = 0xffffffff;
    src1.bits[2] = 0xffffffff;
    src1.bits[3] = 0x80000000;
    s21_decimal src2 = {0};
    src2.bits[0] = 0x00000002;  // 0.002
    src2.bits[1] = 0x00000000;
    src2.bits[2] = 0x00000000;
    src2.bits[3] = 0x00030000;
    s21_decimal original_res = {0};
    original_res.bits[0] = 0xffffffff;
    original_res.bits[1] = 0xffffffff;
    original_res.bits[2] = 0xffffffff;
    original_res.bits[3] = 0x80000000;
    s21_decimal s21_res = {0};

    // s21_add(src1, src2, &s21_res);
    // print_s(src1);
    ck_assert_int_eq(s21_add(src1, src2, &s21_res), 0);
    ck_assert_int_eq(original_res.bits[0], s21_res.bits[0]);
    ck_assert_int_eq(original_res.bits[1], s21_res.bits[1]);
    ck_assert_int_eq(original_res.bits[2], s21_res.bits[2]);
    ck_assert_int_eq(original_res.bits[3], s21_res.bits[3]);
  }
  {
    s21_decimal a = {0};
    a.bits[0] = 0b11111111111111111111111111111111;
    a.bits[1] = 0b11111111111111111111111111111111;
    a.bits[2] = 0b11111111111111111111111111111111;
    a.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000000000000101;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000010000000000000000;
    s21_decimal original_res = {0};
    original_res.bits[0] = 0b11111111111111111111111111111110;
    original_res.bits[1] = 0b11111111111111111111111111111111;
    original_res.bits[2] = 0b11111111111111111111111111111111;
    original_res.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal s21_res = {0};
    s21_add(a, b, &s21_res);
    ck_assert_int_eq(s21_add(a, b, &s21_res), 0);
    ck_assert_int_eq(original_res.bits[0], s21_res.bits[0]);
    ck_assert_int_eq(original_res.bits[1], s21_res.bits[1]);
    ck_assert_int_eq(original_res.bits[2], s21_res.bits[2]);
    ck_assert_int_eq(original_res.bits[3], s21_res.bits[3]);
  }
  {
    s21_decimal a = {0};
    a.bits[0] = 0b11111111111111111111111111111111;
    a.bits[1] = 0b11111111111111111111111111111111;
    a.bits[2] = 0b11111111111111111111111111111111;
    a.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000000000000001;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal original_res = {0};
    original_res.bits[0] = 0b11111111111111111111111111111110;
    original_res.bits[1] = 0b11111111111111111111111111111111;
    original_res.bits[2] = 0b11111111111111111111111111111111;
    original_res.bits[3] = 0b10000000000000000000000000000000;
    s21_decimal s21_res = {0};
    s21_add(a, b, &s21_res);
    ck_assert_int_eq(s21_add(a, b, &s21_res), 0);
    ck_assert_int_eq(original_res.bits[0], s21_res.bits[0]);
    ck_assert_int_eq(original_res.bits[1], s21_res.bits[1]);
    ck_assert_int_eq(original_res.bits[2], s21_res.bits[2]);
    ck_assert_int_eq(original_res.bits[3], s21_res.bits[3]);
  }
  {
    s21_decimal a = {0};
    a.bits[0] = 0b00000000000000000000001111111111;
    a.bits[1] = 0b00000000000000000000000000000000;
    a.bits[2] = 0b00000000000000000000000000000000;
    a.bits[3] = 0b00000000000000010000000000000000;
    s21_decimal b = {0};
    b.bits[0] = 0b00000000000000000000001111111111;
    b.bits[1] = 0b00000000000000000000000000000000;
    b.bits[2] = 0b00000000000000000000000000000000;
    b.bits[3] = 0b00000000000000100000000000000000;
    s21_decimal original_res = {0};
    original_res.bits[0] = 0b00000000000000000010101111110101;
    original_res.bits[1] = 0b00000000000000000000000000000000;
    original_res.bits[2] = 0b00000000000000000000000000000000;
    original_res.bits[3] = 0b00000000000000100000000000000000;
    s21_decimal s21_res = {0};
    s21_add(a, b, &s21_res);
    ck_assert_int_eq(s21_add(a, b, &s21_res), 0);
    ck_assert_int_eq(original_res.bits[0], s21_res.bits[0]);
    ck_assert_int_eq(original_res.bits[1], s21_res.bits[1]);
    ck_assert_int_eq(original_res.bits[2], s21_res.bits[2]);
    ck_assert_int_eq(original_res.bits[3], s21_res.bits[3]);
  }

  {
    s21_decimal src1 = {0};
    src1.bits[0] = 0x00000000;
    src1.bits[1] = 0x0000000f;
    src1.bits[2] = 0x00000000;
    src1.bits[3] = 0x00000000;
    s21_decimal src2 = {0};
    src2.bits[0] = 0x00000001;
    src2.bits[1] = 0x00000000;
    src2.bits[2] = 0x00000000;
    src2.bits[3] = 0x80000000;
    s21_decimal original_res = {0};
    original_res.bits[0] = 0xffffffff;
    original_res.bits[1] = 0x0000000e;
    original_res.bits[2] = 0x00000000;
    original_res.bits[3] = 0x00000000;
    s21_decimal s21_res = {0};
    ck_assert_int_eq(s21_add(src1, src2, &s21_res), 0);
    ck_assert_int_eq(original_res.bits[0], s21_res.bits[0]);
    ck_assert_int_eq(original_res.bits[1], s21_res.bits[1]);
    ck_assert_int_eq(original_res.bits[2], s21_res.bits[2]);
    ck_assert_int_eq(original_res.bits[3], s21_res.bits[3]);
  }
}
END_TEST

START_TEST(test_sub) {
  {
    int num1 = -10;
    int num2 = -10;
    int dif_int = 0;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_sub(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, dif_int);
  }

  {
    int num1 = 10;
    int num2 = 20;
    int dif_int = -10;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_sub(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, dif_int);
  }
  {
    int num1 = -10;
    int num2 = 20;
    int dif_int = -30;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_sub(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, dif_int);
  }
  {
    float num1 = 9403.0;
    float num2 = 2020900.0;
    float dif_float = num1 - num2;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_float_to_decimal(num1, &a);
    s21_from_float_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    float res_float = 0.0;
    s21_sub(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, dif_float);
  } /*
   {
     float num1 = -1.0 / 0.0;
     int num2 = 202;
     double dif_float = 0 - num2;
     s21_decimal a = {0};
     s21_decimal b = {0};
     s21_from_float_to_decimal(num1, &a);
     s21_from_int_to_decimal(num2, &b);
     s21_decimal res_dec = {0};
     float res_float = 0.0;
     s21_sub(a, b, &res_dec);
     s21_from_decimal_to_float(res_dec, &res_float);
     ck_assert_float_eq(res_float, dif_float);
   }*/
  {  // 1045
    s21_decimal src1, src2, origin;
    // src1 = 4;
    // src2 = 97623323;

    src1.bits[0] = 0b00000000000000000000000000000100;
    src1.bits[1] = 0b00000000000000000000000000000000;
    src1.bits[2] = 0b00000000000000000000000000000000;
    src1.bits[3] = 0b00000000000000000000000000000000;

    src2.bits[0] = 0b00000101110100011001110100011011;
    src2.bits[1] = 0b00000000000000000000000000000000;
    src2.bits[2] = 0b00000000000000000000000000000000;
    src2.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal result = {{0, 0, 0, 0}};
    s21_sub(src1, src2, &result);

    origin.bits[0] = 0b00000101110100011001110100010111;
    origin.bits[1] = 0b00000000000000000000000000000000;
    origin.bits[2] = 0b00000000000000000000000000000000;
    origin.bits[3] = 0b10000000000000000000000000000000;
    ck_assert_int_eq(origin.bits[3], result.bits[3]);
    ck_assert_int_eq(origin.bits[2], result.bits[2]);
    ck_assert_int_eq(origin.bits[1], result.bits[1]);
    ck_assert_int_eq(origin.bits[0], result.bits[0]);
  }
}
END_TEST

START_TEST(div_same) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 2;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 1);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(div_simple) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 4;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 2);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(div_simple_remains) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(div_simple_remains_scale) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  num2.bits[0] = 2;
  set_scale(&num1, 3);
  // set_scale(&num2, 4);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 4);
}
END_TEST

START_TEST(div_simple_remains_scale2) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  // set_scale(&num1, 3);
  num2.bits[0] = 2;
  set_scale(&num2, 3);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 1500);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 0);
}
END_TEST

START_TEST(div_sign_num1) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 3);
  set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(div_sign_num2) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 3);
  // set_sign(&num1, 1);
  set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(get_sign(result), 1);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(div_sign_num1_num2) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 3;
  num2.bits[0] = 2;
  // set_scale(&num1, 3);
  // set_scale(&num2, 3);
  set_sign(&num1, 1);
  set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], 15);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 1);
}
END_TEST

START_TEST(div_three_period) {
  s21_decimal result = {0}, check = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  num2.bits[0] = 3;
  string_to_decimal("0,3333333333333333333333333333", &check);
  // set_scale(&num1, 3);
  // set_scale(&num2, 3);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], check.bits[0]);
  ck_assert_int_eq(result.bits[1], check.bits[1]);
  ck_assert_int_eq(result.bits[2], check.bits[2]);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 28);
}
END_TEST

START_TEST(div_three_period_scale) {
  s21_decimal result = {0}, check = {0}, num1 = {0}, num2 = {0};
  num1.bits[0] = 1;
  num2.bits[0] = 3;
  string_to_decimal("33333,333333333333333333333333", &check);
  // set_scale(&num1, 3);
  set_scale(&num2, 5);
  // set_sign(&num1, 1);
  // set_sign(&num2, 1);
  ck_assert_int_eq(s21_div(num1, num2, &result), 0);
  ck_assert_int_eq(result.bits[0], check.bits[0]);
  ck_assert_int_eq(result.bits[1], check.bits[1]);
  ck_assert_int_eq(result.bits[2], check.bits[2]);
  ck_assert_int_eq(get_sign(result), 0);
  ck_assert_int_eq(get_scale(result), 24);
}
END_TEST

START_TEST(big_div_ten) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("1289472198479812", &num1);
  string_to_decimal("10", &num2);
  string_to_decimal("128947219847981,2", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_two) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("1289472198479812", &num1);
  string_to_decimal("2", &num2);
  string_to_decimal("644736099239906", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_three) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("1289472198479812", &num1);
  string_to_decimal("3", &num2);
  string_to_decimal("429824066159937,3333333333333", &check_result);
  // print_s(check_result);
  s21_div(num1, num2, &result);
  // print_s(result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_max) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0};
  string_to_decimal("79228162514264337593543950335", &num1);
  string_to_decimal("0,000001", &num2);
  // string_to_decimal("429824066159937,3333333333333", &check_result);
  // print_s(check_result);
  // print_s(result);
  ck_assert_int_eq(s21_div(num1, num2, &result), 1);
}
END_TEST

START_TEST(big_div_one) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("1289472198479812", &num1);
  string_to_decimal("1", &num2);
  string_to_decimal("1289472198479812", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_eq) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("1289472198479812", &num1);
  string_to_decimal("1289472198479812", &num2);
  string_to_decimal("1", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_eq_big_scale) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("0,3333333333333333333333333333", &num1);
  string_to_decimal("0,3333333333333333333333333333", &num2);
  string_to_decimal("1", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(big_div_big) {
  s21_decimal result = {0}, num1 = {0}, num2 = {0}, check_result = {0};
  string_to_decimal("33333333333333333333333333333", &num1);
  string_to_decimal("11111111111111111111111111111", &num2);
  string_to_decimal("3", &check_result);

  s21_div(num1, num2, &result);
  // ck_assert_int_eq(status, 0);
  ck_assert_int_eq(result.bits[0], check_result.bits[0]);
  ck_assert_int_eq(result.bits[1], check_result.bits[1]);
  ck_assert_int_eq(result.bits[2], check_result.bits[2]);
  ck_assert_int_eq(get_scale(result), get_scale(check_result));
  ck_assert_int_eq(get_sign(result), 0);
}
END_TEST

START_TEST(test_s21_mul_basic) {
  s21_decimal a = {{2, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 6);
}
END_TEST

START_TEST(test_s21_mul_overflow) {
  s21_decimal a = {{
      UINT32_MAX,
      UINT32_MAX,
      UINT32_MAX,
  }};
  s21_decimal b = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &result);

  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_s21_mul_negative_result) {
  uint32_t sign_bit = (uint32_t)1 << 31;
  s21_decimal a = {{2, 0, 0, 0}};
  s21_decimal b = {{3, 0, 0, sign_bit}};
  s21_decimal result = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &result);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(result.bits[0], 6);
  ck_assert_int_eq((uint32_t)result.bits[3] & sign_bit, sign_bit);
}
END_TEST

START_TEST(test_mul1) {
  {
    int num1 = -10;
    int num2 = -10;
    int prod_int = 100;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, prod_int);
  }
  {
    int num1 = 10;
    int num2 = 20;
    int prod_int = 200;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, prod_int);
  }
  {
    int num1 = -10;
    int num2 = 20;
    int prod_int = -200;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, prod_int);
  }
  {
    int num1 = 9403;
    int num2 = 202;
    int res_origin = 1899406;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, res_origin);
  }
  {
    int num1 = -32768;
    int num2 = 2;
    int res_origin = -65536;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, res_origin);
  }
  {
    int num1 = -32768;
    int num2 = 32768;
    int res_origin = -1073741824;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_int_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    int res_int = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_int(res_dec, &res_int);
    ck_assert_int_eq(res_int, res_origin);
  }
  {
    float num1 = 9403.0e2;
    int num2 = 202;
    float res_origin = (float)189940600;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_float_to_decimal(num1, &a);
    s21_from_int_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    float res_float = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);
  }
  {
    float num1 = 9403.0e2;
    float num2 = 9403.0e2;
    float res_origin = (float)884164090000;
    s21_decimal a = {0};
    s21_decimal b = {0};
    s21_from_float_to_decimal(num1, &a);
    s21_from_float_to_decimal(num2, &b);
    s21_decimal res_dec = {0};
    float res_float = 0;
    s21_mul(a, b, &res_dec);
    s21_from_decimal_to_float(res_dec, &res_float);
    ck_assert_float_eq(res_float, res_origin);
  }
  {
    s21_decimal dec1 = {{4, 0, 0, 0}};
    s21_decimal dec2 = {{10, 0, 0, 0}};

    s21_decimal d22;
    s21_mul(dec1, dec2, &d22);

    int qqqq = 0;
    s21_from_decimal_to_int(d22, &qqqq);
    ck_assert_int_eq(qqqq, 40);
  }

  {
    s21_decimal dst1;
    dst1.bits[0] = 0b01110000000000000000000000000000;
    dst1.bits[1] = 0b10110011000000110001000010100111;
    dst1.bits[2] = 0b11100010001011101010010010010011;
    dst1.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal dst2;
    dst2.bits[0] = 0b00000000000000000000000000000001;
    dst2.bits[1] = 0b10000000000000100000000000000000;
    dst2.bits[2] = 0b00000000000000000000000000000000;
    dst2.bits[3] = 0b00000000000000000000000000000000;

    s21_decimal d22;
    int error = s21_mul(dst1, dst2, &d22);

    // int qqqq = 0;
    // s21_from_decimal_to_int(d22, &qqqq);
    ck_assert_int_eq(error, 1);
  }

  {
    s21_decimal dst1;
    dst1.bits[0] = 0b01110000000000000000000000000000;
    dst1.bits[1] = 0b10110011000000110001000010100111;
    dst1.bits[2] = 0b11100010001011101010010010010011;
    dst1.bits[3] = 0b00000000000000000000000000000000;
    s21_decimal dst2;
    dst2.bits[0] = 0b00000000000000000000000000000001;
    dst2.bits[1] = 0b10000000000000100000000000000000;
    dst2.bits[2] = 0b00000000000000000000000000000000;
    dst2.bits[3] = 0b10000000000000000000000000000000;

    s21_decimal d22;
    int error = s21_mul(dst1, dst2, &d22);

    // int qqqq = 0;
    // s21_from_decimal_to_int(d22, &qqqq);
    ck_assert_int_eq(error, 2);
  }

  {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal1 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal1 = {{0, 0, 0, 0x1D0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 0-15 не нули (старший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x1C8000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal1 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // Просто все единицы
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 154 (показатель степени должен быть от 0 до 28)
    // биты 0-15 не нули
    // биты 24-30 не нули
    s21_decimal decimal2 = {{0, 0, 0, 1000000000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 29 (показатель степени должен быть от 0 до 28)
    s21_decimal decimal2 = {{0, 0, 0, 0x1D0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 0-15 не нули (младший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1C0001}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 0-15 не нули (старший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x1C8000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 24-30 не нули (младший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x11C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // степень 28 (что корректно), но биты 24-30 не нули (старший бит)
    s21_decimal decimal2 = {{-1, 0, 0, 0x401C0000}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }

  {
    // Просто все единицы
    s21_decimal decimal2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}};
    s21_decimal decimal1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}};
    s21_decimal result;
    int code = s21_mul(decimal1, decimal2, &result);

    ck_assert_int_ne(code, 0);
  }
}
END_TEST

START_TEST(MUL_TEST1) {
  s21_decimal ds;
  ds.bits[2] = 0x00000000;
  ds.bits[1] = 0x00000000;
  ds.bits[0] = 0x00000001;
  ds.bits[3] = 0x00010000;
  s21_decimal ds2;
  ds2.bits[2] = 0x00000000;
  ds2.bits[1] = 0x00000000;
  ds2.bits[0] = 0x00000002;
  ds2.bits[3] = 0x00000000;
  s21_decimal res;
  int ret = s21_mul(ds, ds2, &res);
  ck_assert(ret == 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[3], 0x00010000);
}
END_TEST

START_TEST(MUL_TEST2) {
  s21_decimal ds;
  ds.bits[2] = 0x00000000;
  ds.bits[1] = 0x00000000;
  ds.bits[0] = 0xffffffff;
  ds.bits[3] = 0x80000000;
  s21_decimal ds2;
  ds2.bits[2] = 0x00000000;
  ds2.bits[1] = 0x00000000;
  ds2.bits[0] = 0x00000002;
  ds2.bits[3] = 0x00000000;
  s21_decimal res;
  int ret = s21_mul(ds, ds2, &res);
  ck_assert(ret == 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[1], 0x00000001);
  ck_assert_int_eq(res.bits[0], 0xfffffffe);
  ck_assert_int_eq(res.bits[3], 0x80000000);
  ds.bits[2] = 0xffffffff;
  ret = s21_mul(ds, ds2, &res);
  ck_assert_int_eq(ret, 2);
}
END_TEST

START_TEST(MUL_TEST3) {
  s21_decimal ds;
  ds.bits[2] = 0x0bffffff;
  ds.bits[1] = 0xffffffff;
  ds.bits[0] = 0xffffffff;
  ds.bits[3] = 0x00000000;
  s21_decimal ds2;
  ds2.bits[2] = 0x13ffffff;
  ds2.bits[1] = 0xffffffff;
  ds2.bits[0] = 0xffffffff;
  ds2.bits[3] = 0x00000000;
  s21_decimal res;
  int ret = s21_mul(ds, ds2, &res);
  ck_assert_int_eq(ret, 1);
  ds.bits[3] = 0x001c0000;
  ds2.bits[3] = 0x001c0000;
  ret = s21_mul(ds, ds2, &res);
  ck_assert_int_eq(ret, 0);
}
END_TEST

Suite* test_arithmetic(void) {
  Suite* s1 = suite_create("\033[45m S21_ARITHMETICS \033[0m");
  TCase* tc1_1 = tcase_create("arithmetics_tc");

  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, add);
  tcase_add_test(tc1_1, add_diff);
  tcase_add_test(tc1_1, add_diff2);
  tcase_add_test(tc1_1, add_minus);
  tcase_add_test(tc1_1, add_small);
  tcase_add_test(tc1_1, add_minus_small);
  tcase_add_test(tc1_1, add_minus_minus);
  tcase_add_test(tc1_1, add_minus_bigger);
  tcase_add_test(tc1_1, add_plus_minus);
  tcase_add_test(tc1_1, add_overflow);

  tcase_add_test(tc1_1, sub);
  tcase_add_test(tc1_1, sub_diff);
  tcase_add_test(tc1_1, sub_lower);
  tcase_add_test(tc1_1, sub_minus_lower);
  tcase_add_test(tc1_1, sub_minus_num2_lower);
  tcase_add_test(tc1_1, sub_minus_minus_eq);
  tcase_add_test(tc1_1, sub_minus_add);
  tcase_add_test(tc1_1, sub_minus_add_lower);
  tcase_add_test(tc1_1, sub_minus_minus_bigger);
  tcase_add_test(tc1_1, sub_minus_bigger);
  tcase_add_test(tc1_1, sub_minus_num2_bigger);

  tcase_add_test(tc1_1, mul_fffff);
  // tcase_add_test(tc1_1, mul_1);
  // tcase_add_test(tc1_1, mul_0);
  // tcase_add_test(tc1_1, mul_big);

  tcase_add_test(tc1_1, div_simple);
  tcase_add_test(tc1_1, div_simple_remains);
  tcase_add_test(tc1_1, div_simple_remains_scale);
  tcase_add_test(tc1_1, div_simple_remains_scale2);
  tcase_add_test(tc1_1, div_sign_num1);
  tcase_add_test(tc1_1, div_sign_num2);
  tcase_add_test(tc1_1, div_sign_num1_num2);
  tcase_add_test(tc1_1, div_same);
  tcase_add_test(tc1_1, div_three_period);
  tcase_add_test(tc1_1, div_three_period_scale);

  tcase_add_test(tc1_1, test_add);
  tcase_add_test(tc1_1, test_sub);

  tcase_add_test(tc1_1, big_div_ten);
  tcase_add_test(tc1_1, big_div_two);
  tcase_add_test(tc1_1, big_div_three);
  tcase_add_test(tc1_1, big_div_one);
  tcase_add_test(tc1_1, big_div_eq);
  tcase_add_test(tc1_1, big_div_eq_big_scale);
  tcase_add_test(tc1_1, big_div_big);
  tcase_add_test(tc1_1, big_div_max);

  tcase_add_test(tc1_1, test_mul1);
  tcase_add_test(tc1_1, test_s21_mul_basic);
  tcase_add_test(tc1_1, test_s21_mul_negative_result);
  tcase_add_test(tc1_1, test_s21_mul_overflow);
  tcase_add_test(tc1_1, MUL_TEST1);
  tcase_add_test(tc1_1, MUL_TEST2);
  tcase_add_test(tc1_1, MUL_TEST3);

  suite_add_tcase(s1, tc1_1);
  return s1;
}
