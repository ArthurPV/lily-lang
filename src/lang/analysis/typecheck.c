/*
 * MIT License
 *
 * Copyright (c) 2022 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <assert.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/platform.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>
#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/parser/ast.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>

#define SUMMARY()                                                             \
    if (count_error > 0) {                                                    \
        emit__Summary(                                                        \
          count_error, count_warning, "the typecheck phase has been failed"); \
        exit(1);                                                              \
    }

static Usize pos = 0;
static Usize count_error = 0;
static Usize count_warning = 0;
static Usize count_fun_id = 0;
static Usize count_const_id = 0;
static Usize count_module_id = 0;
static Usize count_alias_id = 0;
static Usize count_record_id = 0;
static Usize count_enum_id = 0;
static Usize count_error_id = 0;
static Usize count_class_id = 0;
static Usize count_trait_id = 0;
static Usize count_record_obj_id = 0;
static Usize count_enum_obj_id = 0;

const Int128 MaxUInt8 = 0xFF;
const Int128 MaxUInt16 = 0xFFFF;
const Int128 MaxUInt32 = 0xFFFFFFFF;
const Int128 MaxUInt64 = 0xFFFFFFFFFFFFFFFF;
#define MaxUInt128 (1 << 128) - 1

const Int128 MinInt8 = -0x80;
const Int128 MaxInt8 = 0x7F;
const Int128 MinInt16 = -0x8000;
const Int128 MaxInt16 = 0x7FFF;
const Int128 MinInt32 = -0x80000000;
const Int128 MaxInt32 = 0x7FFFFFFF;
const Int128 MinInt64 = -0x8000000000000000;
const Int128 MaxInt64 = 0x7FFFFFFFFFFFFFFF;
#define MinInt128 -(1 << 127)
#define MaxInt128 (1 << 127) - 1

const Float32 MinPosFloat32 = 1.175494351e-38F;
const Float32 MaxFloat32 = 3.402823466e+38F;

const Float64 MinPosFloat64 = 2.2250738585072014e-308;
const Float64 MaxFloat64 = 1.7976931348623158e+308;

const Str BuiltinFirstLayer[] = {
    "Int8",   "Int16",  "Int32",   "Int64",    "Int128",  "Uint8",   "Uint16",
    "Uint32", "Uint64", "Uint128", "Bool",     "Float32", "Float64", "Char",
    "Str",    "Isize",  "Usize",   "Optional", "Ptr",     "Tuple",   "Array",
    "Fun",    "Ref",    "Custom",  "Mem",      "Io",      "Never"
};

// It is not used to_str__UnaryOpKind, because it is a little less efficient.
Str UnaryOperatorToFunName[UnaryOpKindCustom + 1] = {
    [UnaryOpKindNegative] = "-",
    [UnaryOpKindNot] = "not",
    [UnaryOpKindReference] = "&",
    [UnaryOpKindBitNot] = "~",
    [UnaryOpKindCustom] = "<Custom>"
};

// It is not used to_str__BinaryOpKind, because it is a little less efficient.
Str BinaryOperatorToName[BinaryOpKindCustom + 1] = {
    [BinaryOpKindAdd] = "+",
    [BinaryOpKindSub] = "-",
    [BinaryOpKindMul] = "*",
    [BinaryOpKindDiv] = "/",
    [BinaryOpKindMod] = "%",
    [BinaryOpKindRange] = "..",
    [BinaryOpKindLt] = "<",
    [BinaryOpKindGt] = ">",
    [BinaryOpKindLe] = "<=",
    [BinaryOpKindGe] = ">=",
    [BinaryOpKindEq] = "==",
    [BinaryOpKindNe] = "not=",
    [BinaryOpKindAnd] = "and",
    [BinaryOpKindOr] = "or",
    [BinaryOpKindXor] = "xor",
    [BinaryOpKindAssign] = "=",
    [BinaryOpKindAddAssign] = "+=",
    [BinaryOpKindSubAssign] = "-=",
    [BinaryOpKindMulAssign] = "*=",
    [BinaryOpKindDivAssign] = "/=",
    [BinaryOpKindModAssign] = "%=",
    [BinaryOpKindConcatAssign] = "^=",
    [BinaryOpKindBitLShiftAssign] = "<<=",
    [BinaryOpKindBitRShiftAssign] = ">>=",
    [BinaryOpKindBitOrAssign] = "|=",
    [BinaryOpKindXorAssign] = "xor=",
    [BinaryOpKindBitAndAssign] = "&=",
    [BinaryOpKindMergeAssign] = "++=",
    [BinaryOpKindUnmergeAssign] = "--=",
    [BinaryOpKindExponentAssign] = "**=",
    [BinaryOpKindChain] = "|>",
    [BinaryOpKindMerge] = "++",
    [BinaryOpKindUnmerge] = "--",
    [BinaryOpKindRepeat] = "$",
    [BinaryOpKindConcat] = "^",
    [BinaryOpKindBitLShift] = "<<",
    [BinaryOpKindBitRShift] = ">>",
    [BinaryOpKindBitOr] = "|",
    [BinaryOpKindBitAnd] = "&",
    [BinaryOpKindExponent] = "**",
    [BinaryOpKindCustom] = "<Custom>"
};

enum SymbolTableKind getSymbolTableKindFromDecl[DeclKindImport + 1] = {
    [DeclKindFun] = SymbolTableKindFun,
    [DeclKindConstant] = SymbolTableKindConstant,
    [DeclKindModule] = SymbolTableKindModule,
    [DeclKindAlias] = SymbolTableKindAlias,
    [DeclKindRecord] = SymbolTableKindRecord,
    [DeclKindEnum] = SymbolTableKindEnum,
    [DeclKindError] = SymbolTableKindError,
    [DeclKindClass] = SymbolTableKindClass,
    [DeclKindTrait] = SymbolTableKindTrait
    // Import and Tag doesn't exist in SymbolTableKind
};

inline struct Diagnostic *
__new__DiagnosticWithErrTypecheck(struct Typecheck *self,
                                  struct LilyError *err,
                                  struct Location loc,
                                  struct String *detail_msg,
                                  struct Option *help);
inline struct Diagnostic *
__new__DiagnosticWithWarnTypecheck(struct Typecheck *self,
                                   struct LilyWarning *warn,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help);
inline struct Diagnostic *
__new__DiagnosticWithNoteTypecheck(struct Typecheck *self,
                                   struct String *note,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help);
void
push_all_symbols(struct Compiler *self);
struct DataTypeSymbol *
check_data_type_custom(struct Compiler *self,
                       struct DataType *dt,
                       struct SymbolTable *last);
struct DataTypeSymbol *
check_data_type_custom_in_global(struct Compiler *self, struct DataType *dt);
struct DataTypeSymbol *
check_data_type(struct Compiler *self,
                struct DataType *dt,
                struct SymbolTable *last);
struct SymbolTable *
get_symbol_from_idx(struct Compiler *self, struct SymbolTable *symb, Usize idx);
struct SymbolTable *
search_from_scopes(struct Compiler *self, struct Vec *scopes, Usize *idx);
struct SymbolTable *
search_in_module(struct Compiler *self,
                 struct SymbolTable *symb,
                 struct String *name,
                 bool check_visibility);
struct SymbolTable *
search_in_class(struct Compiler *self,
                struct SymbolTable *symb,
                struct String *name,
                bool check_visibility);
struct SymbolTable *
search_in_record(struct Compiler *self,
                 struct SymbolTable *symb,
                 struct String *name);
struct SymbolTable *
search_in_enum(struct Compiler *self,
               struct SymbolTable *symb,
               struct String *name);
struct SymbolTable *
search_in_record_obj(struct Compiler *self,
                     struct SymbolTable *symb,
                     struct String *name,
                     bool check_visibility);
struct SymbolTable *
search_in_enum_obj(struct Compiler *self,
                   struct SymbolTable *symb,
                   struct String *name,
                   bool check_visibility);
struct SymbolTable *
search_in_global(struct Compiler *self, struct String *name);
struct SymbolTable *
search_in_parent(struct Compiler *self,
                 struct SymbolTable *parent,
                 struct String *name,
                 bool check_visibility);
struct SymbolTable *
search_symb_from_parent(struct Compiler *self,
                        struct SymbolTable *parent,
                        struct Expr *expr);
struct SymbolTable *
search_symb_from_global(struct Compiler *self,
                        struct Expr *expr);
struct SymbolTable *
search_symb(struct Compiler *self,
            struct SymbolTable *parent,
            struct Expr *expr,
            bool *search_fun);
struct Tuple *
search_variable_in_local_values(struct Compiler *self,
                                struct Vec *local_values,
                                struct Expr *identifier);
struct SymbolTable *
get_symb_of_variable(struct Compiler *self,
                     struct Vec *local_values,
                     struct Expr *identifier,
                     struct Vec *body);
struct DataTypeSymbol *
infer_symb(struct Compiler *self, struct SymbolTable *expr_symb);
Str
get_compiler_defined_dt_name(Usize *count_compiler_defined_name);
struct DataTypeSymbol *
get_sub_data_type(struct DataTypeSymbol *dt, Usize *n);
struct ExprSymbol *
check_expression(struct Compiler *self,
                 struct Expr *expr,
                 struct SymbolTable *symb,
                 struct SymbolTable *parent,
                 Usize *count_compiler_defined_name,
				 struct DataTypeSymbol *defined_data_type);
void
check_fun_param(struct Compiler *self,
                struct SymbolTable *fun,
                struct SymbolTable *parent);
void
check_fun(struct Compiler *self,
          struct SymbolTable *fun,
          struct Vec *scopes,
          struct SymbolTable *parent);
void
check_constant(struct Compiler *self,
               struct SymbolTable *constant,
               struct Vec *scopes,
               struct SymbolTable *parent);
void
check_module(struct Compiler *self,
             struct SymbolTable *module,
             struct Vec *scopes,
             struct SymbolTable *parent);
void
check_alias(struct Compiler *self,
            struct SymbolTable *alias,
            struct Vec *scopes,
            struct SymbolTable *parent);
void
check_record(struct Compiler *self,
             struct SymbolTable *record,
             struct Vec *scopes,
             struct SymbolTable *parent);
void
check_record_obj(struct Compiler *self,
                 struct SymbolTable *record_obj,
                 struct Vec *scopes,
                 struct SymbolTable *parent);
void
check_enum(struct Compiler *self,
           struct SymbolTable *enum_,
           struct Vec *scopes,
           struct SymbolTable *parent);
void
check_enum_obj(struct Compiler *self,
               struct SymbolTable *enum_obj,
               struct Vec *scopes,
               struct SymbolTable *parent);
void
check_error(struct Compiler *self,
            struct SymbolTable *error,
            struct Vec *scopes,
            struct SymbolTable *parent);
void
check_class(struct Compiler *self,
            struct SymbolTable *class,
            struct Vec *scopes,
            struct SymbolTable *parent);
void
check_trait(struct Compiler *self,
            struct SymbolTable *trait,
            struct Vec *scopes,
            struct SymbolTable *parent);
void
check_property(struct Compiler *self,
               struct SymbolTable *prop,
               struct Vec *scopes,
               struct SymbolTable *parent);
void
check_method(struct Compiler *self,
             struct SymbolTable *method,
             struct Vec *scopes,
             struct SymbolTable *parent);
void
check_symb(struct Compiler *self,
           struct SymbolTable *symb,
           struct SymbolTable *parent);

#define SEARCH_IN_GLOBAL(v, name)                                             \
    for (Usize i = len__Vec(*v); i--;) {                                      \
        if (eq__String(                                                       \
              get_name__SymbolTable(((struct SymbolTable *)get__Vec(*v, i))), \
              name,                                                           \
              false)) {                                                       \
            check_symb(self, get__Vec(*v, i), NULL);                          \
            return get__Vec(*v, i);                                           \
        }                                                                     \
    }

#define SYMB_IS_TYPE(symb)                                                     \
    symb->kind == SymbolTableKindAlias || symb->kind == SymbolTableKindEnum || \
        symb->kind == SymbolTableKindRecord ||                                 \
        symb->kind == SymbolTableKindEnumObj ||                                \
        symb->kind == SymbolTableKindRecordObj ||                              \
        symb->kind == SymbolTableKindTrait ||                                  \
        symb->kind == SymbolTableKindClass                                     \
      ? true                                                                   \
      : false

struct Typecheck
__new__Typecheck(struct Parser parser)
{
    run__Parser(&parser);

    struct Typecheck self = {
        .parser = parser,
        .global = NEW(Scope,
                      parser.parse_block.scanner.src->file.name,
                      NULL,
                      ScopeItemKindGlobal,
                      -1, // not used
                      NULL,
                      NEW(Vec, sizeof(struct Tuple)),
                      NEW(Vec, sizeof(struct Tuple)),
                      NULL),
        .funs = NEW(Vec, sizeof(struct SymbolTable)),
        .consts = NEW(Vec, sizeof(struct SymbolTable)),
        .modules = NEW(Vec, sizeof(struct SymbolTable)),
        .aliases = NEW(Vec, sizeof(struct SymbolTable)),
        .records = NEW(Vec, sizeof(struct SymbolTable)),
        .enums = NEW(Vec, sizeof(struct SymbolTable)),
        .errors = NEW(Vec, sizeof(struct SymbolTable)),
        .classes = NEW(Vec, sizeof(struct SymbolTable)),
        .traits = NEW(Vec, sizeof(struct SymbolTable)),
        .records_obj = NEW(Vec, sizeof(struct SymbolTable)),
        .enums_obj = NEW(Vec, sizeof(struct SymbolTable)),
    };

    return self;
}

#define CHECK_SYMBS(v)                              \
    for (Usize i = len__Vec(*self->tc->funs); i--;) \
        check_symb(self, get__Vec(*self->tc->funs, i), NULL);

void
run__Typecheck(struct Compiler *self, struct Vec *primary_buffer)
{
    push_all_symbols(self);

    CHECK_SYMBS(self->tc->funs);
    CHECK_SYMBS(self->tc->consts);
    CHECK_SYMBS(self->tc->modules);
    CHECK_SYMBS(self->tc->aliases);
    CHECK_SYMBS(self->tc->records);
    CHECK_SYMBS(self->tc->enums);
    CHECK_SYMBS(self->tc->errors);
    CHECK_SYMBS(self->tc->classes);
    CHECK_SYMBS(self->tc->traits);
    CHECK_SYMBS(self->tc->records_obj);
    CHECK_SYMBS(self->tc->enums_obj);

    SUMMARY();
}

#define FREE_SYMB_VEC(v)                       \
    for (Usize i = len__Vec(*v); i--;)         \
        FREE(SymbolTableAll, get__Vec(*v, i)); \
    FREE(Vec, v);

void
__free__Typecheck(struct Typecheck self)
{
    FREE_SYMB_VEC(self.funs);
    FREE_SYMB_VEC(self.consts);
    FREE_SYMB_VEC(self.modules);
    FREE_SYMB_VEC(self.aliases);
    FREE_SYMB_VEC(self.records);
    FREE_SYMB_VEC(self.records_obj);
    FREE_SYMB_VEC(self.enums);
    FREE_SYMB_VEC(self.enums_obj);
    FREE_SYMB_VEC(self.errors);
    FREE_SYMB_VEC(self.classes);
    FREE_SYMB_VEC(self.traits);
    FREE(Parser, self.parser);
    FREE(Scope, self.global);
}

struct Compiler
__new__Compiler(struct Typecheck *tc)
{
    struct Compiler self = { .tc = tc,
                             .buffers = NEW(Vec, sizeof(struct Compiler)),
                             .builtins = NEW(Vec, sizeof(struct Builtin)),
                             .import_values = NEW(Vec, sizeof(struct Vec)) };

    return self;
}

void
__free__Compiler(struct Compiler self)
{
    // FREE(Typecheck, *self.tc);

    for (Usize i = len__Vec(*self.buffers); i--;) {
        FREE(Compiler, *(struct Compiler *)get__Vec(*self.buffers, i));
        free(get__Vec(*self.buffers, i));
    }

    FREE(Vec, self.buffers);

    for (Usize i = len__Vec(*self.builtins); i--;)
        FREE(BuiltinAll, get__Vec(*self.builtins, i));

    FREE(Vec, self.builtins);

    for (Usize i = len__Vec(*self.import_values); i--;) {
        if ((int)(UPtr)((struct Tuple *)get__Vec(*self.import_values, i))
              ->items[1])
            free(((struct Tuple *)get__Vec(*self.import_values, i))->items[0]);

        FREE(Tuple, get__Vec(*self.import_values, i));
    }

    FREE(Vec, self.import_values);
}

inline struct Diagnostic *
__new__DiagnosticWithErrTypecheck(struct Typecheck *self,
                                  struct LilyError *err,
                                  struct Location loc,
                                  struct String *detail_msg,
                                  struct Option *help)
{
    count_error += 1;
    return NEW(DiagnosticWithErr,
               err,
               loc,
               self->parser.parse_block.scanner.src->file,
               detail_msg,
               help);
}

inline struct Diagnostic *
__new__DiagnosticWithWarnTypecheck(struct Typecheck *self,
                                   struct LilyWarning *warn,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help)
{
    count_warning += 1;
    return NEW(DiagnosticWithWarn,
               warn,
               loc,
               self->parser.parse_block.scanner.src->file,
               detail_msg,
               help);
}

inline struct Diagnostic *
__new__DiagnosticWithNoteTypecheck(struct Typecheck *self,
                                   struct String *note,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help)
{
    return NEW(DiagnosticWithNote,
               note,
               loc,
               self->parser.parse_block.scanner.src->file,
               detail_msg,
               help);
}

void
push_all_symbols(struct Compiler *self)
{
    struct Decl *current = len__Vec(*self->tc->parser.decls) > 0
                             ? get__Vec(*self->tc->parser.decls, 0)
                             : NULL;

    while (pos < len__Vec(*self->tc->parser.decls) && current) {
        switch (current->kind) {
            case DeclKindFun: {
                push__Vec(self->tc->funs,
                          NEW(SymbolTableFun, NEW(FunSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.fun->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.fun->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindConstant: {
                push__Vec(
                  self->tc->consts,
                  NEW(SymbolTableConstant, NEW(ConstantSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.module->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.constant->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindModule: {
                push__Vec(self->tc->modules,
                          NEW(SymbolTableModule, NEW(ModuleSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.module->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.module->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindAlias: {
                push__Vec(self->tc->aliases,
                          NEW(SymbolTableAlias, NEW(AliasSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.alias->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.alias->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindRecord:
                if (current->value.record->is_object) {
                    struct Tuple *name = NEW(Tuple,
                                             2,
                                             current->value.record->name,
                                             SymbolTableKindRecordObj);

                    push__Vec(
                      self->tc->records_obj,
                      NEW(SymbolTableRecordObj, NEW(RecordObjSymbol, current)));
                    push__Vec(self->tc->global->names, name);

                    if (current->value.record->is_pub)
                        push__Vec(self->tc->global->public_names, name);
                } else {
                    struct Tuple *name =
                      NEW(Tuple,
                          2,
                          current->value.record->name,
                          getSymbolTableKindFromDecl[current->kind]);

                    push__Vec(
                      self->tc->records,
                      NEW(SymbolTableRecord, NEW(RecordSymbol, current)));
                    push__Vec(self->tc->global->names, name);

                    if (current->value.record->is_pub)
                        push__Vec(self->tc->global->public_names, name);
                }

                break;

            case DeclKindEnum: {
                if (current->value.enum_->is_object) {
                    struct Tuple *name = NEW(Tuple,
                                             2,
                                             current->value.enum_->name,
                                             SymbolTableKindEnumObj);

                    push__Vec(self->tc->enums_obj, name);
                    push__Vec(self->tc->global->names, name);

                    if (current->value.enum_->is_pub)
                        push__Vec(self->tc->global->public_names, name);
                } else {
                    struct Tuple *name =
                      NEW(Tuple,
                          2,
                          current->value.enum_->name,
                          getSymbolTableKindFromDecl[current->kind]);

                    push__Vec(self->tc->enums_obj, name);
                    push__Vec(self->tc->global->names, name);

                    if (current->value.enum_->is_pub)
                        push__Vec(self->tc->global->public_names, name);
                }

                break;
            }
            case DeclKindError: {
                push__Vec(self->tc->errors,
                          NEW(SymbolTableError, NEW(ErrorSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.error->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.error->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindClass: {
                push__Vec(self->tc->classes,
                          NEW(SymbolTableClass, NEW(ClassSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.class->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.class->is_pub)
                    push__Vec(self->tc->global->public_names, name);

                break;
            }
            case DeclKindTrait: {
                push__Vec(self->tc->traits,
                          NEW(SymbolTableTrait, NEW(TraitSymbol, current)));

                struct Tuple *name =
                  NEW(Tuple,
                      2,
                      current->value.trait->name,
                      getSymbolTableKindFromDecl[current->kind]);

                push__Vec(self->tc->global->names, name);

                if (current->value.trait->is_pub)
                    push__Vec(self->tc->global->names, name);

                break;
            }
            case DeclKindTag:
            case DeclKindImport:
                break;
        }

        current = ++pos < len__Vec(*self->tc->parser.decls)
                    ? get__Vec(*self->tc->parser.decls, pos)
                    : NULL;
    }
}

struct SymbolTable *
get_symbol_from_idx(struct Compiler *self, struct SymbolTable *symb, Usize idx)
{
    switch (symb->kind) {
        case SymbolTableKindModule:
            check_symb(self, get__Vec(*symb->value.module->body, idx), symb);
            return get__Vec(*symb->value.module->body, idx);
        case SymbolTableKindRecord:
            check_symb(self, get__Vec(*symb->value.record->fields, idx), symb);
            return get__Vec(*symb->value.record->fields, idx);
        case SymbolTableKindEnum:
            check_symb(self, get__Vec(*symb->value.enum_->variants, idx), symb);
            return get__Vec(*symb->value.enum_->variants, idx);
        case SymbolTableKindClass:
            check_symb(self, get__Vec(*symb->value.class->body, idx), symb);
            return get__Vec(*symb->value.class->body, idx);
        case SymbolTableKindRecordObj:
            check_symb(
              self, get__Vec(*symb->value.record_obj->fields, idx), symb);
            return get__Vec(*symb->value.record_obj->fields, idx);
        case SymbolTableKindEnumObj:
            check_symb(
              self, get__Vec(*symb->value.enum_obj->variants, idx), symb);
            return get__Vec(*symb->value.enum_obj->variants, idx);
        default:
            UNREACHABLE("impossible to get symbol from this symbol");
    }
}

struct SymbolTable *
search_in_module(struct Compiler *self,
                 struct SymbolTable *symb,
                 struct String *name,
                 bool check_visibility)
{
    assert(symb->kind == SymbolTableKindModule && "expected module");

    for (Usize i = len__Vec(*symb->value.module->scope->names); i--;) {
        if (eq__String(CAST(get__Vec(*symb->value.module->scope->names, i),
                            struct Tuple *)
                         ->items[0],
                       name,
                       false)) {
            check_symb(self, get__Vec(*symb->value.module->body, i), symb);

            struct SymbolTable *res = get__Vec(*symb->value.module->body, i);

            if (check_visibility)
                if (get_visibility__SymbolTable(res) != VisibilityPublic) {
                    assert(0 && "error: impossible to get because is private");
                }

            return res;
        }
    }

    return NULL;
}

struct SymbolTable *
search_in_class(struct Compiler *self,
                struct SymbolTable *symb,
                struct String *name,
                bool check_visibility)
{
    assert(symb->kind == SymbolTableKindClass && "expected class");

    for (Usize i = len__Vec(*symb->value.class->scope->names); i--;) {
        if (eq__String(CAST(get__Vec(*symb->value.class->scope->names, i),
                            struct Tuple *)
                         ->items[0],
                       name,
                       false)) {
            check_symb(self, get__Vec(*symb->value.class->body, i), symb);

            struct SymbolTable *res = get__Vec(*symb->value.class->body, i);

            if (check_visibility)
                if (get_visibility__SymbolTable(res) != VisibilityPublic) {
                    assert(0 && "error: impossible to get because is private");
                }

            return res;
        }
    }

    return NULL;
}

struct SymbolTable *
search_in_record(struct Compiler *self,
                 struct SymbolTable *symb,
                 struct String *name)
{
    assert(symb->kind == SymbolTableKindRecord && "expected record");

    for (Usize i = len__Vec(*symb->value.record->scope->names); i--;) {
        if (eq__String(CAST(get__Vec(*symb->value.record->scope->names, i),
                            struct Tuple *)
                         ->items[0],
                       name,
                       false)) {
            check_symb(self, get__Vec(*symb->value.record->fields, i), symb);

            struct SymbolTable *res = get__Vec(*symb->value.record->fields, i);

            if (get_visibility__SymbolTable(res) != VisibilityPublic) {
                assert(0 && "error: impossible to get because is private");
            }

            return res;
        }
    }

    return NULL;
}

struct SymbolTable *
search_in_enum(struct Compiler *self,
               struct SymbolTable *symb,
               struct String *name)
{
    assert(symb->kind == SymbolTableKindEnum && "expected enum");

    for (Usize i = len__Vec(*symb->value.enum_->scope->names); i--;) {
        if (eq__String(CAST(get__Vec(*symb->value.enum_->scope->names, i),
                            struct Tuple *)
                         ->items[0],
                       name,
                       false)) {
            check_symb(self, get__Vec(*symb->value.enum_->variants, i), symb);

            return get__Vec(*symb->value.enum_->variants, i);
        }
    }

    return NULL;
}

struct SymbolTable *
search_in_record_obj(struct Compiler *self,
                     struct SymbolTable *symb,
                     struct String *name,
                     bool check_visibility)
{
    assert(symb->kind == SymbolTableKindRecordObj && "expected record object");

    for (Usize i = len__Vec(*symb->value.record_obj->scope->names); i--;) {
        struct Tuple *t = get__Vec(*symb->value.record_obj->scope->names, i);

        if (eq__String(t->items[0], name, false)) {
            // NOTE: the first pushed elements in
            // symb->value.record_obj->scope->names are fields.

			struct SymbolTable *res = NULL;

			if ((enum ScopeItemKind)(UPtr)t->items[1] == ScopeItemKindFun) {
				check_symb(
                  self,
                  get__Vec(*symb->value.record_obj->attached,
                           i - len__Vec(*symb->value.record_obj->fields)), symb);

				res = get__Vec(*symb->value.record_obj->attached, i - len__Vec(*symb->value.record_obj->fields));
			} else {
                check_symb(
                  self, get__Vec(*symb->value.record_obj->fields, i), symb);

				res = get__Vec(*symb->value.record_obj->fields, i);
			}

            if (check_visibility)
                if (get_visibility__SymbolTable(res) != VisibilityPublic) {
                    assert(0 && "error: impossible to get because is private");
                }

            return res;
        }
    }

    return NULL;
}

struct SymbolTable *
search_in_enum_obj(struct Compiler *self,
                   struct SymbolTable *symb,
                   struct String *name,
                   bool check_visibility)
{
    assert(symb->kind == SymbolTableKindEnumObj && "expected enum object");

    for (Usize i = len__Vec(*symb->value.enum_obj->scope->names); i--;) {
        struct Tuple *t = get__Vec(*symb->value.enum_obj->scope->names, i);

        if (eq__String(t->items[0], name, false)) {
            // NOTE: the first pushed elements in
            // symb->value.enum_obj->scope->names are variants.

			struct SymbolTable *res = NULL;

			if ((enum ScopeItemKind)(UPtr)t->items[1] == ScopeItemKindFun) {
				check_symb(
                  self,
                  get__Vec(*symb->value.enum_obj->attached,
                           i - len__Vec(*symb->value.enum_obj->variants)), symb);

				res = get__Vec(*symb->value.enum_obj->attached, i - len__Vec(*symb->value.enum_obj->variants));
			} else {
                check_symb(
                  self, get__Vec(*symb->value.enum_obj->variants, i), symb);

				res = get__Vec(*symb->value.enum_obj->variants, i);
			}

            if (check_visibility)
                if (get_visibility__SymbolTable(res) != VisibilityPublic) {
                    assert(0 && "error: impossible to get because is private");
                }

            return res;
        }
    }

    return NULL;
}

struct SymbolTable *
search_from_scopes(struct Compiler *self, struct Vec *scopes, Usize *idx)
{
#define SEARCH()                                                             \
    if (current->kind == SymbolTableKindModule)                              \
        current =                                                            \
          search_in_module(self,                                             \
                           current,                                          \
                           CAST(get__Vec(*scopes, i), struct Scope *)->name, \
                           true);                                            \
    else {                                                                   \
        assert(0 && "error: this path is unusable");                         \
    }

    if (idx) {
        assert(
          ((Usize)(UPtr)idx < len__Vec(*scopes) && (Usize)(UPtr)idx >= 0) &&
          "cannot pass an index greater than length of scopes or an index "
          "equal to 0 or less");

        struct SymbolTable *current =
          len__Vec(*scopes) > 0 && (Usize)(UPtr)idx >= 0
            ? search_in_global(self,
                               CAST(get__Vec(*scopes, 0), struct Scope *)->name)
            : NULL;

        for (Usize i = 1; i < (Usize)(UPtr)idx; i++) {
            SEARCH();
        }

        return current;
    } else {
        struct SymbolTable *current =
          len__Vec(*scopes) > 0
            ? search_in_global(self,
                               CAST(get__Vec(*scopes, 0), struct Scope *)->name)
            : NULL;

        for (Usize i = 1; i < len__Vec(*scopes); i++) {
            SEARCH();
        }

        return current;
    }
}

struct SymbolTable *
search_in_global(struct Compiler *self, struct String *name)
{
    for (Usize i = len__Vec(*self->tc->global->names); i--;) {
        if (eq__String(
              CAST(get__Vec(*self->tc->global->names, i), struct Tuple *)
                ->items[0],
              name,
              false)) {
            switch ((enum ScopeItemKind)(UPtr)CAST(
                      get__Vec(*self->tc->global->names, i), struct Tuple *)
                      ->items[1]) {
                case ScopeItemKindRecord:
                    SEARCH_IN_GLOBAL(self->tc->records, name);
                case ScopeItemKindAlias:
                    SEARCH_IN_GLOBAL(self->tc->aliases, name);
                case ScopeItemKindEnum:
                    SEARCH_IN_GLOBAL(self->tc->enums, name);
                case ScopeItemKindRecordObj:
                    SEARCH_IN_GLOBAL(self->tc->records, name);
                case ScopeItemKindEnumObj:
                    SEARCH_IN_GLOBAL(self->tc->enums_obj, name);
                case ScopeItemKindConstant:
                    SEARCH_IN_GLOBAL(self->tc->consts, name);
                case ScopeItemKindFun:
                    SEARCH_IN_GLOBAL(self->tc->funs, name);
                case ScopeItemKindError:
                    SEARCH_IN_GLOBAL(self->tc->errors, name);
                case ScopeItemKindClass:
                    SEARCH_IN_GLOBAL(self->tc->classes, name);
                case ScopeItemKindModule:
                    SEARCH_IN_GLOBAL(self->tc->modules, name);
                case ScopeItemKindTrait:
                    SEARCH_IN_GLOBAL(self->tc->traits, name);
                default:
                    UNREACHABLE("");
            }
        }
    }

    assert(0 && "error: not found");
}

struct SymbolTable *
search_in_parent(struct Compiler *self,
                 struct SymbolTable *parent,
                 struct String *name,
                 bool check_visibility)
{
    switch (parent->kind) {
        case SymbolTableKindModule:
            return search_in_module(self, parent, name, check_visibility);
        case SymbolTableKindClass:
            return search_in_class(self, parent, name, check_visibility);
        case SymbolTableKindRecord:
            return search_in_record(self, parent, name);
        case SymbolTableKindEnum:
            return search_in_enum(self, parent, name);
        case SymbolTableKindRecordObj:
            return search_in_record_obj(
              self, parent, name, check_visibility);
        case SymbolTableKindEnumObj:
            // assert(search_fun && "expected search_fun != NULL");

            return search_in_enum_obj(
              self, parent, name, check_visibility);
        default:
            UNREACHABLE("it's impossible to search symbol in this parent");
    }
}

struct SymbolTable *
search_symb_from_parent(struct Compiler *self,
                        struct SymbolTable *parent,
                        struct Expr *expr)
{
    switch (expr->kind) {
        case ExprKindIdentifier:
            return search_in_parent(
              self, parent, expr->value.identifier, false);
        case ExprKindIdentifierAccess: {
            struct SymbolTable *current = parent;
            bool check_visibility = true;
            Usize i = 0;

            for (; i < len__Vec(*expr->value.identifier_access) && current;
                 i++) {
                struct Expr *item = get__Vec(*expr->value.identifier_access, i);

                assert(item->kind == ExprKindIdentifier &&
                       "expected identifier");

                current = i == len__Vec(*expr->value.identifier_access) - 1
                            ? search_in_parent(self,
                                               current,
                                               item->value.identifier,
                                               check_visibility)
                            : search_in_parent(self,
                                               current,
                                               item->value.identifier,
                                               check_visibility);

                if (check_visibility)
                    check_visibility = false;
            }

            if (!current && i > 0) {
                assert(0 && "error: symbol not found");
            }

            return current;
        }
        default:
            UNREACHABLE("not expected expression");
    }
}

struct SymbolTable *
search_symb_from_global(struct Compiler *self,
                        struct Expr *expr)
{
    switch (expr->kind) {
        case ExprKindIdentifier: {
            struct SymbolTable *global =
              search_in_global(self, expr->value.identifier);

            if (global)
                return global;

            break;
        }
        case ExprKindIdentifierAccess: {
            struct SymbolTable *current = search_in_global(
              self, get__Vec(*expr->value.identifier_access, 0));

            for (Usize i = 1; i < len__Vec(*expr->value.identifier_access);
                 i++) {
                struct Expr *id = get__Vec(*expr->value.identifier_access, i);

                assert(id->kind == ExprKindIdentifier && "expected identifier");

                current =
                  i == len__Vec(*expr->value.identifier_access) - 1
                    ? search_in_parent(
                        self, current, id->value.identifier, true)
                    : search_in_parent(
                        self, current, id->value.identifier, true);
            }

            if (current)
                return current;

            break;
        }
        default:
            UNREACHABLE("")
    }

    assert(0 && "error: symbol not found");

    return NULL;
}

struct SymbolTable *
search_symb(struct Compiler *self,
            struct SymbolTable *parent,
            struct Expr *expr,
            bool *search_fun)
{
    struct SymbolTable *res = NULL;

    if (!parent)
        goto search_in_global;

    struct SymbolTable *current = NULL;
    struct Vec *scopes = get_scope__SymbolTable(parent)->scopes;

    for (Usize i = 0; i < len__Vec(*scopes); i++) {
        current = i == 0
                    ? parent
                    : search_from_scopes(
                        self, scopes, (Usize *)(i + 1) - len__Vec(*scopes));
        res = search_symb_from_parent(self, current, expr);
    }

    if (!res)
        goto search_in_global;
    else {
        assert(0 && "error: symbol not found");
    }

    if (res == NULL) {
    search_in_global : {
		res = search_symb_from_global(self, expr);
    }
    }

    return res;
}

// local_values = struct Vec<struct Vec<struct Tuple<struct Scope*, struct
// Vec<VariableAddr*>*>*>* struct Tuple* = struct Tuple<struct Scope*, struct
// Vec<VariableAddr*>*>*
struct Tuple *
search_variable_in_local_values(struct Compiler *self,
                                struct Vec *local_values,
                                struct Expr *identifier)
{
    switch (identifier->kind) {
        case ExprKindIdentifier:
            for (Usize i = len__Vec(*local_values); i--;) {
                struct Vec *current = get__Vec(*local_values, i);

                for (Usize j = len__Vec(*current); j--;) {
                    if (eq__String(
                          CAST(CAST(get__Vec(*current, j), struct Tuple *)
                                 ->items[0],
                               struct Scope *)
                            ->name,
                          identifier->value.identifier,
                          false))
                        return get__Vec(*current, j);
                }
            }

            return NULL;
        case ExprKindIdentifierAccess:
            // Make sure it doesn't start with `self`.

            for (Usize i = len__Vec(*local_values); i--;) {
                struct Vec *current = get__Vec(*local_values, i);

                for (Usize j = len__Vec(*current); j--;) {
                    if (eq__String(
                          CAST(CAST(get__Vec(*current, j), struct Tuple *)
                                 ->items[0],
                               struct Scope *)
                            ->name,
                          CAST(
                            get__Vec(*identifier->value.identifier_access, 0),
                            struct Expr *)
                            ->value.identifier,
                          false)) {
                        return get__Vec(*current, j);
                    }
                }
            }

            return NULL;
        default:
            UNREACHABLE("expected only identifier or identifier access");
    }
}

// ex:
// if <cond> do // -> {0, 0}
//   if <cond> do // -> {0, 0}
//    if <cond> do // -> {0, 0}
//     a := 3 // -> {0, NULL}
//    end
//   end
// end
//
// a -> Vec<VariableAddr*> = [{0, 0}, {0, 0}, {0, 0}, {0, NULL}]
//
// ex:
// while <cond> do // -> {0, NULL}
//  while <cond> do // -> {0, NULL}
//    try do
//    catch err do // -> {0, 1}
//      a := 3 // -> {0, NULL}
//    end
//  end
// end
//
// a -> Vec<Usize*> = [{0, NULL}, {0, NULL}, {0, 1}, {0, NULL}]
struct SymbolTable *
get_symb_of_variable(struct Compiler *self,
                     struct Vec *local_values,
                     struct Expr *identifier,
                     struct Vec *body)
{
    struct Tuple *var =
      search_variable_in_local_values(self, local_values, identifier);

    if (!var)
        return NULL;

    struct Vec *addr = var->items[1];
    struct SymbolTable *current =
      get__Vec(*body, CAST(get__Vec(*addr, 0), struct VariableAddr *)->id);
    Usize id_sec = CAST(get__Vec(*addr, 0), struct VariableAddr *)->id_sec;

    for (Usize i = 1; i < len__Vec(*addr); i++) {
        struct VariableAddr *item = get__Vec(*addr, i);

        switch (current->kind) {
            case SymbolTableKindStmt:
                switch (current->value.stmt.kind) {
                    case StmtKindIf:
                        if (id_sec == 0)
                            current =
                              get__Vec(*current->value.stmt.value.if_.if_->body,
                                       item->id);
                        else if (id_sec <
                                 len__Vec(
                                   *current->value.stmt.value.if_.elif)) {
                            struct Vec *body =
                              CAST(get__Vec(*current->value.stmt.value.if_.elif,
                                            item->id_sec),
                                   struct IfBranchSymbol *)
                                ->body;

                            current = get__Vec(*body, item->id);
                        } else
                            current = get__Vec(
                              *current->value.stmt.value.if_.else_->body,
                              item->id);

                        break;
                    case StmtKindFor:
                    case StmtKindWhile:
                        current = get__Vec(
                          *current->value.stmt.value.while_.body, item->id);

                        break;
                    case StmtKindTry:
                        if (id_sec == 0) {
                            current =
                              get__Vec(*current->value.stmt.value.try.try_body,
                                       item->id);
                        } else if (id_sec == 1) {
                            current = get__Vec(
                              *current->value.stmt.value.try.catch_body,
                              item->id);
                        } else
                            UNREACHABLE(
                              "impossible to get an id_sec > 1 on try stmt");
                        break;
                    case StmtKindMatch: {
                        struct Tuple *pattern = get__Vec(
                          *current->value.stmt.value.match.patterns, id_sec);
                        current = get__Vec(
                          *CAST(
                            CAST(pattern->items[2], struct Expr *)->value.block,
                            struct Vec *),
                          item->id);
                        break;
                    }
                    default:
                        UNREACHABLE("this stmt is not expected");
                }
                break;
            case SymbolTableKindExpr:
                break;
            default:
                UNREACHABLE("it's impossible to get this variant");
        }

        id_sec = item->id_sec;
    }

    assert(current->value.expr->kind == ExprKindVariable &&
           "expected variable");

    return current;
}

struct DataTypeSymbol *
infer_symb(struct Compiler *self, struct SymbolTable *expr_symb)
{
    struct String *name = NULL;
    struct Scope *scope = get_scope__SymbolTable(expr_symb);
    struct Vec *generic_params = NULL;

    switch (expr_symb->kind) {
        case SymbolTableKindEnum:
        case SymbolTableKindRecord:
        case SymbolTableKindEnumObj:
        case SymbolTableKindRecordObj:
        case SymbolTableKindClass:
        case SymbolTableKindTrait:
            name = get_name__SymbolTable(expr_symb);
            break;
        default:
            break;
    }

    return NEW(DataTypeSymbolCustom, generic_params, name, scope);
}

Str
get_compiler_defined_dt_name(Usize *count_compiler_defined_name)
{
    char c = (char)(*count_compiler_defined_name % 26 + 65);
    Usize n = (Usize)floor((double)*count_compiler_defined_name / 26);
    Usize size = n > 0 ? snprintf(NULL, 0, "%c%zu", c, n) + 1
                       : snprintf(NULL, 0, "%c", c) + 1;
    Str name = malloc(size);

    if (n > 0)
        snprintf(name, size, "%c%zu", c, n);
    else
        snprintf(name, size, "%c", c);

    ++(*count_compiler_defined_name);

    return name;
}

#define CHECK_IDENTIFIER(name)                                               \
    struct SymbolTable *var = NULL;                                          \
                                                                             \
    if (symb->kind == SymbolTableKindFun) /* Search variable in fun body */  \
        var = get_symb_of_variable(                                          \
          self, symb->value.fun->local_values, expr, symb->value.fun->body); \
    else if (symb->kind ==                                                   \
             SymbolTableKindMethod) /* Search variable in method body */     \
        var = get_symb_of_variable(self,                                     \
                                   symb->value.method->local_values,         \
                                   expr,                                     \
                                   symb->value.method->body);                \
                                                                             \
    if (var)                                                                 \
        return NEW(ExprSymbol##name,                                         \
                   *expr,                                                    \
                   get_scope__SymbolTable(var),                              \
                   infer_symb(self, var));                                   \
    else {                                                                   \
        struct SymbolTable *value = NULL;                                    \
                                                                             \
        if (parent)                                                          \
            value = search_symb_from_parent(self, parent, expr); \
        else                                                                 \
            value = search_symb_from_global(self, expr);         \
                                                                             \
        if (value)                                                           \
            return NEW(ExprSymbol##name,                                     \
                       *expr,                                                \
                       get_scope__SymbolTable(value),                        \
                       infer_symb(self, value));                             \
                                                                             \
        return NULL;                                                         \
    }

