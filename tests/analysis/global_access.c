#include <base/file.h>
#include <base/new.h>
#include <base/print.h>
#include <base/test.h>
#include <lang/analysis/typecheck.h>
#include <lang/parser/parser.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wunused-function"

static int
test_global_access_in_module()
{
  return TEST_SKIPPED;
}

static int
test_global_access_in_class() {
  return TEST_SKIPPED;
}

static int
test_global_access_in_function() {
  return TEST_SKIPPED;
}
