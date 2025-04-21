#pragma once

#define MINUS 0x80000000

typedef struct {
  unsigned int bits[4];
} s21_decimal;

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;

#include <limits.h>
#include "include/arithmetic.h"
#include "include/comparison.h"
#include "include/converters.h"
#include "include/helpers.h"
#include "include/other.h"