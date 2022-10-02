#ifndef LILY_TYPECHECK_H
#define LILY_TYPECHECK_H

#include <lang/parser/parser.h>

typedef struct Typecheck
{
    struct Parser parser;
    struct Vec *funs;        // struct Vec<struct FunSymbol*>*
    struct Vec *consts;      // struct Vec<struct ConstSymbol*>*
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
 * @brief Free the Typecheck type.
 */
void
__free__Typecheck(struct Typecheck self);

#endif // LILY_TYPECHECK_H