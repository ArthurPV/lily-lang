#include "alias.c"
#include "class.c"
#include "enum.c"
#include "error.c"
#include "expr.c"
#include "fun.c"
#include "global_access.c"
#include "identifier_access.c"
#include "import.c"
#include "module.c"
#include "object.c"
#include "record.c"
#include "self_access.c"
#include "stmt.c"
#include "tag.c"
#include "trait.c"
#include "type.c"
#include "variable.c"
#include <base/new.h>
#include <base/test.h>

int
main()
{
    struct Test *t = NEW(Test);
    struct Suite *fun = NEW(Suite, "fun");
    struct Suite *class = NEW(Suite, "class");
    struct Suite *enum_ = NEW(Suite, "enum");
    struct Suite *record = NEW(Suite, "record");
    struct Suite *alias = NEW(Suite, "alias");
    struct Suite *error = NEW(Suite, "error");
    struct Suite *expr = NEW(Suite, "expr");
    struct Suite *global_access = NEW(Suite, "global_access");
    struct Suite *identifier_access = NEW(Suite, "identifier_access");
    struct Suite *import = NEW(Suite, "import");
    struct Suite *module = NEW(Suite, "module");
    struct Suite *object = NEW(Suite, "object");
    struct Suite *self_access = NEW(Suite, "self_access");
    struct Suite *stmt = NEW(Suite, "stmt");
    struct Suite *tag = NEW(Suite, "tag");
    struct Suite *trait = NEW(Suite, "trait");
    struct Suite *type = NEW(Suite, "type");
    struct Suite *variable = NEW(Suite, "variable");

    CASE(fun, infer on fun params, test_fun_param_inference);
    CASE(fun, check generic param, test_fun_param_generic);
    CASE(fun,
         check generic param with restriction,
         test_fun_param_generic_with_restriction);
    CASE(fun, ommite fun generic param, test_fun_generic_param_omitted);
    CASE(fun, infer on fun return data type, test_fun_return_type_inferance);
    CASE(fun, check fun tags, test_fun_tags);

    CASE(class, generic param, test_class_generic_param);
    CASE(class, class constructor, test_class_constructor);
    CASE(class,
         super param in class constructor,
         test_class_super_param_in_constructor);
    CASE(class, check class inheritance, test_class_inheritance);
    CASE(class, check class impl, test_class_impl);
    CASE(class, check class property, test_class_property);
    CASE(class, check class method, test_class_method);
    CASE(class, check class method inference, test_class_method_inference);
    CASE(class, check class visibility, test_class_visibility);

    CASE(
      enum_, check enum with value by default, test_enum_with_value_by_default);
    CASE(enum_, check generic param, test_enum_generic_param);
    CASE(enum_, check enum variant type, test_enum_variant_type);
    CASE(enum_, check enum variant, test_enum_variant);

    CASE(record, check generic param, test_record_generic_param);
    CASE(record, check field type, test_record_field_type);
    CASE(record, check field, test_record_field);
    CASE(record, check field visibility, test_record_field_visibility);

    CASE(alias, check generic param, test_alias_generic_param);
    CASE(alias, check data type, test_alias_data_type);
    CASE(alias, check visibility, test_alias_visibility);

    CASE(error, check visibility, test_error_visibility);

    CASE(expr, check unary op, test_expr_unary_op);
    CASE(expr, check binary op, test_expr_binary_op);
    CASE(expr, check fun call, test_expr_fun_call);
    CASE(expr, check record call, test_expr_record_call);
    CASE(expr, check identifier, test_expr_identifier);
    CASE(expr, check array access, test_expr_array_access);
    CASE(expr, check tuple access, test_expr_tuple_access);
    CASE(expr, check lambda, test_expr_lambda);
    CASE(expr, check tuple, test_expr_tuple);
    CASE(expr, check array, test_expr_array);
    CASE(expr, check variant, test_expr_variant);
    CASE(expr, check try, test_expr_try);
    CASE(expr, check if, test_expr_if);
    CASE(expr, check block, test_expr_block);

    CASE(global_access, check global_access in module, test_global_access_in_module);
    CASE(global_access, check global access in class, test_global_access_in_class);
    CASE(global_access, check global access in function, test_global_access_in_function);

    CASE(identifier_access, check identifier_access in scope, test_identifier_access_in_scope);
    CASE(identifier_access, check identifier_access in global, test_identifier_access_in_global);

    CASE(import, check import builtin, test_import_builtin);
    CASE(import, check import core, test_import_core);
    CASE(import, check import file, test_import_file);
    CASE(import, check import std, test_import_std);
    CASE(import, check import url, test_import_url);
    CASE(import, check import in public, test_import_in_public);
    CASE(import, check import with as, test_import_with_as);

    CASE(module, check module scope, test_module_scope);
    CASE(module, check module in public, test_module_in_public);

    CASE(object, check object record, test_object_record);;
    CASE(object, check object enum, test_object_enum);
    CASE(object, check attached function to object, test_attached_function_to_object);

    CASE(self_access, check self_access in class, test_self_access_in_class);
    CASE(self_access, check self_access in class with inheritance, test_self_access_in_class_with_inheritance);
    CASE(self_access, check self_access in record object, test_self_access_in_record_object);
    CASE(self_access, check self access in enum object, test_self_access_in_enum_object);

    CASE(stmt, check stmt return, test_stmt_return);
    CASE(stmt, check stmt if, test_stmt_if);
    CASE(stmt, check stmt await, test_stmt_await);
    CASE(stmt, check stmt try, test_stmt_try);
    CASE(stmt, check stmt match, test_stmt_match);
    CASE(stmt, check stmt while, test_stmt_while);
    CASE(stmt, check stmt for, test_stmt_for);
    CASE(stmt, check stmt next, test_stmt_next);
    CASE(stmt, check stmt break, test_stmt_break);
    CASE(stmt, check stmt import, test_stmt_import);

    CASE(tag, simple test, test_tag);

    CASE(trait, check trait generic param, test_trait_generic_param);
    CASE(trait, check trati inheritance, test_trait_inheritance);

    CASE(type, simple test, test_type);

    CASE(variable, check variable data type, test_variable_data_type);
    
    SUITE(t, fun);
    SUITE(t, class);
    SUITE(t, enum_);
    SUITE(t, record);
    SUITE(t, alias);
    SUITE(t, error);
    SUITE(t, expr);
    SUITE(t, global_access);
    SUITE(t, identifier_access);
    SUITE(t, import);
    SUITE(t, module);
    SUITE(t, object);
    SUITE(t, self_access);
    SUITE(t, stmt);
    SUITE(t, tag);
    SUITE(t, trait);
    SUITE(t, type);
    SUITE(t, variable);

    RUN_TEST(t);
}
