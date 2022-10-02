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
#include <base/string.h>
#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/parser/ast.h>
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

static const Int128 MaxUInt8 = 0xFF;
static const Int128 MaxUInt16 = 0xFFFF;
static const Int128 MaxUInt32 = 0xFFFFFFFF;
static const Int128 MaxUInt64 = 0xFFFFFFFFFFFFFFFF;
#define MaxUInt128 (1 << 128) - 1

static const Int128 MinInt8 = -0x80;
static const Int128 MaxInt8 = 0x7F;
static const Int128 MinInt16 = -0x8000;
static const Int128 MaxInt16 = 0x7FFF;
static const Int128 MinInt32 = -0x80000000;
static const Int128 MaxInt32 = 0x7FFFFFFF;
static const Int128 MinInt64 = -0x8000000000000000;
static const Int128 MaxInt64 = 0x7FFFFFFFFFFFFFFF;
#define MinInt128 -(1 << 127)
#define MaxInt128 (1 << 127) - 1

static const Str BuiltinFirstLayer[] = {
    "Int8",   "Int16",  "Int32",   "Int64",    "Int128",  "Uint8",   "Uint16",
    "Uint32", "Uint64", "Uint128", "Bool",     "Float32", "Float64", "Char",
    "Str",    "Isize",  "Usize",   "Optional", "Ptr",     "Tuple",   "Array",
    "Fun",    "Ref",    "Custom",  "Mem",      "Io",      "Never"
};

static struct SearchContext
{
    bool search_type;
    bool search_fun;
    bool search_variant;
    bool search_value;
    bool search_trait;
    bool search_class;
    bool search_object; // Enum and Record object
} SearchContext;

static inline struct Diagnostic *
__new__DiagnosticWithErrTypecheck(struct Typecheck *self,
                                  struct LilyError *err,
                                  struct Location loc,
                                  struct String *detail_msg,
                                  struct Option *help);
static inline struct Diagnostic *
__new__DiagnosticWithWarnTypecheck(struct Typecheck *self,
                                   struct LilyWarning *warn,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help);
static inline struct Diagnostic *
__new__DiagnosticWithNoteTypecheck(struct Typecheck *self,
                                   struct String *note,
                                   struct Location loc,
                                   struct String *detail_msg,
                                   struct Option *help);
static void
verify_if_decl_is_duplicate(struct Typecheck self);
static void
verify_if_decl_is_duplicate_in_module(struct Typecheck self,
                                      struct Decl *module);
static void
verify_if_decl_is_duplicate_in_enum(struct Typecheck self, struct Decl *enum_);
static void
verify_if_decl_is_duplicate_in_record(struct Typecheck self,
                                      struct Decl *record);
static void
verify_if_decl_is_duplicate_in_class(struct Typecheck self, struct Decl *class);
static void
verify_if_decl_is_duplicate_in_tag(struct Typecheck self, struct Decl *tag);
static void
verify_if_decl_is_duplicate_in_trait(struct Typecheck self, struct Decl *trait);
static void
push_all_symbols(struct Typecheck *self);
static struct Vec *
get_local_decl(struct Typecheck *self, struct Scope *scope);
static void
check_constant(struct Typecheck *self,
               struct ConstantSymbol *constant,
               Usize id,
               struct Scope *previous);
static void
check_module(struct Typecheck *self,
             struct ModuleSymbol *module,
             Usize id,
             struct Scope *previous);
static void
check_alias(struct Typecheck *self,
            struct AliasSymbol *alias,
            Usize id,
            struct Scope *previous);
static void
check_enum(struct Typecheck *self,
           struct EnumSymbol *enum_,
           Usize id,
           struct Scope *previous);
static void
check_record(struct Typecheck *self,
             struct RecordSymbol *record,
             Usize id,
             struct Scope *previous);
static void
check_error(struct Typecheck *self,
            struct ErrorSymbol *error,
            Usize id,
            struct Scope *previous);
static void
check_enum_obj(struct Typecheck *self,
               struct EnumObjSymbol *enum_obj,
               Usize id,
               struct Scope *previous);
static void
check_record_obj(struct Typecheck *self,
                 struct RecordObjSymbol *record_obj,
                 Usize id,
                 struct Scope *previous);
static void
check_class(struct Typecheck *self,
            struct ClassSymbol *class,
            Usize id,
            struct Scope *previous);
static void
check_trait(struct Typecheck *self,
            struct TraitSymbol *trait,
            Usize id,
            struct Scope *previous);
