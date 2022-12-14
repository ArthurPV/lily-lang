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

struct SearchContext
{
    bool search_type;
    bool search_fun;
    bool search_variant;
    bool search_value;
    bool search_trait;
    bool search_class;
    bool search_object; // Enum and Record object
    bool search_primary_type;
} SearchContext;

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
resolve_global_import(struct Typecheck *self);
void * // when the import_value pos == 1 it return struct Tuple<struct Decl&,
       // void&>* and when import_value pos > 1 it return struct SymbolTable&
search_access_from_buffer(struct Typecheck *self,
                          struct SymbolTable *symb,
                          struct Typecheck *buffer,
                          struct String *access,
                          struct Location loc);
void
resolve_import_value(struct Typecheck *self,
                     struct Typecheck *tc,
                     struct Location import_loc,
                     struct Vec *import_value,
                     struct String *as_value,
                     struct Vec *import_symbs,
                     struct SymbolTable *current_symb,
                     bool is_selector,
                     bool is_pub);
struct Vec *
resolve_import(struct Typecheck *self,
               struct Location import_loc,
               struct ImportStmt *import_stmt);
void
verify_if_decl_is_duplicate(struct Typecheck self);
void
verify_if_decl_is_duplicate_in_module(struct Typecheck self,
                                      struct Decl *module);
void
verify_if_decl_is_duplicate_in_enum(struct Typecheck self, struct Decl *enum_);
void
verify_if_decl_is_duplicate_in_record(struct Typecheck self,
                                      struct Decl *record);
void
verify_if_decl_is_duplicate_in_class(struct Typecheck self, struct Decl *class);
void
verify_if_decl_is_duplicate_in_tag(struct Typecheck self, struct Decl *tag);
void
verify_if_decl_is_duplicate_in_trait(struct Typecheck self, struct Decl *trait);
void
push_all_symbols(struct Typecheck *self);
struct Vec *
get_local_decl(struct Typecheck *self, struct Scope *scope);
void
check_constant(struct Typecheck *self,
               struct ConstantSymbol *constant,
               Usize id,
               struct Scope *previous);
void
check_module(struct Typecheck *self,
             struct ModuleSymbol *module,
             Usize id,
             struct Scope *previous);
void
check_alias(struct Typecheck *self,
            struct AliasSymbol *alias,
            Usize id,
            struct Scope *previous);
void
check_enum(struct Typecheck *self,
           struct EnumSymbol *enum_,
           Usize id,
           struct Scope *previous);
void
check_record(struct Typecheck *self,
             struct RecordSymbol *record,
             Usize id,
             struct Scope *previous);
void
check_error(struct Typecheck *self,
            struct ErrorSymbol *error,
            Usize id,
            struct Scope *previous);
void
check_enum_obj(struct Typecheck *self,
               struct EnumObjSymbol *enum_obj,
               Usize id,
               struct Scope *previous);
void
check_record_obj(struct Typecheck *self,
                 struct RecordObjSymbol *record_obj,
                 Usize id,
                 struct Scope *previous);
void
check_class(struct Typecheck *self,
            struct ClassSymbol *class,
            Usize id,
            struct Scope *previous);
void
check_trait(struct Typecheck *self,
            struct TraitSymbol *trait,
            Usize id,
            struct Scope *previous);
void
check_fun(struct Typecheck *self,
          struct FunSymbol *fun,
          Usize id,
          struct Scope *previous);
struct SymbolTable *
get_info_of_decl_from_scope(struct Typecheck *self,
                            struct Scope *scope,
                            struct Location loc);
void
check_symbols(struct Typecheck *self);
struct ModuleSymbol *
entry_in_module(struct Typecheck *self, struct Vec *id, Usize end_idx);
struct Scope *
search_from_access(struct Typecheck *self,
                   struct Expr *id,
                   struct Vec *name,
                   struct SearchContext search_context);
struct Scope *
search_with_search_module_context(struct Typecheck *self,
                                  struct Vec *name,
                                  struct SearchContext search_module_context);
struct ConstantSymbol *
search_in_consts_from_name(struct Typecheck *self, struct String *name);
struct ModuleSymbol *
search_in_modules_from_name(struct Typecheck *self, struct String *name);
struct FunSymbol *
search_in_funs_from_name(struct Typecheck *self, struct String *name);
struct AliasSymbol *
search_in_aliases_from_name(struct Typecheck *self, struct String *name);
struct EnumSymbol *
search_in_enums_from_name(struct Typecheck *self, struct String *name);
struct RecordSymbol *
search_in_records_from_name(struct Typecheck *self, struct String *name);
struct EnumObjSymbol *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name);
struct RecordObjSymbol *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name);
struct ClassSymbol *
search_in_classes_from_name(struct Typecheck *self, struct String *name);
struct TraitSymbol *
search_in_traits_from_name(struct Typecheck *self, struct String *name);
struct ErrorSymbol *
search_in_errors_from_name(struct Typecheck *self, struct String *name);
struct SymbolTable *
search_module_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope);
struct SymbolTable *
search_enum_item_in_scope(struct Typecheck *self,
                          struct Expr *id,
                          struct SymbolTable *scope);
struct SymbolTable *
search_record_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope);
struct SymbolTable *
search_enum_obj_item_in_scope(struct Typecheck *self,
                              struct Expr *id,
                              struct SymbolTable *scope);
struct SymbolTable *
search_record_obj_item_in_scope(struct Typecheck *self,
                                struct Expr *id,
                                struct SymbolTable *scope);
struct SymbolTable *
search_class_item_in_scope(struct Typecheck *self,
                           struct Expr *id,
                           struct SymbolTable *scope);
struct SymbolTable *
search_in_custom_scope(struct Typecheck *self,
                       struct Expr *id,
                       struct SymbolTable *scope);
struct Vec *
search_in_funs_from_fun_call(struct Typecheck *self, struct Expr *id);
struct Vec *
identifier_access_to_String_vec(struct Expr *id);
struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                struct Vec *local_decl,
                struct SearchContext ctx);
struct StmtSymbol
check_stmt(struct Typecheck *self,
           struct Stmt *stmt,
           struct Vec *local_value,
           bool is_return_type);
struct DataTypeSymbol *
get_data_type_of_expression(struct Typecheck *self,
                            struct ExprSymbol *expr,
                            struct Vec *local_value);
const Str
get_builtin_module_name_from_data_type(struct DataTypeSymbol *dt);
struct BuiltinFun *
search_fun_builtin(struct Typecheck *self,
                   const Str module_name,
                   const Str fun_name,
                   Usize param_count);
void
verify_type_of_fun_builtin(struct BuiltinFun *fun_builtin,
                           Usize param_count,
                           ...);
static inline struct DataTypeSymbol *
get_return_type_of_fun_builtin(struct Typecheck *self,
                               const Str module_name,
                               const Str fun_name,
                               Usize param_count);
struct Scope *
search_in_fun_local_value(struct Vec *local_value, struct String *id);
struct Scope *
search_value_in_function(struct Typecheck *self,
                         struct Vec *local_value,
                         struct String *id,
                         struct Vec *id_access);
struct DataTypeSymbol *
check_if_defined_data_type_is_equal_to_infered_data_type(
  struct Typecheck *self,
  struct DataTypeSymbol *defined_data_type,
  struct DataTypeSymbol *infered_data_type);
struct DataTypeSymbol *
infer_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type);
enum DataTypeKind
get_data_type_kind_from_literal_symbol(enum LiteralSymbolKind kind);
struct ExprSymbol *
check_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type);
struct StmtSymbol
check_await_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec *local_value,
                 struct Vec *local_data_type);
struct StmtSymbol
check_return_stmt(struct Typecheck *self,
                  struct FunSymbol *fun,
                  struct Stmt *stmt,
                  struct Vec *local_value,
                  struct Vec *local_data_type);
struct StmtSymbol
check_for_stmt(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Stmt *stmt,
               struct Vec local_value,
               struct Vec *local_data_type);
struct StmtSymbol
check_if_stmt(struct Typecheck *self,
              struct FunSymbol *fun,
              struct Stmt *stmt,
              struct Vec local_value,
              struct Vec *local_data_type);
struct StmtSymbol
check_match_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec local_value,
                 struct Vec *local_data_type);
struct StmtSymbol
check_try_stmt(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Stmt *stmt,
               struct Vec local_value,
               struct Vec *local_data_type);
struct StmtSymbol
check_while_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec local_value,
                 struct Vec *local_data_type);
void
check_fun_body(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Vec *fun_body,
               struct Vec *local_value,
               struct Vec *local_data_type);

struct Typecheck
__new__Typecheck(struct Parser parser)
{
    run__Parser(&parser);

    struct Typecheck self = {
        .parser = parser,
        .decl =
          len__Vec(*parser.decls) == 0 ? NULL : get__Vec(*parser.decls, 0),
        .buffers = NEW(Vec, sizeof(struct Typecheck)),
        .builtins = Load_C_builtins(),
        .import_values = NEW(Vec, sizeof(struct Tuple)),
        .funs = NULL,
        .consts = NULL,
        .aliases = NULL,
        .records = NULL,
        .enums = NULL,
        .errors = NULL,
        .classes = NULL,
        .traits = NULL,
        .records_obj = NULL,
        .enums_obj = NULL,
    };

    return self;
}

void
run__Typecheck(struct Typecheck *self, struct Vec *primary_buffer)
{
    {
        resolve_global_import(self);
        verify_if_decl_is_duplicate(*self);
        SUMMARY();
    }

    push_all_symbols(self);
    check_symbols(self);
    SUMMARY();
}

