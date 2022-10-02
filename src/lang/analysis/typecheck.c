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
search_in_enums_from_name(struct Typecheck *self, struct String *name);
static Usize *
search_in_records_from_name(struct Typecheck *self, struct String *name);
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
                                          true);

                        if (dts != NULL)
                            push__Vec(tagged_type, dts);
                    }
                }

                if (fun_decl->generic_params != NULL) {
                    TODO("generic params");
                }

                if (fun_decl->params != NULL) {
                    TODO("params");
                }

                if (fun_decl->return_type != NULL) {
                    TODO("return type");
                }

                if (fun_decl->body != NULL) {
                    TODO("body");
                }

                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->scope = scope;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->tagged_type = tagged_type;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->generic_params = generic_params;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->params = params;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->return_type = return_type;
                ((struct FunSymbol *)get__Vec(*self->funs, current_fun_id))
                  ->body = body;
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
                    struct Vec *generic_params = NULL;

                    if (data_type->value.custom->items[1] == NULL) {
                    return_data_type : {
                        if (id_enums == NULL && id_records == NULL &&
                            id_enums_obj == NULL && id_records_obj == NULL) {
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
                        } else if ((id_enums != NULL || id_records != NULL) &&
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
                                           false));
            case DataTypeKindRef:
                return NEW(DataTypeSymbolRef,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.ref,
                                           local_data_type,
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
                                           false));
            case DataTypeKindUnit:
                return NEW(DataTypeSymbol, DataTypeKindUnit);
            case DataTypeKindException:
                return NEW(DataTypeSymbolException,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.optional,
                                           local_data_type,
                                           false));
            case DataTypeKindMut:
                return NEW(DataTypeSymbolMut,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.mut,
                                           local_data_type,
                                           false));
            case DataTypeKindLambda:
                TODO("check lambda type");
                return NEW(DataTypeSymbolLambda, NULL, NULL);
            case DataTypeKindArray:
                return NEW(DataTypeSymbolArray,
                           check_data_type(self,
                                           data_type_loc,
                                           data_type->value.array->items[0],
                                           local_data_type,
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
                                      false));

                return NEW(DataTypeSymbolTuple, tuple);
            }
            case DataTypeKindCustom:
                goto custom_data_type;
        }
    }

    return NULL;
}