static void
check_fun(struct Typecheck *self,
          struct FunSymbol *fun,
          Usize id,
          struct Scope *previous);
static void
check_symbols(struct Typecheck *self);
static struct ModuleSymbol *
entry_in_module(struct Typecheck *self, struct Vec *id, Usize end_idx);
static struct Scope *
search_from_access(struct Typecheck *self,
                   struct Expr *id,
                   struct Vec *name,
                   struct SearchContext search_module_context);
static struct Scope *
search_with_search_module_context(struct Typecheck *self,
                                  struct Vec *name,
                                  struct SearchContext search_module_context);
static struct ModuleSymbol *
search_in_modules_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc);
static struct FunSymbol *
search_in_funs_from_name(struct Typecheck *self,
                         struct String *name,
                         struct Location loc);
static struct EnumSymbol *
search_in_enums_from_name(struct Typecheck *self,
                          struct String *name,
                          struct Location loc);
static struct RecordSymbol *
search_in_records_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc);
static struct EnumObjSymbol *
search_in_enums_obj_from_name(struct Typecheck *self,
                              struct String *name,
                              struct Location loc);
static struct RecordObjSymbol *
search_in_records_obj_from_name(struct Typecheck *self,
                                struct String *name,
                                struct Location loc);
static struct ClassSymbol *
search_in_classes_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc);
static struct TraitSymbol *
search_in_traits_from_name(struct Typecheck *self,
                           struct String *name,
                           struct Location loc);
static struct SymbolTable *
search_module_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope);
static struct SymbolTable *
search_enum_item_in_scope(struct Typecheck *self,
                          struct Expr *id,
                          struct SymbolTable *scope);
static struct SymbolTable *
search_record_item_in_scope(struct Typecheck *self,
                            struct Expr *id,
                            struct SymbolTable *scope);
static struct SymbolTable *
search_enum_obj_item_in_scope(struct Typecheck *self,
                              struct Expr *id,
                              struct SymbolTable *scope);
static struct SymbolTable *
search_record_obj_item_in_scope(struct Typecheck *self,
                                struct Expr *id,
                                struct SymbolTable *scope);
static struct SymbolTable *
search_class_item_in_scope(struct Typecheck *self,
                           struct Expr *id,
                           struct SymbolTable *scope);
static struct SymbolTable *
search_in_custom_scope(struct Typecheck *self,
                       struct Expr *id,
                       struct SymbolTable *scope);
static struct Vec *
search_in_funs_from_fun_call(struct Typecheck *self, struct Expr *id);
static struct Vec *
identifier_access_to_string_vec(struct Expr *id);
static struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                struct Vec *local_decl,
                struct SearchContext ctx);
static struct StmtSymbol
check_stmt(struct Typecheck *self,
           struct Stmt *stmt,
           struct Vec *local_value,
           bool is_return_type);
static struct DataTypeSymbol *
get_data_type_of_expression(struct Typecheck *self,
                            struct ExprSymbol *expr,
                            struct Vec *local_value);
static const Str
get_builtin_module_name_from_data_type(struct DataTypeSymbol *dt);
static struct BuiltinFun *
search_fun_builtin(struct Typecheck *self,
                   const Str module_name,
                   const Str fun_name,
                   Usize param_count);
static void
verify_type_of_fun_builtin(struct BuiltinFun *fun_builtin,
                           Usize param_count,
                           ...);
static inline struct DataTypeSymbol *
get_return_type_of_fun_builtin(struct Typecheck *self,
                               const Str module_name,
                               const Str fun_name,
                               Usize param_count);
static struct DataTypeSymbol *
check_if_defined_data_type_is_equal_to_infered_data_type(
  struct Typecheck *self,
  struct DataTypeSymbol *defined_data_type,
  struct DataTypeSymbol *infered_data_type);
static struct DataTypeSymbol *
infer_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type);
static struct ExprSymbol *
check_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type);
static void
check_fun_body(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Vec *local_value,
               struct Vec *local_data_type);