void
__free__Typecheck(struct Typecheck self)
{
    for (Usize i = len__Vec(*self.buffers); i--;) {
        FREE(Typecheck, *(struct Typecheck *)get__Vec(*self.buffers, i));
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

    if (self.funs) {
        for (Usize i = len__Vec(*self.funs); i--;)
            FREE(FunSymbol, get__Vec(*self.funs, i));

        FREE(Vec, self.funs);
    }

    if (self.consts) {
        for (Usize i = len__Vec(*self.consts); i--;)
            FREE(ConstantSymbol, get__Vec(*self.consts, i));

        FREE(Vec, self.consts);
    }

    if (self.modules) {
        for (Usize i = len__Vec(*self.modules); i--;)
            FREE(ModuleSymbol, get__Vec(*self.modules, i));

        FREE(Vec, self.modules);
    }

    if (self.aliases) {
        for (Usize i = len__Vec(*self.aliases); i--;)
            FREE(AliasSymbol, get__Vec(*self.aliases, i));

        FREE(Vec, self.aliases);
    }

    if (self.records) {
        for (Usize i = len__Vec(*self.records); i--;)
            FREE(RecordSymbol, get__Vec(*self.records, i));

        FREE(Vec, self.records);
    }

    if (self.records_obj) {
        for (Usize i = len__Vec(*self.records_obj); i--;)
            FREE(RecordObjSymbol, get__Vec(*self.records_obj, i));

        FREE(Vec, self.records_obj);
    }

    if (self.enums) {
        for (Usize i = len__Vec(*self.enums); i--;)
            FREE(EnumSymbol, get__Vec(*self.enums, i));

        FREE(Vec, self.enums);
    }

    if (self.enums_obj) {
        for (Usize i = len__Vec(*self.enums_obj); i--;)
            FREE(EnumObjSymbol, get__Vec(*self.enums_obj, i));

        FREE(Vec, self.enums_obj);
    }

    if (self.errors) {
        for (Usize i = len__Vec(*self.errors); i--;)
            FREE(ErrorSymbol, get__Vec(*self.errors, i));

        FREE(Vec, self.errors);
    }

    if (self.classes) {
        for (Usize i = len__Vec(*self.classes); i--;)
            FREE(ClassSymbol, get__Vec(*self.classes, i));

        FREE(Vec, self.classes);
    }

    if (self.traits) {
        for (Usize i = len__Vec(*self.traits); i--;)
            FREE(TraitSymbol, get__Vec(*self.traits, i));

        FREE(Vec, self.traits);
    }

    FREE(Parser, self.parser);
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
resolve_global_import(struct Typecheck *self)
{
    struct Vec *imports = NEW(Vec, sizeof(struct Decl));

    for (Usize i = len__Vec(*self->parser.decls); i--;)
        if (((struct Decl *)get__Vec(*self->parser.decls, i))->kind ==
            DeclKindImport)
            push__Vec(imports, get__Vec(*self->parser.decls, i));

    // Resolve import in priority @core and @std import value
    for (Usize i = 0; i < len__Vec(*imports); i++) {
        enum ImportStmtValueKind kind =
          ((struct ImportStmtValue *)get__Vec(
             *((struct Decl *)get__Vec(*imports, i))
                ->value.import->import_value,
             i))
            ->kind;

        if (kind == ImportStmtValueKindCore || kind == ImportStmtValueKindStd) {
            struct Vec *res = resolve_import(
              self,
              ((struct Decl *)get__Vec(*imports, i))->loc,
              ((struct Decl *)get__Vec(*imports, i))->value.import);

            concat__Vec(self->import_values, res);
            remove__Vec(imports, i);

            if (res)
                FREE(Vec, res);

            i = 0;
        }
    }

    for (Usize i = 0; i < len__Vec(*imports); i++) {
        struct Vec *res =
          resolve_import(self,
                         ((struct Decl *)get__Vec(*imports, i))->loc,
                         ((struct Decl *)get__Vec(*imports, i))->value.import);

        concat__Vec(self->import_values, res);

        if (res)
            FREE(Vec, res);
    }

    if (imports)
        FREE(Vec, imports);
}

void *
search_access_from_buffer(struct Typecheck *self,
                          struct SymbolTable *symb,
                          struct Typecheck *buffer,
                          struct String *access,
                          struct Location loc)
{
    if (buffer) {
        struct Decl *decl = NULL;
        void *symb = NULL;

        for (Usize i = len__Vec(*buffer->parser.decls); i--;) {
            if (eq__String(access,
                           get_name__Decl(get__Vec(*buffer->parser.decls, i)),
                           false)) {
                decl = get__Vec(*buffer->parser.decls, i);

                if (decl->kind != DeclKindTag)
                    break;
                else
                    decl = NULL;
            }
        }

        if (!decl) {
            assert(0 && "error");
            SUMMARY();
        }

        switch (decl->kind) {
            case DeclKindFun:
                symb = search_in_funs_from_name(buffer, access);

                break;
            case DeclKindConstant:
                symb = search_in_consts_from_name(buffer, access);

                break;
            case DeclKindModule:
                symb = search_in_modules_from_name(buffer, access);

                break;
            case DeclKindAlias:
                symb = search_in_aliases_from_name(buffer, access);

                break;
            case DeclKindEnum:
                if (decl->value.enum_->is_object)
                    symb = search_in_enums_obj_from_name(buffer, access);
                else
                    symb = search_in_enums_from_name(buffer, access);

                break;
            case DeclKindRecord:
                if (decl->value.record->is_object)
                    symb = search_in_records_obj_from_name(buffer, access);
                else
                    symb = search_in_records_from_name(buffer, access);

                break;
            case DeclKindError:
                symb = search_in_errors_from_name(buffer, access);

                break;
            case DeclKindClass:
                symb = search_in_classes_from_name(buffer, access);

                break;
            case DeclKindTrait:
                symb = search_in_traits_from_name(buffer, access);

                break;
            default:
                UNREACHABLE("it's impossible to found this decl kind");
        }

        return NEW(Tuple, 2, decl, symb);
    } else if (symb) {
#define ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND()                         \
    {                                                                  \
        struct Diagnostic *error =                                     \
          NEW(DiagnosticWithErrTypecheck,                              \
              self,                                                    \
              NEW(LilyError, LilyErrorImportValueAccessIsNotFound),    \
              loc,                                                     \
              from__String(""),                                        \
              Some(format("remove the access named: `{S}`", access))); \
                                                                       \
        emit__Diagnostic(error);                                       \
    }

        switch (symb->kind) {
            case SymbolTableKindModule:
                for (Usize i = len__Vec(*symb->value.module->body); i--;) {
                    if (eq__String(get_name__SymbolTable(
                                     get__Vec(*symb->value.module->body, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.module->body, i);
                }

                ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND();

                return NULL;
            case SymbolTableKindEnum:
                for (Usize i = len__Vec(*symb->value.enum_->variants); i--;) {
                    if (eq__String(get_name__SymbolTable(
                                     get__Vec(*symb->value.enum_->variants, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.enum_->variants, i);
                }

                ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND();

                return NULL;
            case SymbolTableKindClass:
                for (Usize i = len__Vec(*symb->value.class->body); i--;) {
                    if (eq__String(get_name__SymbolTable(
                                     get__Vec(*symb->value.class->body, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.class->body, i);
                }

                ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND();

                return NULL;
            case SymbolTableKindRecordObj:
                for (Usize i = len__Vec(*symb->value.record_obj->attached);
                     i--;) {
                    if (eq__String(get_name__SymbolTable(get__Vec(
                                     *symb->value.record_obj->attached, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.record_obj->attached, i);
                }

                ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND();

                return NULL;
            case SymbolTableKindEnumObj:
                for (Usize i = len__Vec(*symb->value.enum_obj->variants);
                     i--;) {
                    if (eq__String(get_name__SymbolTable(get__Vec(
                                     *symb->value.enum_obj->variants, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.enum_obj->variants, i);
                }

                for (Usize i = len__Vec(*symb->value.enum_obj->attached);
                     i--;) {
                    if (eq__String(get_name__SymbolTable(get__Vec(
                                     *symb->value.enum_obj->attached, i)),
                                   access,
                                   false))
                        return get__Vec(*symb->value.enum_obj->attached, i);
                }

                ERR_IMPORT_VALUE_ACCESS_IS_NOT_FOUND();

                return NULL;
            case SymbolTableKindFun:
            case SymbolTableKindConstant:
            case SymbolTableKindAlias:
            case SymbolTableKindRecord:
            case SymbolTableKindError:
            case SymbolTableKindTrait:
            case SymbolTableKindVariant:
            case SymbolTableKindProperty:
            case SymbolTableKindMethod: {
                struct Diagnostic *error =
                  NEW(DiagnosticWithErrTypecheck,
                      self,
                      NEW(LilyError, LilyErrorExpectedFinalAccessInImportValue),
                      loc,
                      from__String(""),
                      Some(from__String(
                        "remove the last access of this import value")));

                emit__Diagnostic(error);

                return NULL;
            }
            default:
                UNREACHABLE("");
        }
    } else
        UNREACHABLE("symb and buffer dont't be equal to NULL in the same time");
}

void
resolve_import_value(struct Typecheck *self,
                     struct Typecheck *tc,
                     struct Location import_loc,
                     struct Vec *import_value,
                     struct String *as_value,
                     struct Vec *import_symbs,
                     struct SymbolTable *current_symb,
                     bool is_selector,
                     bool is_pub)
{
    for (Usize i = !current_symb ? 1 : 0; i < len__Vec(*import_value); i++) {
        switch (((struct ImportStmtValue *)get__Vec(*import_value, i))->kind) {
            case ImportStmtValueKindAccess: {
                if (i == 1 && !current_symb) {
                    struct Tuple *res = search_access_from_buffer(
                      self,
                      NULL,
                      tc,
                      ((struct ImportStmtValue *)get__Vec(*import_value, i))
                        ->value.access,
                      import_loc);

                    struct SymbolTable *symb = NULL;

                    switch (((struct Decl *)res->items[0])->kind) {
                        case DeclKindFun:
                            symb = NEW(SymbolTableFun, res->items[1]);

                            break;
                        case DeclKindConstant:
                            symb = NEW(SymbolTableConstant, res->items[1]);

                            break;
                        case DeclKindModule:
                            symb = NEW(SymbolTableModule, res->items[1]);

                            break;
                        case DeclKindAlias:
                            symb = NEW(SymbolTableAlias, res->items[1]);

                            break;
                        case DeclKindRecord:
                            if (((struct Decl *)res->items[0])
                                  ->value.record->is_object)
                                symb = NEW(SymbolTableRecordObj, res->items[1]);
                            else
                                symb = NEW(SymbolTableRecord, res->items[1]);

                            break;
                        case DeclKindEnum:
                            if (((struct Decl *)res->items[0])
                                  ->value.enum_->is_object)
                                symb = NEW(SymbolTableEnumObj, res->items[1]);
                            else
                                symb = NEW(SymbolTableEnum, res->items[1]);

                            break;
                        case DeclKindError:
                            symb = NEW(SymbolTableError, res->items[1]);

                            break;
                        case DeclKindClass:
                            symb = NEW(SymbolTableClass, res->items[1]);

                            break;
                        case DeclKindTrait:
                            symb = NEW(SymbolTableTrait, res->items[1]);

                            break;
                        default:
                            UNREACHABLE("");
                    }

                    if (!symb)
                        goto exit;

                    if (++i < len__Vec(*import_value)) {
                        current_symb = search_access_from_buffer(
                          self,
                          symb,
                          NULL,
                          ((struct ImportStmtValue *)get__Vec(*import_value, i))
                            ->value.access,
                          import_loc);

                        if (symb)
                            free(symb);
                    } else
                        current_symb = symb;
                } else {
                    current_symb = search_access_from_buffer(
                      self,
                      current_symb,
                      NULL,
                      ((struct ImportStmtValue *)get__Vec(*import_value, i))
                        ->value.access,
                      import_loc);
                }

                break;
            }
            case ImportStmtValueKindSelector: {
                // "a.b.c.{d, e, f}"
                // 		^ last access
                struct SymbolTable *last_access = current_symb;
                struct Vec *selector =
                  ((struct ImportStmtValue *)get__Vec(*import_value, i))
                    ->value.selector;
                struct Vec *import_symbs_selector =
                  NEW(Vec, sizeof(struct Tuple));

                for (Usize i = len__Vec(*selector); i--;) {
                    resolve_import_value(
                      self,
                      tc,
                      import_loc,
                      get__Vec(*selector, i),
                      NULL, // NULL by default, note: if in the future it is
                            // implemented `as` in the NULL selector it will no
                            // longer be
                      import_symbs_selector,
                      current_symb,
                      true,
                      is_pub);

                    current_symb = last_access;
                }

                if (i + 1 != len__Vec(*import_value)) {
                    struct Diagnostic *error =
                      NEW(DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorExpectedFinalImportValue),
                          import_loc,
                          from__String(""),
                          Some(from__String("remove the next import values")));

                    emit__Diagnostic(error);
                }

                if (as_value) {
                    struct Vec *body = NEW(Vec, sizeof(struct SymbolTable));

                    for (Usize i = len__Vec(*import_symbs_selector); i--;) {
                        push__Vec(
                          body,
                          ((struct Tuple *)get__Vec(*import_symbs_selector, i))
                            ->items[1]);
                    }

                    free(last_access);

                    current_symb =
                      NEW(SymbolTableModule, NEW(ModuleSymbol, NULL));
                    current_symb->value.module->name = as_value;
                    current_symb->value.module->body = body;
                    current_symb->value.module->visibility =
                      is_pub ? VisibilityPublic : VisibilityPrivate;
                    current_symb->value.module->import_loc = &import_loc;
                } else
                    concat__Vec(import_symbs, import_symbs_selector);

                if (import_symbs_selector)
                    FREE(Vec, import_symbs_selector);

                goto exit;
            }
            default:
                UNREACHABLE(
                  "found @core, @std or @builtin import value is impossible");
        }

        if (!current_symb)
            goto exit;
    }

exit : {
    if (((ImportStmtValue *)get__Vec(*import_value,
                                     len__Vec(*import_value) - 1))
            ->kind != ImportStmtValueKindSelector &&
        !as_value) {
        if ((!is_selector &&
             (len__Vec(*import_value) == 2 ||
              (len__Vec(*import_value) == 3 &&
               ((struct ImportStmtValue *)get__Vec(*import_value,
                                                   len__Vec(*import_value) - 1))
                   ->kind == ImportStmtValueKindWildcard)) &&
             current_symb) ||
            as_value) {
            push__Vec(import_symbs,
                      NEW(Tuple,
                          3,
                          current_symb,
                          (int *)1,
                          tc->parser.parse_block.scanner.src->file.name));
        } else if (current_symb) {
            push__Vec(import_symbs,
                      NEW(Tuple,
                          3,
                          current_symb,
                          (int *)0,
                          tc->parser.parse_block.scanner.src->file.name));
        }
    }
}
}

struct Vec *
resolve_import(struct Typecheck *self,
               struct Location import_loc,
               struct ImportStmt *import_stmt)
{
    struct String *path = NULL;
    Str path_str = NULL;
    struct Vec *import_symbs = NEW(Vec, sizeof(struct Tuple));

    switch (((struct ImportStmtValue *)get__Vec(*import_stmt->import_value, 0))
              ->kind) {
        case ImportStmtValueKindStd:
#ifdef LOCAL
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
            path = from__String("lib/std/std.lily");
            path_str = to_Str__String(*path);
#elif defined(LILY_WINDOWS_OS)
            path = from__String("lib\\std\\std.lily");
            path_str = to_Str__String(*path);
#else
#error "unknown OS"
#endif
#else
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
            path = from__String("");
            path_str = to_Str__String(*path);
#elif defined(LILY_WINDOWS_OS)
            path = from__String("");
            path_str = to_Str__String(*path);
#else
#error "unknown OS"
#endif
#endif
            break;
        case ImportStmtValueKindCore:
#ifdef LOCAL
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
            path = from__String("lib/core/core.lily");
            path_str = to_Str__String(*path);
#elif defined(LILY_WINDOWS_OS)
            path = from__String("lib\\core\\core.lily");
            path_str = to_Str__String(*path);
#else
#error "unknown OS"
#endif
#else
#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
            path = from__String("");
            path_str = to_Str__String(*path);
#elif defined(LILY_WINDOWS_OS)
            path = from__String("");
            path_str = to_Str__String(*path);
#else
#error "unknown OS"
#endif
#endif
            break;
        case ImportStmtValueKindBuiltin:
            TODO("@builtin");
            break;
        case ImportStmtValueKindFile:
            path = format("{S}",
                          ((struct ImportStmtValue *)get__Vec(
                             *import_stmt->import_value, 0))
                            ->value.file);
            path_str = to_Str__String(*path);

            break;
        case ImportStmtValueKindUrl:
            TODO("@url");
        case ImportStmtValueKindAccess:
            path = format("{S}.lily",
                          ((struct ImportStmtValue *)get__Vec(
                             *import_stmt->import_value, 0))
                            ->value.access);
            path_str = to_Str__String(*path);

            break;
        case ImportStmtValueKindSelector:
        case ImportStmtValueKindWildcard: {
            struct Diagnostic *error =
              NEW(DiagnosticWithErrTypecheck,
                  self,
                  NEW(LilyError, LilyErrorNotExpectedImportValue),
                  import_loc,
                  from__String(""),
                  Some(from__String(
                    "selector or wildcard is not expected value in first")));

            emit__Diagnostic(error);

            goto exit;
        }
    }

    // This error is fatal
    if (!strcmp(path_str, self->parser.parse_block.scanner.src->file.name)) {
        struct Diagnostic *error =
          NEW(DiagnosticWithErrTypecheck,
              self,
              NEW(LilyError, LilyErrorDependencyCycleOnImportValue),
              import_loc,
              from__String(""),
              Some(from__String("remove this import")));

        emit__Diagnostic(error);
        SUMMARY();
    }

    struct File file = NEW(File, path_str);
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);

    run__Scanner(&scanner);

    struct ParseBlock parse_block = NEW(ParseBlock, scanner);

    run__ParseBlock(&parse_block);

    struct Parser parser = NEW(Parser, parse_block);

    run__Parser(&parser);

    struct Typecheck tc = NEW(Typecheck, parser);

    run__Typecheck(&tc, self->buffers);

    struct Typecheck *tc_copy = malloc(sizeof(struct Typecheck));

    memcpy(tc_copy, &tc, sizeof(struct Typecheck));
    push__Vec(self->buffers, tc_copy);

    if (path)
        FREE(String, path);

    FREE(Typecheck, tc);

    resolve_import_value(self,
                         tc_copy,
                         import_loc,
                         import_stmt->import_value,
                         import_stmt->as,
                         import_symbs,
                         NULL,
                         false,
                         import_stmt->is_pub);

exit : {
    return import_symbs;
}
}

void
verify_if_decl_is_duplicate(struct Typecheck self)
{
    for (Usize i = 0; i < len__Vec(*self.parser.decls); i++) {
        if (((struct Decl *)get__Vec(*self.parser.decls, i))->kind !=
            DeclKindImport) {
            for (Usize j = i + 1; j < len__Vec(*self.parser.decls); j++) {
                if (((struct Decl *)get__Vec(*self.parser.decls, j))->kind !=
                    DeclKindImport) {
                    if (eq__String(
                          get_name__Decl(get__Vec(*self.parser.decls, i)),
                          get_name__Decl(get__Vec(*self.parser.decls, j)),
                          false)) {
                        {
                            struct Decl *di = get__Vec(*self.parser.decls, i);
                            struct Decl *dj = get__Vec(*self.parser.decls, j);

                            if ((di->kind == DeclKindTag &&
                                 (dj->kind == DeclKindRecord ||
                                  dj->kind == DeclKindEnum)) ||
                                (dj->kind == DeclKindTag &&
                                 (di->kind == DeclKindRecord ||
                                  di->kind == DeclKindEnum))) {
                                goto exit;
                            } else
                                goto error;
                        }

                    error : {
                        struct Diagnostic *error = NEW(
                          DiagnosticWithErrTypecheck,
                          &self,
                          NEW(LilyError, LilyErrorDuplicateDeclaration),
                          ((struct Decl *)get__Vec(*self.parser.decls, j))->loc,
                          from__String(""),
                          Some(
                            from__String("remove this declaration or move the "
                                         "declaration in other scope")));

                        struct Diagnostic *note = NEW(
                          DiagnosticWithNoteTypecheck,
                          &self,
                          format(
                            "this declaration is in conflict with a "
                            "declaration "
                            "declared at the location ({d}:{d})",
                            ((struct Decl *)get__Vec(*self.parser.decls, j))
                              ->loc.s_line,
                            ((struct Decl *)get__Vec(*self.parser.decls, j))
                              ->loc.s_col),
                          ((struct Decl *)get__Vec(*self.parser.decls, i))->loc,
                          from__String(""),
                          None());

                        emit__Diagnostic(error);
                        emit__Diagnostic(note);

                        break;
                    }
                    }

                exit : {
                }
                }
            }
        }

        switch (((struct Decl *)get__Vec(*self.parser.decls, i))->kind) {
            case DeclKindModule:
                verify_if_decl_is_duplicate_in_module(
                  self, get__Vec(*self.parser.decls, i));

                break;
            case DeclKindEnum:
                verify_if_decl_is_duplicate_in_enum(
                  self, get__Vec(*self.parser.decls, i));

                break;
            case DeclKindRecord:
                verify_if_decl_is_duplicate_in_record(
                  self, get__Vec(*self.parser.decls, i));

                break;
            case DeclKindClass:
                verify_if_decl_is_duplicate_in_class(
                  self, get__Vec(*self.parser.decls, i));

                break;
            case DeclKindTag:
                verify_if_decl_is_duplicate_in_tag(
                  self, get__Vec(*self.parser.decls, i));

                break;
            case DeclKindTrait:
                verify_if_decl_is_duplicate_in_trait(
                  self, get__Vec(*self.parser.decls, i));

                break;
            default:
                break;
        }
    }
}

void
verify_if_decl_is_duplicate_in_module(struct Typecheck self,
                                      struct Decl *module)
{
    if (module->value.module->body) {
        for (Usize i = 0; i < len__Vec(*module->value.module->body); i++) {
            if (((struct ModuleBodyItem *)get__Vec(*module->value.module->body,
                                                   i))
                  ->kind == ModuleBodyItemKindDecl) {
                for (Usize j = i + 1; j < len__Vec(*module->value.module->body);
                     j++) {
                    if (((struct ModuleBodyItem *)get__Vec(
                           *module->value.module->body, j))
                          ->kind == ModuleBodyItemKindDecl) {
                        if (eq__String(
                              get_name__Decl(((struct ModuleBodyItem *)get__Vec(
                                                *module->value.module->body, i))
                                               ->value.decl),
                              get_name__Decl(((struct ModuleBodyItem *)get__Vec(
                                                *module->value.module->body, j))
                                               ->value.decl),
                              false)) {
                            {
                                struct Decl *di =
                                  ((struct ModuleBodyItem *)get__Vec(
                                     *module->value.module->body, i))
                                    ->value.decl;
                                struct Decl *dj =
                                  ((struct ModuleBodyItem *)get__Vec(
                                     *module->value.module->body, j))
                                    ->value.decl;

                                if ((di->kind == DeclKindTag &&
                                     (dj->kind == DeclKindRecord ||
                                      dj->kind == DeclKindEnum)) ||
                                    (dj->kind == DeclKindTag &&
                                     (di->kind == DeclKindRecord ||
                                      di->kind == DeclKindEnum))) {
                                    goto exit;
                                } else
                                    goto error;
                            }

                        error : {
                            struct Diagnostic *error =
                              NEW(DiagnosticWithErrTypecheck,
                                  &self,
                                  NEW(LilyError, LilyErrorDuplicateDeclaration),
                                  ((struct ModuleBodyItem *)get__Vec(
                                     *module->value.module->body, j))
                                    ->value.decl->loc,
                                  from__String(""),
                                  Some(from__String(
                                    "remove this declaration or move the "
                                    "declaration in other scope")));

                            struct Diagnostic *note = NEW(
                              DiagnosticWithNoteTypecheck,
                              &self,
                              format("this declaration is in conflict with a "
                                     "declaration "
                                     "declared at the location ({d}:{d})",
                                     ((struct ModuleBodyItem *)get__Vec(
                                        *module->value.module->body, j))
                                       ->value.decl->loc.s_line,
                                     ((struct ModuleBodyItem *)get__Vec(
                                        *module->value.module->body, j))
                                       ->value.decl->loc.s_col),
                              ((struct ModuleBodyItem *)get__Vec(
                                 *module->value.module->body, i))
                                ->value.decl->loc,
                              from__String(""),
                              None());

                            emit__Diagnostic(error);
                            emit__Diagnostic(note);

                            break;
                        }

                        exit : {
                        }
                        }
                    }
                }

                switch (((struct Decl *)((struct ModuleBodyItem *)get__Vec(
                                           *module->value.module->body, i))
                           ->value.decl)
                          ->kind) {
                    case DeclKindModule:
                        verify_if_decl_is_duplicate_in_module(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    case DeclKindEnum:
                        verify_if_decl_is_duplicate_in_enum(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    case DeclKindRecord:
                        verify_if_decl_is_duplicate_in_record(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    case DeclKindClass:
                        verify_if_decl_is_duplicate_in_class(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    case DeclKindTag:
                        verify_if_decl_is_duplicate_in_tag(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    case DeclKindTrait:
                        verify_if_decl_is_duplicate_in_trait(
                          self,
                          ((struct ModuleBodyItem *)get__Vec(
                             *module->value.module->body, i))
                            ->value.decl);

                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void
verify_if_decl_is_duplicate_in_enum(struct Typecheck self, struct Decl *enum_)
{
    if (enum_->value.enum_->variants) {
        for (Usize i = 0; i < len__Vec(*enum_->value.enum_->variants); i++) {
            for (Usize j = i + 1; j < len__Vec(*enum_->value.enum_->variants);
                 j++) {
                if (eq__String(((struct VariantEnum *)get__Vec(
                                  *enum_->value.enum_->variants, i))
                                 ->name,
                               ((struct VariantEnum *)get__Vec(
                                  *enum_->value.enum_->variants, j))
                                 ->name,
                               false)) {
                    struct Diagnostic *error =
                      NEW(DiagnosticWithErrTypecheck,
                          &self,
                          NEW(LilyError, LilyErrorDuplicateVariant),
                          ((struct VariantEnum *)get__Vec(
                             *enum_->value.enum_->variants, j))
                            ->loc,
                          from__String(""),
                          Some(from__String("remove this variant")));

                    emit__Diagnostic(error);

                    break;
                }
            }
        }
    }
}

void
verify_if_decl_is_duplicate_in_record(struct Typecheck self,
                                      struct Decl *record)
{
    if (record->value.record->fields) {
        for (Usize i = 0; i < len__Vec(*record->value.record->fields); i++) {
            for (Usize j = i + 1; j < len__Vec(*record->value.record->fields);
                 j++) {
                if (eq__String(((struct FieldRecord *)get__Vec(
                                  *record->value.record->fields, i))
                                 ->name,
                               ((struct FieldRecord *)get__Vec(
                                  *record->value.record->fields, j))
                                 ->name,
                               false)) {
                    struct Diagnostic *error =
                      NEW(DiagnosticWithErrTypecheck,
                          &self,
                          NEW(LilyError, LilyErrorDuplicateField),
                          ((struct FieldRecord *)get__Vec(
                             *record->value.record->fields, j))
                            ->loc,
                          from__String(""),
                          Some(from__String("remove this field")));

                    emit__Diagnostic(error);

                    break;
                }
            }
        }
    }
}

void
verify_if_decl_is_duplicate_in_class(struct Typecheck self, struct Decl *class)
{
    if (class->value.class->body) {
        for (Usize i = 0; i < len__Vec(*class->value.class->body); i++) {
            if (((struct ClassBodyItem *)get__Vec(*class->value.class->body, i))
                  ->kind != ClassBodyItemKindImport) {
                for (Usize j = i + 1; j < len__Vec(*class->value.class->body);
                     j++) {
                    if (((struct ClassBodyItem *)get__Vec(
                           *class->value.class->body, j))
                          ->kind != ClassBodyItemKindImport) {
                        if (eq__String(get_name__ClassBodyItem(get__Vec(
                                         *class->value.class->body, i)),
                                       get_name__ClassBodyItem(get__Vec(
                                         *class->value.class->body, j)),
                                       false)) {
                            struct Diagnostic *error =
                              NEW(DiagnosticWithErrTypecheck,
                                  &self,
                                  NEW(LilyError, LilyErrorDuplicateDeclaration),
                                  ((struct ClassBodyItem *)get__Vec(
                                     *class->value.class->body, j))
                                    ->loc,
                                  from__String(""),
                                  Some(from__String(
                                    "remove this declaration or move the "
                                    "declaration in other scope")));

                            struct Diagnostic *note = NEW(
                              DiagnosticWithNoteTypecheck,
                              &self,
                              format("this declaration is in conflict with a "
                                     "declaration "
                                     "declared at the location ({d}:{d})",
                                     ((struct ClassBodyItem *)get__Vec(
                                        *class->value.class->body, j))
                                       ->loc.s_line,
                                     ((struct ClassBodyItem *)get__Vec(
                                        *class->value.class->body, j))
                                       ->loc.s_col),
                              ((struct ClassBodyItem *)get__Vec(
                                 *class->value.class->body, i))
                                ->loc,
                              from__String(""),
                              None());

                            emit__Diagnostic(error);
                            emit__Diagnostic(note);

                            break;
                        }
                    }
                }
            }
        }
    }
}

void
verify_if_decl_is_duplicate_in_tag(struct Typecheck self, struct Decl *tag)
{
    if (tag->value.tag->body)
        verify_if_decl_is_duplicate_in_module(self, tag);
}

void
verify_if_decl_is_duplicate_in_trait(struct Typecheck self, struct Decl *trait)
{
    if (trait->value.trait->body) {
    }
}

#define NEXT_DECL()                                                      \
    pos += 1;                                                            \
    self->decl = pos < len__Vec(*self->parser.decls)                     \
                   ? ((struct Decl *)get__Vec(*self->parser.decls, pos)) \
                   : NULL

#define ALLOC_FUNS() \
    if (!self->funs) \
    self->funs = NEW(Vec, sizeof(struct FunSymbol))

#define ALLOC_CONSTS() \
    if (!self->consts) \
    self->consts = NEW(Vec, sizeof(struct ConstantSymbol))

#define ALLOC_MODULES() \
    if (!self->modules) \
    self->modules = NEW(Vec, sizeof(struct ModuleSymbol))

#define ALLOC_ALIASES() \
    if (!self->aliases) \
    self->aliases = NEW(Vec, sizeof(struct AliasSymbol))

#define ALLOC_RECORDS() \
    if (!self->records) \
    self->records = NEW(Vec, sizeof(struct RecordSymbol))

#define ALLOC_RECORDS_OBJ() \
    if (!self->records_obj) \
    self->records_obj = NEW(Vec, sizeof(struct RecordObjSymbol))

#define ALLOC_ENUMS() \
    if (!self->enums) \
    self->enums = NEW(Vec, sizeof(struct EnumSymbol))

#define ALLOC_ENUMS_OBJ() \
    if (!self->enums_obj) \
    self->enums_obj = NEW(Vec, sizeof(struct EnumObjSymbol))

#define ALLOC_ERRORS() \
    if (!self->errors) \
    self->errors = NEW(Vec, sizeof(struct ErrorSymbol))

#define ALLOC_CLASSES() \
    if (!self->classes) \
    self->classes = NEW(Vec, sizeof(struct ClassSymbol))

#define ALLOC_TRAITS() \
    if (!self->traits) \
    self->traits = NEW(Vec, sizeof(struct TraitSymbol))

void
push_all_symbols(struct Typecheck *self)
{
    while (pos < len__Vec(*self->parser.decls)) {
        switch (self->decl->kind) {
            case DeclKindFun:
                ALLOC_FUNS();
                push__Vec(self->funs, NEW(FunSymbol, self->decl));
                break;
            case DeclKindConstant:
                ALLOC_CONSTS();
                push__Vec(self->consts, NEW(ConstantSymbol, self->decl));
                break;
            case DeclKindModule:
                ALLOC_MODULES();
                push__Vec(self->modules, NEW(ModuleSymbol, self->decl));
                break;
            case DeclKindAlias:
                ALLOC_ALIASES();
                push__Vec(self->aliases, NEW(AliasSymbol, self->decl));
                break;
            case DeclKindRecord:
                if (self->decl->value.record->is_object) {
                    ALLOC_RECORDS_OBJ();
                    push__Vec(self->records_obj,
                              NEW(RecordObjSymbol, self->decl));
                } else {
                    ALLOC_RECORDS();
                    push__Vec(self->records, NEW(RecordSymbol, self->decl));
                }
                break;
            case DeclKindEnum:
                if (self->decl->value.enum_->is_object) {
                    ALLOC_ENUMS_OBJ();
                    push__Vec(self->enums_obj, NEW(EnumObjSymbol, self->decl));
                } else {
                    ALLOC_ENUMS();
                    push__Vec(self->enums, NEW(EnumSymbol, self->decl));
                }
                break;
            case DeclKindError:
                ALLOC_ERRORS();
                push__Vec(self->errors, NEW(ErrorSymbol, self->decl));
                break;
            case DeclKindClass:
                ALLOC_CLASSES();
                push__Vec(self->classes, NEW(ClassSymbol, self->decl));
                break;
            case DeclKindTrait:
                ALLOC_TRAITS();
                push__Vec(self->traits, NEW(TraitSymbol, self->decl));
                break;
            case DeclKindTag:
                break;
            case DeclKindImport:
                TODO("import");
                break;
            default:
                UNREACHABLE("unknown decl kind");
        }

        NEXT_DECL();
    }

    self->decl = pos > 0 ? get__Vec(*self->parser.decls, 0) : NULL;
    pos = 0;
}

struct Vec *
get_local_decl(struct Typecheck *self, struct Scope *scope)
{
    struct Vec *local = NULL;

    switch (scope->item_kind) {
        case ScopeItemKindEnum:
        case ScopeItemKindClass:
        case ScopeItemKindRecord:
        case ScopeItemKindRecordObj:
        case ScopeItemKindEnumObj:
        case ScopeItemKindTrait:
            break;
        default:
            UNREACHABLE("");
    }

    return local;
}

void
check_constant(struct Typecheck *self,
               struct ConstantSymbol *constant,
               Usize id,
               struct Scope *previous)
{
    if (!constant->scope) {
        constant->scope =
          NEW(Scope,
              self->parser.parse_block.scanner.src->file.name,
              constant->name,
              id,
              ScopeItemKindConstant,
              constant->visibility ? ScopeKindGlobal : ScopeKindLocal,
              previous);

        constant->data_type = check_data_type(
          self,
          constant->constant_decl->loc,
          constant->constant_decl->value.constant->data_type,
          NULL,
          NULL,
          (struct SearchContext){ .search_class = true,
                                  .search_type = true,
                                  .search_value = false,
                                  .search_fun = false,
                                  .search_object = true,
                                  .search_trait = true,
                                  .search_variant = false,
                                  .search_primary_type = true });

        constant->expr_symbol =
          check_expression(self,
                           NULL,
                           constant->constant_decl->value.constant->expr,
                           NULL,
                           NULL,
                           NULL,
                           false);

        if (!previous)
            ++count_const_id;
    }
}

void
check_module(struct Typecheck *self,
             struct ModuleSymbol *module,
             Usize id,
             struct Scope *previous)
{
    if (!module->scope) {
        module->scope =
          NEW(Scope,
              self->parser.parse_block.scanner.src->file.name,
              module->name,
              id,
              ScopeItemKindModule,
              module->visibility ? ScopeKindGlobal : ScopeKindLocal,
              previous);

        struct Vec *local_import_value = NEW(Vec, sizeof(struct Vec));

        if (module->module_decl->value.module->body) {
            module->body = NEW(Vec, sizeof(struct SymbolTable));

            for (Usize i = 0;
                 i < len__Vec(*module->module_decl->value.module->body);
                 i++) {
                if (((struct ModuleBodyItem *)get__Vec(
                       *module->module_decl->value.module->body, i))
                      ->kind == ModuleBodyItemKindDecl)
                    for (Usize j = i + 1;
                         j < len__Vec(*module->module_decl->value.module->body);
                         j++)
                        if (((struct ModuleBodyItem *)get__Vec(
                               *module->module_decl->value.module->body, j))
                              ->kind == ModuleBodyItemKindDecl)
                            if (eq__String(
                                  get_name__Decl(
                                    ((struct ModuleBodyItem *)get__Vec(
                                       *module->module_decl->value.module->body,
                                       i))
                                      ->value.decl),
                                  get_name__Decl(
                                    ((struct ModuleBodyItem *)get__Vec(
                                       *module->module_decl->value.module->body,
                                       j))
                                      ->value.decl),
                                  false)) {
                                struct Decl *i1 =
                                  ((struct ModuleBodyItem *)get__Vec(
                                     *module->module_decl->value.module->body,
                                     i))
                                    ->value.decl;
                                struct Decl *i2 =
                                  ((struct ModuleBodyItem *)get__Vec(
                                     *module->module_decl->value.module->body,
                                     j))
                                    ->value.decl;

                                if (((i1->kind == DeclKindAlias ||
                                      i1->kind == DeclKindClass ||
                                      i1->kind == DeclKindEnum ||
                                      i1->kind == DeclKindError ||
                                      i1->kind == DeclKindRecord ||
                                      i1->kind == DeclKindTrait) &&
                                     (i2->kind == DeclKindAlias ||
                                      i2->kind == DeclKindClass ||
                                      i2->kind == DeclKindEnum ||
                                      i2->kind == DeclKindError ||
                                      i2->kind == DeclKindRecord ||
                                      i2->kind == DeclKindTrait)) ||
                                    (i1->kind == DeclKindFun &&
                                     i2->kind == DeclKindFun) ||
                                    ((i1->kind == DeclKindModule ||
                                      i1->kind == DeclKindConstant) &&
                                     (i2->kind == DeclKindModule ||
                                      i2->kind == DeclKindConstant))) {
                                    struct Diagnostic *error = NEW(
                                      DiagnosticWithErrTypecheck,
                                      self,
                                      NEW(LilyError,
                                          LilyErrorDuplicateDeclaration),
                                      i2->loc,
                                      from__String(""),
                                      Some(from__String(
                                        "remove this declaration or move the "
                                        "declaration in other scope")));

                                    struct Diagnostic *note = NEW(
                                      DiagnosticWithNoteTypecheck,
                                      self,
                                      format(
                                        "this declaration is in conflict with "
                                        "a "
                                        "declaration "
                                        "declared at the location ({d}:{d})",
                                        i2->loc.s_line,
                                        i2->loc.s_col),
                                      i1->loc,
                                      from__String(""),
                                      None());

                                    emit__Diagnostic(error);
                                    emit__Diagnostic(note);
                                }
                            }

                switch (((struct Decl *)get__Vec(
                           *module->module_decl->value.module->body, i))
                          ->kind) {
                    case DeclKindFun: {
                        struct SymbolTable *fun_symb = NEW(
                          SymbolTableFun,
                          NEW(FunSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));
                        check_fun(self, fun_symb->value.fun, i, module->scope);
                        push__Vec(module->body, fun_symb);

                        break;
                    }
                    case DeclKindConstant: {
                        struct SymbolTable *constant_symb = NEW(
                          SymbolTableConstant,
                          NEW(ConstantSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));
                        check_constant(self,
                                       constant_symb->value.constant,
                                       i,
                                       module->scope);
                        push__Vec(module->body, constant_symb);

                        break;
                    }
                    case DeclKindModule: {
                        struct SymbolTable *module_symb = NEW(
                          SymbolTableModule,
                          NEW(ModuleSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));
                        check_module(
                          self, module_symb->value.module, i, module->scope);
                        push__Vec(module->body, module_symb);

                        break;
                    }
                    case DeclKindAlias: {
                        struct SymbolTable *alias_symb = NEW(
                          SymbolTableAlias,
                          NEW(AliasSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));
                        check_alias(
                          self, alias_symb->value.alias, i, module->scope);
                        push__Vec(module->body, alias_symb);

                        break;
                    }
                    case DeclKindRecord: {
                        struct SymbolTable *record_symb = NEW(
                          SymbolTableRecord,
                          NEW(RecordSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));

                        check_record(
                          self, record_symb->value.record, i, module->scope);
                        push__Vec(module->body, record_symb);

                        break;
                    }
                    case DeclKindEnum: {
                        struct SymbolTable *enum_symb = NEW(
                          SymbolTableEnum,
                          NEW(EnumSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));

                        check_enum(
                          self, enum_symb->value.enum_, i, module->scope);
                        push__Vec(module->body, enum_symb);

                        break;
                    }
                    case DeclKindError: {
                        struct SymbolTable *error_symb = NEW(
                          SymbolTableError,
                          NEW(ErrorSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));

                        check_error(
                          self, error_symb->value.error, i, module->scope);
                        push__Vec(module->body, error_symb);

                        break;
                    }
                    case DeclKindClass: {
                        struct SymbolTable *class_symb = NEW(
                          SymbolTableClass,
                          NEW(ClassSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));

                        check_class(
                          self, class_symb->value.class, i, module->scope);
                        push__Vec(module->body, class_symb);

                        break;
                    }
                    case DeclKindTrait: {
                        struct SymbolTable *trait_symb = NEW(
                          SymbolTableTrait,
                          NEW(TraitSymbol,
                              get__Vec(*module->module_decl->value.module->body,
                                       i)));

                        check_trait(
                          self, trait_symb->value.trait, i, module->scope);
                        push__Vec(module->body, trait_symb);

                        break;
                    }
                    case DeclKindImport:
                        break;
                    case DeclKindTag:
                        break;
                }
            }
        }

        if (!previous)
            ++count_module_id;
    }
}

void
check_alias(struct Typecheck *self,
            struct AliasSymbol *alias,
            Usize id,
            struct Scope *previous)
{
    if (!alias->scope) {
        alias->scope = NEW(Scope,
                           self->parser.parse_block.scanner.src->file.name,
                           alias->name,
                           id,
                           ScopeItemKindAlias,
                           alias->visibility ? ScopeKindGlobal : ScopeKindLocal,
                           previous);

        if (alias->alias_decl->value.alias->generic_params) {

            alias->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*alias->alias_decl->value.alias->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j <
                     len__Vec(*alias->alias_decl->value.alias->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *alias->alias_decl->value.alias->generic_params,
                            i)),
                          get_name__Generic(get__Vec(
                            *alias->alias_decl->value.alias->generic_params,
                            j)),
                          false)) {
                        struct Diagnostic *err = NEW(
                          DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorDuplicateGenericParam),
                          ((struct Generic *)get__Vec(
                             *alias->alias_decl->value.alias->generic_params,
                             j))
                            ->loc,
                          from__String(""),
                          Some(from__String("rename this generic argument")));

                        emit__Diagnostic(err);
                    }

                // Push generic params of enum_decl in enum_->generic_params
                switch (((struct Generic *)get__Vec(
                           *alias->alias_decl->value.alias->generic_params, i))
                          ->kind) {
                    case GenericKindDataType:
                        push__Vec(
                          alias->generic_params,
                          NEW(
                            GenericDataType,
                            get_name__Generic(get__Vec(
                              *alias->alias_decl->value.alias->generic_params,
                              i)),
                            ((struct Generic *)get__Vec(
                               *alias->alias_decl->value.alias->generic_params,
                               i))
                              ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (!previous)
            ++count_alias_id;

        TODO("Search data type");
    }
}

void
check_enum(struct Typecheck *self,
           struct EnumSymbol *enum_,
           Usize id,
           struct Scope *previous)
{
    if (!enum_->scope) {
        enum_->scope = NEW(Scope,
                           self->parser.parse_block.scanner.src->file.name,
                           enum_->name,
                           id,
                           ScopeItemKindEnum,
                           enum_->visibility ? ScopeKindGlobal : ScopeKindLocal,
                           previous);

        if (enum_->enum_decl->value.enum_->generic_params) {

            enum_->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*enum_->enum_decl->value.enum_->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j <
                     len__Vec(*enum_->enum_decl->value.enum_->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *enum_->enum_decl->value.enum_->generic_params, i)),
                          get_name__Generic(get__Vec(
                            *enum_->enum_decl->value.enum_->generic_params, j)),
                          false)) {
                        struct Diagnostic *err = NEW(
                          DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorDuplicateGenericParam),
                          ((struct Generic *)get__Vec(
                             *enum_->enum_decl->value.enum_->generic_params, j))
                            ->loc,
                          from__String(""),
                          Some(from__String("rename this generic argument")));

                        emit__Diagnostic(err);
                    }

                // Push generic params of enum_decl in enum_->generic_params
                switch (((struct Generic *)get__Vec(
                           *enum_->enum_decl->value.enum_->generic_params, i))
                          ->kind) {
                    case GenericKindDataType:
                        push__Vec(
                          enum_->generic_params,
                          NEW(GenericDataType,
                              get_name__Generic(get__Vec(
                                *enum_->enum_decl->value.enum_->generic_params,
                                i)),
                              ((struct Generic *)get__Vec(
                                 *enum_->enum_decl->value.enum_->generic_params,
                                 i))
                                ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (enum_->enum_decl->value.enum_->variants) {
            enum_->variants = NEW(Vec, sizeof(struct VariantEnumSymbol));

            // Check if enum's variant name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*enum_->enum_decl->value.enum_->variants);
                 i++) {
                for (Usize j = i + 1;
                     j < len__Vec(*enum_->enum_decl->value.enum_->variants);
                     j++)
                    if (eq__String(
                          ((struct VariantEnum *)get__Vec(
                             *enum_->enum_decl->value.enum_->variants, i))
                            ->name,
                          ((struct VariantEnum *)get__Vec(
                             *enum_->enum_decl->value.enum_->variants, j))
                            ->name,
                          false)) {
                        struct Diagnostic *err =
                          NEW(DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError, LilyErrorDuplicateVariant),
                              ((struct Generic *)get__Vec(
                                 *enum_->enum_decl->value.enum_->variants, j))
                                ->loc,
                              from__String(""),
                              Some(from__String("rename this variant")));

                        emit__Diagnostic(err);
                    }

                // Push variant enum in enum_->variants
                if (((struct VariantEnum *)get__Vec(
                       *enum_->enum_decl->value.enum_->variants, i))
                      ->data_type) {
                    TODO("Check data type");
                } else
                    push__Vec(
                      enum_->variants,
                      NEW(
                        VariantEnumSymbol,
                        get__Vec(*enum_->enum_decl->value.enum_->variants, i)));
            }
        }

        if (!previous)
            ++count_enum_id;
    }
}

void
check_record(struct Typecheck *self,
             struct RecordSymbol *record,
             Usize id,
             struct Scope *previous)
{
    if (!record->scope) {
        record->scope =
          NEW(Scope,
              self->parser.parse_block.scanner.src->file.name,
              record->name,
              id,
              ScopeItemKindRecord,
              record->visibility ? ScopeKindGlobal : ScopeKindLocal,
              previous);

        if (record->record_decl->value.record->generic_params) {

            record->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i <
                 len__Vec(*record->record_decl->value.record->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j < len__Vec(
                           *record->record_decl->value.record->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *record->record_decl->value.record->generic_params,
                            i)),
                          get_name__Generic(get__Vec(
                            *record->record_decl->value.record->generic_params,
                            j)),
                          false)) {
                        struct Diagnostic *err = NEW(
                          DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorDuplicateGenericParam),
                          ((struct Generic *)get__Vec(
                             *record->record_decl->value.record->generic_params,
                             j))
                            ->loc,
                          from__String(""),
                          Some(from__String("rename this generic argument")));

                        emit__Diagnostic(err);
                    }

                // Push generic params of record_decl in record->generic_params
                switch (
                  ((struct Generic *)get__Vec(
                     *record->record_decl->value.record->generic_params, i))
                    ->kind) {
                    case GenericKindDataType:
                        push__Vec(record->generic_params,
                                  NEW(GenericDataType,
                                      get_name__Generic(
                                        get__Vec(*record->record_decl->value
                                                    .record->generic_params,
                                                 i)),
                                      ((struct Generic *)get__Vec(
                                         *record->record_decl->value.record
                                            ->generic_params,
                                         i))
                                        ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (record->record_decl->value.record->fields) {
            record->fields = NEW(Vec, sizeof(struct VariantEnumSymbol));

            // Check if record's field name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*record->record_decl->value.record->fields);
                 i++) {
                for (Usize j = i + 1;
                     j < len__Vec(*record->record_decl->value.record->fields);
                     j++)
                    if (eq__String(
                          ((struct FieldRecord *)get__Vec(
                             *record->record_decl->value.record->fields, i))
                            ->name,
                          ((struct FieldRecord *)get__Vec(
                             *record->record_decl->value.record->fields, j))
                            ->name,
                          false)) {
                        struct Diagnostic *err =
                          NEW(DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError, LilyErrorDuplicateField),
                              ((struct Generic *)get__Vec(
                                 *record->record_decl->value.record->fields, j))
                                ->loc,
                              from__String(""),
                              Some(from__String("rename this field")));

                        emit__Diagnostic(err);
                    }

                // Push field record in record->fields
                if (((struct FieldRecord *)get__Vec(
                       *record->record_decl->value.record->fields, i))
                      ->data_type) {
                    TODO("Check data type");
                } else
                    push__Vec(
                      record->fields,
                      NEW(FieldRecordSymbol,
                          get__Vec(*record->record_decl->value.record->fields,
                                   i)));
            }
        }

        if (!previous)
            ++count_record_id;
    }
}

void
check_error(struct Typecheck *self,
            struct ErrorSymbol *error,
            Usize id,
            struct Scope *previous)
{
    if (!error->scope) {
        error->scope = NEW(Scope,
                           self->parser.parse_block.scanner.src->file.name,
                           error->name,
                           id,
                           ScopeItemKindError,
                           error->visibility ? ScopeKindGlobal : ScopeKindLocal,
                           previous);

        // Check generic params
        if (error->error_decl->value.error->generic_params) {
            error->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*error->error_decl->value.error->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j <
                     len__Vec(*error->error_decl->value.error->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *error->error_decl->value.error->generic_params,
                            i)),
                          get_name__Generic(get__Vec(
                            *error->error_decl->value.error->generic_params,
                            j)),
                          false)) {
                        struct Diagnostic *err = NEW(
                          DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorDuplicateGenericParam),
                          ((struct Generic *)get__Vec(
                             *error->error_decl->value.error->generic_params,
                             j))
                            ->loc,
                          from__String(""),
                          Some(from__String("rename this generic argument")));

                        emit__Diagnostic(err);
                    }

                // Push generic params of error in error->generic_params
                switch (((struct Generic *)get__Vec(
                           *error->error_decl->value.error->generic_params, i))
                          ->kind) {
                    case GenericKindDataType:
                        push__Vec(
                          error->generic_params,
                          NEW(
                            GenericDataType,
                            get_name__Generic(get__Vec(
                              *error->error_decl->value.error->generic_params,
                              i)),
                            ((struct Generic *)get__Vec(
                               *error->error_decl->value.error->generic_params,
                               i))
                              ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (error->error_decl->value.error->data_type) {
            TODO("check data type");
        }

        if (!previous)
            ++count_error_id;
    }
}

void
check_enum_obj(struct Typecheck *self,
               struct EnumObjSymbol *enum_obj,
               Usize id,
               struct Scope *previous)
{
    if (!enum_obj->scope) {
        enum_obj->scope =
          NEW(Scope,
              self->parser.parse_block.scanner.src->file.name,
              enum_obj->name,
              id,
              ScopeItemKindEnumObj,
              enum_obj->visibility ? ScopeKindGlobal : ScopeKindLocal,
              previous);

        if (enum_obj->enum_decl->value.enum_->generic_params) {
            enum_obj->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i <
                 len__Vec(*enum_obj->enum_decl->value.enum_->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j < len__Vec(
                           *enum_obj->enum_decl->value.enum_->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *enum_obj->enum_decl->value.enum_->generic_params,
                            i)),
                          get_name__Generic(get__Vec(
                            *enum_obj->enum_decl->value.enum_->generic_params,
                            j)),
                          false)) {
                        struct Diagnostic *err = NEW(
                          DiagnosticWithErrTypecheck,
                          self,
                          NEW(LilyError, LilyErrorDuplicateGenericParam),
                          ((struct Generic *)get__Vec(
                             *enum_obj->enum_decl->value.enum_->generic_params,
                             j))
                            ->loc,
                          from__String(""),
                          Some(from__String("rename this generic argument")));

                        emit__Diagnostic(err);
                    }

                // Push generic params of enum_obj in enum_obj->generic_params
                switch (
                  ((struct Generic *)get__Vec(
                     *enum_obj->enum_decl->value.enum_->generic_params, i))
                    ->kind) {
                    case GenericKindDataType:
                        push__Vec(
                          enum_obj->generic_params,
                          NEW(
                            GenericDataType,
                            get_name__Generic(get__Vec(
                              *enum_obj->enum_decl->value.enum_->generic_params,
                              i)),
                            ((struct Generic *)get__Vec(
                               *enum_obj->enum_decl->value.enum_
                                  ->generic_params,
                               i))
                              ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (enum_obj->enum_decl->value.enum_->type_value) {
        }

        if (enum_obj->enum_decl->value.enum_->variants) {
        }

        if (!previous)
            ++count_enum_obj_id;
    }
}

void
check_record_obj(struct Typecheck *self,
                 struct RecordObjSymbol *record_obj,
                 Usize id,
                 struct Scope *previous)
{
    if (!record_obj->scope) {
        record_obj->scope =
          NEW(Scope,
              self->parser.parse_block.scanner.src->file.name,
              record_obj->name,
              id,
              ScopeItemKindRecordObj,
              record_obj->visibility ? ScopeKindGlobal : ScopeKindLocal,
              previous);

        if (record_obj->record_decl->value.record->generic_params) {
            record_obj->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i < len__Vec(
                       *record_obj->record_decl->value.record->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j <
                     len__Vec(
                       *record_obj->record_decl->value.record->generic_params);
                     j++)
                    if (eq__String(get_name__Generic(
                                     get__Vec(*record_obj->record_decl->value
                                                 .record->generic_params,
                                              i)),
                                   get_name__Generic(
                                     get__Vec(*record_obj->record_decl->value
                                                 .record->generic_params,
                                              j)),
                                   false))
                        assert(0 && "error: duplicate generic param name");

                // Push generic params of record_obj in
                // record_obj->generic_params
                switch (
                  ((struct Generic *)get__Vec(
                     *record_obj->record_decl->value.record->generic_params, i))
                    ->kind) {
                    case GenericKindDataType:
                        push__Vec(record_obj->generic_params,
                                  NEW(GenericDataType,
                                      get_name__Generic(
                                        get__Vec(*record_obj->record_decl->value
                                                    .record->generic_params,
                                                 i)),
                                      ((struct Generic *)get__Vec(
                                         *record_obj->record_decl->value.record
                                            ->generic_params,
                                         i))
                                        ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (record_obj->record_decl->value.record->fields) {
        }

        // Search tag
        {
        }

        if (!previous)
            ++count_record_obj_id;
    }
}

void
check_class(struct Typecheck *self,
            struct ClassSymbol *class,
            Usize id,
            struct Scope *previous)
{
    if (!class->scope) {
        class->scope = NEW(Scope,
                           self->parser.parse_block.scanner.src->file.name,
                           class->name,
                           id,
                           ScopeItemKindClass,
                           class->visibility ? ScopeKindGlobal : ScopeKindLocal,
                           previous);

        if (class->class_decl->value.class->generic_params) {
            class->generic_params = NEW(Vec, sizeof(struct Generic));

            // Check if generic param name is duplicate
            for (Usize i = 0;
                 i < len__Vec(*class->class_decl->value.class->generic_params);
                 i++) {
                for (Usize j = i + 1;
                     j <
                     len__Vec(*class->class_decl->value.class->generic_params);
                     j++)
                    if (eq__String(
                          get_name__Generic(get__Vec(
                            *class->class_decl->value.class->generic_params,
                            i)),
                          get_name__Generic(get__Vec(
                            *class->class_decl->value.class->generic_params,
                            j)),
                          false))
                        assert(0 && "error: duplicate generic param name");

                // Push generic params of class in class->generic_params
                switch (((struct Generic *)get__Vec(
                           *class->class_decl->value.class->generic_params, i))
                          ->kind) {
                    case GenericKindDataType:
                        push__Vec(
                          class->generic_params,
                          NEW(
                            GenericDataType,
                            get_name__Generic(get__Vec(
                              *class->class_decl->value.class->generic_params,
                              i)),
                            ((struct Generic *)get__Vec(
                               *class->class_decl->value.class->generic_params,
                               i))
                              ->loc));
                        break;
                    case GenericKindRestrictedDataType:
                        TODO("check data type");
                        break;
                }
            }
        }

        if (class->class_decl->value.class->impl) {
            struct SearchContext s = { .search_value = false,
                                       .search_fun = false,
                                       .search_type = false,
                                       .search_variant = false,
                                       .search_trait = true,
                                       .search_class = false,
                                       .search_primary_type = false };

            for (Usize i = len__Vec(*class->class_decl->value.class->impl);
                 i--;) {
                TODO("check data type");
            }
        }

        if (class->class_decl->value.class->inheritance) {
            struct SearchContext ctx = { .search_type = false,
                                         .search_fun = false,
                                         .search_variant = false,
                                         .search_value = false,
                                         .search_trait = false,
                                         .search_class = true,
                                         .search_object = false };
            struct Vec *local_decl = NULL;

            if (!previous)
                local_decl = get_local_decl(self, class->scope);

            for (Usize i =
                   len__Vec(*class->class_decl->value.class->inheritance);
                 i--;) {
                push__Vec(
                  class->inheritance,
                  check_data_type(
                    self,
                    *(struct Location
                        *)((struct Tuple *)get__Vec(
                             *class->class_decl->value.class->inheritance, i))
                       ->items[1],
                    ((struct Tuple *)get__Vec(
                       *class->class_decl->value.class->inheritance, i))
                      ->items[0],
                    NULL,
                    local_decl,
                    ctx));
            }
        }

        if (class->class_decl->value.class->body) {
        }

        if (!previous)
            ++count_class_id;
    }
}

void
check_trait(struct Typecheck *self,
            struct TraitSymbol *trait,
            Usize id,
            struct Scope *previous)
{
    if (!trait->scope) {
        trait->scope = NEW(Scope,
                           self->parser.parse_block.scanner.src->file.name,
                           trait->name,
                           id,
                           ScopeItemKindTrait,
                           trait->visibility ? ScopeKindGlobal : ScopeKindLocal,
                           previous);

        if (trait->trait_decl->value.trait->inh) {
            for (Usize i = len__Vec(*trait->trait_decl->value.trait->inh);
                 i--;) {
                TODO("check data type");
            }
        }

        if (trait->trait_decl->value.trait->body) {
            TODO("check body");
        }

        if (!previous)
            ++count_trait_id;
    }
}

void
check_fun(struct Typecheck *self,
          struct FunSymbol *fun,
          Usize id,
          struct Scope *previous)
{
    if (!fun->scope) {
        fun->scope = NEW(Scope,
                         self->parser.parse_block.scanner.src->file.name,
                         fun->name,
                         id,
                         ScopeItemKindFun,
                         fun->visibility ? ScopeKindGlobal : ScopeKindLocal,
                         previous);

        struct FunDecl *fun_decl = fun->fun_decl->value.fun;
        struct Vec *local_data_type = NULL; // struct Vec<struct Scope*>*
        struct Vec *local_value = NULL;     // struct Vec<struct Scope*>*
        struct Vec *local_decl = NULL;      // struct Vec<struct Scope*>*
        struct Vec *tagged_type = NULL;
        struct Vec *generic_params = NULL;
        struct Vec *params = NULL;
        struct DataTypeSymbol *return_type = NULL;
        struct Vec *body = NULL;

        if (fun_decl->tags) {
            tagged_type = NEW(Vec, sizeof(struct Tuple));

            for (Usize i = len__Vec(*fun_decl->tags); i--;) {
                struct Tuple *current = get__Vec(*fun_decl->tags, i);
                struct DataTypeSymbol *dts = check_data_type(
                  self,
                  *(struct Location *)current->items[1],
                  current->items[0],
                  local_data_type,
                  local_decl,
                  (struct SearchContext){ .search_type = false,
                                          .search_fun = false,
                                          .search_variant = false,
                                          .search_value = false,
                                          .search_trait = false,
                                          .search_class = false,
                                          .search_object = true,
                                          .search_primary_type = false });

                if (dts)
                    push__Vec(tagged_type, dts);
            }

            SUMMARY();
        }

        if (fun_decl->generic_params) {
            local_data_type = NEW(Vec, sizeof(struct LocalDataType));

            for (Usize i = len__Vec(*fun_decl->generic_params); i--;) {
                switch (
                  ((struct Generic *)get__Vec(*fun_decl->generic_params, i))
                    ->kind) {
                    case GenericKindDataType:
                        push__Vec(local_data_type,
                                  NEW(LocalDataType,
                                      ((struct Generic *)get__Vec(
                                         *fun_decl->generic_params, i))
                                        ->value.data_type,
                                      NULL));
                        break;
                    case GenericKindRestrictedDataType: {
                        struct DataTypeSymbol *dts = check_data_type(
                          self,
                          *(struct Location
                              *)((struct Tuple *)((struct Generic *)get__Vec(
                                                    *fun_decl->generic_params,
                                                    i))
                                   ->value.restricted_data_type->items[1])
                             ->items[1],
                          ((struct Tuple *)((struct Generic *)get__Vec(
                                              *fun_decl->generic_params, i))
                             ->value.restricted_data_type->items[1])
                            ->items[0],
                          local_data_type,
                          NULL,
                          (struct SearchContext){ .search_type = false,
                                                  .search_fun = false,
                                                  .search_variant = false,
                                                  .search_value = false,
                                                  .search_trait = false,
                                                  .search_class = false,
                                                  .search_object = false,
                                                  .search_primary_type =
                                                    false });

                        if (dts)
                            push__Vec(
                              local_data_type,
                              NEW(LocalDataType,
                                  ((struct Generic *)get__Vec(
                                     *fun_decl->generic_params, i))
                                    ->value.restricted_data_type->items[0],
                                  NEW(Tuple,
                                      2,
                                      dts,
                                      *(struct Location
                                          *)((struct Tuple
                                                *)((struct Generic *)get__Vec(
                                                     *fun_decl->generic_params,
                                                     i))
                                               ->value.restricted_data_type
                                               ->items[1])
                                         ->items[1])));
                        else
                            assert(0 && "error");

                        break;
                    }
                }
            }

            SUMMARY();
        }

        if (fun_decl->params) {
            params = NEW(Vec, sizeof(struct FunParamSymbol));
            local_value = NEW(Vec, sizeof(struct Scope));

            for (Usize i = 0; i < len__Vec(*fun_decl->params); i++) {
                struct FunParamSymbol *param =
                  NEW(FunParamSymbol, get__Vec(*fun_decl->params, i));
                struct Vec *ids = NEW(Vec, sizeof(Usize));
                struct DataTypeSymbol *dts = check_data_type(
                  self,
                  *(struct Location *)((struct FunParam *)get__Vec(
                                         *fun_decl->params, i))
                     ->param_data_type->items[1],
                  ((struct FunParam *)get__Vec(*fun_decl->params, i))
                    ->param_data_type->items[0],
                  local_data_type,
                  local_decl,
                  (struct SearchContext){ .search_type = true,
                                          .search_fun = false,
                                          .search_variant = false,
                                          .search_value = false,
                                          .search_trait = true,
                                          .search_class = true,
                                          .search_object = true,
                                          .search_primary_type = false });

                if (dts) {
                    param->param_data_type =
                      ((struct FunParam *)get__Vec(*fun_decl->params, i))
                          ->param_data_type
                        ? NEW(
                            Tuple,
                            2,
                            dts,
                            ((struct FunParam *)get__Vec(*fun_decl->params, i))
                              ->param_data_type->items[1])
                        : NULL;
                }

                if (param->default_)
                    TODO("");

                push__Vec(params, param);
                push__Vec(local_value,
                          NEW(Scope,
                              self->parser.parse_block.scanner.src->file.name,
                              param->name,
                              i,
                              ScopeItemKindParam,
                              ScopeKindLocal,
                              NULL));
            }
        }

        if (fun_decl->return_type) {
            struct DataTypeSymbol *dts = check_data_type(
              self,
              *(struct Location *)fun_decl->return_type->items[1],
              fun_decl->return_type->items[0],
              local_data_type,
              local_decl,
              (struct SearchContext){ .search_type = true,
                                      .search_fun = false,
                                      .search_variant = false,
                                      .search_value = false,
                                      .search_trait = true,
                                      .search_class = true,
                                      .search_object = true,
                                      .search_primary_type = false });

            if (dts)
                return_type = dts;

            SUMMARY();
        }

        if (fun_decl->body) {
            local_value = NEW(Vec, sizeof(struct Scope));
            body = NEW(Vec, sizeof(struct SymbolTable));

            check_fun_body(self, fun, body, local_value, local_data_type);
        }

        fun->tagged_type = tagged_type;
        fun->generic_params = generic_params;
        fun->params = params;
        fun->return_type = return_type;
        fun->body = body;

        if (!previous)
            ++count_fun_id;

        if (local_data_type) {
            for (Usize i = len__Vec(*local_data_type); i--;)
                FREE(LocalDataType, get__Vec(*local_data_type, i));

            FREE(Vec, local_data_type);
        }

        if (local_value)
            FREE(Vec, local_value);
    }
}

struct SymbolTable *
get_info_of_decl_from_scope(struct Typecheck *self,
                            struct Scope *scope,
                            struct Location loc)
{
    struct Vec *scopes = NEW(Vec, sizeof(struct Scope));

    {
        struct Scope *previous = scope;

        while (previous) {
            push__Vec(scopes, previous);
            previous = scope->previous;
        }
    }

    struct SymbolTable *current = NULL;

    {
        struct Scope *first = get__Vec(*scopes, len__Vec(*scopes) - 1);

        switch (first->item_kind) {
            case ScopeItemKindAlias:
                if (len__Vec(*scopes) > 1) {
                    UNREACHABLE("impossible to have more one scope with "
                                "ScopeItemkindAlias");
                } else {
                    FREE(Vec, scopes);

                    return NEW(SymbolTableAlias,
                               search_in_aliases_from_name(self, first->name));
                }
            case ScopeItemKindRecord:
                if (len__Vec(*scopes) == 1) {
                    FREE(Vec, scopes);

                    return NEW(SymbolTableRecord,
                               search_in_records_from_name(self, first->name));
                } else
                    UNREACHABLE("impossible to have more on scope with "
                                "ScopeItemKindRecord");
            case ScopeItemKindEnum:
                if (len__Vec(*scopes) == 1) {
                    FREE(Vec, scopes);

                    return NEW(SymbolTableEnum,
                               search_in_enums_from_name(self, first->name));
                } else if (len__Vec(*scopes) == 2) {
                    struct Scope *last = get__Vec(*scopes, 0);
                    struct SymbolTable *enum_ =
                      NEW(SymbolTableEnum,
                          search_in_enums_from_name(self, first->name));
                    struct Expr *id = NEW(ExprIdentifier, last->name, loc);
                    struct SymbolTable *variant =
                      search_enum_item_in_scope(self, id, enum_);

                    free(id);
                    free(enum_);
                    FREE(Vec, scopes);

                    return variant;
                } else
                    UNREACHABLE("impossible to have more on scope with "
                                "ScopeItemKindEnum");
            case ScopeItemKindRecordObj:
                break;
            case ScopeItemKindEnumObj:
                break;
            case ScopeItemKindConstant:
                break;
            case ScopeItemKindVariant:
                break;
            case ScopeItemKindFun:
                break;
            case ScopeItemKindError:
                break;
            case ScopeItemKindClass:
                break;
            case ScopeItemKindModule:
                break;
            case ScopeItemKindTrait:
                break;
            default:
                UNREACHABLE("cannot search local value");
        }
    }

    for (Usize i = len__Vec(*scopes) - 1; i--;) {
    }
}

void
check_symbols(struct Typecheck *self)
{
    while (pos < len__Vec(*self->parser.decls)) {
        switch (self->decl->kind) {
            case DeclKindFun:
                check_fun(self,
                          get__Vec(*self->funs, count_fun_id),
                          count_fun_id,
                          NULL);

                break;
            case DeclKindConstant:
                check_constant(self,
                               get__Vec(*self->consts, count_const_id),
                               count_const_id,
                               NULL);

                break;
            case DeclKindModule:
                check_module(self,
                             get__Vec(*self->modules, count_module_id),
                             count_module_id,
                             NULL);

                break;
            case DeclKindAlias:
                check_alias(self,
                            get__Vec(*self->aliases, count_alias_id),
                            count_alias_id,
                            NULL);

                break;
            case DeclKindRecord:
                if (((struct RecordDecl *)get__Vec(*self->parser.decls, pos))
                      ->is_object)
                    check_record_obj(
                      self,
                      get__Vec(*self->records_obj, count_record_obj_id),
                      count_record_obj_id,
                      NULL);
                else
                    check_record(self,
                                 get__Vec(*self->records, count_record_id),
                                 count_record_id,
                                 NULL);

                break;
            case DeclKindEnum:
                if (((struct EnumDecl *)get__Vec(*self->parser.decls, pos))
                      ->is_object)
                    check_enum_obj(
                      self,
                      get__Vec(*self->enums_obj, count_enum_obj_id),
                      count_enum_obj_id,
                      NULL);
                else
                    check_enum(self,
                               get__Vec(*self->enums, count_enum_id),
                               count_enum_id,
                               NULL);

                break;
            case DeclKindError:
                check_error(self,
                            get__Vec(*self->errors, count_error_id),
                            count_error_id,
                            NULL);

                break;
            case DeclKindClass:
                check_class(self,
                            get__Vec(*self->classes, count_class_id),
                            count_class_id,
                            NULL);

                break;
            case DeclKindTrait:
                check_trait(self,
                            get__Vec(*self->traits, count_trait_id),
                            count_trait_id,
                            NULL);

                break;
            case DeclKindTag:
                TODO("push tag");
            case DeclKindImport:
                break;
        }

        NEXT_DECL();
    }
}

struct ModuleSymbol *
entry_in_module(struct Typecheck *self, struct Vec *id, Usize end_idx)
{
    if (!self->modules)
        assert(0 && "error unknown module");

    if (len__Vec(*id) == 0)
        return NULL;

    struct ModuleSymbol *current_module = get__Vec(*self->modules, 0);

    for (Usize i = 1; i < end_idx; i++) {
        current_module =
          ((struct SymbolTable *)get__Vec(*current_module->body,
                                          (Usize)(UPtr)get__Vec(*id, i)))
            ->value.module;
    }

    return current_module;
}

struct Scope *
search_from_access(struct Typecheck *self,
                   struct Expr *id,
                   struct Vec *name,
                   struct SearchContext search_context)
{
    switch (id->kind) {
        case ExprKindGlobalAccess:
            break;
        case ExprKindIdentifierAccess:
            break;
        case ExprKindPropertyAccessInit:
            break;
        default:
            UNREACHABLE("");
    }
}

struct ConstantSymbol *
search_in_consts_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->consts)
        return NULL;

    for (Usize i = len__Vec(*self->consts); i--;) {
        if (eq__String(
              name,
              ((struct ConstantSymbol *)get__Vec(*self->consts, i))->name,
              false)) {
            check_constant(self, get__Vec(*self->consts, i), i, NULL);

            return get__Vec(*self->consts, i);
        }
    }

    return NULL;
}

struct ModuleSymbol *
search_in_modules_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->modules)
        return NULL;

    for (Usize i = len__Vec(*self->modules); i--;) {
        if (eq__String(
              name,
              ((struct ModuleSymbol *)get__Vec(*self->modules, i))->name,
              false)) {
            check_module(self, get__Vec(*self->modules, i), i, NULL);

            return get__Vec(*self->modules, i);
        }
    }

    return NULL;
}

struct FunSymbol *
search_in_funs_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->modules)
        return NULL;

    for (Usize i = len__Vec(*self->funs); i--;) {
        if (eq__String(name,
                       ((struct FunSymbol *)get__Vec(*self->funs, i))->name,
                       false)) {
            check_fun(self, get__Vec(*self->funs, i), i, NULL);

            return get__Vec(*self->funs, i);
        }
    }

    return NULL;
}

struct AliasSymbol *
search_in_aliases_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->aliases)
        return NULL;

    for (Usize i = len__Vec(*self->aliases); i--;) {
        if (eq__String(
              name,
              ((struct AliasSymbol *)get__Vec(*self->aliases, i))->name,
              false)) {
            check_alias(self, get__Vec(*self->aliases, i), i, NULL);

            return get__Vec(*self->aliases, i);
        }
    }

    return NULL;
}

struct EnumSymbol *
search_in_enums_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->enums)
        return NULL;

    for (Usize i = len__Vec(*self->enums); i--;) {
        if (eq__String(name,
                       ((struct EnumSymbol *)get__Vec(*self->enums, i))->name,
                       false)) {
            check_enum(self, get__Vec(*self->enums, i), i, NULL);

            return get__Vec(*self->enums, i);
        }
    }

    return NULL;
}

struct RecordSymbol *
search_in_records_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->records)
        return NULL;

    for (Usize i = len__Vec(*self->records); i--;) {
        if (eq__String(
              name,
              ((struct RecordSymbol *)get__Vec(*self->records, i))->name,
              false)) {
            check_record(self, get__Vec(*self->records, i), i, NULL);

            return get__Vec(*self->records, i);
        }
    }

    return NULL;
}

struct EnumObjSymbol *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->enums_obj)
        return NULL;

    for (Usize i = len__Vec(*self->enums_obj); i--;) {
        if (eq__String(
              name,
              ((struct EnumObjSymbol *)get__Vec(*self->enums_obj, i))->name,
              false)) {
            check_enum_obj(self, get__Vec(*self->enums_obj, i), i, NULL);

            return get__Vec(*self->enums_obj, i);
        }
    }

    return NULL;
}

struct RecordObjSymbol *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->records_obj)
        return NULL;

    for (Usize i = len__Vec(*self->records_obj); i--;) {
        if (eq__String(
              name,
              ((struct RecordObjSymbol *)get__Vec(*self->records_obj, i))->name,
              false)) {
            check_record_obj(self, get__Vec(*self->records_obj, i), i, NULL);

            return get__Vec(*self->records_obj, i);
        }
    }

    return NULL;
}

struct ClassSymbol *
search_in_classes_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->classes)
        return NULL;

    for (Usize i = len__Vec(*self->classes); i--;) {
        if (eq__String(
              name,
              ((struct ClassSymbol *)get__Vec(*self->classes, i))->name,
              false)) {
            check_class(self, get__Vec(*self->classes, i), i, NULL);

            return get__Vec(*self->classes, i);
        }
    }

    return NULL;
}

struct TraitSymbol *
search_in_traits_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->traits)
        return NULL;

    for (Usize i = len__Vec(*self->traits); i--;) {
        if (eq__String(name,
                       ((struct TraitSymbol *)get__Vec(*self->traits, i))->name,
                       false)) {
            check_trait(self, get__Vec(*self->traits, i), i, NULL);

            return get__Vec(*self->traits, i);
        }
    }

    return NULL;
}

struct ErrorSymbol *
search_in_errors_from_name(struct Typecheck *self, struct String *name)
{
    if (!self->errors)
        return NULL;

    for (Usize i = len__Vec(*self->errors); i--;) {
        if (eq__String(name,
                       ((struct ErrorSymbol *)get__Vec(*self->errors, i))->name,
                       false)) {
            check_error(self, get__Vec(*self->errors, i), i, NULL);

            return get__Vec(*self->errors, i);
        }
    }

    return NULL;
}

struct SymbolTable *
search_module_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.module->body); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           ((struct SymbolTable *)get__Vec(
                              *scope->value.module->body, i))
                             ->value.variant->name,
                           false))
                return get__Vec(*scope->value.module->body, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown module item");

    return NULL;
}

struct SymbolTable *
search_enum_item_in_scope(struct Typecheck *self,
                          struct Expr *id,
                          struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.enum_->variants); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           ((struct SymbolTable *)get__Vec(
                              *scope->value.enum_->variants, i))
                             ->value.variant->name,
                           false))
                return get__Vec(*scope->value.enum_->variants, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown variant");

    return NULL;
}

struct SymbolTable *
search_record_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.record->fields); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           ((struct SymbolTable *)get__Vec(
                              *scope->value.record->fields, i))
                             ->value.field->name,
                           false))
                return get__Vec(*scope->value.record->fields, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown field");

    return NULL;
}

