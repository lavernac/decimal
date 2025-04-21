#pragma once

#include <check.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#include "../s21_decimal.h"

Suite *test_converters();
Suite *test_arithmetic();
Suite *test_cmp();
Suite *test_other();