struct Typecheck
__new__Typecheck(struct Parser parser)
{
    struct Typecheck self = {
        .parser = parser,
        .decl =
          len__Vec(*parser.decls) == 0 ? NULL : get__Vec(*parser.decls, 0),
        .builtins = Load_C_builtins(),
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
run__Typecheck(struct Typecheck *self)
{
    {
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
    for (Usize i = len__Vec(*self.builtins); i--;)
        FREE(BuiltinAll, get__Vec(*self.builtins, i));

    FREE(Vec, self.builtins);

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

static inline struct Diagnostic *
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

static inline struct Diagnostic *
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

static inline struct Diagnostic *
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

static void
verify_if_decl_is_duplicate(struct Typecheck self)
{
    for (Usize i = 0; i < len__Vec(*self.parser.decls); i++) {
        for (Usize j = i + 1; j < len__Vec(*self.parser.decls); j++) {
            if (eq__String(get_name__Decl(get__Vec(*self.parser.decls, i)),
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
                struct Diagnostic *error =
                  NEW(DiagnosticWithErrTypecheck,
                      &self,
                      NEW(LilyError, LilyErrorDuplicateDeclaration),
                      ((struct Decl *)get__Vec(*self.parser.decls, j))->loc,
                      from__String(""),
                      Some(from__String("remove this declaration or move the "
                                        "declaration in other scope")));

                struct Diagnostic *note = NEW(
                  DiagnosticWithNoteTypecheck,
                  &self,
                  format("this declaration is in conflict with a declaration "
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

            exit : {
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

static void
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

static void
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

static void
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

static void
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

static void
verify_if_decl_is_duplicate_in_tag(struct Typecheck self, struct Decl *tag)
{
    if (tag->value.tag->body)
        verify_if_decl_is_duplicate_in_module(self, tag);
}

static void
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

static void
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

static struct Vec *
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

static void
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

        if (!previous)
            ++count_const_id;
    }
}

static void
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
                                      i2->kind == DeclKindConstant)))
                                    assert(0 && "error: duplicate module item");
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

static void
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
                          false))
                        assert(0 && "error: duplicate generic param name");

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

static void
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
                          false))
                        assert(0 && "error: duplicate generic param name");

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
                          false))
                        assert(0 && "error: duplicate variant name");

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

static void
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
                          false))
                        assert(0 && "error: duplicate generic param name");

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
                          false))
                        assert(0 && "error: duplicate variant name");

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

static void
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
                          false))
                        assert(0 && "error: duplicate generic param name");

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

static void
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
                          false))
                        assert(0 && "error: duplicate generic param name");

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

static void
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

static void
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
                                       .search_class = false };

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

static void
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

static void
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
        struct Vec *local_data_type = NULL;
        struct Vec *local_value = NULL;
        struct Vec *local_decl = NULL;
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
                                          .search_object = true });

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
                                                  .search_object = false });

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
                                          .search_object = true });

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
                                      .search_object = true });

            if (dts)
                return_type = dts;

            SUMMARY();
        }

        if (fun_decl->body) {
            TODO("body");
        }

        fun->tagged_type = tagged_type;
        fun->generic_params = generic_params;
        fun->params = params;
        fun->return_type = return_type;
        fun->body = body;

        if (!previous)
            ++count_fun_id;

        if (local_data_type)
            for (Usize i = len__Vec(*local_data_type); i--;)
                FREE(LocalDataType, get__Vec(*local_data_type, i));

        if (local_value)
            for (Usize i = len__Vec(*local_value); i--;)
                FREE(Scope, get__Vec(*local_value, i));
    }
}

static void
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

static struct ModuleSymbol *
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

static struct Scope *
search_from_access(struct Typecheck *self,
                   struct Expr *id,
                   struct Vec *name,
                   struct SearchContext search_module_context)
{
    switch (id->kind) {
        case ExprKindGlobalAccess:
            break;
        case ExprKindIdentifierAccess:
            break;
        default:
            UNREACHABLE("");
    }
}

static struct ModuleSymbol *
search_in_modules_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc)
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

static struct FunSymbol *
search_in_funs_from_name(struct Typecheck *self,
                         struct String *name,
                         struct Location loc)
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

static struct EnumSymbol *
search_in_enums_from_name(struct Typecheck *self,
                          struct String *name,
                          struct Location loc)
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

static struct RecordSymbol *
search_in_records_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc)
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

static struct EnumObjSymbol *
search_in_enums_obj_from_name(struct Typecheck *self,
                              struct String *name,
                              struct Location loc)
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

static struct RecordObjSymbol *
search_in_records_obj_from_name(struct Typecheck *self,
                                struct String *name,
                                struct Location loc)
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

static struct ClassSymbol *
search_in_classes_from_name(struct Typecheck *self,
                            struct String *name,
                            struct Location loc)
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

static struct TraitSymbol *
search_in_traits_from_name(struct Typecheck *self,
                           struct String *name,
                           struct Location loc)
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

