#include "test_decimal.h"

int main() {
  int failed = 0;
  Suite *decimal_test[] = {test_converters(), test_arithmetic(), test_cmp(),
                           test_other(), NULL};

  for (int i = 0; decimal_test[i] != NULL; i++) {
    SRunner *sr = srunner_create(decimal_test[i]);

    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_NORMAL);

    failed += srunner_ntests_failed(sr);
    srunner_free(sr);
  }
  printf("========= FAILED: %d =========\n", failed);

  return failed == 0 ? 0 : 1;
}
