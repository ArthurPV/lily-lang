#include <base/format.h>
#include <base/macros.h>
#include <base/string.h>
#include <lang/analysis/symbol_table.h>
#include <lang/analysis/typecheck.h>
#include <lang/builtin/builtin.h>
#include <lang/builtin/builtin_c.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/parser/ast.h>

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
check_symbols(struct Typecheck *self);
static Usize *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name);
static struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                bool must_object);

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
check_symbols(struct Typecheck *self)
{
    while (pos < len__Vec(*self->parser.decls)) {
        switch (self->decl->kind) {
            case DeclKindFun: {
                struct Vec *scope_id = NEW(Vec, sizeof(Usize));

                push__Vec(scope_id, (Usize *)current_fun_id);

                struct Scope *scope =
                  NEW(Scope,
                      self->parser.parse_block.scanner.src->file.name,
                      get__Vec(*self->funs, current_fun_id),
                      scope_id,
                      ScopeItemKindFun,
                      ScopeKindGlobal);
                struct FunSymbol *fun_symbol =
                  ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id));
                struct FunDecl *fun_decl = fun_symbol->fun_decl->value.fun;
                struct Vec *tagged_type = NULL;

                if (fun_decl->tags != NULL) {
                    if (len__Vec(*fun_decl->tags) > 0) {
                        tagged_type = NEW(Vec, sizeof(struct Tuple));

                        for (Usize i = len__Vec(*fun_decl->tags); i--;) {
                            struct Tuple *current =
                              get__Vec(*fun_decl->tags, i);
                            struct DataTypeSymbol *dts = check_data_type(
                              self,
                              *(struct Location *)current->items[1],
                              current->items[0],
                              NULL,
                              true);

                            if (dts != NULL)
                                push__Vec(tagged_type, dts);
                        }
                    }
                }

                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->scope = scope;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->tagged_type = tagged_type;
                ++current_fun_id;

                break;
            }
            default:
                TODO("check symbols");
        }

        NEXT_DECL();
    }
}

static Usize *
search_in_enums_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (self->enums_obj == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->enums_obj); i--;) {
        struct EnumObjSymbol *symb = get__Vec(*self->enums_obj, i);

        if (eq__String(name, symb->name, true))
            return (Usize *)i;
    }

    return NULL;
}

static Usize *
search_in_records_obj_from_name(struct Typecheck *self, struct String *name)
{
    if (self->records_obj == NULL)
        return NULL;

    for (Usize i = len__Vec(*self->records_obj); i--;) {
        struct RecordObjSymbol *symb = get__Vec(*self->records_obj, i);

        if (eq__String(name, symb->name, true))
            return (Usize *)i;
    }

    return NULL;
}

static struct DataTypeSymbol *
check_data_type(struct Typecheck *self,
                struct Location data_type_loc,
                struct DataType *data_type,
                struct Vec *local_data_type,
                bool must_object)
{
    struct Vec *id = NULL;

    if (must_object) {
        switch (data_type->kind) {
            case DataTypeKindCustom: {
                if (len__Vec(
                      *(struct Vec *)data_type->value.custom->items[0]) == 1) {
                    Usize *id_enums_obj = search_in_enums_obj_from_name(
                      self,
                      get__Vec(
                        (*(struct Vec *)data_type->value.custom->items[0]), 0));
                    Usize *id_records_obj =
                      id_enums_obj == NULL
                        ? search_in_records_obj_from_name(
                            self,
                            get__Vec((*(struct Vec *)
                                         data_type->value.custom->items[0]),
                                     0))
                        : NULL;
                    struct Vec *generic_params = NULL;

                    if (data_type->value.custom->items[1] == NULL) {
                    return_data_type : {
                        if (id_enums_obj == NULL && id_records_obj == NULL) {
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
                        }
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
                                                      false));
                        }

                        goto return_data_type;
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
}
