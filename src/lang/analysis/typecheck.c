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
static Usize current_fun_id = 0;
static Usize current_const_id = 0;
static Usize current_module_id = 0;
static Usize current_alias_id = 0;
static Usize current_record_id = 0;
static Usize current_enum_id = 0;
static Usize current_error_id = 0;
static Usize current_class_id = 0;
static Usize current_trait_id = 0;
static Usize current_record_obj_id = 0;
static Usize current_enum_obj_id = 0;

static Int128 MinUInt8 = 0x0;
static Int128 MaxUInt8 = 0xFF;
static Int128 MinUInt16 = 0x100;
static Int128 MaxUInt16 = 0xFFFF;
static Int128 MinUInt32 = 0x10000;
static Int128 MaxUInt32 = 0xFFFFFFFF;
static Int128 MinUInt64 = 0x100000000;
static Int128 MaxUInt64 = 0xFFFFFFFFFFFFFFFF;

static Int128 MinInt8 = -0x80;
static Int128 MaxInt8 = 0x7F;
static Int128 MinInt16 = -0x8000;
static Int128 MaxInt16 = 0x7FFF;
static Int128 MinInt32 = -0x80000000;
static Int128 MaxInt32 = 0x7FFFFFFF;
static Int128 MinInt64 = -0x8000000000000000;
static Int128 MaxInt64 = 0x7FFFFFFFFFFFFFFF;

static struct SearchModuleContext
{
    bool search_type;
    bool search_fun;
    bool search_variant;
    bool search_value;
} SearchModuleContext;

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
push_all_symbols(struct Typecheck *self);
static void
check_fun(struct Typecheck *self, struct FunSymbol *fun, struct Vec *scope_id);
static void
check_symbols(struct Typecheck *self);
static struct Scope *
search_in_modules_from_name(struct Typecheck *self,
                            struct Vec *name,
                            struct SearchModuleContext search_module_context);
static Usize *
search_in_enums_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_records_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_classes_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_traits_from_name(struct Typecheck *self, struct String *name);
static struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                bool must_object,
                bool must_trait);
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
    push_all_symbols(self);
    check_symbols(self);
}