struct SymbolTable *
search_enum_obj_item_in_scope(struct Typecheck *self,
                              struct Expr *id,
                              struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.enum_obj->variants); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           ((struct SymbolTable *)get__Vec(
                              *scope->value.enum_obj->variants, i))
                             ->value.variant->name,
                           false))
                return get__Vec(*scope->value.enum_obj->variants, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    for (Usize i = len__Vec(*scope->value.enum_obj->attached); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           get_name__SymbolTable(
                             get__Vec(*scope->value.enum_obj->attached, i)),
                           false))
                return get__Vec(*scope->value.enum_obj->attached, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown field");

    return NULL;
}

struct SymbolTable *
search_record_obj_item_in_scope(struct Typecheck *self,
                                struct Expr *id,
                                struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.record_obj->fields); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           ((struct SymbolTable *)get__Vec(
                              *scope->value.record_obj->fields, i))
                             ->value.field->name,
                           false))
                return get__Vec(*scope->value.record_obj->fields, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    for (Usize i = len__Vec(*scope->value.record_obj->attached); i--;) {
        if (id->kind == TokenKindIdentifier) {
            if (eq__String(id->value.identifier,
                           get_name__SymbolTable(
                             get__Vec(*scope->value.record_obj->attached, i)),
                           false))
                return get__Vec(*scope->value.record_obj->attached, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown field");

    return NULL;
}

struct SymbolTable *
search_class_item_in_scope(struct Typecheck *self,
                           struct Expr *id,
                           struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.class->body); i--;) {
        if (id->kind == ExprKindIdentifier) {
            if (eq__String(
                  id->value.identifier,
                  get_name__SymbolTable(get__Vec(*scope->value.class->body, i)),
                  false))
                return get__Vec(*scope->value.class->body, i);
        } else
            UNREACHABLE("only identifier expression is expected");
    }

    assert(0 && "error: unknown property or method");

    return NULL;
}

struct SymbolTable *
search_in_custom_scope(struct Typecheck *self,
                       struct Expr *id,
                       struct SymbolTable *scope)
{
    struct Vec *body = NULL;     // struct Vec<struct SymbolTable*>*
    struct Vec *variants = NULL; // struct Vec<struct SymbolTable*>*
    struct Vec *fields = NULL;   // struct Vec<struct SymbolTable*>*

    switch (scope->kind) {
        case SymbolTableKindModule:
            if (!scope->value.module->scope) {
                TODO("ananlysis the module");
            }

            body = scope->value.module->body;
            break;
        case SymbolTableKindClass:
            if (!scope->value.class->scope) {
                TODO("analysis the class");
            }

            body = scope->value.class->body;
            break;
        case SymbolTableKindEnum:
            if (!scope->value.enum_->scope) {
                TODO("analysis the enum");
            }

            variants = scope->value.enum_->variants;
            break;
        case SymbolTableKindRecord:
            if (!scope->value.record->scope) {
                TODO("analysis the record");
            }

            fields = scope->value.record->fields;
            break;
        case SymbolTableKindEnumObj:
            if (!scope->value.enum_obj->scope) {
                TODO("analysis the enum obj");
            }

            variants = scope->value.enum_obj->variants;
            body = scope->value.enum_obj->attached;
            break;
        case SymbolTableKindRecordObj:
            if (!scope->value.record_obj->scope) {
                TODO("analysis the record obj");
            }

            fields = scope->value.record_obj->fields;
            body = scope->value.record_obj->attached;
            break;
        default:
            assert(0 && "error: cannot search in scope in this case");
    }

    if (body)
        switch (scope->kind) {
            case SymbolTableKindClass:
                return search_class_item_in_scope(self, id, scope);
            case SymbolTableKindModule:
                return search_module_item_in_scope(self, id, scope);
            default:
                UNREACHABLE("expected module or class");
        }
    else if (body && variants)
        return search_enum_obj_item_in_scope(self, id, scope);
    else if (body && fields)
        return search_record_obj_item_in_scope(self, id, scope);
    else if (variants)
        return search_enum_item_in_scope(self, id, scope);
    else if (fields)
        return search_record_item_in_scope(self, id, scope);

    return NULL;
}

struct Vec *
search_in_funs_from_fun_call(struct Typecheck *self, struct Expr *id)
{
    if (id->kind == ExprKindIdentifier) {
        for (Usize i = len__Vec(*self->funs); i--;) {
            if (eq__String(((struct FunSymbol *)get__Vec(*self->funs, i))->name,
                           id->value.identifier,
                           false))
                return get__Vec(*self->funs, i);
        }
    } else if (id->kind == ExprKindIdentifierAccess) {
        /* struct Scope *scope = search_in_modules_from_name(
          self,
          id->value.identifier_access,
          (struct SearchContext){ .search_fun = true,
                                        .search_type = false,
                                        .search_value = false,
                                        .search_variant = false }); */
    } else
        assert(0 && "error");
}

struct Vec *
identifier_access_to_String_vec(struct Expr *id)
{
    struct Vec *name = NEW(Vec, sizeof(struct String));

    if (id->kind == ExprKindIdentifier) {
        push__Vec(name, id->value.identifier);

        return name;
    } else if (id->kind == ExprKindIdentifierAccess ||
               id->kind == ExprKindGlobalAccess) {
        for (Usize i = 0; i < len__Vec(*id->value.identifier_access); i++) {
            struct Expr *temp = get__Vec(*id->value.identifier_access, i);

            if (temp->kind == TokenKindIdentifier)
                push__Vec(name, temp->value.identifier);
            else
                UNREACHABLE("expected identifier");
        }

        return name;
    } else
        UNREACHABLE("expected identifier or identifier access");
}

struct DataTypeSymbol *
check_data_type(
  struct Typecheck *self,
  struct Location data_type_loc,
  struct DataType *data_type,
  struct Vec *local_data_type,
  struct Vec *local_decl, // If local_decl is equal to NULL the check_data_type
                          // function search by default in global
  struct SearchContext ctx)
{
    if ((ctx.search_object || ctx.search_type || ctx.search_class ||
         ctx.search_trait) &&
        (!ctx.search_primary_type || data_type->kind == DataTypeKindCustom)) {
        switch (data_type->kind) {
            case DataTypeKindCustom: {
            custom_data_type : {
                if (len__Vec(
                      *(struct Vec *)data_type->value.custom->items[0]) == 1) {
                    if (!local_decl) {
                        struct EnumSymbol *enum_ = search_in_enums_from_name(
                          self,
                          get__Vec(
                            (*(struct Vec *)data_type->value.custom->items[0]),
                            0));
                        struct RecordSymbol *record =
                          !enum_
                            ? search_in_records_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0))
                            : NULL;
                        struct EnumObjSymbol *enum_obj =
                          !record && !enum_
                            ? search_in_enums_obj_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0))
                            : NULL;
                        struct RecordObjSymbol *record_obj =
                          !enum_ && !record && !enum_obj
                            ? search_in_records_obj_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0))
                            : NULL;
                        struct ClassSymbol *class =
                          !enum_ && !record && !enum_obj && !record_obj
                            ? search_in_classes_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0))
                            : NULL;
                        struct TraitSymbol *trait =
                          !enum_ && !record && !enum_obj && !record_obj &&
                              !class
                            ? search_in_traits_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0))
                            : NULL;
                        struct Vec *generic_params = NULL;

                        if (!data_type->value.custom->items[1]) {
                        return_data_type : {
                            if (!enum_ && !record && !enum_obj && !record_obj &&
                                !class && !trait) {
                                if (len__Vec(*(struct Vec *)data_type->value
                                                .custom->items[0]) == 1 &&
                                    local_data_type) {
                                    for (Usize i = len__Vec(*local_data_type);
                                         i--;) {
                                        if (eq__String(
                                              ((struct LocalDataType *)get__Vec(
                                                 *local_data_type, i))
                                                ->name,
                                              get__Vec(
                                                *(struct Vec *)data_type->value
                                                   .custom->items[0],
                                                0),
                                              false)) {
                                            return NEW(
                                              DataTypeSymbolCustom,
                                              generic_params,
                                              NULL,
                                              NEW(
                                                Scope,
                                                self->parser.parse_block.scanner
                                                  .src->file.name,
                                                (struct String *)get__Vec(
                                                  (*(struct Vec *)data_type
                                                      ->value.custom->items[0]),
                                                  0),
                                                i,
                                                ScopeItemKindGeneric,
                                                ScopeKindLocal,
                                                NULL));
                                        }
                                    }
                                }

                                /* struct Diagnostic *err =
                                  NEW(DiagnosticWithErrTypecheck,
                                      self,
                                      NEW(LilyError, LilyErrorUnknownDataType),
                                      data_type_loc,
                                      from__String(""),
                                      None());

                                err->err->s = format(
                                  "{S}",
                                  (struct String *)get__Vec(
                                    (*(struct Vec *)
                                        data_type->value.custom->items[0]),
                                    0));

                                emit__Diagnostic(err); */

                                return NULL;
                            } else if (enum_ && ctx.search_type) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           enum_->scope);
                            } else if (record && ctx.search_type) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           record->scope);
                            } else if (!enum_ && !record && ctx.search_object) {
                                struct Diagnostic *err = NEW(
                                  DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(
                                    LilyError,
                                    LilyErrorExpectedEnumObjectOrRecordObject),
                                  data_type_loc,
                                  from__String(""),
                                  None());

                                struct Diagnostic *note = NEW(
                                  DiagnosticWithNoteTypecheck,
                                  self,
                                  from__String("replace `type` by `object` in "
                                               "current declaration"),
                                  enum_ ? enum_->enum_decl->loc
                                        : record->record_decl->loc,
                                  from__String(""),
                                  None());

                                emit__Diagnostic(err);
                                emit__Diagnostic(note);

                                return NULL;
                            } else if (enum_obj) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           enum_obj->scope);

                            } else if (record_obj) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           record_obj->scope);
                            } else if (class) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           class->scope);
                            } else if (trait && ctx.search_trait) {
                                return NEW(DataTypeSymbolCustom,
                                           generic_params,
                                           NULL,
                                           trait->scope);
                            } else if (trait && !ctx.search_trait)
                                assert(0 && "error");
                        }
                        } else {
                            generic_params =
                              NEW(Vec, sizeof(struct DataTypeSymbol));

                            for (Usize i = len__Vec(
                                   *(struct Vec *)
                                      data_type->value.custom->items[1]);
                                 i--;) {
                                struct DataType *current =
                                  get__Vec(*(struct Vec *)
                                              data_type->value.custom->items[1],
                                           i);
                                push__Vec(generic_params,
                                          check_data_type(self,
                                                          data_type_loc,
                                                          current,
                                                          local_data_type,
                                                          local_decl,
                                                          ctx));
                            }

                            goto return_data_type;
                        }
                    } else {
                        struct SearchContext search_module_context = {
                            .search_fun = false,
                            .search_type = true,
                            .search_value = false,
                            .search_variant = false,
                            .search_trait = false,
                            .search_class = false,
                            .search_primary_type = false
                        };
                        /* return NEW(DataTypeSymbolCustom,
                                   data_type->value.custom->items[1],
                                   NULL,
                                   search_in_modules_from_name(
                                     self,
                                     data_type->value.custom->items[0],
                                     search_module_context)); */
                        return NULL;
                    }
                }
                break;
            }
            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrTypecheck,
                      self,
                      NEW(LilyError, LilyErrorExpectedUserDefinedDataType),
                      data_type_loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);

                return NULL;
            }
            }
        }
    } else {
        switch (data_type->kind) {
            case DataTypeKindSelf:
                return NEW(DataTypeSymbol, DataTypeKindSelf);
            case DataTypeKindPtr:
                return NEW(DataTypeSymbolPtr,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.ptr,
                                           local_data_type,
                                           local_decl,
                                           ctx));
            case DataTypeKindRef:
                return NEW(DataTypeSymbolRef,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.ref,
                                           local_data_type,
                                           local_decl,
                                           ctx));
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
            case DataTypeKindAny:
                return NEW(DataTypeSymbol, DataTypeKindAny);
            case DataTypeKindNever:
                return NEW(DataTypeSymbol, DataTypeKindNever);
            case DataTypeKindOptional:
                return NEW(DataTypeSymbolOptional,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.optional,
                                           local_data_type,
                                           local_decl,
                                           ctx));
            case DataTypeKindUnit:
                return NEW(DataTypeSymbol, DataTypeKindUnit);
            case DataTypeKindException:
                return NEW(DataTypeSymbolException,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.optional,
                                           local_data_type,
                                           local_decl,
                                           ctx));
            case DataTypeKindMut:
                return NEW(DataTypeSymbolMut,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.mut,
                                           local_data_type,
                                           local_decl,
                                           ctx));
            case DataTypeKindLambda: {
                struct Vec *params = NEW(Vec, sizeof(struct DataTypeSymbol));

                for (Usize i = len__Vec(
                       *(struct Vec *)data_type->value.lambda->items[0]);
                     i--;)
                    push__Vec(
                      params,
                      check_data_type(
                        self,
                        data_type_loc,
                        get__Vec(
                          *(struct Vec *)data_type->value.lambda->items[0], i),
                        local_data_type,
                        local_decl,
                        ctx));

                return NEW(DataTypeSymbolLambda,
                           params,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.lambda->items[1],
                                           local_data_type,
                                           local_decl,
                                           ctx));
            }
            case DataTypeKindArray:
                return NEW(DataTypeSymbolArray,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.array->items[0],
                                           local_data_type,
                                           local_decl,
                                           ctx),
                           data_type->value.array->items[1]);
            case DataTypeKindTuple: {
                struct Vec *tuple = NEW(Vec, sizeof(struct DataTypeSymbol));

                for (Usize i = len__Vec(*data_type->value.tuple); i--;)
                    push__Vec(
                      tuple,
                      check_data_type(self,
                                      data_type_loc,
                                      get__Vec(*data_type->value.tuple, i),
                                      local_data_type,
                                      local_decl,
                                      ctx));

                return NEW(DataTypeSymbolTuple, tuple);
            }
            case DataTypeKindCustom:
                goto custom_data_type;
            case DataTypeKindCompilerDefined:
                UNREACHABLE("this type is not accessible in AST");
        }
    }

    return NULL;
}

