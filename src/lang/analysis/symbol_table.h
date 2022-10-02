#ifndef LILY_SYMBOL_TABLE_H
#define LILY_SYMBOL_TABLE_H

#include <lang/parser/ast.h>

enum ScopeItemKind
{
    ScopeItemKindVariable,
    ScopeItemKindParam,
    ScopeItemKindRecord,
    ScopeItemKindAlias,
    ScopeItemKindEnum,
    ScopeItemKindRecordObj,
    ScopeItemKindEnumObj,
    ScopeItemKindConstant,
    ScopeItemKindVariant,
    ScopeItemKindFun,
    ScopeItemKindError,
    ScopeItemKindClass,
    ScopeItemKindModule,
    ScopeItemKindTrait,
    ScopeItemKindGeneric
};

enum ScopeKind
{
    ScopeKindGlobal,
    ScopeKindLocal
};

typedef struct Scope
{
    Str filename;
    struct String *name; // struct String&
    struct Vec *id;      // struct Vec<Usize*>*
    enum ScopeItemKind item_kind;
    enum ScopeKind kind;
} ScopdId;

/**
 *
 * @brief Construct the Scope type.
 */
inline struct Scope *
__new__Scope(const Str filename,
             struct String *name,
             struct Vec *id,
             enum ScopeItemKind item_kind,
             enum ScopeKind kind)
{
    struct Scope *self = malloc(sizeof(struct Scope));
    self->filename = filename;
    self->name = name;
    self->id = id;
    self->item_kind = item_kind;
    self->kind = kind;
    return self;
}

/**
 *
 * @brief Free the Scope type.
 */
inline void
__free__Scope(struct Scope *scope)
{
    FREE(Vec, scope->id);
    free(scope);
}

typedef struct LocalDataType
{
    struct String *name; // struct String&
    struct Tuple
      *restricted; // struct Tuple<struct DataTypeSymbol*, struct Location&>*
} LocalDataType;

/**
 *
 * @brief Construct the LocalDataType type.
 */
inline struct LocalDataType *
__new__LocalDataType(struct String *name, struct Tuple *restricted)
{
    struct LocalDataType *self = malloc(sizeof(struct LocalDataType));
    self->name = name;
    self->restricted = restricted;
    return self;
}

/**
 *
 * @brief Free the LocalDataType type.
 */
void
__free__LocalDataType(struct LocalDataType *self);

enum Visibility
{
    VisibilityPublic,
    VisibilityPrivate
};

#define VISIBILITY(self) \
    self->is_pub == true ? VisibilityPublic : VisibilityPrivate;

typedef struct CompilerDefinedDataType
{
    Str name;
    bool is_args;
} CompilerDefinedDataType;

/**
 *
 * @brief Construct the CompilerDefinedDataType type.
 */
inline struct CompilerDefinedDataType
__new__CompilerDefinedDataType(Str name, bool is_args)
{
    struct CompilerDefinedDataType self = {.name = name, .is_args = is_args};
    return self;
}

typedef struct DataTypeSymbol
{
    enum DataTypeKind kind;
    struct Scope *scope;
    union
    {
        struct DataTypeSymbol *ptr;
        struct DataTypeSymbol *ref;
        struct DataTypeSymbol *optional;
        struct DataTypeSymbol *exception;
        struct DataTypeSymbol *mut;
        struct Tuple *lambda; // struct Tuple<struct Vec<struct
                              // DataTypeSymbol*>*, struct DataTypeSymbol*>*
        struct Tuple *array;  // struct Tuple<struct DataTypeSymbol*, Usize*>*
        struct Vec *custom;   // struct Vec<struct DataTypeSymbol*>*
        struct Vec *tuple;    // struct Vec<struct DataTypeSymbol*>*
        struct CompilerDefinedDataType compiler_defined; // struct CompilerDefinedDataType
    } value;

    union
    {
        struct String *custom_name;
    };
} DataTypeSymbol;

/**
 *
 * @brief Construct the DataTypeSymbol type.
 */
struct DataTypeSymbol *
__new__DataTypeSymbol(enum DataTypeKind kind);

/**
 *
 * @brief Construct the DataTypeSymbol type (Ptr variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolPtr(struct DataTypeSymbol *ptr);

/**
 *
 * @brief Construct the DataTypeSymbol type (Ref variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolRef(struct DataTypeSymbol *ref);

/**
 *
 * @brief Construct the DataTypeSymbol type (Optional variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolOptional(struct DataTypeSymbol *optional);

/**
 *
 * @brief Construct the DataTypeSymbol type (Exception variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolException(struct DataTypeSymbol *exception);

/**
 *
 * @brief Construct the DataTypeSymbol type (Mut variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolMut(struct DataTypeSymbol *mut);

/**
 *
 * @brief Construct the DataTypeSymbol type (Lambda variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolLambda(struct Vec *params,
                            struct DataTypeSymbol *return_type);

/**
 *
 * @brief Construct the DataTypeSymbol type (Array variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolArray(struct DataTypeSymbol *data_type, Usize *size);

/**
 *
 * @brief Construct the DataTypeSymbol type (Custom variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolCustom(struct Vec *generic_params,
                            struct String *custom_name,
                            struct Scope *scope);

/**
 *
 * @brief Construct the DataTypeSymbol type (Tuple variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolTuple(struct Vec *tuple);

/**
 *
 * @brief Construct the DataTypeSymbol type (CompilerDefinedDataType variant).
 */
struct DataTypeSymbol *
__new__DataTypeSymbolCompilerDefined(struct CompilerDefinedDataType compiler_defined);

/**
 *
 * @brief Copy the DataTypeSymbol type.
 */
struct DataTypeSymbol *
copy__DataTypeSymbol(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (all variants).
 */
void
__free__DataTypeSymbolAll(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type.
 */
inline void
__free__DataTypeSymbol(struct DataTypeSymbol *self)
{
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Ptr variant).
 */
inline void
__free__DataTypeSymbolPtr(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.ptr);
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Ref variant).
 */
inline void
__free__DataTypeSymbolRef(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.ref);
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Optional variant).
 */
inline void
__free__DataTypeSymbolOptional(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.optional);
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Exception variant).
 */