void
__free__Typecheck(struct Typecheck self)
{
    for (Usize i = len__Vec(*self.builtins); i--;)
        FREE(BuiltinAll, get__Vec(*self.builtins, i));

    FREE(Vec, self.builtins);

    if (self.funs != NULL) {
        for (Usize i = len__Vec(*self.funs); i--;)
            FREE(FunSymbol, get__Vec(*self.funs, i));

        FREE(Vec, self.funs);
    }

    if (self.consts != NULL) {
        for (Usize i = len__Vec(*self.consts); i--;)
            FREE(ConstantSymbol, get__Vec(*self.consts, i));

        FREE(Vec, self.consts);
    }

    if (self.modules != NULL) {
        for (Usize i = len__Vec(*self.modules); i--;)
            FREE(ModuleSymbol, get__Vec(*self.modules, i));

        FREE(Vec, self.modules);
    }

    if (self.aliases != NULL) {
        for (Usize i = len__Vec(*self.aliases); i--;)
            FREE(AliasSymbol, get__Vec(*self.aliases, i));

        FREE(Vec, self.aliases);
    }

    if (self.records != NULL) {
        for (Usize i = len__Vec(*self.records); i--;)
            FREE(RecordSymbol, get__Vec(*self.records, i));

        FREE(Vec, self.records);
    }

    if (self.records_obj != NULL) {
        for (Usize i = len__Vec(*self.records_obj); i--;)
            FREE(RecordObjSymbol, get__Vec(*self.records_obj, i));

        FREE(Vec, self.records_obj);
    }

    if (self.enums != NULL) {
        for (Usize i = len__Vec(*self.enums); i--;)
            FREE(EnumSymbol, get__Vec(*self.enums, i));

        FREE(Vec, self.enums);
    }

    if (self.enums_obj != NULL) {
        for (Usize i = len__Vec(*self.enums_obj); i--;)
            FREE(EnumObjSymbol, get__Vec(*self.enums_obj, i));

        FREE(Vec, self.enums_obj);
    }

    if (self.errors != NULL) {
        for (Usize i = len__Vec(*self.errors); i--;)
            FREE(ErrorSymbol, get__Vec(*self.errors, i));

        FREE(Vec, self.errors);
    }

    if (self.classes != NULL) {
        for (Usize i = len__Vec(*self.classes); i--;)
            FREE(ClassSymbol, get__Vec(*self.classes, i));

        FREE(Vec, self.classes);
    }

    if (self.traits != NULL) {
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

#define NEXT_DECL()                                                      \
    pos += 1;                                                            \
    self->decl = pos < len__Vec(*self->parser.decls)                     \
                   ? ((struct Decl *)get__Vec(*self->parser.decls, pos)) \
                   : NULL

#define ALLOC_FUNS()        \
    if (self->funs == NULL) \
    self->funs = NEW(Vec, sizeof(struct FunSymbol))

#define ALLOC_CONSTS()        \
    if (self->consts == NULL) \
    self->consts = NEW(Vec, sizeof(struct ConstantSymbol))

#define ALLOC_MODULES()        \
    if (self->modules == NULL) \
    self->modules = NEW(Vec, sizeof(struct ModuleSymbol))

#define ALLOC_ALIASES()        \
    if (self->aliases == NULL) \
    self->aliases = NEW(Vec, sizeof(struct AliasSymbol))

#define ALLOC_RECORDS()        \
    if (self->records == NULL) \
    self->records = NEW(Vec, sizeof(struct RecordSymbol))

#define ALLOC_RECORDS_OBJ()        \
    if (self->records_obj == NULL) \
    self->records_obj = NEW(Vec, sizeof(struct RecordObjSymbol))

#define ALLOC_ENUMS()        \
    if (self->enums == NULL) \
    self->enums = NEW(Vec, sizeof(struct EnumSymbol))

#define ALLOC_ENUMS_OBJ()        \
    if (self->enums_obj == NULL) \
    self->enums_obj = NEW(Vec, sizeof(struct EnumObjSymbol))

#define ALLOC_ERRORS()        \
    if (self->errors == NULL) \
    self->errors = NEW(Vec, sizeof(struct ErrorSymbol))

#define ALLOC_CLASSES()        \
    if (self->classes == NULL) \
    self->classes = NEW(Vec, sizeof(struct ClassSymbol))

#define ALLOC_TRAITS()        \
    if (self->traits == NULL) \
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

static void
check_fun(struct Typecheck *self, struct FunSymbol *fun, struct Vec *scope_id)
{
    if (fun->scope == NULL) {
        struct FunDecl *fun_decl = fun->fun_decl->value.fun;
        struct Vec *local_data_type = NULL;
        struct Vec *local_value = NULL;
        struct Vec *tagged_type = NULL;
        struct Vec *generic_params = NULL;
        struct Vec *params = NULL;
        struct DataTypeSymbol *return_type = NULL;
        struct Vec *body = NULL;

        if (fun_decl->tags != NULL) {
            tagged_type = NEW(Vec, sizeof(struct Tuple));

            for (Usize i = len__Vec(*fun_decl->tags); i--;) {
                struct Tuple *current = get__Vec(*fun_decl->tags, i);
                struct DataTypeSymbol *dts =
                  check_data_type(self,
                                  *(struct Location *)current->items[1],
                                  current->items[0],
                                  NULL,
                                  true,
                                  false);

                if (dts != NULL)
                    push__Vec(tagged_type, dts);
            }

            SUMMARY();
        }

        if (fun_decl->generic_params != NULL) {
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
                          NULL,
                          false,
                          false);

                        if (dts != NULL)
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

        if (fun_decl->params != NULL) {
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
                  false,
                  false);

                if (dts != NULL) {
                    param->param_data_type =
                      ((struct FunParam *)get__Vec(*fun_decl->params, i))
                            ->param_data_type != NULL
                        ? NEW(
                            Tuple,
                            2,
                            dts,
                            ((struct FunParam *)get__Vec(*fun_decl->params, i))
                              ->param_data_type->items[1])
                        : NULL;
                }

                if (param->default_ != NULL)
                    TODO("");

                push__Vec(ids, (Usize *)i);
                push__Vec(params, param);
                push__Vec(local_value,
                          NEW(Scope,
                              self->parser.parse_block.scanner.src->file.name,
                              param->name,
                              ids,
                              ScopeItemKindParam,
                              ScopeKindLocal));
            }
        }

        if (fun_decl->return_type != NULL) {
            struct DataTypeSymbol *dts = check_data_type(
              self,
              *(struct Location *)fun_decl->return_type->items[1],
              fun_decl->return_type->items[0],
              local_data_type,
              false,
              false);

            if (dts != NULL)
                return_type = dts;

            SUMMARY();
        }

        if (fun_decl->body != NULL) {
            TODO("body");
        }

        fun->scope = NEW(Scope,
                         self->parser.parse_block.scanner.src->file.name,
                         fun->name,
                         scope_id,
                         ScopeItemKindFun,
                         ScopeKindGlobal);
        fun->tagged_type = tagged_type;
        fun->generic_params = generic_params;
        fun->params = params;
        fun->return_type = return_type;
        fun->body = body;
        ++current_fun_id;

        if (local_data_type != NULL)
            for (Usize i = len__Vec(*local_data_type); i--;)
                FREE(LocalDataType, get__Vec(*local_data_type, i));

        if (local_value != NULL)
            for (Usize i = len__Vec(*local_value); i--;)
                FREE(Scope, get__Vec(*local_value, i));
    }
}