struct StmtSymbol
check_stmt(struct Typecheck *self,
           struct Stmt *stmt,
           struct Vec *local_value,
           bool is_return_type)
{
    switch (stmt->kind) {
        case StmtKindBreak:
            return NEW(StmtSymbol, *stmt);
        case StmtKindNext:
            return NEW(StmtSymbol, *stmt);
        case StmtKindReturn:
            break;
        case StmtKindAwait:
            break;
        default:
            TODO("stmt");
    }
}

struct DataTypeSymbol *
get_data_type_of_expression(struct Typecheck *self,
                            struct ExprSymbol *expr,
                            struct Vec *local_value)
{
    switch (expr->kind) {
        case ExprKindUnaryOp:
            return get_data_type_of_expression(
              self, expr->value.unary_op.right, local_value);
        case ExprKindBinaryOp:
            return get_data_type_of_expression(
              self, expr->value.binary_op.left, local_value);
        case ExprKindFunCall:
            TODO("get data type of fun call");
        case ExprKindRecordCall:
            TODO("get data type of record call");
        case ExprKindIdentifier:
            TODO("get data type of identifier");
        case ExprKindIdentifierAccess:
            TODO("get data type of identifier access");
        case ExprKindArrayAccess:
            TODO("get data type of array access");
        case ExprKindTupleAccess:
            TODO("get data type of tuple access");
        case ExprKindLambda:
            TODO("get data type of lambda");
        case ExprKindTuple:
            TODO("get data type of tuple");
        case ExprKindArray:
            TODO("get data type of array");
        case ExprKindVariant:
            TODO("get data type of variant");
        case ExprKindTry:
            TODO("get data type of try");
        case ExprKindIf:
            TODO("get data type of if");
        case ExprKindBlock:
            UNREACHABLE("block variant is not used");
        case ExprKindQuestionMark:
            TODO("get data type of question mark");
        case ExprKindDereference:
            TODO("get data type of dereference");
        case ExprKindRef:
            TODO("get data type of ref");
        case ExprKindSelf:
            UNREACHABLE("self variant is not used");
        case ExprKindUndef:
            TODO("get data type of undef");
        case ExprKindNil:
            TODO("get data type of nil");
        case ExprKindWildcard:
            UNREACHABLE("wildcard variant is not used");
        case ExprKindLiteral:
            return expr->value.literal.data_type;
        case ExprKindVariable:
            UNREACHABLE("variant variant is not used");
        case ExprKindGrouping:
            return get_data_type_of_expression(
              self, expr->value.grouping->items[0], local_value);
    }
}