inline void
__free__DataTypeSymbolException(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.exception);
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Mut variant).
 */
inline void
__free__DataTypeSymbolMut(struct DataTypeSymbol *self)
{
    FREE(DataTypeSymbolAll, self->value.mut);
    free(self);
}

/**
 *
 * @brief Free the DataTypeSymbol type (Lambda variant).
 */
void
__free__DataTypeSymbolLambda(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Array variant).
 */
void
__free__DataTypeSymbolArray(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Custom variant).
 */
void
__free__DataTypeSymbolCustom(struct DataTypeSymbol *self);

/**
 *
 * @brief Free the DataTypeSymbol type (Tuple variant).
 */
inline void
__free__DataTypeSymbolTuple(struct DataTypeSymbol *self)
{
    for (Usize i = len__Vec(*self->value.tuple); i--;)
        FREE(DataTypeSymbolAll, get__Vec(*self->value.tuple, i));

    FREE(Vec, self->value.tuple);
    free(self);
}

inline void
__free__DataTypeSymbolCompilerDefined(struct DataTypeSymbol *self)
{
    free(self);
}

/**
 *
 * @brief Free the Generic type (RestrictedDataType with DataTypeSymbol).
 */
void
__free__GenericSymbolRestrictedDataType(struct Generic *self);

/**
 *
 * @brief Free the Generic type (all variants).
 */
void
__free__GenericSymbolAll(struct Generic *self);

typedef struct FunParamSymbol
{
    enum FunParamKind kind;
    struct Tuple *param_data_type; // struct Tuple<struct DataTypeSymbol*,
                                   // struct Location&>*
    struct Location loc;

    union
    {
        struct String *name; // struct String&
    };

    union
    {
        struct ExprSymbol *default_;
    };
} FunParamSymbol;

/**
 *
 * @brief Construct the FunParamSymbol type.
 */
struct FunParamSymbol *
__new__FunParamSymbol(struct FunParam *param);

/**
 *
 * @brief Free the FunParamSymbol type.
 */
void
__free__FunParamSymbol(struct FunParamSymbol *self);

typedef struct FunSymbol
{
    struct String *name;     // struct String&
    struct Vec *tagged_type; // struct Vec<struct Tuple<struct DataTypeSymbol*,
                             // struct Location&>*>*
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *params;         // struct Vec<struct FunParamSymbol*>*
    enum Visibility visibility;
    bool is_async;
    struct DataTypeSymbol *return_type;
    struct Vec *body;      // struct Vec<struct SymbolTable*>*
    struct Scope *scope;   // struct Scope&
    struct Decl *fun_decl; // struct Decl&
} FunSymbol;

/**
 *
 * @brief Construct the FunSymbol type.
 */
struct FunSymbol *
__new__FunSymbol(struct Decl *fun_decl);

/**
 *
 * @brief Free the FunSymbol type.
 */
void
__free__FunSymbol(struct FunSymbol *self);

typedef struct ConstantSymbol
{
    struct String *name;        // struct String&
    struct DataType *data_type; // struct DataType&
    struct ExprSymbol *expr_symbol;
    struct Scope *scope;        // struct Scope&
    struct Decl *constant_decl; // struct Decl&
    enum Visibility visibility;
} ConstantSymbol;

/**
 *
 * @brief Construct the ConstantSymbol type.
 */
struct ConstantSymbol *
__new__ConstantSymbol(struct Decl *constant_decl);

/**
 *
 * @brief Free the ConstantSymbol type.
 */
void
__free__ConstantSymbol(struct ConstantSymbol *self);

typedef struct ModuleSymbol
{
    struct String *name;      // struct String&
    struct Vec *body;         // struct Vec<SymbolTable*>*
    struct Scope *scope;      // struct Scope&
    struct Decl *module_decl; // struct Decl&
    enum Visibility visibility;
} ModuleSymbol;

/**
 *
 * @brief Construct the ModuleSymbol type.
 */
struct ModuleSymbol *
__new__ModuleSymbol(struct Decl *module_decl);

/**
 *
 * @brief Free the ModuleSymbol type.
 */
void
__free__ModuleSymbol(struct ModuleSymbol *self);

typedef struct AliasSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct DataTypeSymbol *data_type;
    struct Scope *scope;     // struct Scope&
    struct Decl *alias_decl; // struct Decl&
    enum Visibility visibility;
} AliasSymbol;

/**
 *
 * @brief Construct the AliasSymbol type.
 */
struct AliasSymbol *
__new__AliasSymbol(struct Decl *alias_decl);

/**
 *
 * @brief Free the AliasSymbol type.
 */
void
__free__AliasSymbol(struct AliasSymbol *self);

typedef struct FieldRecordSymbol
{
    struct String *name; // struct String&
    struct DataTypeSymbol *data_type;
    struct ExprSymbol *value;
    enum Visibility visibility;
    struct Location loc;
} FieldRecordSymbol;

/**
 *
 * @brief Construct the FieldRecordSymbol type.
 */
struct FieldRecordSymbol *
__new__FieldRecordSymbol(struct FieldRecord *field_record);

/**
 *
 * @brief Free the FieldRecordSymbol type.
 */
void
__free__FieldRecordSymbol(struct FieldRecordSymbol *self);

typedef struct RecordSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *fields;         // struct Vec<struct FieldRecordSymbol*>*
    struct Scope *scope;        // struct Scope&
    struct Decl *record_decl;   // struct Decl&
    enum Visibility visibility;
} RecordSymbol;

/**
 *
 * @brief Construct the RecordSymbol type.
 */
struct RecordSymbol *
__new__RecordSymbol(struct Decl *record_decl);

/**
 *
 * @brief Free the RecordSymbol type.
 */
void
__free__RecordSymbol(struct RecordSymbol *self);

typedef struct RecordObjSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *fields;         // struct Vec<struct FieldRecordSymbol*>*
    struct Vec *attached;       // struct Vec<struct SymbolTable*>*
    struct Scope *scope;        // struct Scope&
    struct Decl *record_decl;   // struct Decl&
    enum Visibility visibility;
} RecordObjSymbol;

/**
 *
 * @brief Construct the RecordObjSymbol type.
 */
