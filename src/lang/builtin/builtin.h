#ifndef LILY_BUILTIN_H
#define LILY_BUILTIN_H

#include <base/types.h>

enum BuiltinKind
{
    BuiltinKindFun,
    BuiltinKindModule
};

typedef struct BuiltinFun
{
    Str name;
    struct Vec *params; // struct Vec<struct DataType*>*
    // Last params is the return type
} BuiltinFun;

/**
 *
 * @brief Construct the BuiltinFun type.
 */
struct BuiltinFun *
__new__BuiltinFun(const Str name, struct Vec *params);

/**
 *
 * @brief Free the BuiltinFun type.
 */
void
__free__BuiltinFun(struct BuiltinFun *self);

typedef struct BuiltinModule
{
    Str name;
    struct Vec *items; // struct Vec<struct Builtin*>*
} BuiltinModule;

/**
 *
 * @brief Construct the BuiltinModule type.
 */
struct BuiltinModule *
__new__BuiltinModule(const Str name, struct Vec *items);

/**
 *
 * @brief Free the BuiltinFun type.
 */
void
__free__BuiltinModule(struct BuiltinModule *self);

typedef struct Builtin
{
    enum BuiltinKind kind;

    union
    {
        struct BuiltinFun *fun;
        struct BuiltinModule *module;
    };
} Builtin;

/**
 *
 * @brief Construct the Builtin type (Fun variant).
 */
struct Builtin *
__new__BuiltinFunVar(struct BuiltinFun *fun);

/**
 *
 * @brief Construct the Builtin type (Module variant).
 */
struct Builtin *
__new__BuiltinModuleVar(struct BuiltinModule *module);

/**
 *
 * @brief Free the Builtin type (Fun variant).
 */
void
__free__BuiltinFunVar(struct Builtin *self);

/**
 *
 * @brief Free the Builtin type (Module variant).
 */
void
__free__BuiltinModuleVar(struct Builtin *self);

/**
 *
 * @brief Free the Builtin type (all variants).
 */
void
__free__BuiltinAll(struct Builtin *self);

#endif // LILY_BUILTIN_H