const Str
get_builtin_module_name_from_data_type(struct DataTypeSymbol *dt)
{
    switch (dt->kind) {
        case DataTypeKindPtr:
            return "Ptr";
        case DataTypeKindStr:
            return "Str";
        case DataTypeKindChar:
            return "Char";
        case DataTypeKindI8:
            return "Int8";
        case DataTypeKindI16:
            return "Int16";
        case DataTypeKindI32:
            return "Int32";
        case DataTypeKindI64:
            return "Int64";
        case DataTypeKindI128:
            return "Int128";
        case DataTypeKindU8:
            return "Uint8";
        case DataTypeKindU16:
            return "Uint16";
        case DataTypeKindU32:
            return "Uint32";
        case DataTypeKindU64:
            return "Uint64";
        case DataTypeKindU128:
            return "Uint128";
        case DataTypeKindF32:
            return "Float32";
        case DataTypeKindF64:
            return "Float64";
        case DataTypeKindBool:
            return "Bool";
        case DataTypeKindIsize:
            return "Isize";
        case DataTypeKindUsize:
            return "Usize";
        case DataTypeKindOptional:
            return "Optonal";
        case DataTypeKindArray:
            return "Array";
        case DataTypeKindTuple:
            return "Tuple";
        case DataTypeKindCustom:
            return "Custom";
        default:
            return NULL;
    }
}