struct RecordObjSymbol *
__new__RecordObjSymbol(struct Decl *record_decl);

/**
 *
 * @brief Free the RecordObjSymbol type.
 */
void
__free__RecordObjSymbol(struct RecordObjSymbol *self);

typedef struct VariantEnumSymbol
{
    struct String *name; // struct String&
    struct DataTypeSymbol *data_type;
    struct Location loc;
} VariantEnumSymbol;

/**
 *
 * @brief Construct the VariantEnumSymbol type.
 */
struct VariantEnumSymbol *
__new__VariantEnumSymbol(struct VariantEnum *variant_enum);

/**
 *
 * @brief Free the VariantEnumSymbol type.
 */
inline void
__free__VariantEnumSymbol(struct VariantEnumSymbol *self)
{
    FREE(DataTypeSymbolAll, self->data_type);
    free(self);
}

typedef struct EnumSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *variants;       // struct Vec<struct VariantEnumSymbol*>*
    struct DataTypeSymbol *type_value;
    struct Scope *scope;
    struct Decl *enum_decl; // struct Decl&
    enum Visibility visibility;
    bool is_error;
} EnumSymbol;

/**
 *
 * @brief Construct the EnumSymbol type.
 */
struct EnumSymbol *
__new__EnumSymbol(struct Decl *enum_decl);

/**
 *
 * @brief Free the EnumSymbol type.
 */
void
__free__EnumSymbol(struct EnumSymbol *self);

typedef struct EnumObjSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *variants;       // struct Vec<struct VariantEnumSymbol*>*
    struct Vec *attached;       // struct Vec<struct SymbolTable*>*
    struct DataTypeSymbol *type_value;
    struct Scope *scope;
    struct Decl *enum_decl; // struct Decl&
    enum Visibility visibility;
    bool is_error;
} EnumObjSymbol;

/**
 *
 * @brief Construct the EnumObjSymbol type.
 */
struct EnumObjSymbol *
__new__EnumObjSymbol(struct Decl *enum_decl);

/**
 *
 * @brief Free the EnumObjSymbol type.
 */
void
__free__EnumObjSymbol(struct EnumObjSymbol *self);

typedef struct ErrorSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // srtruct Vec<struct Generic*>&
    struct DataTypeSymbol *data_type;
    struct Scope *scope;
    struct Decl *error_decl; // struct Decl&
    enum Visibility visibility;
} ErrorSymbol;

/**
 *
 * @brief Construct the ErrorSymbol type.
 */
struct ErrorSymbol *
__new__ErrorSymbol(struct Decl *error_decl);

/**
 *
 * @brief Free the ErrorSymbol type.
 */
inline void
__free__ErrorSymbol(struct ErrorSymbol *self)
{
    FREE(DataTypeSymbolAll, self->data_type);
    FREE(Scope, self->scope);
    free(self);
}

typedef struct MethodSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *params;         // struct Vec<struct FunParamSymbol*>*
    struct DataTypeSymbol *return_type;
    struct Vec *body;                  // structg Vec<struct SymbolTable*>*
    struct Scope *scope;               // struct Scope&
    struct ClassBodyItem *method_decl; // struct ClassBodyItem&
    enum Visibility visibility;
    bool has_first_self_param;
    bool is_async;
} MethodSymbol;

/**
 *
 * @brief Construct the MethodSymbol type.
 */
struct MethodSymbol *
__new__MethodSymbol(struct ClassBodyItem *method_decl);

/**
 *
 * @brief Free the MethodSymbol type.
 */
void
__free__MethodSymbol(struct MethodSymbol *self);

typedef struct PropertySymbol
{
    struct String *name;
    struct DataTypeSymbol *data_type;
    struct Scope *scope;                 // struct Scope&
    struct ClassBodyItem *property_decl; // struct ClassBodyItem&
    enum Visibility visibility;
} PropertySymbol;

/**
 *
 * @brief Construct the PropertySymbol type.
 */
struct PropertySymbol *
__new__PropertySymbol(struct ClassBodyItem *property_decl);

/**
 *
 * @brief Free the PropertySymbol type.
 */
void
__free__PropertySymbol(struct PropertySymbol *self);

typedef struct ClassSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *inheritance;    // struct Vec<struct DataTypeSymbol*>*
    struct Vec *impl;           // struct Vec<struct DataTypeSymbol*>*
    struct Vec *body;           // struct Vec<struct SymbolTable*>*
    struct Scope *scope;        // struct Scope&
    struct Decl *class_decl;    // struct Decl&
    enum Visibility visibility;
} ClassSymbol;

/**
 *
 * @brief Construct the ClassSymbol type.
 */
struct ClassSymbol *
__new__ClassSymbol(struct Decl *class_decl);

/**
 *
 * @brief Free the ClassSymbol type.
 */
void
__free__ClassSymbol(struct ClassSymbol *self);

typedef struct PrototypeSymbol
{
    struct String *name;     // struct String&
    struct Vec *params_type; // struct Vec<struct DataTypeSymbol*>*
    struct DataTypeSymbol *return_type;
    struct Scope *scope;                  // struct Scope&
    struct TraitBodyItem *prototype_decl; // struct TraitBodyItem&
    bool is_async;
    bool has_first_self_param;
} PrototypeSymbol;

/**
 *
 * @brief Construct the PrototypeSymbol type.
 */
struct PrototypeSymbol *
__new__PrototypeSymbol(struct TraitBodyItem *prototype_decl);

/**
 *
 * @brief Free the PrototypeSymbol type.
 */
void
__free__PrototypeSymbol(struct PrototypeSymbol *self);

typedef struct TraitSymbol
{
    struct String *name;        // struct String&
    struct Vec *generic_params; // struct Vec<struct Generic*>&
    struct Vec *inh;            // struct Vec<struct DataTypeSymbol*>*
    struct Vec *body;           // struct Vec<struct SymbolTable*>*
    struct Scope *scope;        // struct Scope&
    struct Decl *trait_decl;    // struct Decl&
    enum Visibility visibility;
} TraitSymbol;

/**
 *
 * @brief Construct the TraitSymbol type.
 */
struct TraitSymbol *
__new__TraitSymbol(struct Decl *trait_decl);

/**
 *
 * @brief Free the TraitSymbol type.
 */
