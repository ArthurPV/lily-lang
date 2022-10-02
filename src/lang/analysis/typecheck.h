#ifndef LILY_TYPECHECK_H
#define LILY_TYPECHECK_H

#include <lang/parser/parser.h>

typedef struct Typecheck
{
    struct Parser parser;
    struct Decl *decl;
    Usize pos;
    Usize count_error;
    Usize count_warning;
    Usize current_fun_id;
    Usize current_const_id;
    Usize current_module_id;
    Usize current_alias_id;
    Usize current_record_id;
    Usize current_enum_id;
    Usize current_error_id;
    Usize current_class_id;
    Usize current_trait_id;
    Usize current_record_obj_id;
    Usize current_enum_obj_id;
    struct Vec *builtins;    // struct Vec<struct Builtin*>*
    struct Vec *funs;        // struct Vec<struct FunSymbol*>*
    struct Vec *consts;      // struct Vec<struct ConstantSymbol*>*
    struct Vec *modules;     // struct Vec<struct ModuleSymbol*>*
    struct Vec *aliases;     // struct Vec<struct AliasSymbol*>*
    struct Vec *records;     // struct Vec<struct RecordSymbol*>*
    struct Vec *enums;       // struct Vec<struct EnumSymbol*>*
    struct Vec *errors;      // struct Vec<struct ErrorSymbol*>*
    struct Vec *classes;     // struct Vec<struct ClassSymbol*>*
    struct Vec *traits;      // struct Vec<struct TraitSymbol*>*
    struct Vec *records_obj; // struct Vec<struct RecordObjSymbol*>*
    struct Vec *enums_obj;   // struct Vec<struct EnumObjSymbol*>
} Typecheck;

/**
 *
 * @brief Construct the Typecheck type.
 */
struct Typecheck
__new__Typecheck(struct Parser parser);

/**
 *
 * @brief Run the typecheck phase.
 */
void
run__Typecheck(struct Typecheck *self);

/**
 *
 * @brief Free the Typecheck type.
 */
void
__free__Typecheck(struct Typecheck self);

#endif // LILY_TYPECHECK_H