struct BuiltinFun *
search_fun_builtin(struct Typecheck *self,
                   const Str module_name,
                   const Str fun_name,
                   Usize param_count)
{
    struct BuiltinModule *module = NULL;

    for (Usize i = len__Vec(*self->builtins); i--;)
        if (!strcmp(
              ((struct Builtin *)get__Vec(*self->builtins, i))->module->name,
              module_name)) {
            module = get__Vec(*self->builtins, i);
            break;
        }

    assert(module && "module is not found");

    for (Usize i = len__Vec(*module->items); i--;)
        if (!strcmp(((struct BuiltinFun *)get__Vec(*module->items, i))->name,
                    fun_name) &&
            param_count ==
              len__Vec(
                *((struct Builtin *)get__Vec(*module->items, i))->fun->params))
            return ((struct BuiltinFun *)get__Vec(*module->items, i));

    assert(0 && "fun is not found");
}

void
verify_type_of_fun_builtin(struct BuiltinFun *fun_builtin,
                           Usize param_count,
                           ...)
{
    va_list vl;

    va_start(vl, param_count);

    for (Usize i = param_count; i--;) {
        if (((struct DataTypeSymbol *)get__Vec(*fun_builtin->params, i)) !=
            NULL) {
            if (((struct DataTypeSymbol *)get__Vec(*fun_builtin->params, i))
                  ->kind != DataTypeKindCompilerDefined) {
                struct DataTypeSymbol *params =
                  va_arg(vl, struct DataTypeSymbol *);

                switch (params->kind) {
                    case DataTypeKindMut:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *fun_builtin->params, i))
                              ->value.mut->kind == DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindException:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *fun_builtin->params, i))
                              ->value.exception->kind ==
                            DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindOptional:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *fun_builtin->params, i))
                              ->value.optional->kind ==
                            DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindPtr:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *fun_builtin->params, i))
                              ->value.ptr->kind == DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindRef:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *fun_builtin->params, i))
                              ->value.ref->kind == DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindTuple:
                        if (((struct DataTypeSymbol *)get__Vec(
                               *((struct DataTypeSymbol *)get__Vec(
                                   *fun_builtin->params, i))
                                  ->value.tuple,
                               0))
                              ->kind == DataTypeKindCompilerDefined)
                            return;
                    case DataTypeKindArray:
                        if (((struct DataTypeSymbol
                                *)((struct DataTypeSymbol *)get__Vec(
                                     *fun_builtin->params, i))
                               ->value.array->items[0])
                              ->kind == DataTypeKindCompilerDefined)
                            return;
                    default:
                        break;
                }
                if (((struct DataTypeSymbol *)get__Vec(*fun_builtin->params,
                                                       i)) !=
                    va_arg(vl, struct DataTypeSymbol *))
                    assert(0 && "error");
            }
        }
    }

    va_end(vl);
}

static inline struct DataTypeSymbol *
get_return_type_of_fun_builtin(struct Typecheck *self,
                               const Str module_name,
                               const Str fun_name,
                               Usize param_count)
{
    struct BuiltinFun *fun =
      search_fun_builtin(self, module_name, fun_name, param_count);
    struct DataTypeSymbol *dt = get__Vec(*fun->params, fun->params->len - 1);

    if (dt->kind == DataTypeKindCompilerDefined)
        return NULL;

    return dt;
}

struct Scope *
search_in_fun_local_value(struct Vec *local_value, struct String *id)
{
    struct Scope *value = NULL;

    for (Usize i = len__Vec(*local_value); i--;) {
        if (eq__String(
              ((struct Scope *)get__Vec(*local_value, i))->name, id, false))
            return get__Vec(*local_value, i);
    }

    return value;
}

struct Scope *
search_value_in_function(struct Typecheck *self,
                         struct Vec *local_value,
                         struct String *id,
                         struct Vec *id_access)
{
    if (id) {
        struct Scope *found_local_value =
          search_in_fun_local_value(local_value, id);

        if (!found_local_value) {
            assert(0 && "error");
        } else if (found_local_value)
            return found_local_value;
    } else if (id_access) {
    }

    return NULL;
}

struct DataTypeSymbol *
check_if_defined_data_type_is_equal_to_infered_data_type(
  struct Typecheck *self,
  struct DataTypeSymbol *defined_data_type,
  struct DataTypeSymbol *infered_data_type)
{
    if (defined_data_type)
        if (defined_data_type == infered_data_type) {
            assert(0 && "error");
            return NULL;
        }

    return infered_data_type;
}

struct DataTypeSymbol *
infer_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type)
{
    switch (expr->kind) {
        case ExprKindFunCall:
            TODO("infer fun call");
        case ExprKindRecordCall:
            TODO("infer record call");
        case ExprKindIdentifier:
            TODO("infer identifier");
        case ExprKindIdentifierAccess:
            TODO("infer identifier access");
        case ExprKindGlobalAccess:
            TODO("infer global access");
        case ExprKindArrayAccess:
            TODO("infer array access");
        case ExprKindTupleAccess:
            TODO("infer tuple access");
        case ExprKindLambda:
            TODO("infer lambda");
        case ExprKindTuple: {
            if (defined_data_type)
                return copy__DataTypeSymbol(defined_data_type);
            else {
                struct Vec *dts = NEW(Vec, sizeof(struct DataTypeSymbol));

                for (Usize i = len__Vec(*expr->value.tuple); i--;)
                    push__Vec(dts,
                              check_expression(self,
                                               fun,
                                               get__Vec(*expr->value.tuple, i),
                                               local_value,
                                               local_data_type,
                                               NULL,
                                               is_return_type));

                return NEW(DataTypeSymbolTuple, dts);
            }
        }
        case ExprKindArray:
            TODO("infer array");
        case ExprKindVariant:
            TODO("infer variant");
        case ExprKindTry:
            TODO("infer try");
        case ExprKindIf:
            TODO("infer if");
        case ExprKindBlock:
            TODO("infer block");
        case ExprKindQuestionMark:
            if (defined_data_type)
                return NEW(DataTypeSymbolOptional,
                           infer_expression(self,
                                            fun,
                                            expr->value.question_mark,
                                            local_value,
                                            local_data_type,
                                            defined_data_type->value.optional,
                                            is_return_type));
            else
                return NEW(DataTypeSymbolOptional,
                           infer_expression(self,
                                            fun,
                                            expr->value.question_mark,
                                            local_value,
                                            local_data_type,
                                            NULL,
                                            is_return_type));
        case ExprKindDereference:
            if (defined_data_type)
                return NEW(DataTypeSymbolPtr,
                           infer_expression(self,
                                            fun,
                                            expr->value.dereference,
                                            local_value,
                                            local_data_type,
                                            defined_data_type->value.ptr,
                                            is_return_type));
            else
                return NEW(DataTypeSymbolPtr,
                           infer_expression(self,
                                            fun,
                                            expr->value.dereference,
                                            local_value,
                                            local_data_type,
                                            NULL,
                                            is_return_type));
        case ExprKindRef:
            return NEW(DataTypeSymbolRef,
                       infer_expression(self,
                                        fun,
                                        expr->value.ref,
                                        local_value,
                                        local_data_type,
                                        defined_data_type,
                                        is_return_type));
        case ExprKindSelf:
            TODO("infer self");
        case ExprKindUndef:
            return NEW(DataTypeSymbolCompilerDefined,
                       NEW(CompilerDefinedDataType, "T", false));
        case ExprKindNil:
            return NEW(DataTypeSymbolPtr,
                       NEW(DataTypeSymbolCompilerDefined,
                           NEW(CompilerDefinedDataType, "T", false)));
        case ExprKindNone:
            return NEW(DataTypeSymbolOptional,
                       NEW(DataTypeSymbolCompilerDefined,
                           NEW(CompilerDefinedDataType, "T", false)));
        case ExprKindWildcard:
            break;
        case ExprKindLiteral:
            switch (expr->value.literal.kind) {
                case LiteralKindBool:
                    return NEW(DataTypeSymbol, DataTypeKindBool);
                case LiteralKindChar:
                    return NEW(DataTypeSymbol, DataTypeKindChar);
                case LiteralKindBitChar:
                    return NEW(DataTypeSymbol, DataTypeKindU8);
                case LiteralKindInt32:
                    return (NEW(DataTypeSymbol, DataTypeKindI32));
                case LiteralKindInt64:
                    return (NEW(DataTypeSymbol, DataTypeKindI64));
                case LiteralKindInt128:
                    return (NEW(DataTypeSymbol, DataTypeKindI128));
                case LiteralKindFloat:
                    return (NEW(DataTypeSymbol, DataTypeKindF64));
                case LiteralKindBitStr:
                    return (NEW(DataTypeSymbolArray,
                                NEW(DataTypeSymbol, DataTypeKindU8),
                                NULL));
                case LiteralKindStr:
                    return (NEW(DataTypeSymbol, DataTypeKindStr));
                case LiteralKindUnit:
                    return (NEW(DataTypeSymbol, DataTypeKindUnit));
            }
        case ExprKindGrouping:
            return infer_expression(self,
                                    fun,
                                    expr->value.grouping,
                                    local_value,
                                    local_data_type,
                                    defined_data_type,
                                    is_return_type);
        case ExprKindUnaryOp:
        case ExprKindBinaryOp:
            UNREACHABLE("data type is already analyzed");
        case ExprKindVariable:
            UNREACHABLE("cannot infer on variable");
    }

    return NULL;
}

enum DataTypeKind
  data_type_kind_from_literal_symbol_kind[LiteralSymbolKindUnit + 1] = {
      [LiteralSymbolKindBool] = DataTypeKindBool,
      [LiteralSymbolKindChar] = DataTypeKindChar,
      [LiteralSymbolKindBitChar] = DataTypeKindBitChar,
      [LiteralSymbolKindInt8] = DataTypeKindI8,
      [LiteralSymbolKindInt16] = DataTypeKindI16,
      [LiteralSymbolKindInt32] = DataTypeKindI32,
      [LiteralSymbolKindInt64] = DataTypeKindI64,
      [LiteralSymbolKindInt128] = DataTypeKindI128,
      [LiteralSymbolKindUint8] = DataTypeKindU8,
      [LiteralSymbolKindUint16] = DataTypeKindU16,
      [LiteralSymbolKindUint32] = DataTypeKindU32,
      [LiteralSymbolKindUint64] = DataTypeKindU64,
      [LiteralSymbolKindUint128] = DataTypeKindU128,
      [LiteralSymbolKindFloat32] = DataTypeKindF32,
      [LiteralSymbolKindFloat64] = DataTypeKindF64,
      [LiteralSymbolKindStr] = DataTypeKindStr,
      [LiteralSymbolKindBitStr] = DataTypeKindBitStr,
      [LiteralSymbolKindUnit] = DataTypeKindUnit
  };