struct DataTypeSymbol *
get_sub_data_type(struct DataTypeSymbol *dt, Usize *n)
{
	if (!dt)
		return NULL;

	switch (dt->kind) {
		case DataTypeKindSelf:
		case DataTypeKindStr:
		case DataTypeKindBitStr:
		case DataTypeKindChar:
		case DataTypeKindBitChar:
		case DataTypeKindI8:
		case DataTypeKindI16:
		case DataTypeKindI32:
		case DataTypeKindI64:
		case DataTypeKindI128:
		case DataTypeKindU8:
		case DataTypeKindU16:
		case DataTypeKindU32:
		case DataTypeKindU64:
		case DataTypeKindU128:
		case DataTypeKindF32:
		case DataTypeKindF64:
		case DataTypeKindBool:
		case DataTypeKindIsize:
		case DataTypeKindUsize:
		case DataTypeKindNever:
		case DataTypeKindAny:
		case DataTypeKindUnit:
		case DataTypeKindLambda:
		case DataTypeKindCustom:
		case DataTypeKindCompilerDefined:
			return NULL;
		case DataTypeKindPtr:
			return dt->value.ptr;
		case DataTypeKindRef:
			return dt->value.ref;
		case DataTypeKindOptional:
			return dt->value.optional;
		case DataTypeKindException:
			return dt->value.exception;
		case DataTypeKindMut:
			return dt->value.mut;
		case DataTypeKindArray:
			return dt->value.array->items[0];
		case DataTypeKindTuple:
			assert(n && "n != NULL when it's use for get sub data type of tuple");
			return get__Vec(*dt->value.tuple, (Usize)(UPtr)n);
	}
}