static void
check_symbols(struct Typecheck *self)
{
    while (pos < len__Vec(*self->parser.decls)) {
        switch (self->decl->kind) {
            case DeclKindFun: {
                struct Vec *scope_id = NEW(Vec, sizeof(Usize));

                push__Vec(scope_id, (Usize *)current_fun_id);
                check_fun(
                  self, get__Vec(*self->funs, current_fun_id), scope_id);

                break;
            }
            default:
                TODO("check symbols");
        }

        NEXT_DECL();
    }
}

static struct Scope *
search_in_modules_from_name(struct Typecheck *self,
                            struct Vec *name,
                            struct SearchModuleContext search_module_context)
{
    if (self->modules != NULL)
        return NULL;

    struct Vec *ids = NEW(Vec, sizeof(Usize));

    for (Usize i = len__Vec(*self->modules); i--;)
        if (eq__String(
              get__Vec(*name, 0),
              ((struct ModuleSymbol *)get__Vec(*self->modules, i))->name,
              false)) {
            push__Vec(ids, (Usize *)i);
            break;
        }

    if (len__Vec(*ids) == 0) {
        FREE(Vec, ids);
        assert(0 && "error");
        return NULL;
    }

#define VALID_CONTEXT(kind)                          \
    switch (kind) {                                  \
        case DeclKindRecord:                         \
        case DeclKindEnum:                           \
        case DeclKindClass:                          \
        case DeclKindTrait:                          \
        case DeclKindAlias:                          \
            if (!search_module_context.search_type)  \
                assert(0 && "error");                \
            break;                                   \
        case DeclKindFun:                            \
            if (!search_module_context.search_fun)   \
                assert(0 && "error");                \
            break;                                   \
        case DeclKindConstant:                       \
            if (!search_module_context.search_value) \
                assert(0 && "error");                \
            break;                                   \
        default:                                     \
            break;                                   \
    }

#define CHECK_ID(decl, kind)                           \
    if (eq__String(current_name, decl->name, false)) { \
        *id = j;                                       \
        push__Vec(ids, id);                            \
        VALID_CONTEXT(kind);                           \
    }                                                  \
    if (len__Vec(*name) != i)                          \
        assert(0 && "error");                          \
    break;

    struct Decl *current_module =
      get__Vec(*self->parser.decls, *(Usize *)get__Vec(*ids, 0));
    struct Decl *current_decl = NULL;
    struct String *current_name = NULL;