enum DataTypeKind
get_data_type_kind_from_literal_symbol(enum LiteralSymbolKind kind)
{
    return data_type_kind_from_literal_symbol_kind[(int)kind];
}

struct ExprSymbol *
check_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type)
{
    switch (expr->kind) {
        case ExprKindUnaryOp: {
            struct ExprSymbol *right =
              check_expression(self,
                               fun,
                               expr->value.unary_op.right,
                               local_value,
                               local_data_type,
                               NULL,
                               is_return_type);

            right->data_type = infer_expression(self,
                                                fun,
                                                expr->value.unary_op.right,
                                                local_value,
                                                local_data_type,
                                                defined_data_type,
                                                is_return_type);

            switch (expr->value.unary_op.kind) {
                case UnaryOpKindReference: {
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Ref", "&", 2),
                      2,
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    struct DataTypeSymbol *return_type =
                      get_return_type_of_fun_builtin(self, "Ref", "&", 2);

                    return NEW(
                      ExprSymbolUnaryOp,
                      *expr,
                      NEW(UnaryOpSymbol, *expr, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        !return_type ? get_data_type_of_expression(
                                         self, right, local_value)
                                     : return_type));
                }
                case UnaryOpKindBitNot:
                case UnaryOpKindNegative: {
                    const Str module_name =
                      get_builtin_module_name_from_data_type(
                        get_data_type_of_expression(self, right, local_value));
                    const Str op_str =
                      to_str__UnaryOpKind(expr->value.unary_op.kind);

                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, module_name, op_str, 2),
                      2,
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolUnaryOp,
                      *expr,
                      NEW(UnaryOpSymbol, *expr, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, module_name, op_str, 2)));
                }
                case UnaryOpKindNot:
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Bool", "not", 2),
                      2,
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolUnaryOp,
                      *expr,
                      NEW(UnaryOpSymbol, *expr, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, "Bool", "not", 2)));
                case UnaryOpKindCustom:
                    TODO("check unary operator kind custom");
            }
        }
        case ExprKindBinaryOp: {
            struct ExprSymbol *left =
              check_expression(self,
                               fun,
                               expr->value.binary_op.left,
                               local_value,
                               local_data_type,
                               NULL,
                               is_return_type);

            struct ExprSymbol *right =
              check_expression(self,
                               fun,
                               expr->value.binary_op.right,
                               local_value,
                               local_data_type,
                               NULL,
                               is_return_type);
            const Str op_str = to_str__BinaryOpKind(expr->value.binary_op.kind);

            left->data_type = infer_expression(self,
                                               fun,
                                               expr->value.binary_op.left,
                                               local_value,
                                               local_data_type,
                                               defined_data_type,
                                               is_return_type);
            right->data_type = infer_expression(self,
                                                fun,
                                                expr->value.binary_op.right,
                                                local_value,
                                                local_data_type,
                                                defined_data_type,
                                                is_return_type);

            switch (expr->value.binary_op.kind) {
                case BinaryOpKindAdd:
                case BinaryOpKindSub:
                case BinaryOpKindMul:
                case BinaryOpKindDiv:
                case BinaryOpKindMod:
                case BinaryOpKindRange:
                case BinaryOpKindLt:
                case BinaryOpKindGt:
                case BinaryOpKindLe:
                case BinaryOpKindGe:
                case BinaryOpKindEq:
                case BinaryOpKindNe:
                case BinaryOpKindAssign:
                case BinaryOpKindAddAssign:
                case BinaryOpKindSubAssign:
                case BinaryOpKindMulAssign:
                case BinaryOpKindDivAssign:
                case BinaryOpKindModAssign:
                case BinaryOpKindBitLShiftAssign:
                case BinaryOpKindBitRShiftAssign:
                case BinaryOpKindBitOrAssign:
                case BinaryOpKindXorAssign:
                case BinaryOpKindBitAndAssign:
                case BinaryOpKindExponentAssign:
                case BinaryOpKindBitLShift:
                case BinaryOpKindBitRShift:
                case BinaryOpKindBitOr:
                case BinaryOpKindBitAnd:
                case BinaryOpKindXor:
                case BinaryOpKindExponent: {
                    const Str module_name =
                      get_builtin_module_name_from_data_type(
                        get_data_type_of_expression(self, left, local_value));
                    struct BuiltinFun *fun_builtin =
                      search_fun_builtin(self, module_name, op_str, 3);

                    if (!fun_builtin)
                        assert(0 && "error");

                    verify_type_of_fun_builtin(
                      fun_builtin,
                      3,
                      get_data_type_of_expression(self, left, local_value),
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolBinaryOp,
                      *expr,
                      NEW(BinaryOpSymbol, *expr, left, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, module_name, op_str, 3)));
                }
                case BinaryOpKindAnd:
                case BinaryOpKindOr: {
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Bool", op_str, 3),
                      3,
                      get_data_type_of_expression(self, left, local_value),
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolBinaryOp,
                      *expr,
                      NEW(BinaryOpSymbol, *expr, left, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, "Bool", op_str, 3)));
                }
                case BinaryOpKindMergeAssign:
                case BinaryOpKindUnmergeAssign:
                case BinaryOpKindMerge:
                case BinaryOpKindUnmerge: {
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Array", op_str, 3),
                      3,
                      get_data_type_of_expression(self, left, local_value),
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolBinaryOp,
                      *expr,
                      NEW(BinaryOpSymbol, *expr, left, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, "Array", op_str, 3)));
                }
                case BinaryOpKindChain:
                    TODO("check chain binary op");
                case BinaryOpKindRepeat:
                case BinaryOpKindConcat:
                case BinaryOpKindConcatAssign: {
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Str", op_str, 3),
                      3,
                      get_data_type_of_expression(self, left, local_value),
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolBinaryOp,
                      *expr,
                      NEW(BinaryOpSymbol, *expr, left, right),
                      check_if_defined_data_type_is_equal_to_infered_data_type(
                        self,
                        defined_data_type,
                        get_return_type_of_fun_builtin(
                          self, "Str", op_str, 3)));
                }
                case BinaryOpKindCustom:
                    TODO("check binary op custom");
            }
        }
        case ExprKindFunCall: {
            TODO("check fun call");
        }
        case ExprKindRecordCall:
            TODO("check record call");
        case ExprKindIdentifier:
            return NEW(ExprSymbolIdentifier,
                       *expr,
                       search_value_in_function(
                         self, local_value, expr->value.identifier, NULL),
                       infer_expression(self,
                                        fun,
                                        expr,
                                        local_value,
                                        local_data_type,
                                        defined_data_type,
                                        is_return_type));
        case ExprKindIdentifierAccess:
            return NEW(
              ExprSymbolIdentifierAccess,
              *expr,
              search_value_in_function(
                self, local_value, NULL, expr->value.identifier_access),
              infer_expression(self,
                               fun,
                               expr,
                               local_value,
                               local_data_type,
                               defined_data_type,
                               is_return_type));
        case ExprKindGlobalAccess:
            TODO("check global access");
        case ExprKindArrayAccess: {
            struct Expr *identifier_access_expr =
              NEW(ExprIdentifierAccess,
                  copy__Vec(expr->value.array_access.access),
                  expr->loc);
            struct DataTypeSymbol *defined_data_type_identifier_access_symb =
              defined_data_type ? NEW(DataTypeSymbolArray,
                                      copy__DataTypeSymbol(defined_data_type),
                                      NULL)
                                : NEW(DataTypeSymbolArray, NULL, NULL);
            struct ExprSymbol *identifier_access_symb =
              check_expression(self,
                               fun,
                               identifier_access_expr,
                               local_value,
                               local_data_type,
                               defined_data_type_identifier_access_symb,
                               is_return_type);
            struct ExprSymbol *res =
              NEW(ExprSymbolArrayAccess,
                  *expr,
                  copy__Scope(identifier_access_symb->value.identifier_access),
                  infer_expression(self,
                                   fun,
                                   expr,
                                   local_value,
                                   local_data_type,
                                   defined_data_type,
                                   is_return_type));

            FREE(ExprAll, identifier_access_expr);
            FREE(DataTypeSymbolAll, defined_data_type_identifier_access_symb);
            FREE(ExprSymbolAll, identifier_access_symb);

            return res;
        }
        case ExprKindTupleAccess: {
            struct Expr *identifier_access_expr =
              NEW(ExprIdentifierAccess,
                  copy__Vec(expr->value.tuple_access.access),
                  expr->loc);
            struct DataTypeSymbol *defined_data_type_identifier_access_symb =
              NEW(DataTypeSymbolTuple, NULL);
            struct ExprSymbol *identifier_access_symb =
              check_expression(self,
                               fun,
                               identifier_access_expr,
                               local_value,
                               local_data_type,
                               defined_data_type_identifier_access_symb,
                               is_return_type);
            struct ExprSymbol *res =
              NEW(ExprSymbolArrayAccess,
                  *expr,
                  copy__Scope(identifier_access_symb->value.identifier_access),
                  infer_expression(self,
                                   fun,
                                   expr,
                                   local_value,
                                   local_data_type,
                                   defined_data_type,
                                   is_return_type));

            FREE(ExprAll, identifier_access_expr);
            FREE(DataTypeSymbolAll, defined_data_type_identifier_access_symb);
            FREE(ExprSymbolAll, identifier_access_symb);

            return res;
        }
        case ExprKindLambda:
            TODO("check lambda");
        case ExprKindTuple:
            TODO("check tuple");
        case ExprKindArray: {
            if (defined_data_type) {
                struct Vec *array = NEW(Vec, sizeof(struct ExprSymbol));

                for (Usize i = 0; i < len__Vec(*expr->value.array); i++)
                    push__Vec(
                      array,
                      check_expression(self,
                                       fun,
                                       get__Vec(*expr->value.array, i),
                                       local_value,
                                       local_data_type,
                                       defined_data_type->value.array->items[0],
                                       is_return_type));

                // Check if the data type is the same as the defined data type
                for (Usize i = len__Vec(*array); i--;)
                    if (!eq__DataTypeSymbol(
                          ((struct ExprSymbol *)get__Vec(*array, i))->data_type,
                          defined_data_type->value.array->items[0])) {
                        assert(0 && "error: this expression doens't match with "
                                    "defined data type in array");
                        break;
                    }

                return NEW(ExprSymbolArray,
                           *expr,
                           array,
                           copy__DataTypeSymbol(defined_data_type));
            } else {
                struct Vec *array = NEW(Vec, sizeof(struct ExprSymbol));

                for (Usize i = 0; i < len__Vec(*expr->value.array); i++)
                    push__Vec(array,
                              check_expression(self,
                                               fun,
                                               get__Vec(*expr->value.array, i),
                                               local_value,
                                               local_data_type,
                                               NULL,
                                               is_return_type));

                struct DataTypeSymbol *dt = infer_expression(self,
                                                             fun,
                                                             expr,
                                                             local_value,
                                                             local_data_type,
                                                             NULL,
                                                             is_return_type);

                // Check if the data type is the same as the defined data type
                for (Usize i = len__Vec(*array); i--;)
                    if (!eq__DataTypeSymbol(
                          ((struct ExprSymbol *)get__Vec(*array, i))->data_type,
                          dt->value.array->items[0])) {
                        assert(0 && "error: this expression doens't match with "
                                    "defined data type in array");
                        break;
                    }

                return NEW(ExprSymbolArray, *expr, array, dt);
            }
        }
        case ExprKindVariant:
            if (defined_data_type) {
                struct ExprSymbol *value = NULL;
                struct Scope *scope = search_from_access(
                  self,
                  expr->value.variant.id,
                  NEW(Vec, sizeof(struct String)),
                  (struct SearchContext){ .search_class = false,
                                          .search_fun = false,
                                          .search_object = false,
                                          .search_trait = false,
                                          .search_type = false,
                                          .search_value = false,
                                          .search_variant = true,
                                          .search_primary_type = false });

                if (expr->value.variant.value) {
                    value = check_expression(self,
                                             fun,
                                             expr->value.variant.value,
                                             local_value,
                                             local_data_type,
                                             NULL,
                                             is_return_type);
                }
            } else {
            }
            // return NEW(ExprSymbolVariant, *expr, )
            TODO("check variant");
        case ExprKindTry:
            TODO("check try");
        case ExprKindBlock:
            TODO("check block");
        case ExprKindQuestionMark:
            if (defined_data_type) {
                if (defined_data_type->kind == DataTypeKindOptional) {
                    struct DataTypeSymbol *expr_qm_dt =
                      infer_expression(self,
                                       fun,
                                       expr->value.question_mark,
                                       local_value,
                                       local_data_type,
                                       NULL,
                                       is_return_type);

                    if (expr_qm_dt->kind != DataTypeKindOptional) {
                        assert(0 && "error: expected Ptr or Ref data type");
                    }

                    struct DataTypeSymbol *expr_dt =
                      infer_expression(self,
                                       fun,
                                       expr,
                                       local_value,
                                       local_data_type,
                                       NULL,
                                       is_return_type);

                    if (!eq__DataTypeSymbol(defined_data_type, expr_dt)) {
                        assert(0 && "error: data type doesn't match with "
                                    "defined data type");
                    }

                    FREE(DataTypeSymbolAll, expr_dt);

                    return NEW(
                      ExprSymbolQuestionMark,
                      *expr,
                      search_from_access(
                        self,
                        expr->value.dereference,
                        NEW(Vec, sizeof(struct String)),
                        (struct SearchContext){ .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                      expr_qm_dt);
                }
            } else {
                return NEW(ExprSymbolQuestionMark,
                           *expr,
                           search_from_access(self,
                                              expr->value.ref,
                                              NEW(Vec, sizeof(struct String)),
                                              (struct SearchContext){
                                                .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                           NEW(DataTypeSymbolOptional,
                               infer_expression(self,
                                                fun,
                                                expr->value.question_mark,
                                                local_value,
                                                local_data_type,
                                                defined_data_type,
                                                is_return_type)));
            }
            TODO("check question mark");
        case ExprKindDereference:
            if (defined_data_type) {
                struct DataTypeSymbol *expr_dereference_dt =
                  infer_expression(self,
                                   fun,
                                   expr->value.dereference,
                                   local_value,
                                   local_data_type,
                                   NULL,
                                   is_return_type);

                if (!(expr_dereference_dt->kind == DataTypeKindPtr ||
                      expr_dereference_dt->kind == DataTypeKindRef)) {
                    assert(0 && "error: expected Ptr or Ref data type");
                }

                struct DataTypeSymbol *expr_dt =
                  infer_expression(self,
                                   fun,
                                   expr,
                                   local_value,
                                   local_data_type,
                                   NULL,
                                   is_return_type);

                if (!eq__DataTypeSymbol(defined_data_type, expr_dt)) {
                    assert(0 && "error: data type doesn't match with "
                                "defined data type");
                }

                FREE(DataTypeSymbolAll, expr_dt);

                return NEW(ExprSymbolDereference,
                           *expr,
                           search_from_access(self,
                                              expr->value.dereference,
                                              NEW(Vec, sizeof(struct String)),
                                              (struct SearchContext){
                                                .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                           expr_dereference_dt);
            } else {
                return NEW(ExprSymbolDereference,
                           *expr,
                           search_from_access(self,
                                              expr->value.dereference,
                                              NEW(Vec, sizeof(struct String)),
                                              (struct SearchContext){
                                                .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                           infer_expression(self,
                                            fun,
                                            expr,
                                            local_value,
                                            local_data_type,
                                            defined_data_type,
                                            is_return_type));
            }
        case ExprKindRef:
            if (defined_data_type)
                if (defined_data_type->kind == DataTypeKindRef) {
                    struct DataTypeSymbol *expr_dt =
                      infer_expression(self,
                                       fun,
                                       expr,
                                       local_value,
                                       local_data_type,
                                       NULL,
                                       is_return_type);

                    if (!(eq__DataTypeSymbol(expr_dt, defined_data_type))) {
                        assert(0 && "error: data type doesn't match with "
                                    "defined data type");
                    }

                    return NEW(
                      ExprSymbolRef,
                      *expr,
                      search_from_access(
                        self,
                        expr->value.ref,
                        NEW(Vec, sizeof(struct String)),
                        (struct SearchContext){ .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                      expr_dt);
                } else {
                    assert(0 && "error: expected Ref data type");
                }
            else {
                return NEW(ExprSymbolRef,
                           *expr,
                           search_from_access(self,
                                              expr->value.ref,
                                              NEW(Vec, sizeof(struct String)),
                                              (struct SearchContext){
                                                .search_type = false,
                                                .search_fun = false,
                                                .search_variant = false,
                                                .search_value = true,
                                                .search_trait = false,
                                                .search_class = false,
                                                .search_object = false,
                                                .search_primary_type = false }),
                           NEW(DataTypeSymbolRef,
                               infer_expression(self,
                                                fun,
                                                expr->value.ref,
                                                local_value,
                                                local_data_type,
                                                defined_data_type,
                                                is_return_type)));
            }
        case ExprKindSelf:
            TODO("check self");
        case ExprKindUndef:
            if (defined_data_type)
                return NEW(
                  ExprSymbol, expr, copy__DataTypeSymbol(defined_data_type));
            else
                return NEW(ExprSymbol,
                           expr,
                           NEW(DataTypeSymbolCompilerDefined,
                               NEW(CompilerDefinedDataType, "T", false)));
        case ExprKindNil:
            if (defined_data_type)
                if (defined_data_type->kind == DataTypeKindPtr)
                    return NEW(ExprSymbol,
                               expr,
                               copy__DataTypeSymbol(defined_data_type));
                else {
                    assert(0 && "error: expected Ptr data type");
                }
            else
                return NEW(ExprSymbol,
                           expr,
                           NEW(DataTypeSymbolPtr,
                               NEW(DataTypeSymbolCompilerDefined,
                                   NEW(CompilerDefinedDataType, "T", false))));
        case ExprKindNone:
            if (defined_data_type)
                if (defined_data_type->kind == DataTypeKindOptional)
                    return NEW(ExprSymbol,
                               expr,
                               copy__DataTypeSymbol(defined_data_type));
                else {
                    assert(0 && "error: expected Optional data type");
                }
            else
                return NEW(ExprSymbol,
                           expr,
                           NEW(DataTypeSymbolOptional,
                               NEW(DataTypeSymbolCompilerDefined,
                                   NEW(CompilerDefinedDataType, "T", false))));
            break;
        case ExprKindWildcard:
            break;
            TODO("check wildcard");
        case ExprKindLiteral: {
            int *kind = NULL; // int* = LiteralSymbolKind*

            switch (expr->value.literal.kind) {
                case LiteralKindBool:
                    kind = (int *)LiteralSymbolKindBool;
                    break;
                case LiteralKindChar:
                    kind = (int *)LiteralSymbolKindChar;
                    break;
                case LiteralKindBitChar:
                    kind = (int *)LiteralSymbolKindBitChar;
                    break;
                case LiteralKindInt32WithoutSuffix:
                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case DataTypeKindI8:
                                kind = (int *)LiteralSymbolKindInt8;
                                break;
                            case DataTypeKindI16:
                                kind = (int *)LiteralSymbolKindInt16;
                                break;
                            case DataTypeKindI32:
                                kind = (int *)LiteralSymbolKindInt32;
                                break;
                            case DataTypeKindI64:
                                kind = (int *)LiteralSymbolKindInt64;
                                break;
                            case DataTypeKindI128:
                                kind = (int *)LiteralSymbolKindInt128;
                                break;
                            case DataTypeKindU8:
                                kind = (int *)LiteralSymbolKindUint8;
                                break;
                            case DataTypeKindU16:
                                kind = (int *)LiteralSymbolKindUint16;
                                break;
                            case DataTypeKindU32:
                                kind = (int *)LiteralSymbolKindUint32;
                                break;
                            case DataTypeKindU64:
                                kind = (int *)LiteralSymbolKindUint64;
                                break;
                            case DataTypeKindU128:
                                kind = (int *)LiteralSymbolKindUint128;
                                break;
                            default: {
                                kind = (int *)LiteralSymbolKindInt32;

                                struct Diagnostic *err =
                                  NEW(DiagnosticWithErrTypecheck,
                                      self,
                                      NEW(LilyError,
                                          LilyErrorExpectedIntegerDataType),
                                      expr->loc,
                                      from__String(
                                        "incompatibility between defined data "
                                        "type and expression"),
                                      Some(from__String(
                                        "expected integer typed expression")));

                                emit__Diagnostic(err);
                            }
                        }
                    } else
                        kind = (int *)LiteralSymbolKindInt32;

                    break;
                case LiteralKindInt64WithoutSuffix:
                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case DataTypeKindI64:
                                kind = (int *)LiteralSymbolKindInt64;
                                break;
                            case DataTypeKindI128:
                                kind = (int *)LiteralSymbolKindInt128;
                                break;
                            case DataTypeKindU64:
                                kind = (int *)LiteralSymbolKindUint64;
                                break;
                            case DataTypeKindU128:
                                kind = (int *)LiteralSymbolKindUint128;
                                break;
                            case DataTypeKindI8:
                            case DataTypeKindI16:
                            case DataTypeKindI32:
                            case DataTypeKindU8:
                            case DataTypeKindU16:
                            case DataTypeKindU32: {
                                kind = (int *)LiteralSymbolKindInt64;

                                struct Diagnostic *err = NEW(
                                  DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError,
                                      LilyErrorExpectedALargerIntegerDataType),
                                  expr->loc,
                                  from__String(
                                    "Int8, Int16, Int32, Uint8, Uint16 or "
                                    "Uint32 integer data types are too short"),
                                  Some(
                                    from__String("define Int64, Int128, Uint64 "
                                                 "or Uint128 data type")));

                                emit__Diagnostic(err);
                            }
                            default: {
                                kind = (int *)LiteralSymbolKindInt64;

                                struct Diagnostic *err =
                                  NEW(DiagnosticWithErrTypecheck,
                                      self,
                                      NEW(LilyError,
                                          LilyErrorExpectedIntegerDataType),
                                      expr->loc,
                                      from__String(
                                        "incompatibility between defined data "
                                        "type and expression"),
                                      Some(from__String(
                                        "expected integer typed expression")));

                                emit__Diagnostic(err);
                            }
                        }
                    } else
                        kind = (int *)LiteralSymbolKindInt64;

                    break;
                case LiteralKindInt128WithoutSuffix:
                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case DataTypeKindI128:
                                kind = (int *)LiteralSymbolKindInt128;
                                break;
                            case DataTypeKindU128:
                                kind = (int *)LiteralSymbolKindUint128;
                            case DataTypeKindI8:
                            case DataTypeKindI16:
                            case DataTypeKindI32:
                            case DataTypeKindI64:
                            case DataTypeKindU8:
                            case DataTypeKindU16:
                            case DataTypeKindU32:
                            case DataTypeKindU64:
                                kind = (int *)LiteralSymbolKindInt128;
                                struct Diagnostic *err = NEW(
                                  DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError,
                                      LilyErrorExpectedALargerIntegerDataType),
                                  expr->loc,
                                  from__String(
                                    "Int8, Int16, Int32, Int64, Uint8, Uint16, "
                                    "Uint32 or Uint64 integer data types are "
                                    "too short"),
                                  Some(from__String("define Int128 "
                                                    "or Uint128 data type")));

                                emit__Diagnostic(err);
                            default: {
                                kind = (int *)LiteralSymbolKindInt128;

                                struct Diagnostic *err =
                                  NEW(DiagnosticWithErrTypecheck,
                                      self,
                                      NEW(LilyError,
                                          LilyErrorExpectedIntegerDataType),
                                      expr->loc,
                                      from__String(
                                        "incompatibility between defined data "
                                        "type and expression"),
                                      Some(from__String(
                                        "expected integer typed expression")));

                                emit__Diagnostic(err);
                            }
                        }
                    } else
                        kind = (int *)LiteralSymbolKindInt128;

                    break;
                case LiteralKindInt8:
                    kind = (int *)LiteralSymbolKindInt8;
                    break;
                case LiteralKindInt16:
                    kind = (int *)LiteralSymbolKindInt16;
                    break;
                case LiteralKindInt32:
                    kind = (int *)LiteralSymbolKindInt32;
                    break;
                case LiteralKindInt64:
                    kind = (int *)LiteralSymbolKindInt64;
                    break;
                case LiteralKindInt128:
                    kind = (int *)LiteralSymbolKindInt128;
                    break;
                case LiteralKindUint8:
                    kind = (int *)LiteralSymbolKindUint8;
                    break;
                case LiteralKindUint16:
                    kind = (int *)LiteralSymbolKindUint16;
                    break;
                case LiteralKindUint32:
                    kind = (int *)LiteralSymbolKindUint32;
                    break;
                case LiteralKindUint64:
                    kind = (int *)LiteralSymbolKindUint64;
                    break;
                case LiteralKindUint128:
                    kind = (int *)LiteralSymbolKindUint128;
                    break;
                case LiteralKindFloat:
                    if (defined_data_type) {
                        switch (defined_data_type->kind) {
                            case DataTypeKindF32:
                                kind = (int *)LiteralSymbolKindFloat32;
                                break;
                            case DataTypeKindF64:
                                kind = (int *)LiteralSymbolKindFloat64;
                                break;
                            default:
                                assert(0 && "error: expected float data type");
                        }
                    } else
                        kind = (int *)LiteralSymbolKindFloat64;

                    break;
                case LiteralKindFloat32:
                    kind = (int *)LiteralSymbolKindFloat32;
                    break;
                case LiteralKindFloat64:
                    kind = (int *)LiteralSymbolKindFloat64;
                    break;
                case LiteralKindBitStr:
                    kind = (int *)LiteralSymbolKindBitStr;
                    break;
                case LiteralKindStr:
                    kind = (int *)LiteralSymbolKindStr;
                    break;
                case LiteralKindUnit:
                    kind = (int *)LiteralSymbolKindUnit;
                    break;
                default:
                    UNREACHABLE("unknown LiteralKind");
            }

            // return NEW(DataTypeSymbol,
            // get_data_type_kind_from_literal_symbol((enum
            // LiteralSymbolKind)(UPtr)kind));
            if (kind) {
                struct LiteralSymbol ls;

                switch ((enum LiteralSymbolKind)(UPtr)kind) {
                    case LiteralSymbolKindBool:
                        ls = NEW(LiteralSymbolBool,
                                 expr->value.literal.value.bool_);
                        break;
                    case LiteralSymbolKindChar:
                        ls = NEW(LiteralSymbolChar,
                                 expr->value.literal.value.char_);
                        break;
                    case LiteralSymbolKindBitChar:
                        ls = NEW(LiteralSymbolBitChar,
                                 expr->value.literal.value.bit_char);
                        break;
                    case LiteralSymbolKindInt8:
                        if (expr->value.literal.value.int32 >= MinInt8 &&
                            expr->value.literal.value.int32 <= MaxInt8)
                            ls = NEW(LiteralSymbolInt8,
                                     (Int8)expr->value.literal.value.int32);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                                  expr->loc,
                                  from__String("integer is too large for Int8"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolInt8, 0);
                        }
                        break;
                    case LiteralSymbolKindInt16:
                        if (expr->value.literal.value.int32 >= MinInt16 &&
                            expr->value.literal.value.int32 <= MaxInt16)
                            ls = NEW(LiteralSymbolInt16,
                                     (Int16)expr->value.literal.value.int32);
                        else {
                            struct Diagnostic *err = NEW(
                              DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                              expr->loc,
                              from__String("integer is too large for Int16"),
                              None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolInt16, 0);
                        }
                        break;
                    case LiteralSymbolKindInt32:
                        ls = NEW(LiteralSymbolInt32,
                                 expr->value.literal.value.int32);
                        break;
                    case LiteralSymbolKindInt64:
                        ls = NEW(LiteralSymbolInt64,
                                 expr->value.literal.value.int64);
                        break;
                    case LiteralSymbolKindInt128:
                        if (expr->value.literal.kind == LiteralKindInt64)
                            ls = NEW(LiteralSymbolInt128,
                                     (Int128)expr->value.literal.value.int64);
                        else
                            ls = NEW(LiteralSymbolInt128,
                                     expr->value.literal.value.int128);
                        break;
                    case LiteralSymbolKindUint8:
                        if (expr->value.literal.value.int32 >= 0 &&
                            expr->value.literal.value.int32 <= MaxUInt8)
                            ls = NEW(LiteralSymbolUint8,
                                     (UInt8)expr->value.literal.value.int32);
                        else {
                            struct Diagnostic *err = NEW(
                              DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                              expr->loc,
                              from__String("integer is too large for Uint8"),
                              None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolUint8, 0);
                        }
                        break;
                    case LiteralSymbolKindUint16:
                        if (expr->value.literal.value.int32 >= 0 &&
                            expr->value.literal.value.int32 <= MaxUInt16)
                            ls = NEW(LiteralSymbolUint16,
                                     (UInt16)expr->value.literal.value.int32);
                        else {
                            struct Diagnostic *err = NEW(
                              DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                              expr->loc,
                              from__String("integer is too large for Uint16"),
                              None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolUint16, 0);
                        }
                        break;
                    case LiteralSymbolKindUint32:
                        if (expr->value.literal.value.int32 >= 0)
                            ls = NEW(LiteralSymbolUint32,
                                     expr->value.literal.value.int32);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                                  expr->loc,
                                  from__String("integer is less than 0"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolUint32, 0);
                        }
                        break;
                    case LiteralSymbolKindUint64:
                        if (expr->value.literal.value.int64 >= 0)
                            ls = NEW(LiteralSymbolUint64,
                                     expr->value.literal.value.int64);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                                  expr->loc,
                                  from__String("integer is less than 0"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolUint64, 0);
                        }
                        break;
                    case LiteralSymbolKindUint128:
                        if (expr->value.literal.value.int128 >= 0)
                            ls = NEW(LiteralSymbolUint128,
                                     expr->value.literal.value.int128);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                                  expr->loc,
                                  from__String("integer is less than 0"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolUint128, 0);
                        }
                        break;
                    case LiteralSymbolKindFloat32: {
                        Float64 abs_value =
                          fabs(expr->value.literal.value.float_);

                        if (abs_value >= MinPosFloat32 &&
                            abs_value <= MaxFloat32)
                            ls = NEW(LiteralSymbolFloat32,
                                     expr->value.literal.value.float_);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorFloatIsOutOfRange),
                                  expr->loc,
                                  from__String("Float32 is out of range"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolFloat32, 0);
                        }
                        break;
                    }
                    case LiteralSymbolKindFloat64: {
                        Float64 abs_value =
                          fabs(expr->value.literal.value.float_);

                        if (abs_value >= MinPosFloat64 &&
                            abs_value <= MaxFloat64)
                            ls = NEW(LiteralSymbolFloat64,
                                     expr->value.literal.value.float_);
                        else {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorFloatIsOutOfRange),
                                  expr->loc,
                                  from__String("Float64 is out of range"),
                                  None());

                            emit__Diagnostic(err);

                            ls = NEW(LiteralSymbolFloat64, 0);
                        }
                        break;
                    }
                    case LiteralSymbolKindStr:
                        ls =
                          NEW(LiteralSymbolStr, expr->value.literal.value.str);
                        break;
                    case LiteralSymbolKindBitStr:
                        ls = NEW(LiteralSymbolBitStr,
                                 expr->value.literal.value.bit_str);
                        break;
                    case LiteralSymbolKindUnit:
                        ls = NEW(LiteralSymbolUnit);
                        break;
                }

                return NEW(ExprSymbolLiteral, *expr, ls);
            }
        }
        case ExprKindIf:
            TODO("check if");
        case ExprKindGrouping:
            return check_expression(self,
                                    fun,
                                    expr->value.grouping,
                                    local_value,
                                    local_data_type,
                                    defined_data_type,
                                    is_return_type);
        case ExprKindVariable: {
            struct DataTypeSymbol *defined_data_type_expr_variable = NULL;

            if (expr->value.variable.data_type) {
                switch (expr->value.variable.data_type->kind) {
                    case DataTypeKindCustom:
                        defined_data_type_expr_variable =
                          check_data_type(self,
                                          expr->loc,
                                          expr->value.variable.data_type,
                                          local_data_type,
                                          NULL,
                                          (struct SearchContext){
                                            .search_type = true,
                                            .search_fun = false,
                                            .search_variant = false,
                                            .search_value = false,
                                            .search_trait = true,
                                            .search_class = true,
                                            .search_object = true,
                                            .search_primary_type = false });
                        break;
                    default:
                        defined_data_type_expr_variable =
                          check_data_type(self,
                                          expr->loc,
                                          expr->value.variable.data_type,
                                          local_data_type,
                                          NULL,
                                          (struct SearchContext){
                                            .search_type = false,
                                            .search_fun = false,
                                            .search_variant = false,
                                            .search_value = false,
                                            .search_trait = false,
                                            .search_class = false,
                                            .search_object = false,
                                            .search_primary_type = false });
                        break;
                }
            }

            struct ExprSymbol *expr_variable =
              check_expression(self,
                               fun,
                               expr->value.variable.expr,
                               local_value,
                               local_data_type,
                               defined_data_type_expr_variable,
                               is_return_type);

            struct ExprSymbol *res =
              NEW(ExprSymbolVariable,
                  *expr,
                  NEW(VariableSymbol, expr->value.variable, expr->loc));

            if (defined_data_type_expr_variable) {
                switch (defined_data_type_expr_variable->kind) {
                    case DataTypeKindOptional:
                        if (!eq__DataTypeSymbol(
                              defined_data_type_expr_variable->value.optional,
                              expr_variable->data_type))
                            goto error;

                        break;
                    case DataTypeKindPtr:
                        if (!eq__DataTypeSymbol(
                              defined_data_type_expr_variable->value.ptr,
                              expr_variable->data_type))
                            goto error;

                        break;
                    case DataTypeKindRef:
                        if (!eq__DataTypeSymbol(
                              defined_data_type_expr_variable->value.ref,
                              expr_variable->data_type))
                            goto error;

                        break;
                    case DataTypeKindException:
                        if (!eq__DataTypeSymbol(
                              defined_data_type_expr_variable->value.exception,
                              expr_variable->data_type))
                            goto error;

                        break;
                    default:
                        if (!eq__DataTypeSymbol(defined_data_type_expr_variable,
                                                expr_variable->data_type)) {
                        error : {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorUnmatchedDataType),
                                  expr->loc,
                                  from__String(""),
                                  None());

                            emit__Diagnostic(err);
                        }
                        }

                        break;
                }
            }

            res->value.variable->data_type = defined_data_type_expr_variable;
            res->value.variable->expr = expr_variable;
            res->value.variable->scope =
              NEW(Scope,
                  self->parser.parse_block.scanner.src->file.name,
                  res->value.variable->name,
                  len__Vec(*local_value),
                  ScopeItemKindVariable,
                  ScopeKindLocal,
                  NULL);

            push__Vec(local_value, res->value.variable->scope);

            return res;
        }
    }
}

