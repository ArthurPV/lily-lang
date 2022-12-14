#include "alias.c"
#include "class.c"
#include "constant.c"
#include "enum.c"
#include "error.c"
#include "expr.c"
#include "fun.c"
#include "import.c"
#include "module.c"
#include "record.c"
#include "stmt.c"
#include "tag.c"
#include "trait.c"
#include <base/new.h>
#include <base/test.h>

int
main()
{
    struct Test *t = NEW(Test);
    struct Suite *fun = NEW(Suite, "fun");
    struct Suite *constant = NEW(Suite, "constant");
    struct Suite *module = NEW(Suite, "module");
    struct Suite *alias = NEW(Suite, "alias");
    struct Suite *tag = NEW(Suite, "tag");
    struct Suite *class = NEW(Suite, "class");
    struct Suite *error = NEW(Suite, "error");
    struct Suite *enum_ = NEW(Suite, "enum");
    struct Suite *import = NEW(Suite, "import");
    struct Suite *record = NEW(Suite, "record");
    struct Suite *trait = NEW(Suite, "trait");
    struct Suite *expr = NEW(Suite, "expr");
    struct Suite *stmt = NEW(Suite, "stmt");

    CASE(fun, simple, test_fun);
    CASE(constant, simple, test_constant);
    CASE(module, simple, test_module);
    CASE(alias, simple, test_alias);
    CASE(tag, simple, test_tag);
    CASE(class, simple, test_class);
    CASE(error, simple, test_error);
    CASE(enum_, simple, test_enum);

    CASE(import, std, test_import_std);
    CASE(import, builtin, test_import_builtin);
    CASE(import, core, test_import_core);
    CASE(import, url, test_import_url);
    CASE(import, file, test_import_file);
    CASE(import, simple, test_import);

    CASE(record, simple, test_record);
    CASE(trait, simple, test_trait);

    CASE(expr, unaryop, test_expr_unaryop);
    CASE(expr, binaryop, test_expr_binaryop);
    CASE(expr, fun call, test_expr_fun_call);
    CASE(expr, record call, test_expr_record_call);
    CASE(expr, identifier, test_expr_identifier);
    CASE(expr, identifier access, test_expr_identifier_access);
    CASE(expr, global access, test_expr_global_access);
    CASE(expr, array access, test_expr_array_access);
    CASE(expr, tuple access, test_expr_tuple_access);
    CASE(expr, lambda, test_expr_lambda);
    CASE(expr, tuple, test_expr_tuple);
    CASE(expr, array, test_expr_array);
    CASE(expr, variant, test_expr_variant);
    CASE(expr, try, test_expr_try);
    CASE(expr, if, test_expr_if);
    CASE(expr, block, test_expr_block);
    CASE(expr, question mark, test_expr_question_mark);
    CASE(expr, dereference, test_expr_dereference);
    CASE(expr, ref, test_expr_ref);
    CASE(expr, self, test_expr_self);
    CASE(expr, undef, test_expr_undef);
    CASE(expr, nil, test_expr_nil);
    CASE(expr, none, test_expr_none);
    CASE(expr, wildcard, test_expr_wildcard);
    CASE(expr, literal, test_expr_literal);
    CASE(expr, variable, test_expr_variable);
    CASE(expr, grouping, test_expr_grouping);

    CASE(stmt, return, test_stmt_return);
    CASE(stmt, if, test_stmt_if);
    CASE(stmt, await, test_stmt_await);
    CASE(stmt, try, test_stmt_try);
    CASE(stmt, match, test_stmt_match);
    CASE(stmt, while, test_stmt_while);
    CASE(stmt, for, test_stmt_for);
    CASE(stmt, next, test_stmt_next);
    CASE(stmt, break, test_stmt_break);
    CASE(stmt, import, test_stmt_import);
    CASE(stmt, simple, test_stmt);

    SUITE(t, fun);
    SUITE(t, constant);
    SUITE(t, module);
    SUITE(t, alias);
    SUITE(t, tag);
    SUITE(t, class);
    SUITE(t, error);
    SUITE(t, enum_);
    SUITE(t, import);
    SUITE(t, record);
    SUITE(t, trait);
    SUITE(t, expr);
    SUITE(t, stmt);

    RUN_TEST(t);
}