void
__free__TraitSymbol(struct TraitSymbol *self);

enum LiteralSymbolKind
{
    LiteralSymbolKindBool,
    LiteralSymbolKindChar,
    LiteralSymbolKindBitChar,
    LiteralSymbolKindInt8,
    LiteralSymbolKindInt16,
    LiteralSymbolKindInt32,
    LiteralSymbolKindInt64,
    LiteralSymbolKindInt128,
    LiteralSymbolKindUint8,
    LiteralSymbolKindUint16,
    LiteralSymbolKindUint32,
    LiteralSymbolKindUint64,
    LiteralSymbolKindUint128,
    LiteralSymbolKindFloat32,
    LiteralSymbolKindFloat64,
    LiteralSymbolKindStr,
    LiteralSymbolKindBitStr,
    LiteralSymbolKindUnit
};

typedef struct LiteralSymbol
{
    enum LiteralSymbolKind kind;
    struct DataTypeSymbol *data_type;

    union
    {
        bool bool_;
        char char_;
        UInt8 bit_char;
        Int8 int8;
        Int16 int16;
        Int32 int32;
        Int64 int64;
        Int128 int128;
        UInt8 uint8;
        UInt16 uint16;
        UInt32 uint32;
        UInt64 uint64;
        Int128 uint128;
        Float32 float32;
        Float64 float64;
        Str str;         // Str&
        UInt8 **bit_str; // Uint8*&
    } value;
} LiteralSymbol;

/**
 *
 * @brief Construct LiteralSymbol (Bool variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolBool(bool bool_)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindBool,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindBool),
                                  .value.bool_ = bool_ };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Char variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolChar(char char_)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindChar,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindChar),
                                  .value.char_ = char_ };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (BitChar variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolBitChar(UInt8 bit_char)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindBitChar,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU8),
                                  .value.bit_char = bit_char };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Int8 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolInt8(Int8 int8)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt8,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindI8),
                                  .value.int8 = int8 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Int16 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolInt16(Int16 int16)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt16,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindI16),
                                  .value.int16 = int16 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Int32 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolInt32(Int32 int32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt32,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindI32),
                                  .value.int32 = int32 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Int64 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolInt64(Int64 int64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt64,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindI64),
                                  .value.int64 = int64 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Int128 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolInt128(Int128 int128)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindInt128,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindI128),
                                  .value.int128 = int128 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Uint8 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUint8(UInt8 uint8)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint8,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU8),
                                  .value.uint8 = uint8 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Uint16 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUint16(UInt16 uint16)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint16,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU16),
                                  .value.uint16 = uint16 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Uint32 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUint32(UInt32 uint32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint32,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU32),
                                  .value.uint32 = uint32 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Uint64 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUint64(UInt64 uint64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint64,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU64),
                                  .value.uint64 = uint64 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Uint128 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUint128(Int128 uint128)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUint128,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindU128),
                                  .value.uint128 = uint128 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Float32 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolFloat32(Float32 float32)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindFloat32,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindF32),
                                  .value.float32 = float32 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Float64 variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolFloat64(Float64 float64)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindFloat64,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindF64),
                                  .value.float64 = float64 };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Str variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolStr(Str str)
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindStr,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindStr),
                                  .value.str = str };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (BitStr variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolBitStr(UInt8 **bit_str)
{
    struct LiteralSymbol self = {
        .kind = LiteralSymbolKindBitStr,
        .data_type =
          NEW(DataTypeSymbolArray, NEW(DataTypeSymbol, DataTypeKindU8), NULL),
        .value.bit_str = bit_str
    };

    return self;
}

/**
 *
 * @brief Construct LiteralSymbol (Unit variant).
 */
inline struct LiteralSymbol
__new__LiteralSymbolUnit()
{
    struct LiteralSymbol self = { .kind = LiteralSymbolKindUnit,
                                  .data_type =
                                    NEW(DataTypeSymbol, DataTypeKindUnit) };

    return self;
}

inline void
__free__LiteralSymbol(struct LiteralSymbol self)
{
    FREE(DataTypeSymbolAll, self.data_type);
}

typedef struct UnaryOpSymbol
{
    enum UnaryOpKind kind;
    struct DataTypeSymbol *data_type;
    struct ExprSymbol *right;
} UnaryOpSymbol;

/**
 *
 * @brief Construct the UnaryOpSymbol type.
 */
struct UnaryOpSymbol
__new__UnaryOpSymbol(struct Expr unary_op,
                     struct DataTypeSymbol *data_type,
                     struct ExprSymbol *right);

/**
 *
 * @brief Convert UnaryOpKind to Str.
 */
const Str
to_Str__UnaryOpSymbol(unsigned int kind);

/**
 *
 * @brief Free the UnaryOpSymbol type.
 */
void
__free__UnaryOpSymbol(struct UnaryOpSymbol self);

typedef struct BinaryOpSymbol
{
    enum BinaryOpKind kind;
    struct DataTypeSymbol *data_type;
    struct ExprSymbol *left;
    struct ExprSymbol *right;
} BinaryOpSymbol;

/**
 *
 * @brief Construct the BinaryOpSymbol type.
 */
struct BinaryOpSymbol
__new__BinaryOpSymbol(struct Expr binary_op,
                      struct DataTypeSymbol *data_type,
                      struct ExprSymbol *left,
                      struct ExprSymbol *right);

/**
 *
 * @brief Convert BinaryOpKind to Str.
 */
const Str
to_Str__BinaryOpSymbol(unsigned int kind);

/**
 *
 * @brief Free the BinaryOpSymbol type.
 */
void
__free__BinaryOpSymbol(struct BinaryOpSymbol self);

typedef struct FunCallSymbol
{
    bool is_builtin;
    struct Scope *id;
    struct DataType *data_type;
    struct Vec *params; // struct Vec<struct Tuple<struct SymbolFunParamCall*,
                        // struct Location&>*>*
} FunCallSymbol;

/**
 *
 * @brief Construct the FunCallSymbol type.
 */
struct FunCallSymbol
__new__FunCallSymbol(bool is_builtin,
                     struct Scope *id,
                     struct DataType *data_type,
                     struct Vec *params);