    for (Usize i = 1; i < len__Vec(*name); i++) {
        current_name = get__Vec(*name, i);

        if (((struct ModuleBodyItem *)get__Vec(
               *current_module->value.module->body, 0))
              ->kind == ModuleBodyItemKindDecl) {
            current_decl =
              ((struct ModuleBodyItem *)get__Vec(
                 *current_module->value.module->body,
                 len__Vec(*current_module->value.module->body) - 1))
                ->value.decl;
            Usize *id = NULL;
            bool entry_in_module = false;

            for (Usize j = len__Vec(*current_module->value.module->body);
                 j--;) {
                switch (current_decl->kind) {
                    case DeclKindRecord:
                        CHECK_ID(current_decl->value.record,
                                 current_decl->kind);
                    case DeclKindEnum:
                        CHECK_ID(current_decl->value.enum_, current_decl->kind);
                    case DeclKindClass:
                        CHECK_ID(current_decl->value.class, current_decl->kind);
                    case DeclKindTrait:
                        CHECK_ID(current_decl->value.trait, current_decl->kind);
                    case DeclKindFun:
                        CHECK_ID(current_decl->value.fun, current_decl->kind);
                    case DeclKindError:
                        CHECK_ID(current_decl->value.error, current_decl->kind);
                    case DeclKindAlias:
                        CHECK_ID(current_decl->value.alias, current_decl->kind);
                    case DeclKindConstant:
                        CHECK_ID(current_decl->value.constant,
                                 current_decl->kind);
                    case DeclKindModule:
                        if (eq__String(current_decl->value.module->name,
                                       current_name,
                                       false)) {
                            entry_in_module = true;
                            *id = j;
                            current_module = current_decl;

                            push__Vec(ids, id);
                        }
                        break;
                    case DeclKindTag:
                        break;
                    case DeclKindImport:
                        UNREACHABLE("");
                }

                if (id != NULL || entry_in_module)
                    break;

                current_decl = ((struct ModuleBodyItem *)get__Vec(
                                  *current_module->value.module->body, j))
                                 ->value.decl;
            }

            if (id == NULL) {
                assert(0 && "error");
                return NULL;
            }
        }
    }

    if (current_decl == NULL)
        assert(0 && "error");

    if (len__Vec(*ids) != len__Vec(*name)) {
        assert(0 && "error");
        return NULL;
    }

#define SCOPE(kind)                                      \
    NEW(Scope,                                           \
        self->parser.parse_block.scanner.src->file.name, \
        current_name,                                    \
        ids,                                             \
        kind,                                            \
        ScopeKindGlobal)

    switch (current_decl->kind) {
        case DeclKindRecord:
            if (current_decl->value.record->is_object)
                return SCOPE(ScopeItemKindRecordObj);
            else
                return SCOPE(ScopeItemKindRecord);
        case DeclKindEnum:
            if (current_decl->value.record->is_object)
                return SCOPE(ScopeItemKindEnumObj);
            else
                return SCOPE(ScopeItemKindEnum);
        case DeclKindClass:
            return SCOPE(ScopeItemKindClass);
        case DeclKindTrait:
            return SCOPE(ScopeItemKindTrait);
        case DeclKindFun:
            return SCOPE(ScopeItemKindFun);
        case DeclKindError:
            return SCOPE(ScopeItemKindError);
        case DeclKindAlias:
            return SCOPE(ScopeItemKindAlias);
        case DeclKindConstant:
            return SCOPE(ScopeItemKindConstant);
        case DeclKindModule:
            return SCOPE(ScopeItemKindModule);
        default:
            UNREACHABLE("");
    }
}

static Usize *
search_in_enums_from_name(struct Typecheck *self, struct String *name)
{
    if (self->enums == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->enums); i--;)
        if (eq__String(name,
                       ((struct EnumSymbol *)get__Vec(*self->enums, i))->name,
                       false))
            return (Usize *)i;

    return NULL;
}

static Usize *
search_in_records_from_name(struct Typecheck *self, struct String *name)
{
    if (self->records == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->records); i--;)
        if (eq__String(
              name,
              ((struct RecordSymbol *)get__Vec(*self->records, i))->name,
              false))
            return (Usize *)i;

    return NULL;
}

static Usize *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (self->enums_obj == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->enums_obj); i--;)
        if (eq__String(
              name,
              ((struct EnumObjSymbol *)get__Vec(*self->enums_obj, i))->name,
              false))
            return (Usize *)i;

    return NULL;
}

static Usize *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (self->records_obj == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->records_obj); i--;)
        if (eq__String(
              name,
              ((struct RecordObjSymbol *)get__Vec(*self->records_obj, i))->name,
              false))
            return (Usize *)i;

    return NULL;
}

static Usize *
search_in_classes_from_name(struct Typecheck *self, struct String *name)
{
    if (self->classes == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->classes); i--;)
        if (eq__String(
              name,
              ((struct ClassSymbol *)get__Vec(*self->classes, i))->name,
              false))
            return (Usize *)i;

    return NULL;
}