struct ExprSymbol *
check_expression(struct Compiler *self,
                 struct Expr *expr,
                 struct SymbolTable *symb,
                 struct SymbolTable *parent,
                 Usize *count_compiler_defined_name,
				 struct DataTypeSymbol *defined_data_type)
{
    switch (expr->kind) {
        case ExprKindIf:
            TODO("check if branch");
            break;
        case ExprKindTry: {
            struct ExprSymbol *try_expr =
              check_expression(self,
                               expr->value.try,
                               symb,
                               parent,
                               count_compiler_defined_name,
							   get_sub_data_type(defined_data_type, NULL));

            if (try_expr->data_type->kind != DataTypeKindOptional) {
                assert(0 &&
                       "error: expected expression with optional data type");
            }

            return NEW(
              ExprSymbolTry,
              *expr,
              try_expr,
              copy__DataTypeSymbol(try_expr->data_type->value.optional));
        }
        case ExprKindRef: {
            struct ExprSymbol *ref_expr =
              check_expression(self,
                               expr->value.ref,
                               symb,
                               parent,
                               count_compiler_defined_name,
							   get_sub_data_type(defined_data_type, NULL));

            return NEW(ExprSymbolRef,
                       *expr,
                       ref_expr,
                       NEW(DataTypeSymbolRef,
                           copy__DataTypeSymbol(ref_expr->data_type)));
        }
        case ExprKindNil:
            return NEW(ExprSymbol,
                       expr,
                       NEW(DataTypeSymbolPtr,
                           NEW(DataTypeSymbolCompilerDefined,
                               NEW(CompilerDefinedDataType,
                                   get_compiler_defined_dt_name(
                                     count_compiler_defined_name),
                                   false))));
        case ExprKindSelf:
            break;
        case ExprKindNone:
            return NEW(ExprSymbol,
                       expr,
                       NEW(DataTypeSymbolOptional,
                           NEW(DataTypeSymbolCompilerDefined,
                               NEW(CompilerDefinedDataType,
                                   get_compiler_defined_dt_name(
                                     count_compiler_defined_name),
                                   false))));
        case ExprKindTuple: {
            struct Vec *tuple = NEW(Vec, sizeof(struct ExprSymbol));
            struct Vec *dts = NEW(Vec, sizeof(struct DataTypeSymbol));

            for (Usize i = 0; i < len__Vec(*expr->value.tuple); i++) {
                push__Vec(tuple,
                          check_expression(self,
                                           get__Vec(*expr->value.tuple, i),
                                           symb,
                                           parent,
                                           count_compiler_defined_name,
										   get_sub_data_type(defined_data_type, (Usize*)i)));
                push__Vec(
                  dts,
                  copy__DataTypeSymbol(
                    CAST(get__Vec(*tuple, i), struct ExprSymbol *)->data_type));
            }

            return NEW(
              ExprSymbolTuple, *expr, tuple, NEW(DataTypeSymbolTuple, dts));
        }
        case ExprKindArray: {
            struct Vec *array = NEW(Vec, sizeof(struct ExprSymbol));

            if (len__Vec(*expr->value.array) > 0) {
                push__Vec(array,
                          check_expression(self,
                                           get__Vec(*expr->value.array, 0),
                                           symb,
                                           parent,
                                           count_compiler_defined_name,
										   get_sub_data_type(defined_data_type, NULL)));

                struct DataTypeSymbol *dt =
                  CAST(get__Vec(*array, 0), struct ExprSymbol *)->data_type;

                for (Usize i = 1; i < len__Vec(*expr->value.array); i++) {
                    push__Vec(array,
                              check_expression(self,
                                               get__Vec(*expr->value.array, i),
                                               symb,
                                               parent,
                                               count_compiler_defined_name,
											   get_sub_data_type(defined_data_type, NULL)));

                    if (!eq__DataTypeSymbol(
                          dt,
                          CAST(get__Vec(*array, i), struct ExprSymbol *)
                            ->data_type)) {
                        assert(0 && "error: mismatched data type");
                    }
                }

                return NEW(
                  ExprSymbolArray,
                  *expr,
                  array,
                  NEW(DataTypeSymbolArray, copy__DataTypeSymbol(dt), NULL));
            } else {
                return NEW(ExprSymbolArray,
                           *expr,
                           array,
                           NEW(DataTypeSymbolArray,
                               NEW(DataTypeSymbolCompilerDefined,
                                   NEW(CompilerDefinedDataType,
                                       get_compiler_defined_dt_name(
                                         count_compiler_defined_name),
                                       false)),
                               NULL));
            }
        }
        case ExprKindBlock:
            TODO("check block");
            break;
        case ExprKindUndef:
            return NEW(
              ExprSymbol,
              expr,
              NEW(DataTypeSymbolCompilerDefined,
                  NEW(CompilerDefinedDataType,
                      get_compiler_defined_dt_name(count_compiler_defined_name),
                      false)));
        case ExprKindLambda:
            break;
        case ExprKindUnaryOp:
            break;
        case ExprKindFunCall:
            break;
        case ExprKindVariant:
            break;
        case ExprKindLiteral:
            break;
        case ExprKindBinaryOp:
            break;
        case ExprKindWildcard:
            return NEW(ExprSymbol, expr, NULL);
        case ExprKindVariable: {
            Println("variable");
            TODO("check variable");
            break;
        }
        case ExprKindGrouping:
            return check_expression(self,
                                    expr->value.grouping,
                                    symb,
                                    parent,
                                    count_compiler_defined_name,
									get_sub_data_type(defined_data_type, NULL));
        case ExprKindRecordCall: {
            struct ExprSymbol *id =
              check_expression(self,
                               expr->value.record_call.id,
                               symb,
                               parent,
                               count_compiler_defined_name,
							   defined_data_type);
            struct Scope *scope = get_scope__ExprSymbol(id);

            if (scope && (scope->item_kind == ScopeItemKindRecord ||
                          scope->item_kind == ScopeItemKindRecordObj)) {
                struct SymbolTable *symb =
                  search_from_scopes(self, scope->scopes, NULL);

                switch (symb->kind) {
                    case SymbolTableKindRecord:
                        TODO("record");
                        break;
                    case SymbolTableKindRecordObj:
                        TODO("record obj");
                        break;
                    default:
                        UNREACHABLE("");
                }
            } else if (!scope) {
                assert(0 && "error: symbol not found");
            } else if (scope->item_kind != ScopeItemKindRecord &&
                       scope->item_kind != ScopeItemKindRecordObj) {
                assert(0 && "error: expected record");
            }

            break;
        }
        case ExprKindIdentifier: {
            CHECK_IDENTIFIER(Identifier);
        }
        case ExprKindArrayAccess: {
            struct ExprSymbol *value =
              check_expression(self,
                               expr->value.array_access.id,
                               symb,
                               parent,
                               count_compiler_defined_name,
							   defined_data_type);
            struct DataTypeSymbol *current = value->data_type;
            struct Vec *ids = NEW(Vec, sizeof(struct ExprSymbol));

            for (Usize i = 0; i < len__Vec(*expr->value.array_access.access);
                 i++) {
                if (current->kind == DataTypeKindArray) {
                    current = current->value.array->items[0];
                    push__Vec(ids,
                              check_expression(
                                self,
                                get__Vec(*expr->value.array_access.access, i),
                                symb,
                                parent,
                                count_compiler_defined_name, get_sub_data_type(defined_data_type, NULL)));
                } else {
                    assert(0 && "error: expected array");
                }
            }

            return NEW(ExprSymbolArrayAccess,
                       *expr,
                       NEW(Tuple, 2, value, ids),
                       copy__DataTypeSymbol(current));
        }
        case ExprKindTupleAccess: {
            struct ExprSymbol *value =
              check_expression(self,
                               expr->value.tuple_access.id,
                               symb,
                               parent,
                               count_compiler_defined_name, defined_data_type);

            struct DataTypeSymbol *current = value->data_type;

            for (Usize i = 0; i < len__Vec(*expr->value.tuple_access.access);
                 i++) {
                if (expr->value.tuple_access.id->value.literal.kind ==
                    LiteralKindInt32) {
                    if (current->kind == DataTypeKindTuple) {
                        current = get__Vec(*current->value.tuple,
                                           expr->value.tuple_access.id->value
                                             .literal.value.int32);
                    } else {
                        assert(0 && "error: expected tuple");
                    }
                } else {
                    assert(0 && "error: expected literal Int32");
                }
            }

            return NEW(
              ExprSymbolTupleAccess,
              *expr,
              NEW(Tuple,
                  2,
                  value,
                  expr->value.tuple_access.id->value.literal.value.int32),
              copy__DataTypeSymbol(current));
        }
        case ExprKindDereference: {
			struct DataTypeSymbol *dt = defined_data_type ? NEW(DataTypeSymbolPtr, defined_data_type) : NULL;
            struct ExprSymbol *expr_dereference =
              check_expression(self,
                               expr->value.dereference,
                               symb,
                               parent,
                               count_compiler_defined_name,
							   dt);

            if (expr_dereference->data_type->kind != DataTypeKindPtr) {
                assert(0 && "error: expected pointer data type");
            }

			if (dt)
				free(dt);

            return NEW(
              ExprSymbolDereference,
              *expr,
              expr_dereference,
              copy__DataTypeSymbol(expr_dereference->data_type->value.ptr));
        }
        case ExprKindQuestionMark:
            break;
        case ExprKindGlobalAccess: {
            struct SymbolTable *value =
              search_symb_from_global(self, expr);

            return NEW(ExprSymbolGlobalAccess,
                       *expr,
                       get_scope__SymbolTable(value),
                       infer_symb(self, value));
        }
        case ExprKindIdentifierAccess: {
            CHECK_IDENTIFIER(IdentifierAccess);
        }
        case ExprKindPropertyAccessInit:
            if (symb->kind == SymbolTableKindMethod) {
                TODO("property access init");
            } else {
                assert(
                  0 &&
                  "error: unexpected property access init outside of method");
            }
            break;
    }
}