/**
 *
 * @brief Free the FunCallSymbol type.
 */
void
__free__FunCallSymbol(struct FunCallSymbol self);

typedef struct FieldCallSymbol
{
    struct String *name; // struct String&
    struct ExprSymbol *value;
} FieldCallSymbol;

/**
 *
 * @brief Construct the FieldCallSymbol type.
 */
inline struct FieldCallSymbol *
__new__FieldCallSymbol(struct String *name, struct ExprSymbol *value)
{
    struct FieldCallSymbol *self = malloc(sizeof(struct FieldCallSymbol));
    self->name = name;
    self->value = value;
    return self;
}

/**
 *
 * @brief Free the FieldCallSymbol type.
 */
void
__free__FieldCallSymbol(struct FieldCallSymbol *self);

typedef struct RecordCallSymbol
{
    struct Scope id;
    struct Vec *fields; // struct Vec<struct Tuple<struct FieldCallSymbol*,
                        // struct Location&>*>*
} RecordCallSymbol;

/**
 *
 * @brief Construct the RecordCallSymbol type.
 */
inline struct RecordCallSymbol
__new__RecordCallSymbol(struct Scope id, struct Vec *fields)
{
    struct RecordCallSymbol self = { .id = id, .fields = fields };

    return self;
}

/**
 *
 * @brief Free the RecordCallSymbol type.
 */
void
__free__RecordCallSymbol(struct RecordCallSymbol self);

typedef struct ArrayAccessSymbol
{
    struct Scope id;
    struct Vec *access; // struct Vec<struct ExprSymbol*>*
} ArrayAccessSymbol;

/**
 *
 * @brief Construct the ArrayAccessSymbol type.
 */
inline struct ArrayAccessSymbol
__new__ArrayAccessSymbol(struct Scope id, struct Vec *access)
{
    struct ArrayAccessSymbol self = { .id = id, .access = access };

    return self;
}

/**
 *
 * @brief Free the ArrayAccessSymbol type.
 */
void
__free__ArrayAccessSymbol(struct ArrayAccessSymbol self);

typedef struct TupleAccessSymbol
{
    struct Scope id;
    struct Vec *access; // struct Vec<struct ExprSymbol*>*
} TupleAccessSymbol;

/**
 *
 * @brief Construct the TupleAccessSymbol type.
 */
inline struct TupleAccessSymbol
__new__TupleAccessSymbol(struct Scope id, struct Vec *access)
{
    struct TupleAccessSymbol self = { .id = id, .access = access };

    return self;
}

/**
 *
 * @brief Free the TupleAccessSymbol type.
 */
void
__free__TupleAccessSymbol(struct TupleAccessSymbol self);

typedef struct LambdaSymbol
{
    struct Vec *params; // struct Vec<struct FunParam*>* TODO!!
    struct DataTypeSymbol *return_type;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
    Usize id;
} LambdaSymbol;

typedef struct VariantSymbol
{
    struct Scope id;
    struct ExprSymbol *value;
} VariantSymbol;

/**
 *
 * @brief Construct the VariantSymbol type.
 */
inline struct VariantSymbol
__new__VariantSymbol(struct Scope id, struct ExprSymbol *value)
{
    struct VariantSymbol self = { .id = id, .value = value };

    return self;
}

/**
 *
 * @brief Free the VariantSymbol type.
 */
void
__free__VariantSymbol(struct VariantSymbol self);

typedef struct ExprSymbol
{
    enum ExprKind kind;
    struct Location loc;

    union
    {
        struct UnaryOpSymbol unary_op;
        struct BinaryOpSymbol binary_op;
        struct FunCallSymbol fun_call;
        struct RecordCallSymbol record_call;
        struct Scope *identifier;
        struct Scope *identifier_access;
        struct ArrayAccessSymbol array_access;
        struct TupleAccessSymbol tuple_access;
        struct LambdaSymbol lambda;
        struct Tuple *array; // struct Tuple<struct Vec<struct ExprSymbol*>*,
                             // struct DataTypeSymbol*>*
        struct Tuple *tuple; // struct Tuple<struct Vec<struct ExprSymbol*>*,
                             // struct DataTypeSymbol*>*
        struct VariantSymbol variant;
        struct ExprSymbol *try;
        // struct IfCond
        struct Vec *block; // struct Vec<struct SymbolTable*>*
        struct Scope *question_mark;
        struct Scope *dereference;
        struct Scope *ref;
        struct LiteralSymbol literal;
        struct VariableSymbol *variable;
        struct Tuple *
          grouping; // struct Tuple<struct ExprSymbol*, struct DataTypeSymbol*>*
    } value;
} ExprSymbol;

/**
 *
 * @brief Construct the ExprSymbol type.
 */
struct ExprSymbol *
__new__ExprSymbol(struct Expr *expr);

/**
 *
 * @brief Construct the ExprSymbol type (UnaryOp variant).
 */
struct ExprSymbol *
__new__ExprSymbolUnaryOp(struct Expr expr, struct UnaryOpSymbol unary_op);

/**
 *
 * @brief Construct the ExprSymbol type (BinaryOp variant).
 */
struct ExprSymbol *
__new__ExprSymbolBinaryOp(struct Expr expr, struct BinaryOpSymbol binary_op);

/**
 *
 * @brief Construct the ExprSymbol type (FunCall variant).
 */
struct ExprSymbol *
__new__ExprSymbolFunCall(struct Expr expr, struct FunCallSymbol fun_call);

/**
 *
 * @brief Construct the ExprSymbol type (RecordCall variant).
 */
struct ExprSymbol *
__new__ExprSymbolRecordCall(struct Expr expr,
                            struct RecordCallSymbol record_call);

/**
 *
 * @brief Construct the ExprSymbol type (Identifier variant).
 */
struct ExprSymbol *
__new__ExprSymbolIdentifier(struct Expr expr, struct Scope *identifier);

/**
 *
 * @brief Construct the ExprSymbol type (IdentifierAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolIdentifierAccess(struct Expr expr,
                                  struct Scope *identifier_access);

/**
 *
 * @brief Construct the ExprSymbol type (ArrayAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolArrayAccess(struct Expr expr,
                             struct ArrayAccessSymbol array_access);

/**
 *
 * @brief Construct the ExprSymbol type (TupleAccess variant).
 */