struct StmtSymbol
check_await_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec *local_value,
                 struct Vec *local_data_type)
{
    return NEW(StmtSymbolAwait,
               *stmt,
               check_expression(self,
                                fun,
                                stmt->value.await,
                                local_value,
                                local_data_type,
                                NULL,
                                false));
}

struct StmtSymbol
check_return_stmt(struct Typecheck *self,
                  struct FunSymbol *fun,
                  struct Stmt *stmt,
                  struct Vec *local_value,
                  struct Vec *local_data_type)
{
    return NEW(StmtSymbolAwait,
               *stmt,
               check_expression(self,
                                fun,
                                stmt->value.return_,
                                local_value,
                                local_data_type,
                                NULL,
                                false));
}

struct StmtSymbol
check_for_stmt(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Stmt *stmt,
               struct Vec local_value,
               struct Vec *local_data_type)
{}

struct StmtSymbol
check_if_stmt(struct Typecheck *self,
              struct FunSymbol *fun,
              struct Stmt *stmt,
              struct Vec local_value,
              struct Vec *local_data_type)
{}

struct StmtSymbol
check_match_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec local_value,
                 struct Vec *local_data_type)
{}

struct StmtSymbol
check_try_stmt(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Stmt *stmt,
               struct Vec local_value,
               struct Vec *local_data_type)
{}

struct StmtSymbol
check_while_stmt(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Stmt *stmt,
                 struct Vec local_value,
                 struct Vec *local_data_type)
{}

void
check_fun_body(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Vec *fun_body,
               struct Vec *local_value,
               struct Vec *local_data_type)
{
    for (Usize i = 0; i < len__Vec(*fun->fun_decl->value.fun->body); i++) {
        switch (
          ((struct FunBodyItem *)get__Vec(*fun->fun_decl->value.fun->body, i))
            ->kind) {
            case FunBodyItemKindExpr:
                push__Vec(
                  fun_body,
                  NEW(SymbolTableExpr,
                      check_expression(self,
                                       fun,
                                       ((struct FunBodyItem *)get__Vec(
                                          *fun->fun_decl->value.fun->body, i))
                                         ->expr,
                                       local_value,
                                       local_data_type,
                                       NULL,
                                       false)));

                break;
            case FunBodyItemKindStmt: {
                struct Stmt *stmt = ((struct FunBodyItem *)get__Vec(
                                       *fun->fun_decl->value.fun->body, i))
                                      ->stmt;

                switch (stmt->kind) {
                    case StmtKindAwait:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_await_stmt(
                              self, fun, stmt, local_value, local_data_type)));

                        break;
                    case StmtKindReturn:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_return_stmt(
                              self, fun, stmt, local_value, local_data_type)));

                        break;
                    case StmtKindFor:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_for_stmt(
                              self, fun, stmt, *local_value, local_data_type)));

                        break;
                    case StmtKindIf:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_if_stmt(
                              self, fun, stmt, *local_value, local_data_type)));

                        break;
                    case StmtKindMatch:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_match_stmt(
                              self, fun, stmt, *local_value, local_data_type)));

                        break;
                    case StmtKindTry:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_try_stmt(
                              self, fun, stmt, *local_value, local_data_type)));

                        break;
                    case StmtKindWhile:
                        push__Vec(
                          fun_body,
                          NEW(
                            SymbolTableStmt,
                            check_while_stmt(
                              self, fun, stmt, *local_value, local_data_type)));

                        break;
                    case StmtKindImport:
                    case StmtKindBreak:
                    case StmtKindNext:
                        break;
                }

                break;
            }
        }
    }
}