struct DataTypeSymbol *
check_data_type_custom(struct Compiler *self,
                       struct DataType *dt,
                       struct SymbolTable *last)
{
    struct Vec *names = dt->value.custom->items[0];
    struct SymbolTable *current = last; // Search in this SymbolTable.
    Usize count_scope = 0;
    bool in_primary_scope = true;
    struct Vec *generic_params = NULL;

search_in_the_prev_scope : {
    in_primary_scope = true;

    for (Usize i = 0;
         i < len__Vec(*names) &&
         count_scope < len__Vec(*last->value.module->scope->scopes);
         i++) {
        // Search in last module
        if (current->kind == SymbolTableKindModule)
            current = search_in_module(self,
                                       current,
                                       get__Vec(*names, i),
                                       in_primary_scope ? false : true);

        if (i == 0)
            in_primary_scope = false;

        if (!current && i == 0) {
            ++count_scope;

            if (count_scope < len__Vec(*last->value.module->scope->scopes)) {
                current = search_from_scopes(
                  self,
                  last->value.module->scope->scopes,
                  (Usize *)count_scope + 1 -
                    len__Vec(*last->value.module->scope->scopes));

                goto search_in_the_prev_scope;
            } else if (i == 0)
                break;
        } else if (SYMB_IS_TYPE(current)) {
            if (len__Vec(*names) != i + 1) {
                assert(0 && "error: this path to data type is unusable");
            }
        } else {
            assert(0 && "error: expected class, record, enum, alias, "
                        "record object, enum object or module in the path");
        }
    }
}

