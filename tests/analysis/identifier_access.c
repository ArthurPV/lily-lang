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
test_identifier_access_in_scope()
{
  return TEST_SKIPPED;
}

static int
test_identifier_access_in_global()
{
  return TEST_SKIPPED;
}