static Usize *
search_in_traits_from_name(struct Typecheck *self, struct String *name)
{
    if (self->traits == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->traits); i--;)
        if (eq__String(name,
                       ((struct TraitSymbol *)get__Vec(*self->traits, i))->name,
                       false))
            return (Usize *)i;

    return NULL;
}

static struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                bool must_object,
                bool must_trait)
{
    struct Vec *id = NULL;

    if (must_object) {
        switch (data_type->kind) {
            case DataTypeKindCustom: {
            custom_data_type : {
                if (len__Vec(
                      *(struct Vec *)data_type->value.custom->items[0]) == 1) {
                    Usize *id_enums = search_in_enums_from_name(
                      self,
                      get__Vec(
                        (*(struct Vec *)data_type->value.custom->items[0]), 0));
                    Usize *id_records =
                      id_enums == NULL
                        ? search_in_records_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    Usize *id_enums_obj =
                      id_records == NULL && id_enums == NULL
                        ? search_in_enums_obj_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    Usize *id_records_obj =
                      id_enums == NULL && id_records == NULL &&
                          id_enums_obj == NULL
                        ? search_in_records_obj_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    Usize *id_classes =
                      id_enums == NULL && id_records == NULL &&
                          id_enums_obj == NULL && id_records_obj == NULL
                        ? search_in_classes_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    Usize *id_traits =
                      id_enums == NULL && id_records == NULL &&
                          id_enums_obj == NULL && id_records_obj == NULL &&
                          id_classes == NULL
                        ? search_in_traits_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    struct Vec *generic_params = NULL;

                    if (data_type->value.custom->items[1] == NULL) {
                    return_data_type : {
                        if (id_enums == NULL && id_records == NULL &&
                            id_enums_obj == NULL && id_records_obj == NULL &&
                            id_classes == NULL && id_traits == NULL) {
                            if (len__Vec(*(struct Vec *)data_type->value.custom
                                            ->items[0]) == 1 &&
                                local_data_type != NULL) {
                                for (Usize i = len__Vec(*local_data_type);
                                     i--;) {
                                    if (eq__String(
                                          ((struct LocalDataType *)get__Vec(
                                             *local_data_type, i))
                                            ->name,
                                          get__Vec(*(struct Vec *)data_type
                                                      ->value.custom->items[0],
                                                   0),
                                          false)) {
                                        id = NEW(Vec, sizeof(Usize));

                                        push__Vec(id, (Usize *)i);

                                        return NEW(
                                          DataTypeSymbolCustom,
                                          generic_params,
                                          NULL,
                                          NEW(Scope,
                                              self->parser.parse_block.scanner
                                                .src->file.name,
                                              (struct String *)get__Vec(
                                                (*(struct Vec *)data_type->value
                                                    .custom->items[0]),
                                                0),
                                              id,
                                              ScopeItemKindGeneric,
                                              ScopeKindLocal));
                                    }
                                }
                            }

                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrTypecheck,
                                  self,
                                  NEW(LilyError, LilyErrorUnknownDataType),
                                  data_type_loc,
                                  from__String(""),
                                  None());

                            err->err->s =
                              format("{S}",
                                     (struct String *)get__Vec(
                                       (*(struct Vec *)
                                           data_type->value.custom->items[0]),
                                       0));

                            emit__Diagnostic(err);

                            return NULL;
                        } else if (id_enums_obj != NULL) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_enums_obj);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindEnumObj,
                                ScopeKindGlobal));
                        } else if (id_records_obj != NULL) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_records_obj);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindRecordObj,
                                ScopeKindGlobal));
                        } else if (id_enums == NULL && id_records == NULL &&
                                   must_object) {
                            struct Diagnostic *err = NEW(
                              DiagnosticWithErrTypecheck,
                              self,
                              NEW(LilyError,
                                  LilyErrorExpectedEnumObjectOrRecordObject),
                              data_type_loc,
                              from__String(""),
                              None());

                            struct Diagnostic *note =
                              NEW(DiagnosticWithNoteTypecheck,
                                  self,
                                  from__String("replace `type` by `object` in "
                                               "current declaration"),
                                  id_enums != NULL
                                    ? ((struct Decl *)get__Vec(
                                         *self->parser.decls, *id_enums))
                                        ->loc
                                    : ((struct Decl *)get__Vec(
                                         *self->parser.decls, *id_records))
                                        ->loc,
                                  from__String(""),
                                  None());

                            emit__Diagnostic(err);
                            emit__Diagnostic(note);

                            return NULL;
                        } else if (id_enums != NULL) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_enums);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindEnum,
                                ScopeKindGlobal));

                        } else if (id_records != NULL) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_records);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindRecord,
                                ScopeKindGlobal));
                        } else if (id_classes != NULL) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_classes);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindClass,
                                ScopeKindGlobal));
                        } else if (id_traits != NULL && must_trait) {
                            id = NEW(Vec, sizeof(Usize));

                            push__Vec(id, id_traits);

                            return NEW(
                              DataTypeSymbolCustom,
                              generic_params,
                              NULL,
                              NEW(
                                Scope,
                                self->parser.parse_block.scanner.src->file.name,
                                (struct String *)get__Vec(
                                  (*(struct Vec *)
                                      data_type->value.custom->items[0]),
                                  0),
                                id,
                                ScopeItemKindTrait,
                                ScopeKindGlobal));
                        } else if (id_traits != NULL && !must_trait)
                            assert(0 && "error");
                    }
                    } else {
                        generic_params =
                          NEW(Vec, sizeof(struct DataTypeSymbol));

                        for (Usize i = len__Vec(*(struct Vec *)data_type->value
                                                   .custom->items[1]);
                             i--;) {
                            struct DataType *current = get__Vec(
                              *(struct Vec *)data_type->value.custom->items[1],
                              i);
                            push__Vec(generic_params,
                                      check_data_type(self,
                                                      data_type_loc,
                                                      current,
                                                      local_data_type,
                                                      false,
                                                      false));
                        }

                        goto return_data_type;
                    }
                } else {
                    struct SearchModuleContext search_module_context = {
                        .search_fun = false,
                        .search_type = true,
                        .search_value = false,
                        .search_variant = false
                    };
                    return NEW(DataTypeSymbolCustom,
                               data_type->value.custom->items[1],
                               NULL,
                               search_in_modules_from_name(
                                 self,
                                 data_type->value.custom->items[0],
                                 search_module_context));
                    return NULL;
                }
            } break;
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
                                           false,
                                           false));
            case DataTypeKindRef:
                return NEW(DataTypeSymbolRef,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.ref,
                                           local_data_type,
                                           false,
                                           false));
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
                                           false,
                                           false));
            case DataTypeKindUnit:
                return NEW(DataTypeSymbol, DataTypeKindUnit);
            case DataTypeKindException:
                return NEW(DataTypeSymbolException,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.optional,
                                           local_data_type,
                                           false,
                                           false));
            case DataTypeKindMut:
                return NEW(DataTypeSymbolMut,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.mut,
                                           local_data_type,
                                           false,
                                           false));
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
                        false,
                        false));

                return NEW(DataTypeSymbolLambda,
                           params,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.lambda->items[1],
                                           local_data_type,
                                           false,
                                           false));
            }
            case DataTypeKindArray:
                return NEW(DataTypeSymbolArray,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.array->items[0],
                                           local_data_type,
                                           false,
                                           false),
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
                                      false,
                                      false));

                return NEW(DataTypeSymbolTuple, tuple);
            }
            case DataTypeKindCustom:
                goto custom_data_type;
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

    assert(module != NULL && "module is not found");

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
    if (defined_data_type != NULL)
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
    if (defined_data_type != NULL)                                             \
        return copy__DataTypeSymbol(defined_data_type);                        \
    else if (is_return_type && fun != NULL && dt != NULL)                      \
        if (fun->return_type != NULL)                                          \
            return copy__DataTypeSymbol(fun->return_type);                     \
        else                                                                   \
            return dt;                                                         \
    else if (dt == NULL)                                                       \
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
                          return_type == NULL ? get_data_type_of_expression(
                                                  self, right, local_value)
                                              : return_type),
                        right));
                }
                case UnaryOpKindBitNot:
                case UnaryOpKindNegative: {
                    const Str module_name =
                      get_builtin_module_name_from_data_type(
                        get_data_type_of_expression(self, right, local_value));
                    const Str op_str = to_str__UnaryOpKind(expr->value.unary_op.kind);

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

                    if (fun_builtin == NULL)
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
                    TODO("check binary op");
            }
        }
        case ExprKindFunCall:
            TODO("check fun call");
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