    if (current) {
        if (SYMB_IS_TYPE(current)) {
            return NEW(DataTypeSymbolCustom,
                       generic_params,
                       dt->value.custom->items[0],
                       get_scope__SymbolTable(current));
        } else {
            assert(0 && "error: expected type");
        }
    } else
        return check_data_type_custom_in_global(
          self, dt); // If data type is not found in the previous scope, then
                     // you search in global.
}

struct DataTypeSymbol *
check_data_type_custom_in_global(struct Compiler *self, struct DataType *dt)
{
    struct Vec *names = dt->value.custom->items[0];
    struct SymbolTable *current = search_in_global(
      self, get__Vec(*names, 0)); // Search in this SymbolTable.
    struct Vec *generic_params = NULL;

    for (Usize i = 1; i < len__Vec(*names); i++) {
        if (current->kind == SymbolTableKindModule) {
            current =
              search_in_module(self, current, get__Vec(*names, i), true);
        } else {
            assert(0 && "error");
        }
    }

    if (!current) {
        assert(0 && "error: data type not found");
    }

    return NEW(DataTypeSymbolCustom,
               generic_params,
               dt->value.custom->items[0],
               get_scope__SymbolTable(current));
}

struct DataTypeSymbol *
check_data_type(struct Compiler *self,
                struct DataType *dt,
                struct SymbolTable *last)
{
    if (last)
        assert((last->kind == SymbolTableKindModule ||
                last->kind == SymbolTableKindClass ||
                last->kind == SymbolTableKindRecord ||
                last->kind == SymbolTableKindRecordObj ||
                last->kind == SymbolTableKindEnum ||
                last->kind == SymbolTableKindEnumObj ||
                last->kind == SymbolTableKindTrait) &&
               "last->kind must be equal to SymbolTableKindClass, "
               "SymbolTableKindRecord, SymbolTableKindRecordObj, "
               "SymbolTableKindEnum, "
               "SymbolTableKindEnumObj or SymbolTableKindTrait");

