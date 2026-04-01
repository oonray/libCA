#include <test_defs.h>
#include <vector_tests.h>

MunitSuite ca_tests_suites[] = {
    {"Vector tests/", vector_tests, NULL, 1, MUNIT_SUITE_OPTION_NONE},
    {NULL, NULL, NULL, 0, MUNIT_SUITE_OPTION_NONE},
};

int main(int argc, char *argv[]) {
  return munit_suite_main(ca_tests_suites, NULL, argc, argv);
}