static struct SymbolTable *
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

static struct SymbolTable *
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

static struct SymbolTable *
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

static struct SymbolTable *
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

static struct SymbolTable *
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

static struct SymbolTable *
search_class_item_in_scope(struct Typecheck *self,
                           struct Expr *id,
                           struct SymbolTable *scope)
{
    for (Usize i = len__Vec(*scope->value.class->body); i--;) {
        if (id->kind == TokenKindIdentifier) {
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

static struct SymbolTable *
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

static struct Vec *
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

static struct Vec *
identifier_access_to_string_vec(struct Expr *id)
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

static struct DataTypeSymbol *
check_data_type(
  struct Typecheck *self,
  struct Location data_type_loc,
  struct DataType *data_type,
  struct Vec *local_data_type,
  struct Vec *local_decl, // If local_decl is equal to NULL the check_data_type
                          // function search by default in global
  struct SearchContext ctx)
{
    if (ctx.search_object) {
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
                            0),
                          data_type_loc);
                        struct RecordSymbol *record =
                          !enum_
                            ? search_in_records_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0),
                                data_type_loc)
                            : NULL;
                        struct EnumObjSymbol *enum_obj =
                          !record && !enum_
                            ? search_in_enums_obj_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0),
                                data_type_loc)
                            : NULL;
                        struct RecordObjSymbol *record_obj =
                          !enum_ && !record && !enum_obj
                            ? search_in_records_obj_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0),
                                data_type_loc)
                            : NULL;
                        struct ClassSymbol *class =
                          !enum_ && !record && !enum_obj && !record_obj
                            ? search_in_classes_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0),
                                data_type_loc)
                            : NULL;
                        struct TraitSymbol *trait =
                          !enum_ && !record && !enum_obj && !record_obj &&
                              !class
                            ? search_in_traits_from_name(
                                self,
                                get__Vec((*(struct Vec *)
                                             data_type->value.custom->items[0]),
                                         0),
                                data_type_loc)
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
                            .search_class = false
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
            case DataTypeKindChar:
                return NEW(DataTypeSymbol, DataTypeKindChar);
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

static struct StmtSymbol
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

static struct DataTypeSymbol *
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

static const Str
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

static struct BuiltinFun *
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

static void
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

static struct DataTypeSymbol *
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

static struct DataTypeSymbol *
infer_expression(struct Typecheck *self,
                 struct FunSymbol *fun,
                 struct Expr *expr,
                 struct Vec *local_value,
                 struct Vec *local_data_type,
                 struct DataTypeSymbol *defined_data_type,
                 bool is_return_type)
{
#define INFER(dt)                                                              \
    if (defined_data_type)                                                     \
        return copy__DataTypeSymbol(defined_data_type);                        \
    else if (is_return_type && fun && dt)                                      \
        if (fun->return_type)                                                  \
            return copy__DataTypeSymbol(fun->return_type);                     \
        else                                                                   \
            return dt;                                                         \
    else if (!dt)                                                              \
        assert(0 && "error: cannot infer on nil or undef or other value with " \
                    "no specified data type");                                 \
    else                                                                       \
        return dt

    switch (expr->kind) {
        case ExprKindUnaryOp:
            TODO("infer unary op");
        case ExprKindBinaryOp:
            TODO("infer binary op");
        case ExprKindFunCall:
            TODO("infer fun call");
        case ExprKindRecordCall:
            TODO("infer record call");
        case ExprKindIdentifier:
            TODO("infer identifier");
        case ExprKindIdentifierAccess:
            TODO("infer identifier access");
        case ExprKindArrayAccess:
            TODO("infer array access");
        case ExprKindTupleAccess:
            TODO("infer tuple access");
        case ExprKindLambda:
            TODO("infer lambda");
        case ExprKindTuple:
            TODO("infer tuple");
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
            TODO("infer question mark");
        case ExprKindDereference:
            TODO("infer dereference");
        case ExprKindRef:
            INFER(NEW(DataTypeSymbolRef,
                      infer_expression(self,
                                       fun,
                                       expr->value.ref,
                                       local_value,
                                       local_data_type,
                                       defined_data_type,
                                       is_return_type)));
        case ExprKindSelf:
            TODO("infer self");
        case ExprKindUndef:
            INFER(NULL);
        case ExprKindNil:
            INFER(
              NEW(DataTypeSymbolException,
                  NEW(DataTypeSymbolCustom, NULL, from__String("T"), NULL)));
        case ExprKindWildcard:
            break;
        case ExprKindLiteral:
            switch (expr->value.literal.kind) {
                case LiteralKindBool:
                    INFER(NEW(DataTypeSymbol, DataTypeKindBool));
                case LiteralKindChar:
                    INFER(NEW(DataTypeSymbol, DataTypeKindChar));
                case LiteralKindBitChar:
                    INFER(NEW(DataTypeSymbol, DataTypeKindU8));
                case LiteralKindInt32:
                    INFER(NEW(DataTypeSymbol, DataTypeKindI32));
                case LiteralKindInt64:
                    INFER(NEW(DataTypeSymbol, DataTypeKindI64));
                case LiteralKindInt128:
                    INFER(NEW(DataTypeSymbol, DataTypeKindI128));
                case LiteralKindFloat:
                    INFER(NEW(DataTypeSymbol, DataTypeKindF64));
                case LiteralKindBitStr:
                    INFER(NEW(DataTypeSymbolArray,
                              NEW(DataTypeSymbol, DataTypeKindU8),
                              NULL));
                case LiteralKindStr:
                    INFER(NEW(DataTypeSymbol, DataTypeKindStr));
                case LiteralKindUnit:
                    INFER(NEW(DataTypeSymbol, DataTypeKindUnit));
            }
        case ExprKindVariable:
            UNREACHABLE("cannot infer on variable");
    }
}