    switch (dt->kind) {
        case DataTypeKindSelf:
            return NEW(DataTypeSymbol, DataTypeKindSelf);
        case DataTypeKindPtr:
            return NEW(DataTypeSymbolPtr,
                       check_data_type(self, dt->value.ptr, last));
        case DataTypeKindRef:
            return NEW(DataTypeSymbolRef,
                       check_data_type(self, dt->value.ref, last));
        case DataTypeKindStr:
            return NEW(DataTypeSymbol, DataTypeKindStr);
        case DataTypeKindBitStr:
            return NEW(DataTypeSymbol, DataTypeKindBitStr);
        case DataTypeKindChar:
            return NEW(DataTypeSymbol, DataTypeKindChar);
        case DataTypeKindBitChar:
            return NEW(DataTypeSymbol, DataTypeKindBitChar);
        case DataTypeKindI8:
            return NEW(DataTypeSymbol, DataTypeKindI8);
        case DataTypeKindI16:
            return NEW(DataTypeSymbol, DataTypeKindI16);
        case DataTypeKindI32:
            return NEW(DataTypeSymbol, DataTypeKindI32);
        case DataTypeKindI64:
            return NEW(DataTypeSymbol, DataTypeKindI64);
        case DataTypeKindI128:
            return NEW(DataTypeSymbol, DataTypeKindI128);
        case DataTypeKindU8:
            return NEW(DataTypeSymbol, DataTypeKindU8);
        case DataTypeKindU16:
            return NEW(DataTypeSymbol, DataTypeKindU16);
        case DataTypeKindU32:
            return NEW(DataTypeSymbol, DataTypeKindU32);
        case DataTypeKindU64:
            return NEW(DataTypeSymbol, DataTypeKindU64);
        case DataTypeKindU128:
            return NEW(DataTypeSymbol, DataTypeKindU128);
        case DataTypeKindF32:
            return NEW(DataTypeSymbol, DataTypeKindF32);
        case DataTypeKindF64:
            return NEW(DataTypeSymbol, DataTypeKindF64);
        case DataTypeKindBool:
            return NEW(DataTypeSymbol, DataTypeKindBool);
        case DataTypeKindIsize:
            return NEW(DataTypeSymbol, DataTypeKindIsize);
        case DataTypeKindUsize:
            return NEW(DataTypeSymbol, DataTypeKindUsize);
        case DataTypeKindNever:
            return NEW(DataTypeSymbol, DataTypeKindNever);
        case DataTypeKindAny:
            return NEW(DataTypeSymbol, DataTypeKindAny);
        case DataTypeKindOptional:
            return NEW(DataTypeSymbolOptional,
                       check_data_type(self, dt->value.optional, last));
        case DataTypeKindUnit:
            return NEW(DataTypeSymbol, DataTypeKindUnit);
        case DataTypeKindException:
            return NEW(DataTypeSymbolException,
                       check_data_type(self, dt->value.exception, last));
        case DataTypeKindMut:
            return NEW(DataTypeSymbolMut,
                       check_data_type(self, dt->value.mut, last));
        case DataTypeKindLambda: {
            struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

            for (Usize i =
                   len__Vec(*CAST(dt->value.lambda->items[0], struct Vec *));
                 i--;)
                push__Vec(
                  params,
                  check_data_type(
                    self,
                    get__Vec(*CAST(dt->value.lambda->items[0], struct Vec *),
                             i),
                    last));

            return NEW(DataTypeSymbolLambda,
                       params,
                       check_data_type(self, dt->value.lambda->items[1], last));
        }
        case DataTypeKindArray:
            return NEW(DataTypeSymbolArray,
                       check_data_type(self, dt->value.array->items[0], last),
                       dt->value.array->items[1]);
        case DataTypeKindTuple: {
            struct Vec *tuple = NEW(Vec, sizeof(struct DataTypeSymbol));

            for (Usize i = len__Vec(*dt->value.tuple); i--;)
                push__Vec(
                  tuple,
                  check_data_type(self, get__Vec(*dt->value.tuple, i), last));

            return NEW(DataTypeSymbolTuple, tuple);
        }
        case DataTypeKindCustom:
            if (last)
                return check_data_type_custom(self, dt, last);
            else
                return check_data_type_custom_in_global(self, dt);
        case DataTypeKindCompilerDefined:
            UNREACHABLE("impossible to get CompilerDefined after the parser");
    }
}

