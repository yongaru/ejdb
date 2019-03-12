#include "ejdb_test.h"
#include <CUnit/Basic.h>

int init_suite() {
  int rc = ejdb_init();
  return rc;
}

int clean_suite() {
  return 0;
}

static void ejdb_test4_1() {
  EJDB_OPTS opts = {
    .kv = {
      .path = "ejdb_test4_1.db",
      .oflags = IWKV_TRUNC
    },
    .no_wal = true,
    .http = {
      .enabled = true,
      .port = 9292
    }
  };
  EJDB db;
  iwrc rc = ejdb_open(&opts, &db);
  CU_ASSERT_EQUAL_FATAL(rc, 0);

  //sleep(1);

  rc = ejdb_close(&db);
  CU_ASSERT_EQUAL_FATAL(rc, 0);
}

int main() {
  CU_pSuite pSuite = NULL;
  if (CUE_SUCCESS != CU_initialize_registry()) return CU_get_error();
  pSuite = CU_add_suite("ejdb_test1", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  if (
    (NULL == CU_add_test(pSuite, "ejdb_test4_1", ejdb_test4_1))
  ) {
    CU_cleanup_registry();
    return CU_get_error();
  }
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  int ret = CU_get_error() || CU_get_number_of_failures();
  CU_cleanup_registry();
  return ret;
}