struct ExprSymbol *
__new__ExprSymbolTupleAccess(struct Expr expr,
                             struct TupleAccessSymbol tuple_access);

/**
 *
 * @brief Construct the ExprSymbol type (Lambda variant).
 */
struct ExprSymbol *
__new__ExprSymbolLambda(struct Expr expr, struct LambdaSymbol lambda);

/**
 *
 * @brief Construct the ExprSymbol type (Tuple variant).
 */
struct ExprSymbol *
__new__ExprSymbolTuple(struct Expr expr, struct Tuple *tuple);

/**
 *
 * @brief Construct the ExprSymbol type (Array variant).
 */
struct ExprSymbol *
__new__ExprSymbolArray(struct Expr expr, struct Tuple *array);

/**
 *
 * @brief Construct the ExprSymbol type (Variant variant).
 */
struct ExprSymbol *
__new__ExprSymbolVariant(struct Expr expr, struct VariantSymbol variant);

/**
 *
 * @brief Construct the ExprSymbol type (Try variant).
 */
struct ExprSymbol *
__new__ExprSymbolTry(struct Expr expr, struct ExprSymbol *try);

/**
 *
 * @brief Construct the ExprSymbol type (Block variant).
 */
struct ExprSymbol *
__new__ExprSymbolBlock(struct Expr expr, struct Vec *block);

/**
 *
 * @brief Construct the ExprSymbol type (QuestionMark variant).
 */
struct ExprSymbol *
__new__ExprSymbolQuestionMark(struct Expr expr, struct Scope *question_mark);

/**
 *
 * @brief Construct the ExprSymbol type (Dereference variant).
 */
struct ExprSymbol *
__new__ExprSymbolDereference(struct Expr expr, struct Scope *dereference);

/**
 *
 * @brief Construct the ExprSymbol type (Ref variant).
 */
struct ExprSymbol *
__new__ExprSymbolRef(struct Expr expr, struct Scope *ref);

/**
 *
 * @brief Construct the ExprSymbol type (Literal variant).
 */
struct ExprSymbol *
__new__ExprSymbolLiteral(struct Expr expr, struct LiteralSymbol literal);

/**
 *
 * @brief Construct the ExprSymbol type (Variable variant).
 */
struct ExprSymbol *
__new__ExprSymbolVariable(struct Expr expr, struct VariableSymbol *variable);

/**
 *
 * @brief Construct the ExprSymbol type (Grouping variant).
 */
struct ExprSymbol *
__new__ExprSymbolGrouping(struct Expr expr, struct Tuple *grouping);

/**
 *
 * @brief Free the ExprSymbol type (all variants).
 */