void
check_fun_param(struct Compiler *self,
                struct SymbolTable *fun,
                struct SymbolTable *parent)
{
    struct Vec *params = NEW(Vec, sizeof(struct FunParamSymbol));

    if (fun->value.fun->fun_decl->value.fun->params) {
        // 1. check if param name is not duplicate
        for (Usize i = 0;
             i < len__Vec(*fun->value.fun->fun_decl->value.fun->params);
             i++) {
            for (Usize j = i + 1;
                 j < len__Vec(*fun->value.fun->fun_decl->value.fun->params);
                 j++) {
                if (eq__String(
                      CAST(get__Vec(
                             *fun->value.fun->fun_decl->value.fun->params, i),
                           struct FunParam *)
                        ->name,
                      CAST(get__Vec(
                             *fun->value.fun->fun_decl->value.fun->params, j),
                           struct FunParam *)
                        ->name,
                      false)) {
                    assert(0 && "error: param name is duplicate");
                }
            }

            struct FunParam *fp =
              get__Vec(*fun->value.fun->fun_decl->value.fun->params, i);
            struct FunParamSymbol *res = NEW(FunParamSymbol, fp);

            switch (fp->kind) {
                case FunParamKindDefault: {
                    if (fp->param_data_type)
                        res->param_data_type =
                          check_data_type(self, fp->param_data_type, parent);

					// res->default_ = check_expression(self, fp->value.default_, fun, parent, NULL, 0);

                    break;
                }
                case FunParamKindNormal: {
                    if (fp->param_data_type)
                        res->param_data_type =
                          check_data_type(self, fp->param_data_type, parent);

                    break;
                }
                case FunParamKindSelf:
                    TODO("check self param");
                    break;
            }
        }
    }
}

