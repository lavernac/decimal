#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "../s21_decimal.h"

int main() {
  s21_decimal decimal = {0};
  s21_decimal src1 = {0}, src2 = {0};
  // src1 = 12345677.987654345678987654346;
  // 12345677.987654345678987654346
  // src2 = 87654323456.9876545678987653;
  // 87654323456.9876545678987653
  // printf("\ns21_subTest19 s21_subTest19 s21_subTest19\n\n");

  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;

  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  //
  printf("%d\n", s21_sub(src1, src2, &decimal));
  char str_decimal[128] = {0};
  s21_big_decimal big = decimal_to_big(decimal);
  s21_big_decimal_to_string(big, str_decimal);
  printf("%s\n", str_decimal);
  // printf("%d\n", s21_round(decimal3, &decimal));
  // add 342
  // sub 324
  // mul 1901
  // div 4880

  return 0;
}