void
__free__ExprSymbolAll(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type.
 */
inline void
__free__ExprSymbol(struct ExprSymbol *self)
{
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Unary variant).
 */
inline void
__free__ExprSymbolUnaryOp(struct ExprSymbol *self)
{
    FREE(UnaryOpSymbol, self->value.unary_op);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Binary variant).
 */
inline void
__free__ExprSymbolBinaryOp(struct ExprSymbol *self)
{
    FREE(BinaryOpSymbol, self->value.binary_op);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (FunCall variant).
 */
void
__free__ExprSymbolFunCall(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (RecordCall variant).
 */
inline void
__free__ExprSymbolRecordCall(struct ExprSymbol *self)
{
    FREE(RecordCallSymbol, self->value.record_call);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Identifier variant).
 */
inline void
__free__ExprSymbolIdentifier(struct ExprSymbol *self)
{
    FREE(Scope, self->value.identifier);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (IdentifierAccess variant).
 */
inline void
__free__ExprSymbolIdentifierAccess(struct ExprSymbol *self)
{
    FREE(Scope, self->value.identifier_access);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (ArrayAccess variant).
 */
inline void
__free__ExprSymbolArrayAccess(struct ExprSymbol *self)
{
    FREE(ArrayAccessSymbol, self->value.array_access);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (TupleAccess variant).
 */
inline void
__free__ExprSymbolTupleAccess(struct ExprSymbol *self)
{
    FREE(TupleAccessSymbol, self->value.tuple_access);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Lambda variant).
 */
void
__free__ExprSymbolLambda(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Tuple variant).
 */
void
__free__ExprSymbolTuple(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Array variant).
 */
void
__free__ExprSymbolArray(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Variant variant).
 */
inline void
__free__ExprSymbolVariant(struct ExprSymbol *self)
{
    FREE(VariantSymbol, self->value.variant);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Try variant).
 */
inline void
__free__ExprSymbolTry(struct ExprSymbol *self)
{
    FREE(ExprSymbolAll, self->value.try);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Block variant).
 */
void
__free__ExprSymbolBlock(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (QuestionMark variant).
 */
inline void
__free__ExprSymbolQuestionMark(struct ExprSymbol *self)
{
    FREE(Scope, self->value.question_mark);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Dereference variant).
 */
inline void
__free__ExprSymbolDereference(struct ExprSymbol *self)
{
    FREE(Scope, self->value.dereference);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Ref variant).
 */
inline void
__free__ExprSymbolRef(struct ExprSymbol *self)
{
    FREE(Scope, self->value.ref);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Literal variant).
 */
inline void
__free__ExprSymbolLiteral(struct ExprSymbol *self)
{
    FREE(LiteralSymbol, self->value.literal);
    free(self);
}

/**
 *
 * @brief Free the ExprSymbol type (Variable variant).
 */
void
__free__ExprSymbolVariable(struct ExprSymbol *self);

/**
 *
 * @brief Free the ExprSymbol type (Grouping variant).
 */
void
__free__ExprSymbolGrouping(struct ExprSymbol *self);

typedef struct VariableSymbol
{
    struct String *name; // struct String&
    struct DataTypeSymbol *data_type;
    struct ExprSymbol *expr;
    struct Scope *scope; // struct Scope&
    struct Location loc;
    bool is_mut;
} VariableSymbol;

/**
 *
 * @brief Construct the VariableDecl type.
 */
inline struct VariableSymbol *
__new__VariableSymbol(struct VariableDecl decl, struct Location decl_loc)
{
    struct VariableSymbol *self = malloc(sizeof(struct VariableSymbol));
    self->name = decl.name;
    self->loc = decl_loc;
    self->is_mut = decl.is_mut;
    return self;
}

/**
 *
 * @brief Free the VariableDecl type.
 */
inline void
__free__VariableSymbol(struct VariableSymbol *self)
{
    FREE(DataTypeSymbol, self->data_type);
    FREE(ExprSymbolAll, self->expr);
    free(self);
}

typedef struct MatchSymbol
{
    struct ExprSymbol *matching;
    struct Vec *pattern; // struct Vec<struct Tuple<struct ExprSymbol*, struct
                         // Vec<struct SymbolTable*>*>*
} MatchSymbol;

/**
 *
 * @brief Construct the MatchSymbol type.
 */
inline struct MatchSymbol
__new__MatchSymbol(struct ExprSymbol *matching, struct Vec *pattern)
{
    struct MatchSymbol self = { .matching = matching, .pattern = pattern };

    return self;
}

/**
 *
 * @brief Free the MatchSymbol type.
 */
void
__free__MatchSymbol(struct MatchSymbol self);

typedef struct IfBranchSymbol
{
    struct ExprSymbol *cond;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
} IfBranchSymbol;

/**
 *
 * @brief Construct the IfBranchSymbol type.
 */
struct IfBranchSymbol *
__new__IfBranchSymbol(struct ExprSymbol *cond, struct Vec *body);

/**
 *
 * @brief Free the IfBranchSymbol type.
 */
void
__free__IfBranchSymbol(struct IfBranchSymbol *self);

typedef struct IfCondSymbol
{
    struct IfBranchSymbol *if_;
    struct Vec *elif; // struct Vec<struct IfBranchSymbol*>*
    struct IfBranchSymbol *else_;
} IfCondSymbol;

/**
 *
 * @brief Construct the IfCondSymbol type.
 */
struct IfCondSymbol
__new__IfCondSymbol(struct IfBranchSymbol *if_,
                    struct Vec *elif,
                    struct IfBranchSymbol *else_);

/**
 *
 * @brief Free the IfCondSymbol type.
 */
void
__free__IfCondSymbol(struct IfCondSymbol self);

typedef struct TrySymbol
{
    struct ExprSymbol *try_expr;
    struct Vec *try_body; // struct Vec<struct SymbolTable*>*
    struct ExprSymbol *catch_expr;
    struct Vec *catch_body; // struct Vec<struct SymbolTable*>*
} TrySymbol;

/**
 *
 * @brief Construct the TrySymbol type.
 */
struct TrySymbol
__new__TrySymbol(struct ExprSymbol *try_expr,
                 struct Vec *try_body,
                 struct ExprSymbol *catch_expr,
                 struct Vec *catch_body);

/**
 *
 * @brief Free the TrySymbol type.
 */
void
__free__TrySymbol(struct TrySymbol self);

typedef struct WhileSymbol
{
    struct ExprSymbol *cond;
    struct Vec *body; // struct Vec<struct SymbolTable*>*
} WhileSymbol;

/**
 *
 * @brief Construct the WhileSymbol type.
 */
inline struct WhileSymbol
__new__WhileSymbol(struct ExprSymbol *cond, struct Vec *body)
{
    struct WhileSymbol self = { .cond = cond, .body = body };

    return self;
}

/**
 *
 * @brief Free the WhileSymbol type.
 */
void
__free__WhileSymbol(struct WhileSymbol self);

typedef struct StmtSymbol
{
    enum StmtKind kind;
    struct Location loc;

    union
    {
        struct IfCondSymbol if_;
        struct ExprSymbol *return_;
        struct ExprSymbol *await;
        struct TrySymbol try;
        struct MatchSymbol match;
        struct WhileSymbol while_;
    } value;
} StmtSymbol;

/**
 *
 * @brief Construct the StmtSymbol type.
 */
inline struct StmtSymbol
__new__StmtSymbol(struct Stmt stmt)
{
    struct StmtSymbol self = { .kind = stmt.kind, .loc = stmt.loc };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (If variant).
 */
inline struct StmtSymbol
__new__StmtSymbolIf(struct Stmt stmt, struct IfCondSymbol if_)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.if_ = if_ };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (Return variant).
 */
inline struct StmtSymbol
__new__StmtSymbolReturn(struct Stmt stmt, struct ExprSymbol *return_)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.return_ = return_ };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (Await variant).
 */
inline struct StmtSymbol
__new__StmtSymbolAwait(struct Stmt stmt, struct ExprSymbol *await)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.await = await };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (Try variant).
 */
inline struct StmtSymbol
__new__StmtSymbolTry(struct Stmt stmt, struct TrySymbol try)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.try = try };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (Match variant).
 */
inline struct StmtSymbol
__new__StmtSymbolMatch(struct Stmt stmt, struct MatchSymbol match)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.match = match };

    return self;
}

/**
 *
 * @brief Construct the StmtSymbol type (While variant).
 */
inline struct StmtSymbol
__new__StmtSymbolWhile(struct Stmt stmt, struct WhileSymbol while_)
{
    struct StmtSymbol self = { .kind = stmt.kind,
                               .loc = stmt.loc,
                               .value.while_ = while_ };

    return self;
}

/**
 *
 * @brief Free the StmtSymbol type (If variant).
 */
inline void
__free__StmtSymbolIf(struct StmtSymbol self)
{
    FREE(IfCondSymbol, self.value.if_);
}

/**
 *
 * @brief Free the StmtSymbol type (Return variant).
 */
inline void
__free__StmtSymbolReturn(struct StmtSymbol self)
{
    FREE(ExprSymbolAll, self.value.return_);
}

/**
 *
 * @brief Free the StmtSymbol type (Await variant).
 */
inline void
__free__StmtSymbolAwait(struct StmtSymbol self)
{
    FREE(ExprSymbolAll, self.value.await);
}

/**
 *
 * @brief Free the StmtSymbol type (Try variant).
 */
inline void
__free__StmtSymbolTry(struct StmtSymbol self)
{
    FREE(TrySymbol, self.value.try);
}

/**
 *
 * @brief Free the StmtSymbol type (Match variant).
 */
inline void
__free__StmtSymbolMatch(struct StmtSymbol self)
{
    FREE(MatchSymbol, self.value.match);
}

/**
 *
 * @brief Free the StmtSymbol type (While variant).
 */
inline void
__free__StmtSymbolWhile(struct StmtSymbol self)
{
    FREE(WhileSymbol, self.value.while_);
}

/**
 *
 * @brief Free the StmtSymbol type (all variants).
 */
void
__free__StmtSymbolAll(struct StmtSymbol self);

enum SymbolTableKind
{
    SymbolTableKindFun,
    SymbolTableKindConstant,
    SymbolTableKindModule,
    SymbolTableKindAlias,
    SymbolTableKindRecord,
    SymbolTableKindEnum,
    SymbolTableKindError,
    SymbolTableKindClass,
    SymbolTableKindTrait,
    SymbolTableKindRecordObj,
    SymbolTableKindEnumObj,
    SymbolTableKindExpr,
    SymbolTableKindStmt
};

typedef struct SymbolTable
{
    enum SymbolTableKind kind;

    union
    {
        struct FunSymbol *fun;
        struct ConstantSymbol *constant;
        struct ModuleSymbol *module;
        struct AliasSymbol *alias;
        struct RecordSymbol *record;
        struct RecordObjSymbol *record_obj;
        struct EnumSymbol *enum_;
        struct EnumObjSymbol *enum_obj;
        struct ErrorSymbol *error;
        struct ClassSymbol *class;
        struct TraitSymbol *trait;
        struct ExprSymbol *expr;
        struct StmtSymbol stmt;
    } value;
} SymbolTable;

/**
 *
 * @brief Construct the SymbolTable type (Fun variant).
 */
struct SymbolTable *
__new__SymbolTableFun(struct FunSymbol *fun);

/**
 *
 * @brief Construct the SymbolTable type (Constant variant).
 */
struct SymbolTable *
__new__SymbolTableConstant(struct ConstantSymbol *constant);

/**
 *
 * @brief Construct the SymbolTable type (Module variant).
 */
struct SymbolTable *
__new__SymbolTableModule(struct ModuleSymbol *module);

/**
 *
 * @brief Construct the SymbolTable type (Alias variant).
 */
struct SymbolTable *
__new__SymbolTableAlias(struct AliasSymbol *alias);

/**
 *
 * @brief Construct the SymbolTable type (Record variant).
 */
struct SymbolTable *
__new__SymbolTableRecord(struct RecordSymbol *record);

/**
 *
 * @brief Construct the SymbolTable type (RecordObj variant).
 */
struct SymbolTable *
__new__SymbolTableRecordObj(struct RecordObjSymbol *record_obj);

/**
 *
 * @brief Construct the SymbolTable type (Enum variant).
 */
struct SymbolTable *
__new__SymbolTableEnum(struct EnumSymbol *enum_);

/**
 *
 * @brief Construct the SymbolTable type (EnumObj variant).
 */
struct SymbolTable *
__new__SymbolTableEnumObj(struct EnumObjSymbol *enum_obj);

/**
 *
 * @brief Construct the SymbolTable type (Error variant).
 */
struct SymbolTable *
__new__SymbolTableError(struct ErrorSymbol *error);

/**
 *
 * @brief Construct the SymbolTable type (Class variant).
 */
struct SymbolTable *
__new__SymbolTableClass(struct ClassSymbol *class);

/**
 *
 * @brief Construct the SymbolTable type (Trait variant).
 */
struct SymbolTable *
__new__SymbolTableTrait(struct TraitSymbol *trait);

/**
 *
 * @brief Construct the SymbolTable type (Expr variant).
 */
struct SymbolTable *
__new__SymbolTableExpr(struct ExprSymbol *expr);

/**
 *
 * @brief Construct the SymbolTable type (Stmt variant).
 */
struct SymbolTable *
__new__SymbolTableStmt(struct StmtSymbol stmt);

/**
 *
 * @brief Free the SymbolTable type (Fun variant).
 */
inline void
__free__SymbolTableFun(struct SymbolTable *self)
{
    FREE(FunSymbol, self->value.fun);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Constant variant).
 */
inline void
__free__SymbolTableConstant(struct SymbolTable *self)
{
    FREE(ConstantSymbol, self->value.constant);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Module variant).
 */
inline void
__free__SymbolTableModule(struct SymbolTable *self)
{
    FREE(ModuleSymbol, self->value.module);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Alias variant).
 */
inline void
__free__SymbolTableAlias(struct SymbolTable *self)
{
    FREE(AliasSymbol, self->value.alias);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Record variant).
 */
inline void
__free__SymbolTableRecord(struct SymbolTable *self)
{
    FREE(RecordSymbol, self->value.record);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (RecordObj variant).
 */
inline void
__free__SymbolTableRecordObj(struct SymbolTable *self)
{
    FREE(RecordObjSymbol, self->value.record_obj);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Enum variant).
 */
inline void
__free__SymbolTableEnum(struct SymbolTable *self)
{
    FREE(EnumSymbol, self->value.enum_);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (EnumObj variant).
 */
inline void
__free__SymbolTableEnumObj(struct SymbolTable *self)
{
    FREE(EnumObjSymbol, self->value.enum_obj);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Error variant).
 */
inline void
__free__SymbolTableError(struct SymbolTable *self)
{
    FREE(ErrorSymbol, self->value.error);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Class variant).
 */
inline void
__free__SymbolTableClass(struct SymbolTable *self)
{
    FREE(ClassSymbol, self->value.class);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Trait variant).
 */
inline void
__free__SymbolTableTrait(struct SymbolTable *self)
{
    FREE(TraitSymbol, self->value.trait);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Expr variant).
 */
inline void
__free__SymbolTableExpr(struct SymbolTable *self)
{
    FREE(ExprSymbolAll, self->value.expr);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (Stmt variant).
 */
inline void
__free__SymbolTableStmt(struct SymbolTable *self)
{
    FREE(StmtSymbolAll, self->value.stmt);
    free(self);
}

/**
 *
 * @brief Free the SymbolTable type (all variants).
 */
void
__free__SymbolTableAll(struct SymbolTable *self);

#endif // LILY_SYMBOL_TABLE_H