void
check_fun(struct Compiler *self,
          struct SymbolTable *fun,
          struct Vec *scopes,
          struct SymbolTable *parent)
{
    if (!fun->value.fun->scope) {
        // 1. Build fun scope
        fun->value.fun->scope =
          NEW(Scope,
              self->tc->parser.parse_block.scanner.src->file.name,
              fun->value.fun->fun_decl->value.fun->name,
              ScopeItemKindFun,
              fun->value.fun->visibility,
              scopes,
              NULL,
              NULL,
              &fun->value.fun->fun_decl->loc);

        // 2. Check fun param
        check_fun_param(self, fun, parent);

        // fun add(x, y) = x+y;

        TODO("check fun");
    }
}

void
check_constant(struct Compiler *self,
               struct SymbolTable *constant,
               struct Vec *scopes,
               struct SymbolTable *parent)
{
    if (!constant->value.constant->scope) {
        constant->value.constant->scope =
          NEW(Scope,
              self->tc->parser.parse_block.scanner.src->file.name,
              constant->value.constant->constant_decl->value.constant->name,
              ScopeItemKindConstant,
              constant->value.constant->visibility,
              scopes,
              NULL,
              NULL,
              &constant->value.constant->constant_decl->loc);

        if (constant->value.constant->constant_decl->value.constant->data_type)
            constant->value.constant->data_type =
              check_data_type(self,
                              constant->value.constant->constant_decl->value
                                .constant->data_type,
                              parent);
    }

    TODO("check constant");
}

void
check_module(struct Compiler *self,
             struct SymbolTable *module,
             struct Vec *scopes,
             struct SymbolTable *parent)
{
    TODO("check module");
}

void
check_alias(struct Compiler *self,
            struct SymbolTable *alias,
            struct Vec *scopes,
            struct SymbolTable *parent)
{
    TODO("check alias");
}

void
check_record(struct Compiler *self,
             struct SymbolTable *record,
             struct Vec *scopes,
             struct SymbolTable *parent)
{
    TODO("check record");
}

void
check_record_obj(struct Compiler *self,
                 struct SymbolTable *record_obj,
                 struct Vec *scopes,
                 struct SymbolTable *parent)
{
    TODO("check record obj");
}

void
check_enum(struct Compiler *self,
           struct SymbolTable *enum_,
           struct Vec *scopes,
           struct SymbolTable *parent)
{
    TODO("check enum");
}

void
check_enum_obj(struct Compiler *self,
               struct SymbolTable *enum_obj,
               struct Vec *scopes,
               struct SymbolTable *parent)
{
    TODO("check enum obj");
}

void
check_error(struct Compiler *self,
            struct SymbolTable *error,
            struct Vec *scopes,
            struct SymbolTable *parent)
{
    TODO("check error");
}

void
check_class(struct Compiler *self,
            struct SymbolTable *class,
            struct Vec *scopes,
            struct SymbolTable *parent)
{
    TODO("check class");
}

void
check_trait(struct Compiler *self,
            struct SymbolTable *trait,
            struct Vec *scopes,
            struct SymbolTable *parent)
{
    TODO("check trait");
}

void
check_property(struct Compiler *self,
               struct SymbolTable *prop,
               struct Vec *scopes,
               struct SymbolTable *parent)
{
    TODO("check property");
}

void
check_method(struct Compiler *self,
             struct SymbolTable *method,
             struct Vec *scopes,
             struct SymbolTable *parent)
{
    TODO("check method");
}

void
check_symb(struct Compiler *self,
           struct SymbolTable *symb,
           struct SymbolTable *parent)
{
    if (get_scope__SymbolTable(symb))
        return;

    struct Vec *scopes = NEW(Vec, sizeof(struct Scope));

    if (parent) {
        struct Vec *scopes_p = get_scope__SymbolTable(parent)->scopes;

        for (Usize i = 0; i < len__Vec(*scopes_p); i++)
            push__Vec(scopes, get__Vec(*scopes_p, i));

        push__Vec(scopes, get_scope__SymbolTable(parent));
    }

    switch (symb->kind) {
        case SymbolTableKindFun:
            check_fun(self, symb, scopes, parent);
            break;
        case SymbolTableKindConstant:
            check_constant(self, symb, scopes, parent);
            break;
        case SymbolTableKindModule:
            check_module(self, symb, scopes, parent);
            break;
        case SymbolTableKindAlias:
            check_alias(self, symb, scopes, parent);
            break;
        case SymbolTableKindRecord:
            check_record(self, symb, scopes, parent);
            break;
        case SymbolTableKindEnum:
            check_enum(self, symb, scopes, parent);
            break;
        case SymbolTableKindError:
            check_error(self, symb, scopes, parent);
            break;
        case SymbolTableKindClass:
            check_class(self, symb, scopes, parent);
            break;
        case SymbolTableKindTrait:
            check_trait(self, symb, scopes, parent);
            break;
        case SymbolTableKindRecordObj:
            check_record_obj(self, symb, scopes, parent);
            break;
        case SymbolTableKindEnumObj:
            check_enum_obj(self, symb, scopes, parent);
            break;
        case SymbolTableKindProperty:
            check_property(self, symb, scopes, parent);
            break;
        default:
            UNREACHABLE("this SymbolTableKind doesn't exist in global");
    }
}