static struct ExprSymbol *
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
                      NEW(
                        UnaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          !return_type ? get_data_type_of_expression(
                                           self, right, local_value)
                                       : return_type),
                        right));
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
                      NEW(
                        UnaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, module_name, op_str, 2)),
                        right));
                }
                case UnaryOpKindNot: {
                    verify_type_of_fun_builtin(
                      search_fun_builtin(self, "Bool", "not", 2),
                      2,
                      get_data_type_of_expression(self, right, local_value),
                      defined_data_type);

                    return NEW(
                      ExprSymbolUnaryOp,
                      *expr,
                      NEW(
                        UnaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, "Bool", "not", 2)),
                        right));
                }
                    TODO("");
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
                      NEW(
                        BinaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, module_name, op_str, 3)),
                        left,
                        right));
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
                      NEW(
                        BinaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, "Bool", op_str, 3)),
                        left,
                        right));
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
                      NEW(
                        BinaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, "Array", op_str, 3)),
                        left,
                        right));
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
                      NEW(
                        BinaryOpSymbol,
                        *expr,
                        check_if_defined_data_type_is_equal_to_infered_data_type(
                          self,
                          defined_data_type,
                          get_return_type_of_fun_builtin(
                            self, "Str", op_str, 3)),
                        left,
                        right));
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
            TODO("check identifier");
        case ExprKindIdentifierAccess:
            TODO("check identifier access");
        case ExprKindArrayAccess:
            TODO("check array access");
        case ExprKindTupleAccess:
            TODO("check tuple access");
        case ExprKindLambda:
            TODO("check lambda");
        case ExprKindTuple:
            TODO("check tuple");
        case ExprKindArray:
            TODO("check array");
        case ExprKindVariant:
            TODO("check variant");
        case ExprKindTry:
            TODO("check try");
        case ExprKindBlock:
            TODO("check block");
        case ExprKindQuestionMark:
            TODO("check question mark");
        case ExprKindDereference:
            TODO("check dereference");
        case ExprKindRef:
            TODO("check ref");
        case ExprKindSelf:
            TODO("check self");
        case ExprKindUndef:
            TODO("check undef");
        case ExprKindNil:
            TODO("check nil");
        case ExprKindWildcard:
            TODO("check wildcard");
        case ExprKindLiteral:
            TODO("check literal");
        case ExprKindVariable:
            TODO("check variable");
    }
}

static void
check_fun_body(struct Typecheck *self,
               struct FunSymbol *fun,
               struct Vec *local_value,
               struct Vec *local_data_type)
{
    for (Usize i = 0; i < len__Vec(*fun->fun_decl->value.fun->body); i++) {
        switch (
          ((struct FunBodyItem *)get__Vec(*fun->fun_decl->value.fun->body, i))
            ->kind) {
            case FunBodyItemKindExpr: {
                struct Expr *expr = ((struct FunBodyItem *)get__Vec(
                                       *fun->fun_decl->value.fun->body, i))
                                      ->expr;

                switch (expr->kind) {
                }
                break;
            }
            case FunBodyItemKindStmt:
                break;
        }
    }
}
