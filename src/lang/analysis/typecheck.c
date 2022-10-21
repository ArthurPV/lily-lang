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
search_in_global(struct Compiler *self, struct String *name);
void
check_fun(struct Compiler *self, struct FunSymbol *fun, struct Vec *scopes);
void
check_constant(struct Compiler *self,
               struct ConstantSymbol *constant,
               struct Vec *scopes);
void
check_module(struct Compiler *self,
             struct ModuleSymbol *module,
             struct Vec *scopes);
void
check_alias(struct Compiler *self,
            struct AliasSymbol *alias,
            struct Vec *scopes);
void
check_record(struct Compiler *self,
             struct RecordSymbol *record,
             struct Vec *scopes);
void
check_record_obj(struct Compiler *self,
                 struct RecordObjSymbol *record_obj,
                 struct Vec *scopes);
void
check_enum(struct Compiler *self, struct EnumSymbol *enum_, struct Vec *scopes);
void
check_enum_obj(struct Compiler *self,
               struct EnumObjSymbol *enum_obj,
               struct Vec *scopes);
void
check_error(struct Compiler *self,
            struct ErrorSymbol *error,
            struct Vec *scopes);
void
check_class(struct Compiler *self,
            struct ClassSymbol *class,
            struct Vec *scopes);
void
check_trait(struct Compiler *self,
            struct TraitSymbol *trait,
            struct Vec *scopes);
void
check_property(struct Compiler *self,
               struct PropertySymbol *prop,
               struct Vec *scopes);
void
check_method(struct Compiler *self,
             struct MethodSymbol *method,
             struct Vec *scopes);
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

void
run__Typecheck(struct Compiler *self, struct Vec *primary_buffer)
{
    push_all_symbols(self);

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
        if (eq__String(CAST(get__Vec(*symb->value.module->scope->scopes, i),
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
        if (current) {
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

                if (count_scope <
                    len__Vec(*last->value.module->scope->scopes)) {
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
        } else {
            // Search in global
            current = search_in_global(self, get__Vec(*names, i));
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
    assert((last == NULL || last->kind == SymbolTableKindModule) &&
           "last == NULL && last->kind != SymbolTableKindModule");

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
check_fun(struct Compiler *self, struct FunSymbol *fun, struct Vec *scopes)
{
}

void
check_constant(struct Compiler *self,
               struct ConstantSymbol *constant,
               struct Vec *scopes)
{
    if (!constant->scope) {
        constant->scope =
          NEW(Scope,
              self->tc->parser.parse_block.scanner.src->file.name,
              constant->constant_decl->value.constant->name,
              ScopeItemKindConstant,
              constant->visibility,
              scopes,
              NULL,
              NULL,
              &constant->constant_decl->loc);
    }
}

void
check_module(struct Compiler *self,
             struct ModuleSymbol *module,
             struct Vec *scopes)
{
}

void
check_alias(struct Compiler *self,
            struct AliasSymbol *alias,
            struct Vec *scopes)
{
}

void
check_record(struct Compiler *self,
             struct RecordSymbol *record,
             struct Vec *scopes)
{
}

void
check_record_obj(struct Compiler *self,
                 struct RecordObjSymbol *record_obj,
                 struct Vec *scopes)
{
}

void
check_enum(struct Compiler *self, struct EnumSymbol *enum_, struct Vec *scopes)
{
}

void
check_enum_obj(struct Compiler *self,
               struct EnumObjSymbol *enum_obj,
               struct Vec *scopes)
{
}

void
check_error(struct Compiler *self,
            struct ErrorSymbol *error,
            struct Vec *scopes)
{
}

void
check_class(struct Compiler *self,
            struct ClassSymbol *class,
            struct Vec *scopes)
{
}

void
check_trait(struct Compiler *self,
            struct TraitSymbol *trait,
            struct Vec *scopes)
{
}

void
check_property(struct Compiler *self,
               struct PropertySymbol *prop,
               struct Vec *scopes)
{
}

void
check_method(struct Compiler *self,
             struct MethodSymbol *method,
             struct Vec *scopes)
{
}

void
check_symb(struct Compiler *self,
           struct SymbolTable *symb,
           struct SymbolTable *parent)
{
    struct Vec *scopes = NULL;

    if (get_scope__SymbolTable(symb))
        return;

    if (parent) {
        scopes = copy__Vec(get_scope__SymbolTable(parent)->scopes);
        push__Vec(scopes, get_scope__SymbolTable(parent));
    }

    switch (symb->kind) {
        case SymbolTableKindFun:
            check_fun(self, symb->value.fun, scopes);
            break;
        case SymbolTableKindConstant:
            check_constant(self, symb->value.constant, scopes);
            break;
        case SymbolTableKindModule:
            check_module(self, symb->value.module, scopes);
            break;
        case SymbolTableKindAlias:
            check_alias(self, symb->value.alias, scopes);
            break;
        case SymbolTableKindRecord:
            check_record(self, symb->value.record, scopes);
            break;
        case SymbolTableKindEnum:
            check_enum(self, symb->value.enum_, scopes);
            break;
        case SymbolTableKindError:
            check_error(self, symb->value.error, scopes);
            break;
        case SymbolTableKindClass:
            check_class(self, symb->value.class, scopes);
            break;
        case SymbolTableKindTrait:
            check_trait(self, symb->value.trait, scopes);
            break;
        case SymbolTableKindRecordObj:
            check_record_obj(self, symb->value.record_obj, scopes);
            break;
        case SymbolTableKindEnumObj:
            check_enum_obj(self, symb->value.enum_obj, scopes);
            break;
        case SymbolTableKindProperty:
            check_property(self, symb->value.property, scopes);
            break;
        default:
            UNREACHABLE("this SymbolTableKind doesn't exist in global");
    }
}
