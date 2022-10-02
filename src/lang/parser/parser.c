#include <assert.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/platform.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/parser/parser.h>
#include <string.h>

#define EXPECTED_TOKEN_ERR(parse_block, expected) \
    NEW(DiagnosticWithErrParser,                  \
        parse_block,                              \
        NEW(LilyError, LilyErrorExpectedToken),   \
        *parse_block->current->loc,               \
        format(""),                               \
        Some(format("expected {s}, found `{Sr}`", \
                    expected,                     \
                    token_kind_to_string__Token(*parse_block->current))));

#define CLOSE_BLOCK_NOTE()                                   \
    NEW(DiagnosticWithNoteParser,                            \
        parse_block,                                         \
        format("you may have forgotten to close the block"), \
        *parse_block->current->loc,                          \
        format(""),                                          \
        None());

#define PARSE_GENERIC_TYPE_AND_OBJECT(self)                              \
    if (((struct Token *)get__Vec(*self->scanner.tokens, self->pos + 1)) \
            ->kind == TokenKindLHook ||                                  \
        self->current->kind == TokenKindIdentifier)                      \
        next_token_pb(self);                                             \
                                                                         \
    if (self->current->kind == TokenKindLHook) {                         \
        struct Vec *content = get_generic_params(self);                  \
                                                                         \
        concat__Vec(generic_params, content);                            \
                                                                         \
        FREE(Vec, content);                                              \
    }                                                                    \
                                                                         \
    if (len__Vec(*generic_params) != 0)                                  \
        has_generic_params = true;                                       \
                                                                         \
    if (self->current->kind != TokenKindColon && !is_object) {           \
                                                                         \
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(self, "`:`");        \
                                                                         \
        err->err->s = from__String("`:`");                               \
                                                                         \
        emit__Diagnostic(err);                                           \
    } else if (self->current->kind == TokenKindColon && !is_object)      \
        next_token_pb(self);

#define EXPECTED_TOKEN(self, token_kind, err) \
    if (self->current->kind != token_kind) {  \
        err;                                  \
    } else                                    \
        next_token_pb(self);

#define VERIFY_EOF(parse_block, bad_token, expected)                   \
    if (parse_block->current->kind == TokenKindEof && !bad_token) {    \
        struct Diagnostic *err =                                       \
          NEW(DiagnosticWithErrParser,                                 \
              parse_block,                                             \
              NEW(LilyError, LilyErrorMissClosingBlock),               \
              *((struct Token *)get__Vec(*parse_block->scanner.tokens, \
                                         parse_block->pos - 1))        \
                 ->loc,                                                \
              format("expected closing block here"),                   \
              None());                                                 \
                                                                       \
        err->err->s = format("{s}", expected);                         \
                                                                       \
        emit__Diagnostic(err);                                         \
    } else if (parse_block->current->kind == TokenKindEndKw)           \
        next_token_pb(parse_block);

#define PARSE_GENERIC_PARAMS(self)                                             \
    if (parse_block->current->kind == TokenKindLHook) {                        \
        next_token_pb(parse_block);                                            \
                                                                               \
        while (parse_block->current->kind != TokenKindRHook) {                 \
            push__Vec(self->generic_params, &*parse_block->current);           \
            next_token_pb(parse_block);                                        \
        }                                                                      \
                                                                               \
        if (len__Vec(*self->generic_params) == 0) {                            \
            struct Diagnostic *warn = NEW(                                     \
              DiagnosticWithWarnParser,                                        \
              parse_block,                                                     \
              NEW(LilyWarning, LilyWarningIgnoredGenericParams),               \
              *parse_block->current->loc,                                      \
              format("the generic params are ignored because they are empty"), \
              None());                                                         \
                                                                               \
            emit_warning__Diagnostic(warn, parse_block->disable_warning);      \
        }                                                                      \
                                                                               \
        if (len__Vec(*self->generic_params) != 0)                              \
            self->has_generic_params = true;                                   \
                                                                               \
        next_token_pb(parse_block);                                            \
    }

#define PARSE_PARAMS(self)                                      \
    if (parse_block->current->kind == TokenKindLParen) {        \
        next_token_pb(parse_block);                             \
                                                                \
        while (parse_block->current->kind != TokenKindRParen) { \
            push__Vec(self->params, &*parse_block->current);    \
            next_token_pb(parse_block);                         \
        }                                                       \
                                                                \
        if (len__Vec(*self->params) != 0)                       \
            self->has_params = true;                            \
                                                                \
        next_token_pb(parse_block);                             \
    }

#define VERIFY_CLOSING_BODY(parse_block)                                      \
    VERIFY_EOF(parse_block, bad_item, "`;` or `end`");                        \
                                                                              \
    if (parse_block->current->kind == TokenKindSemicolon &&                   \
        start_line != end_line) {                                             \
        struct Diagnostic *err =                                              \
          NEW(DiagnosticWithErrParser,                                        \
              parse_block,                                                    \
              NEW(LilyError, LilyErrorMisuseOfSpecialClosingBlock),           \
              *parse_block->current->loc,                                     \
              format("the `;` is used when a function is write on one line"), \
              Some(format("replace `;` closing, by `end` closing")));         \
                                                                              \
        emit__Diagnostic(err);                                                \
    }                                                                         \
                                                                              \
    if (parse_block->current->kind == TokenKindSemicolon)                     \
        next_token_pb(parse_block);

#define PARSE_PAREN(parse_decl, block)                     \
    next_token(parse_decl);                                \
    while (parse_decl->current->kind != TokenKindRParen) { \
        block;                                             \
    }                                                      \
    next_token(parse_decl);

#define PARSE_HOOK(parse_decl, block)                     \
    next_token(parse_decl);                               \
    while (parse_decl->current->kind != TokenKindRHook) { \
        block;                                            \
    }                                                     \
    next_token(parse_decl);

#define PARSE_BRACE(parse_decl, block)                     \
    next_token(parse_decl);                                \
    while (parse_decl->current->kind != TokenKindRBrace) { \
        block;                                             \
    }                                                      \
    next_token(parse_decl);

static void
get_block(struct ParseBlock *self);
static inline void
next_token_pb(struct ParseBlock *self);
static inline void
skip_to_next_block(struct ParseBlock *self);
static struct String *
get_type_name(struct ParseBlock *self);
static void
get_type_context(struct ParseBlock *self, bool is_pub);
static struct String *
get_object_name(struct ParseBlock *self);
static void
get_object_context(struct ParseBlock *self, bool is_pub);
static struct Vec *
get_generic_params(struct ParseBlock *self);
static inline bool
valid_body_item(struct ParseBlock *parse_block,
                bool already_invalid,
                bool is_fun);
static void
verify_stmt(void *self, struct ParseBlock *parse_block, bool is_fun);
static void
get_body_parse_context(void *self, struct ParseBlock *parse_block, bool is_fun);
static void
get_fun_parse_context(struct FunParseContext *self,
                      struct ParseBlock *parse_block);
static inline bool
valid_token_in_enum_variants(struct ParseBlock *parse_block,
                             bool already_invalid);
static void
get_enum_parse_context(struct EnumParseContext *self,
                       struct ParseBlock *parse_block);
static inline bool
valid_token_in_record_fields(struct ParseBlock *parse_block,
                             bool already_invalid);
static void
get_record_parse_context(struct RecordParseContext *self,
                         struct ParseBlock *parse_block);
static inline bool
valid_token_in_alias_data_type(struct ParseBlock *parse_block,
                               bool already_invalid);
static void
get_alias_parse_context(struct AliasParseContext *self,
                        struct ParseBlock *parse_block);
static inline bool
valid_token_in_trait_body(struct ParseBlock *parse_block, bool already_invalid);
static void
get_trait_parse_context(struct TraitParseContext *self,
                        struct ParseBlock *parse_block);
static inline bool
valid_class_token_in_body(struct ParseBlock *parse_block, bool already_invalid);
static void
get_class_parse_context(struct ClassParseContext *self,
                        struct ParseBlock *parse_block);
static inline bool
valid_tag_token_in_body(struct ParseBlock *parse_block, bool already_invalid);
static void
get_tag_parse_context(struct TagParseContext *self,
                      struct ParseBlock *parse_block);
static void
get_method_parse_context(struct MethodParseContext *self,
                         struct ParseBlock *parse_block);
static void
get_property_parse_context(struct PropertyParseContext *self,
                           struct ParseBlock *parse_block);
static void
get_import_parse_context(struct ImportParseContext *self,
                         struct ParseBlock *parse_block);
static inline struct Diagnostic *
__new__DiagnosticWithErrParser(struct ParseBlock *self,
                               struct LilyError *err,
                               struct Location loc,
                               struct String *detail_msg,
                               struct Option *help);
static inline struct Diagnostic *
__new__DiagnosticWithWarnParser(struct ParseBlock *self,
                                struct LilyWarning *warn,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help);
static inline struct Diagnostic *
__new__DiagnosticWithNoteParser(struct ParseBlock *self,
                                struct String *note,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help);
static inline void
next_token(struct ParseDecl *self);
static inline bool
is_data_type(struct ParseDecl *self);
static struct DataType *
parse_data_type(struct Parser self, struct ParseDecl *parse_decl);
static struct Vec *
parse_generic_params(struct Parser self, struct ParseDecl *parse_decl);
static void
parse_fun_declaration(struct Parser *self);
static void
parse_declaration(struct Parser *self);

#include <base/print.h>
struct ParseBlock
__new__ParseBlock(struct Scanner scanner)
{
    struct ParseBlock self = { .scanner = scanner,
                               .blocks =
                                 NEW(Vec, sizeof(struct ParseContext *)),
                               .current =
                                 &*(struct Token *)get__Vec(*scanner.tokens, 0),
                               .disable_warning = NEW(Vec, sizeof(Str)),
                               .pos = 0,
                               .count_error = 0,
                               .count_warning = 0 };

    return self;
}

static void
get_block(struct ParseBlock *self)
{
    switch (self->current->kind) {
        case TokenKindPubKw:
            next_token_pb(self);

            switch (self->current->kind) {
                case TokenKindFunKw: {
                    struct FunParseContext fun_parse_context =
                      NEW(FunParseContext);
                    fun_parse_context.is_pub = true;

                    next_token_pb(self);
                    get_fun_parse_context(&fun_parse_context, self);

                    push__Vec(self->blocks,
                              NEW(ParseContextFun, fun_parse_context));

                    break;
                }
                case TokenKindAsyncKw: {
                    next_token_pb(self);

                    if (self->current->kind != TokenKindFunKw) {
                        struct Diagnostic *err =
                          NEW(DiagnosticWithErrParser,
                              self,
                              NEW(LilyError, LilyErrorBadUsageOfAsync),
                              *self->current->loc,
                              format("expected `fun` after async declaration"),
                              None());

                        emit__Diagnostic(err);
                    } else
                        next_token_pb(self);

                    struct FunParseContext fun_parse_context =
                      NEW(FunParseContext);

                    fun_parse_context.is_pub = true;
                    fun_parse_context.is_async = true;

                    get_fun_parse_context(&fun_parse_context, self);

                    push__Vec(self->blocks,
                              NEW(ParseContextFun, fun_parse_context));

                    break;
                }
                case TokenKindTypeKw:
                    get_type_context(self, true);
                    break;
                case TokenKindObjectKw:
                    get_object_context(self, true);
                    break;
                case TokenKindTagKw:
                    TODO("");
                    break;
                case TokenKindErrorKw:
                    TODO("");
                    break;
                default: {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          self,
                          NEW(LilyError, LilyErrorBadUsageOfPub),
                          *self->current->loc,
                          format("expected `fun`, `async`, `type`, tag` or "
                                 "`object` after `pub` declaration"),
                          None());

                    emit__Diagnostic(err);
                    skip_to_next_block(self);

                    break;
                }
            }

            break;
        case TokenKindFunKw: {
            struct FunParseContext fun_parse_context = NEW(FunParseContext);

            next_token_pb(self);
            get_fun_parse_context(&fun_parse_context, self);

            push__Vec(self->blocks, NEW(ParseContextFun, fun_parse_context));

            break;
        }
        case TokenKindTagKw:
            break;
        case TokenKindTypeKw:
            get_type_context(self, false);
            break;
        case TokenKindObjectKw:
            get_object_context(self, false);
            break;
        case TokenKindBang: {
            next_token_pb(self);

            if (self->current->kind != TokenKindLHook) {
                EXPECTED_TOKEN(self, TokenKindLHook, {
                    struct Diagnostic *err = EXPECTED_TOKEN_ERR(self, "`[`");

                    err->err->s = from__String("`[`");

                    emit__Diagnostic(err);
                });
            }

            next_token_pb(self);

            if (self->current->kind != TokenKindIdentifier) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      self,
                      NEW(LilyError, LilyErrorExpectedAttribute),
                      *self->current->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
            }

            struct String *attribute = self->current->lit;

            next_token_pb(self);

            if (eq__String(attribute, from__String("warning"), true)) {
                if (self->current->kind == TokenKindStringLit) {
                    push__Vec(self->disable_warning, &*self->current->lit);
                    next_token_pb(self);
                } else {
                    EXPECTED_TOKEN(self, TokenKindStringLit, {
                        struct Diagnostic *err =
                          EXPECTED_TOKEN_ERR(self, "string literal");

                        err->err->s = from__String("string literal");

                        emit__Diagnostic(err);
                    });
                }
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      self,
                      NEW(LilyError, LilyErrorUnknownAttribute),
                      *((struct Token *)get__Vec(*self->scanner.tokens,
                                                 self->pos - 1))
                         ->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
                skip_to_next_block(self);

                goto exit;
            }

            next_token_pb(self);

        exit : {
        } break;
        }
        default: {
            struct Diagnostic *err = NEW(
              DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorUnexpectedTokenForBeginingInGlobal),
              *self->current->loc,
              from__String("expected `pub`, `fun`, `type`, `object`, `tag`, "
                           "`import`, `!`"),
              None());

            err->err->s = token_kind_to_string__Token(*self->current);

            emit__Diagnostic(err);
            skip_to_next_block(self);

            break;
        }
    }
}

void
run__ParseBlock(struct ParseBlock *self)
{
    // List of global block:
    // fun .. end or ;, := .. next block or EOF, module .. end, type .. end,
    // object
    // .. end, error .. to next block or Eof, tag .. end, import .. to next
    // block

    while (self->current->kind != TokenKindEof) {
        get_block(self);
    }
}

static inline void
next_token_pb(struct ParseBlock *self)
{
    if (self->current->kind != TokenKindEof) {
        self->pos += 1;
        self->current =
          &*(struct Token *)get__Vec(*self->scanner.tokens, self->pos);
    }
}

static inline void
skip_to_next_block(struct ParseBlock *self)
{
    while (self->current->kind != TokenKindFunKw &&
           self->current->kind != TokenKindTypeKw &&
           self->current->kind != TokenKindObjectKw &&
           self->current->kind != TokenKindTagKw &&
           self->current->kind != TokenKindErrorKw &&
           self->current->kind != TokenKindMacroKw &&
           self->current->kind != TokenKindPubKw &&
           self->current->kind != TokenKindAsyncKw &&
           self->current->kind != TokenKindEof) {
        next_token_pb(self);
    }

    if (self->current->kind == TokenKindEof)
        return;

    if (self->current->kind == TokenKindEndKw)
        next_token_pb(self);
}

static struct String *
get_type_name(struct ParseBlock *self)
{
    next_token_pb(self);

    if (self->current->kind != TokenKindIdentifier) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorMissTypeName),
              *self->current->loc,
              format(""),
              Some(format("add type's name, found `{Sr}`",
                          token_kind_to_string__Token(*self->current))));

        emit__Diagnostic(err);

        return NULL;
    }

    return &*self->current->lit;
}

static void
get_type_context(struct ParseBlock *self, bool is_pub)
{
    struct String *name = get_type_name(self);
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));
    bool has_generic_params = false;
    bool is_object = false;

    PARSE_GENERIC_TYPE_AND_OBJECT(self);

    switch (self->current->kind) {
        case TokenKindEnumKw: {
            struct EnumParseContext enum_parse_context = NEW(EnumParseContext);

            enum_parse_context.name = name;
            enum_parse_context.generic_params = generic_params;
            enum_parse_context.is_pub = is_pub;
            enum_parse_context.has_generic_params = has_generic_params;

            get_enum_parse_context(&enum_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextEnum, enum_parse_context, false));

            break;
        }
        case TokenKindRecordKw:
            struct RecordParseContext record_parse_context =
              NEW(RecordParseContext);

            record_parse_context.name = name;
            record_parse_context.generic_params = generic_params;
            record_parse_context.is_pub = is_pub;
            record_parse_context.has_generic_params = has_generic_params;

            get_record_parse_context(&record_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextRecord, record_parse_context, false));

            break;
        case TokenKindAliasKw: {
            struct AliasParseContext alias_parse_context =
              NEW(AliasParseContext);

            alias_parse_context.name = name;
            alias_parse_context.generic_params = generic_params;
            alias_parse_context.is_pub = is_pub;
            alias_parse_context.has_generic_params = has_generic_params;

            get_alias_parse_context(&alias_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextAlias, alias_parse_context));

            break;
        }
        default: {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  self,
                  NEW(LilyError, LilyErrorBadUsageOfType),
                  *self->current->loc,
                  format(""),
                  Some(format("expected `enum` or `record`, found `{Sr}`",
                              token_kind_to_string__Token(*self->current))));

            emit__Diagnostic(err);
            emit__Summary(self->count_error,
                          self->count_warning,
                          "the parser has been failed");

            skip_to_next_block(self);

            break;
        }
    }
}

static struct String *
get_object_name(struct ParseBlock *self)
{
    next_token_pb(self);

    if (self->current->kind != TokenKindIdentifier) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorMissObjectName),
              *self->current->loc,
              format(""),
              Some(format("add object's name, found `{Sr}`",
                          token_kind_to_string__Token(*self->current))));

        emit__Diagnostic(err);

        return NULL;
    }

    return &*self->current->lit;
}

static void
get_object_context(struct ParseBlock *self, bool is_pub)
{
    struct String *name = get_object_name(self);
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));
    struct Vec *impl = NEW(Vec, sizeof(struct Token));
    struct Vec *inh = NEW(Vec, sizeof(struct Token));
    bool has_generic_params = false;
    bool is_object = true;

    PARSE_GENERIC_TYPE_AND_OBJECT(self);

    struct Location loc_impl = {};

    start__Location(
      &loc_impl, self->current->loc->s_line, self->current->loc->s_col);

    if (self->current->kind == TokenKindImplKw) {
        next_token_pb(self);

        while (self->current->kind != TokenKindFatArrow &&
               self->current->kind != TokenKindColon &&
               self->current->kind != TokenKindEof) {
            push__Vec(impl, &*self->current);
            next_token_pb(self);
        }
    }

    end__Location(
      &loc_impl,
      ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
        ->loc->s_line,
      ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
        ->loc->s_col);

    struct Location loc_inh = {};

    start__Location(
      &loc_inh, self->current->loc->s_line, self->current->loc->s_col);

    if (self->current->kind == TokenKindFatArrow) {
        next_token_pb(self);

        EXPECTED_TOKEN(self, TokenKindLHook, {
            struct Diagnostic *err = EXPECTED_TOKEN_ERR(self, "`[`");

            err->err->s = from__String("`[`");

            emit__Diagnostic(err);
        });

        while (self->current->kind != TokenKindRHook &&
               self->current->kind != TokenKindEof) {
            push__Vec(inh, &*self->current);
            next_token_pb(self);
        }

        end__Location(
          &loc_inh, self->current->loc->s_line, self->current->loc->s_col);

        next_token_pb(self);
    }

    EXPECTED_TOKEN(self, TokenKindColon, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(self, "`:`");

        err->err->s = from__String("`:`");

        emit__Diagnostic(err);
    });

    if ((self->current->kind == TokenKindEnumKw ||
         self->current->kind == TokenKindRecordKw ||
         self->current->kind == TokenKindTraitKw) &&
        len__Vec(*impl) > 0) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorUnexpectedImplementation),
              loc_impl,
              format("unexpected implementation in record object, enum object "
                     "or trait declaration"),
              None());

        emit__Diagnostic(err);

        FREE(Vec, impl);
    } else if (self->current->kind == TokenKindEnumKw ||
               self->current->kind == TokenKindRecordKw ||
               self->current->kind == TokenKindTraitKw)
        FREE(Vec, impl);

    if ((self->current->kind == TokenKindEnumKw ||
         self->current->kind == TokenKindRecordKw) &&
        len__Vec(*inh) > 0) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorUnexpectedInheritance),
              loc_inh,
              format("unexpected inheritance in record object or enum object "
                     "declaration"),
              None());

        emit__Diagnostic(err);

        FREE(Vec, inh);
    } else if (self->current->kind == TokenKindEnumKw ||
               self->current->kind == TokenKindRecordKw)
        FREE(Vec, inh);

    switch (self->current->kind) {
        case TokenKindEnumKw: {
            struct EnumParseContext enum_parse_context = NEW(EnumParseContext);

            enum_parse_context.name = name;
            enum_parse_context.generic_params = generic_params;
            enum_parse_context.is_pub = is_pub;
            enum_parse_context.has_generic_params = has_generic_params;

            get_enum_parse_context(&enum_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextEnum, enum_parse_context, true));

            break;
        }
        case TokenKindRecordKw: {
            struct RecordParseContext record_parse_context =
              NEW(RecordParseContext);

            record_parse_context.name = name;
            record_parse_context.generic_params = generic_params;
            record_parse_context.is_pub = is_pub;
            record_parse_context.has_generic_params = has_generic_params;

            get_record_parse_context(&record_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextRecord, record_parse_context, true));

            break;
        }
        case TokenKindTraitKw: {
            struct TraitParseContext trait_parse_context =
              NEW(TraitParseContext);

            trait_parse_context.name = name;
            trait_parse_context.generic_params = generic_params;
            trait_parse_context.is_pub = is_pub;
            trait_parse_context.has_generic_params = has_generic_params;
            trait_parse_context.inheritance = inh;

            get_trait_parse_context(&trait_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextTrait, trait_parse_context));

            break;
        }
        case TokenKindClassKw: {
            struct ClassParseContext class_parse_context =
              NEW(ClassParseContext);

            class_parse_context.name = name;
            class_parse_context.generic_params = generic_params;
            class_parse_context.is_pub = is_pub;
            class_parse_context.has_generic_params = has_generic_params;
            class_parse_context.inheritance = inh;
            class_parse_context.impl = impl;

            if (len__Vec(*impl) > 0)
                class_parse_context.has_impl = true;

            if (len__Vec(*inh) > 0)
                class_parse_context.has_inheritance = true;

            get_class_parse_context(&class_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextClass, class_parse_context));

            break;
        }
        default:
            assert(0 && "error");
    }
}

static struct Vec *
get_generic_params(struct ParseBlock *self)
{
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));

    if (self->current->kind == TokenKindLHook) {
        next_token_pb(self);

        while (self->current->kind != TokenKindRHook) {
            push__Vec(generic_params, &*self->current);
            next_token_pb(self);
        }

        next_token_pb(self);
    }

    return generic_params;
}

void
__free__ParseBlock(struct ParseBlock self)
{
    for (Usize i = 0; i < len__Vec(*self.blocks); i++)
        FREE(ParseContextAll, get__Vec(*self.blocks, i));

    FREE(Vec, self.blocks);
    FREE(Vec, self.disable_warning);
    FREE(Scanner, self.scanner);
}

struct FunParseContext
__new__FunParseContext()
{
    struct FunParseContext self = { .is_pub = false,
                                    .is_async = false,
                                    .has_generic_params = false,
                                    .has_tag = false,
                                    .has_tags = false,
                                    .has_params = false,
                                    .name = NULL,
                                    .tags = NEW(Vec, sizeof(struct Token)),
                                    .generic_params =
                                      NEW(Vec, sizeof(struct Token)),
                                    .params = NEW(Vec, sizeof(struct Token)),
                                    .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_body_item(struct ParseBlock *parse_block,
                bool already_invalid,
                bool is_fun)
{
    if (!is_fun && parse_block->current->kind == TokenKindSelfKw)
        return true;

    switch (parse_block->current->kind) {
        case TokenKindEof:
        case TokenKindPubKw:
        case TokenKindTestKw:
        case TokenKindModuleKw:
        case TokenKindTypeKw:
        case TokenKindAliasKw:
        case TokenKindRecordKw:
        case TokenKindTagKw:
        case TokenKindEnumKw:
        case TokenKindErrorKw:
        case TokenKindClassKw:
        case TokenKindTraitKw:
        case TokenKindAsyncKw:
        case TokenKindObjectKw:
        case TokenKindSelfKw:
        case TokenKindAsmKw:
        case TokenKindMacroKw:
        case TokenKindImplKw: {
            if (!already_invalid) {
                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidItemInFunOrMethodBody),
                  *parse_block->current->loc,
                  format("this token is invalid inside the function or method"),
                  None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE();

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
        default:
            return true;
    }
}

#define PUSH_BODY()                                          \
    if (is_fun)                                              \
        push__Vec(((struct FunParseContext *)self)->body,    \
                  &*parse_block->current);                   \
    else                                                     \
        push__Vec(((struct MethodParseContext *)self)->body, \
                  &*parse_block->current);

static void
verify_stmt(void *self, struct ParseBlock *parse_block, bool is_fun)
{
    if (parse_block->current->kind == TokenKindDoKw) {
        Usize start_line = parse_block->current->loc->s_line;
        bool bad_item = false;

        PUSH_BODY();

        next_token_pb(parse_block);

        while (parse_block->current->kind != TokenKindEndKw &&
               parse_block->current->kind != TokenKindSemicolon &&
               parse_block->current->kind != TokenKindEof) {
            if (valid_body_item(parse_block, bad_item, is_fun)) {
                if (parse_block->current->kind == TokenKindDoKw)
                    verify_stmt(self, parse_block, is_fun);

                PUSH_BODY();

                next_token_pb(parse_block);
            } else {
                bad_item = true;
                next_token_pb(parse_block);
            }
        }

        Usize end_line = parse_block->current->loc->e_line;

        VERIFY_CLOSING_BODY(parse_block);
    } else
        return;
}

static void
get_body_parse_context(void *self, struct ParseBlock *parse_block, bool is_fun)
{
    Usize start_line = parse_block->current->loc->s_line;
    bool bad_item = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_body_item(parse_block, bad_item, is_fun)) {
            switch (parse_block->current->kind) {
                case TokenKindDoKw:
                    verify_stmt(self, parse_block, is_fun);
                    break;
                default:
                    if (is_fun)
                        push__Vec(((struct FunParseContext *)self)->body,
                                  &*parse_block->current);
                    else
                        push__Vec(((struct MethodParseContext *)self)->body,
                                  &*parse_block->current);

                    next_token_pb(parse_block);
                    break;
            }
        } else {
            bad_item = true;
            next_token_pb(parse_block);
        }
    }

    Usize end_line = parse_block->current->loc->e_line;

    VERIFY_CLOSING_BODY(parse_block);
}

static void
get_fun_parse_context(struct FunParseContext *self,
                      struct ParseBlock *parse_block)
{
    // 1. Get tags.
    if (parse_block->current->kind == TokenKindHashtag) {
        next_token_pb(parse_block);

        if (parse_block->current->kind == TokenKindIdentifier)
            self->has_tag = true;
        else if (parse_block->current->kind == TokenKindLParen)
            self->has_tags = true;
        else {
            struct Diagnostic *err = NEW(
              DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorUnexpectedToken),
              *parse_block->current->loc,
              format("unexpected token after `#`, expected identifier or `(`"),
              Some(format("remove this token: `{Sr}`",
                          token_kind_to_string__Token(*parse_block->current))));

            err->err->s = token_kind_to_string__Token(*parse_block->current);

            emit__Diagnostic(err);
        }

        if (self->has_tags) {
            struct Location loc_warn = {};

            start__Location(&loc_warn,
                            parse_block->current->loc->s_line,
                            parse_block->current->loc->s_col);

            next_token_pb(parse_block);

            // #(Example[T], ...)
            while (parse_block->current->kind != TokenKindRParen) {
                push__Vec(self->tags, &*parse_block->current);
                next_token_pb(parse_block);
            }

            end__Location(&loc_warn,
                          parse_block->current->loc->s_line,
                          parse_block->current->loc->s_col);

            struct Diagnostic *warn =
              NEW(DiagnosticWithWarnParser,
                  parse_block,
                  NEW(LilyWarning, LilyWarningIgnoredTags),
                  loc_warn,
                  format("tags are ignored because the tag list is empty"),
                  None());

            emit_warning__Diagnostic(warn, parse_block->disable_warning);

            if (len__Vec(*self->tags) == 0)
                self->has_tags = false;

            next_token_pb(parse_block);
        } else {
            push__Vec(self->tags, &*parse_block->current);
            next_token_pb(parse_block);
        }
    }

    // 2. Get name of function.
    if (parse_block->current->kind == TokenKindIdentifier)
        self->name = &*parse_block->current->lit;
    else {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMissFunctionName),
              *parse_block->current->loc,
              format(""),
              Some(format("add function's name, found: `{Sr}`",
                          token_kind_to_string__Token(*parse_block->current))));

        emit__Diagnostic(err);
    }

    next_token_pb(parse_block);

    // 3. Get generic params.
    PARSE_GENERIC_PARAMS(self);

    // 4. Get params.
    PARSE_PARAMS(self);

    // 5. Get body.
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    get_body_parse_context(self, parse_block, true);
}

void
__free__FunParseContext(struct FunParseContext self)
{
    FREE(Vec, self.tags);
    FREE(Vec, self.generic_params);
    FREE(Vec, self.params);
    FREE(Vec, self.body);
}

struct EnumParseContext
__new__EnumParseContext()
{
    struct EnumParseContext self = { .is_pub = false,
                                     .has_generic_params = false,
                                     .has_data_type = false,
                                     .is_error = false,
                                     .name = NULL,
                                     .data_type =
                                       NEW(Vec, sizeof(struct Token)),
                                     .generic_params = NULL,
                                     .variants =
                                       NEW(Vec, sizeof(struct Token)) };

    return self;
}

static void
get_enum_parse_context(struct EnumParseContext *self,
                       struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. DataType
    if (parse_block->current->kind == TokenKindLParen) {
        next_token_pb(parse_block);

        if (parse_block->current->kind == TokenKindErrorKw) {
            self->is_error = true;

            next_token_pb(parse_block);

            EXPECTED_TOKEN(parse_block, TokenKindRParen, {
                struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`)`");

                err->err->s = from__String("`)`");

                emit__Diagnostic(err);
            });

            self->is_error = true;
        } else {
            while (parse_block->current->kind != TokenKindRParen) {
                push__Vec(self->data_type, &*parse_block->current);
                next_token_pb(parse_block);
            }

            self->has_data_type = true;

            next_token_pb(parse_block);
        }
    }

    // 2. Variants
    bool bad_token = false;

    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_enum_variants(parse_block, bad_token)) {
            push__Vec(self->variants, &*parse_block->current);
            next_token_pb(parse_block);
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }
    }

    VERIFY_EOF(parse_block, bad_token, "`end`");
}

static inline bool
valid_token_in_enum_variants(struct ParseBlock *parse_block,
                             bool already_invalid)
{
    switch (parse_block->current->kind) {
        case TokenKindEof:
        case TokenKindLParen:
        case TokenKindRParen:
        case TokenKindLHook:
        case TokenKindRHook:
        case TokenKindComma:
        case TokenKindInterrogation:
        case TokenKindBang:
        case TokenKindDot:
        case TokenKindIdentifier:
            return true;
        default: {
            if (!already_invalid) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      parse_block,
                      NEW(LilyError, LilyErrorInvalidTokenInEnumVariant),
                      *parse_block->current->loc,
                      format("this token is invalid inside the enum, expected: "
                             "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, `,` or `.`"),
                      None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE();

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
    }
}

void
__free__EnumParseContext(struct EnumParseContext self)
{
    FREE(Vec, self.data_type);
    FREE(Vec, self.generic_params);
    FREE(Vec, self.variants);
}

struct RecordParseContext
__new__RecordParseContext()
{
    struct RecordParseContext self = { .is_pub = false,
                                       .has_generic_params = false,
                                       .name = NULL,
                                       .generic_params = NULL,
                                       .fields =
                                         NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_token_in_record_fields(struct ParseBlock *parse_block,
                             bool already_invalid)
{
    switch (parse_block->current->kind) {
        case TokenKindLParen:
        case TokenKindRParen:
        case TokenKindLHook:
        case TokenKindRHook:
        case TokenKindInterrogation:
        case TokenKindBang:
        case TokenKindComma:
        case TokenKindIdentifier:
        case TokenKindDot:
            return true;
        default: {
            if (!already_invalid) {
                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidTokenInRecordField),
                  *parse_block->current->loc,
                  format("this token is invalid inside the record, expected: "
                         "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, `,` or `.`"),
                  None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE();

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
    }
}

static void
get_record_parse_context(struct RecordParseContext *self,
                         struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. Fields
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_record_fields(parse_block, bad_token)) {
            push__Vec(self->fields, &*parse_block->current);
            next_token_pb(parse_block);
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }
    }

    VERIFY_EOF(parse_block, bad_token, "`end`")
}

void
__free__RecordParseContext(struct RecordParseContext self)
{
    FREE(Vec, self.generic_params);
    FREE(Vec, self.fields);
}

struct AliasParseContext
__new__AliasParseContext()
{
    struct AliasParseContext self = { .is_pub = false,
                                      .has_generic_params = false,
                                      .name = NULL,
                                      .generic_params = NULL,
                                      .data_type =
                                        NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_token_in_alias_data_type(struct ParseBlock *parse_block,
                               bool already_invalid)
{
    switch (parse_block->current->kind) {
        case TokenKindLHook:
        case TokenKindRHook:
        case TokenKindLParen:
        case TokenKindRParen:
        case TokenKindInterrogation:
        case TokenKindBang:
        case TokenKindComma:
        case TokenKindIdentifier:
            return true;
        default: {
            if (!already_invalid) {
                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidTokenInAliasDataType),
                  *parse_block->current->loc,
                  format("this token is invalid inside the record, expected: "
                         "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, or `,`"),
                  None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE();

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
    }
}

static void
get_alias_parse_context(struct AliasParseContext *self,
                        struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. DataType
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    })

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_alias_data_type(parse_block, bad_token)) {
            push__Vec(self->data_type, &*parse_block->current);
            next_token_pb(parse_block);
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }
    }

    VERIFY_EOF(parse_block, bad_token, "`;`");
    next_token_pb(parse_block);
}

void
__free__AliasParseContext(AliasParseContext self)
{
    FREE(Vec, self.generic_params);
    FREE(Vec, self.data_type);
}

struct TraitParseContext
__new__TraitParseContext()
{
    struct TraitParseContext self = { .is_pub = false,
                                      .has_generic_params = false,
                                      .has_inheritance = false,
                                      .name = NULL,
                                      .inheritance = NULL,
                                      .generic_params = NULL,
                                      .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_token_in_trait_body(struct ParseBlock *parse_block, bool already_invalid)
{
    switch (parse_block->current->kind) {
        case TokenKindLParen:
        case TokenKindRParen:
        case TokenKindLHook:
        case TokenKindRHook:
        case TokenKindArrow:
        case TokenKindColonColon:
        case TokenKindAt:
        case TokenKindComma:
        case TokenKindDot:
        case TokenKindIdentifier:
            return true;
        default: {
            if (!already_invalid) {
                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidTokenInRecordField),
                  *parse_block->current->loc,
                  format("this token is invalid inside the record, expected: "
                         "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, `,`, `.`, `->`, "
                         "`@` or `::`"),
                  None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE();

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }
        }
    }
}

static void
get_trait_parse_context(struct TraitParseContext *self,
                        struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. Body
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_trait_body(parse_block, bad_token)) {
            push__Vec(self->body, &*parse_block->current);
            next_token_pb(parse_block);
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }
    }

    VERIFY_EOF(parse_block, bad_token, "`end`");
}

void
__free__TraitParseContext(struct TraitParseContext self)
{
    FREE(Vec, self.inheritance);
    FREE(Vec, self.generic_params);
    FREE(Vec, self.body);
}

struct ClassParseContext
__new__ClassParseContext()
{
    struct ClassParseContext self = { .is_pub = false,
                                      .has_generic_params = false,
                                      .has_inheritance = false,
                                      .name = NULL,
                                      .generic_params = NULL,
                                      .inheritance = NULL,
                                      .impl = NULL,
                                      .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_class_token_in_body(struct ParseBlock *parse_block, bool already_invalid)
{
    bool is_valid = valid_body_item(parse_block, already_invalid, false);

    if (parse_block->current->kind == TokenKindAt || is_valid)
        return true;
    else
        return false;
}

static void
get_class_parse_context(struct ClassParseContext *self,
                        struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. Body
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    struct MethodParseContext method_parse_context;
    struct PropertyParseContext property_parse_context;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        bool is_pub = false;
        bool is_async = false;
        struct Location *async_loc = NULL;

        if (valid_class_token_in_body(parse_block, bad_token)) {
            // push__Vec(self->body, &*parse_block->current);
            // next_token_pb(parse_block);

            if (parse_block->current->kind == TokenKindAt)
                goto method_or_property;
            else if (parse_block->current->kind == TokenKindImportKw)
                goto import;
            else if (parse_block->current->kind == TokenKindPubKw) {
                next_token_pb(parse_block);

                is_pub = true;

                goto method_or_property;
            } else if (parse_block->current->kind == TokenKindAsyncKw) {
                async_loc = parse_block->current->loc;
                is_async = true;

                next_token_pb(parse_block);

                goto method_or_property;
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      parse_block,
                      NEW(LilyError, LilyErrorInvalidClassItem),
                      *parse_block->current->loc,
                      format("expected `@`, `import`, `pub` or `async` for "
                             "begin each declaration in class"),
                      None());

                emit__Diagnostic(err);
                skip_to_next_block(parse_block);
            }
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }

    method_or_property : {
        next_token_pb(parse_block);

        if (parse_block->current->kind != TokenKindIdentifier) {
            struct Diagnostic *err = NEW(
              DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMissNameOnPropertyOrMethod),
              *parse_block->current->loc,
              format(""),
              Some(format("add name on property or method, found `{Sr}`",
                          token_kind_to_string__Token(*parse_block->current))));

            emit__Diagnostic(err);
        }

        struct String *name = &*parse_block->current->lit;

        next_token_pb(parse_block);

        if (parse_block->current->kind != TokenKindLParen &&
            parse_block->current->kind != TokenKindLHook &&
            parse_block->current->kind != TokenKindEq) {

            if (is_async) {
                struct Diagnostic *note =
                  NEW(DiagnosticWithNoteParser,
                      parse_block,
                      format(
                        "the declaration of async in property is not expected"),
                      *async_loc,
                      format(""),
                      None());

                struct Diagnostic *err =
                  EXPECTED_TOKEN_ERR(parse_block, "`(`, `[` or `=`");

                err->err->s = from__String("`(`, `[` or `=`");

                emit__Diagnostic(note);
                emit__Diagnostic(err);
            }

            property_parse_context = NEW(PropertyParseContext);
            property_parse_context.name = name;
            property_parse_context.is_pub = is_pub;

            goto property;
        } else {
            method_parse_context = NEW(MethodParseContext);
            method_parse_context.name = name;
            method_parse_context.is_pub = is_pub;
            method_parse_context.is_async = is_async;

            goto method;
        }
    }

    method : {
        get_method_parse_context(&method_parse_context, parse_block);
        push__Vec(self->body, NEW(ParseContextMethod, method_parse_context));

        goto exit;
    }

        import : {
            struct ImportParseContext impor_parse_context =
              NEW(ImportParseContext);

        get_import_parse_context(&impor_parse_context, parse_block);
        push__Vec(self->body, NEW(ParseContextImport, impor_parse_context));

        goto exit;
    }

property : {
    get_property_parse_context(&property_parse_context, parse_block);
    push__Vec(self->body, NEW(ParseContextProperty, property_parse_context));

    goto exit;
}

exit : {
}
}

if (parse_block->current->kind == TokenKindEof && !bad_token) {
    struct Diagnostic *err =
      NEW(DiagnosticWithErrParser,
          parse_block,
          NEW(LilyError, LilyErrorMissClosingBlock),
          *((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                     parse_block->pos - 1))
             ->loc,
          format("expected closing block here"),
          None());

    err->err->s = from__String("`end`");

    emit__Diagnostic(err);
} else
    next_token_pb(parse_block);
}

void
__free__ClassParseContext(struct ClassParseContext self)
{
    FREE(Vec, self.generic_params);
    FREE(Vec, self.inheritance);
    FREE(Vec, self.impl);

    for (Usize i = len__Vec(*self.body); i--;)
        FREE(ParseContextAll, get__Vec(*self.body, i));

    FREE(Vec, self.body);
}

struct TagParseContext *
__new__TagParseContext()
{
    struct TagParseContext *self = malloc(sizeof(struct TagParseContext));
    self->has_generic_params = false;
    self->name = NULL;
    self->generic_params = NULL;
    self->body = NEW(Vec, sizeof(struct Token));
    return self;
}

static inline bool
valid_tag_token_in_body(struct ParseBlock *parse_block, bool already_invalid)
{
    if (valid_body_item(parse_block, true, false))
        return true;
    else {
        if (!already_invalid) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidTokenInTagBody),
                  *parse_block->current->loc,
                  format("invalid token in tag body"),
                  None());

            struct Diagnostic *note = CLOSE_BLOCK_NOTE();

            emit__Diagnostic(err);
            emit__Diagnostic(note);
        }

        return false;
    }
}

static void
get_tag_parse_context(struct TagParseContext *self,
                      struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. Body
    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (parse_block->current->kind == TokenKindFunKw) {

        } else if (parse_block->current->kind == TokenKindImportKw) {
        }
    }
}

void
__free__TagParseContext(struct TagParseContext *self)
{
    FREE(Vec, self->generic_params);
    FREE(Vec, self->body);
    free(self);
}

struct MethodParseContext
__new__MethodParseContext()
{
    struct MethodParseContext self = { .is_pub = false,
                                       .is_async = false,
                                       .has_generic_params = false,
                                       .has_params = false,
                                       .name = NULL,
                                       .generic_params =
                                         NEW(Vec, sizeof(struct Token)),
                                       .params = NEW(Vec, sizeof(struct Token)),
                                       .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static void
get_method_parse_context(struct MethodParseContext *self,
                         struct ParseBlock *parse_block)
{
    PARSE_GENERIC_PARAMS(self);
    PARSE_PARAMS(self);

    EXPECTED_TOKEN(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    get_body_parse_context(self, parse_block, false);
}

void
__free__MethodParseContext(struct MethodParseContext self)
{
    FREE(Vec, self.generic_params);
    FREE(Vec, self.params);
    FREE(Vec, self.body);
}

struct PropertyParseContext
__new__PropertyParseContext()
{
    struct PropertyParseContext self = { .is_pub = false,
                                         .name = NULL,
                                         .data_type =
                                           NEW(Vec, sizeof(struct Token)) };

    return self;
}

static void
get_property_parse_context(struct PropertyParseContext *self,
                           struct ParseBlock *parse_block)
{
    bool bad_token = false;

    while (parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        push__Vec(self->data_type, &*parse_block->current);
        next_token_pb(parse_block);
    }

    VERIFY_EOF(parse_block, bad_token, "`;`");
    next_token_pb(parse_block);
}

void
__free__PropertyParseContext(struct PropertyParseContext self)
{
    FREE(Vec, self.data_type);
}

struct ImportParseContext
__new__ImportParseContext()
{
    struct ImportParseContext self = { .is_pub = false,
                                       .value = NULL,
                                       .as_value = NULL };

    return self;
}

static void
get_import_parse_context(struct ImportParseContext *self,
                         struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    if (parse_block->current->kind == TokenKindStringLit) {
        self->value = &*parse_block->current->lit;
        next_token_pb(parse_block);
    } else {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorMissImportValue),
                                     *parse_block->current->loc,
                                     format(""),
                                     Some(format("please add `import` value")));

        emit__Diagnostic(err);
    }

    if (parse_block->current->kind == TokenKindAsKw) {
        next_token_pb(parse_block);

        if (parse_block->current->kind == TokenKindIdentifier) {
            self->as_value = &*parse_block->current->lit;
            next_token_pb(parse_block);
        } else {
            struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                         parse_block,
                                         NEW(LilyError, LilyErrorMissAsValue),
                                         *parse_block->current->loc,
                                         format(""),
                                         Some(format("please add `as` value")));

            emit__Diagnostic(err);
        }
    }
}

static inline struct Diagnostic *
__new__DiagnosticWithErrParser(struct ParseBlock *self,
                               struct LilyError *err,
                               struct Location loc,
                               struct String *detail_msg,
                               struct Option *help)
{
    self->count_error += 1;
    return NEW(
      DiagnosticWithErr, err, loc, self->scanner.src->file, detail_msg, help);
}

static inline struct Diagnostic *
__new__DiagnosticWithWarnParser(struct ParseBlock *self,
                                struct LilyWarning *warn,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help)
{
    self->count_warning += 1;
    return NEW(
      DiagnosticWithWarn, warn, loc, self->scanner.src->file, detail_msg, help);
}

static inline struct Diagnostic *
__new__DiagnosticWithNoteParser(struct ParseBlock *self,
                                struct String *note,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help)
{
    return NEW(
      DiagnosticWithNote, note, loc, self->scanner.src->file, detail_msg, help);
}

struct ParseContext *
__new__ParseContextFun(struct FunParseContext fun)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindFun;
    self->value.fun = fun;
    return self;
}

struct ParseContext *
__new__ParseContextEnum(struct EnumParseContext enum_, bool is_object)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));

    if (is_object)
        self->kind = ParseContextKindEnumObject;
    else
        self->kind = ParseContextKindEnum;

    self->value.enum_ = enum_;
    return self;
}

struct ParseContext *
__new__ParseContextRecord(struct RecordParseContext record, bool is_object)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));

    if (is_object)
        self->kind = ParseContextKindRecordObject;
    else
        self->kind = ParseContextKindRecord;

    self->value.record = record;
    return self;
}

struct ParseContext *
__new__ParseContextAlias(struct AliasParseContext alias)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindAlias;
    self->value.alias = alias;
    return self;
}

struct ParseContext *
__new__ParseContextTrait(struct TraitParseContext trait)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindTrait;
    self->value.trait = trait;
    return self;
}

struct ParseContext *__new__ParseContextClass(struct ClassParseContext class)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindClass;
    self->value.class = class;
    return self;
}

struct ParseContext *
__new__ParseContextMethod(struct MethodParseContext method)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindMethod;
    self->value.method = method;
    return self;
}

struct ParseContext *
__new__ParseContextProperty(struct PropertyParseContext property)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindProperty;
    self->value.property = property;
    return self;
}

struct ParseContext *
__new__ParseContextImport(struct ImportParseContext import)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindImport;
    self->value.import = import;
    return self;
}

void
__free__ParseContextFun(struct ParseContext *self)
{
    FREE(FunParseContext, self->value.fun);
    free(self);
}

void
__free__ParseContextEnum(struct ParseContext *self)
{
    FREE(EnumParseContext, self->value.enum_);
    free(self);
}

void
__free__ParseContextRecord(struct ParseContext *self)
{
    FREE(RecordParseContext, self->value.record);
    free(self);
}

void
__free__ParseContextAlias(struct ParseContext *self)
{
    FREE(AliasParseContext, self->value.alias);
    free(self);
}

void
__free__ParseContextTrait(struct ParseContext *self)
{
    FREE(TraitParseContext, self->value.trait);
    free(self);
}

void
__free__ParseContextClass(struct ParseContext *self)
{
    FREE(ClassParseContext, self->value.class);
    free(self);
}

void
__free__ParseContextMethod(struct ParseContext *self)
{
    FREE(MethodParseContext, self->value.method);
    free(self);
}

void
__free__ParseContextProperty(struct ParseContext *self)
{
    FREE(PropertyParseContext, self->value.property);
    free(self);
}

void
__free__ParseContextImport(struct ParseContext *self)
{
    free(self);
}

void
__free__ParseContextAll(struct ParseContext *self)
{
    switch (self->kind) {
        case ParseContextKindFun:
            FREE(ParseContextFun, self);
            break;
        case ParseContextKindEnum:
        case ParseContextKindEnumObject:
            FREE(ParseContextEnum, self);
            break;
        case ParseContextKindRecord:
        case ParseContextKindRecordObject:
            FREE(ParseContextRecord, self);
            break;
        case ParseContextKindAlias:
            FREE(ParseContextAlias, self);
            break;
        case ParseContextKindTrait:
            FREE(ParseContextTrait, self);
            break;
        case ParseContextKindClass:
            FREE(ParseContextClass, self);
            break;
        case ParseContextKindMethod:
            FREE(ParseContextMethod, self);
            break;
        case ParseContextKindProperty:
            FREE(ParseContextProperty, self);
            break;
        case ParseContextKindImport:
            FREE(ParseContextImport, self);
            break;
        default:
            UNREACHABLE("unknown parse context kind");
    }
}

struct Parser
__new__Parser(struct ParseBlock parse_block)
{
    struct Parser self = { .parse_block = parse_block,
                           .pos = 0,
                           .decls = NEW(Vec, sizeof(struct Decl)) };

    if (len__Vec(*parse_block.blocks) > 0)
        self.current =
          &*((struct ParseContext *)get__Vec(*parse_block.blocks, 0));
    else
        self.current = NULL;

    return self;
}

struct ParseDecl
__new__ParseDecl(struct Vec *tokens)
{
    struct ParseDecl self = {
        .pos = 0,
        .current = len__Vec(*tokens) == 0 ? NULL : get__Vec(*tokens, 0),
        .previous = len__Vec(*tokens) == 0 ? NULL : get__Vec(*tokens, 0),
        .tokens = tokens
    };

    return self;
}

static inline void
next_token(struct ParseDecl *self)
{
    self->pos += 1;
    self->current = self->pos < len__Vec(*self->tokens)
                      ? get__Vec(*self->tokens, self->pos)
                      : NULL;
    self->previous = get__Vec(*self->tokens, self->pos - 1);
}

static inline bool
is_data_type(struct ParseDecl *self)
{
    switch (self->current->kind) {
        case TokenKindAmpersand:
        case TokenKindIdentifier:
        case TokenKindInterrogation:
        case TokenKindBang:
        case TokenKindLHook:
        case TokenKindLParen:
        case TokenKindHat:
            return true;
        default:
            return false;
    }
}

static struct DataType *
parse_data_type(struct Parser self, struct ParseDecl *parse_decl)
{
    next_token(parse_decl);

    struct DataType *data_type = NULL;

    switch (parse_decl->previous->kind) {
        case TokenKindIdentifier: {
            Str identifier_str = to_Str__String(*parse_decl->previous->lit);

            if (!strcmp(identifier_str, "Int8"))
                data_type = NEW(DataType, DataTypeKindI8);
            else if (!strcmp(identifier_str, "Int16"))
                data_type = NEW(DataType, DataTypeKindI16);
            else if (!strcmp(identifier_str, "Int32"))
                data_type = NEW(DataType, DataTypeKindI32);
            else if (!strcmp(identifier_str, "Int64"))
                data_type = NEW(DataType, DataTypeKindI64);
            else if (!strcmp(identifier_str, "Int128"))
                data_type = NEW(DataType, DataTypeKindI128);
            else if (!strcmp(identifier_str, "Uint8"))
                data_type = NEW(DataType, DataTypeKindU8);
            else if (!strcmp(identifier_str, "Uint16"))
                data_type = NEW(DataType, DataTypeKindU16);
            else if (!strcmp(identifier_str, "Uint32"))
                data_type = NEW(DataType, DataTypeKindU32);
            else if (!strcmp(identifier_str, "Uint64"))
                data_type = NEW(DataType, DataTypeKindU64);
            else if (!strcmp(identifier_str, "Uint128"))
                data_type = NEW(DataType, DataTypeKindU128);
            else if (!strcmp(identifier_str, "Isize"))
                data_type = NEW(DataType, DataTypeKindIsize);
            else if (!strcmp(identifier_str, "Usize"))
                data_type = NEW(DataType, DataTypeKindUsize);
            else if (!strcmp(identifier_str, "Float32"))
                data_type = NEW(DataType, DataTypeKindF32);
            else if (!strcmp(identifier_str, "Float64"))
                data_type = NEW(DataType, DataTypeKindF64);
            else if (!strcmp(identifier_str, "Bool"))
                data_type = NEW(DataType, DataTypeKindBool);
            else if (!strcmp(identifier_str, "Char"))
                data_type = NEW(DataType, DataTypeKindChar);
            else if (!strcmp(identifier_str, "Str"))
                data_type = NEW(DataType, DataTypeKindStr);
            else if (!strcmp(identifier_str, "Any"))
                data_type = NEW(DataType, DataTypeKindAny);
            else if (!strcmp(identifier_str, "Unit"))
                data_type = NEW(DataType, DataTypeKindUnit);
            else {
                if (parse_decl->current->kind == TokenKindLHook) {
                    struct Vec *data_types = NEW(Vec, sizeof(struct DataType));

                    PARSE_HOOK(parse_decl, {
                        push__Vec(data_types,
                                  parse_data_type(self, parse_decl));

                        EXPECTED_TOKEN(parse_decl, TokenKindComma, {
                            struct Diagnostic *err =
                              NEW(DiagnosticWithErrParser,
                                  &self.parse_block,
                                  NEW(LilyError, LilyErrorExpectedToken),
                                  *parse_decl->current->loc,
                                  format(""),
                                  None());

                            err->err->s = from__String("`,`");

                            emit__Diagnostic(err);
                        });
                    });

                    if (len__Vec(*data_types) == 0) {
                        struct Diagnostic *warn =
                          NEW(DiagnosticWithWarnParser,
                              &self.parse_block,
                              NEW(LilyWarning, LilyWarningIgnoredGenericParams),
                              *parse_decl->current->loc,
                              format("the generic params are ignored because "
                                     "they are empty"),
                              None());

                        emit_warning__Diagnostic(
                          warn, self.parse_block.disable_warning);
                    }

                    data_type = NEW(DataTypeCustom,
                                    &*parse_decl->current->lit,
                                    Some(data_types));
                } else
                    data_type =
                      NEW(DataTypeCustom, &*parse_decl->current->lit, None());
            }

            free(identifier_str);

            break;
        }
        case TokenKindLHook: {
            Usize *size = NULL;
            bool is_wildcard = false;

            next_token(parse_decl);

            {
                switch (parse_decl->current->kind) {
                    case TokenKindIntLit: {
                        Str size_str =
                          to_Str__String(*parse_decl->current->lit);

#ifdef LILY_WINDOWS_OS
                        UInt64 size_u = _atoi64(size_str);

                        *size = (Usize)size_u;
#else
                        int size_u = atoi(size_str);

                        *size = (Usize)size_u;
#endif

                        free(size_str);

                        break;
                    }
                    case TokenKindIdentifier: {
                        Str id_str = to_Str__String(*parse_decl->current->lit);

                        if (!strcmp(id_str, "_"))
                            is_wildcard = true;

                        free(id_str);

                        break;
                    }
                    case TokenKindRHook:
                        break;
                    default: {
                        struct Diagnostic *err =
                          NEW(DiagnosticWithErrParser,
                              &self.parse_block,
                              NEW(LilyError, LilyErrorUnexpectedToken),
                              *parse_decl->current->loc,
                              format(""),
                              None());

                        err->err->s =
                          token_kind_to_string__Token(*parse_decl->current);

                        emit__Diagnostic(err);

                        break;
                    }
                }
            }

            if (!is_wildcard && size == NULL) {
                next_token(parse_decl);

                EXPECTED_TOKEN(parse_decl, TokenKindRHook, {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self.parse_block,
                          NEW(LilyError, LilyErrorExpectedToken),
                          *parse_decl->current->loc,
                          format(""),
                          None());

                    err->err->s = from__String("`]`");

                    emit__Diagnostic(err);
                });
            }

            if (is_data_type(parse_decl)) {
                if (size == NULL)
                    data_type = NEW(DataTypeArray,
                                    Some(parse_data_type(self, parse_decl)),
                                    None());
                else
                    data_type = NEW(DataTypeArray,
                                    Some(parse_data_type(self, parse_decl)),
                                    Some(size));
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorMissDataType),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                emit__Diagnostic(err);
            }

            break;
        }
        case TokenKindLParen: {
            struct Vec *data_types = NEW(Vec, sizeof(struct DataType));

            PARSE_PAREN(parse_decl, {
                push__Vec(data_types, parse_data_type(self, parse_decl));

                EXPECTED_TOKEN(parse_decl, TokenKindComma, {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self.parse_block,
                          NEW(LilyError, LilyErrorExpectedToken),
                          *parse_decl->current->loc,
                          format(""),
                          None());

                    err->err->s = from__String("`,`");

                    emit__Diagnostic(err);
                });
            });

            data_type = NEW(DataTypeTuple, data_types);

            break;
        }
        case TokenKindHat:
            data_type = NEW(DataTypePtr, parse_data_type(self, parse_decl));
            break;
        case TokenKindInterrogation:
            data_type =
              NEW(DataTypeOptional, parse_data_type(self, parse_decl));
            break;
        case TokenKindBang:
            data_type =
              NEW(DataTypeException, parse_data_type(self, parse_decl));
            break;
        case TokenKindAmpersand:
            data_type = NEW(DataTypeRef, parse_data_type(self, parse_decl));
        case TokenKindBar: {
            next_token(parse_decl);

            struct Vec *params = NEW(Vec, sizeof(struct Vec));

            push__Vec(params, parse_data_type(self, parse_decl));

            while (parse_decl->current != TokenKindBar) {
                next_token(parse_decl);

                push__Vec(params, parse_data_type(self, parse_decl));
            }

            struct DataType *return_type = pop__Vec(params);

            if (len__Vec(*params) == 0) {
                assert(0 && "warning");
                struct Diagnostic *warn =
                  NEW(DiagnosticWithWarnParser,
                      &self.parse_block,
                      NEW(LilyWarning, LilyWarningIgnoredLambdaDataType),
                      *parse_decl->current->loc,
                      format("the lambda data type is ignored because they are "
                             "no return data type"),
                      None());

                emit_warning__Diagnostic(warn,
                                         self.parse_block.disable_warning);

                FREE(Vec, params);

                return return_type;
            }

            next_token(parse_decl);

            data_type = NEW(DataTypeLambda, params, return_type);

            break;
        }
        default:
            break;
    }

    return data_type;
}

// struct Vec<struct Generic*>*
static struct Vec *
parse_generic_params(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Vec *generic_params = NEW(Vec, sizeof(struct Generic));

    while (parse_decl->pos < len__Vec(*parse_decl->tokens)) {
        struct String *data_type = NULL;
        struct Location loc = NEW(Location);

        start__Location(&loc,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        switch (parse_decl->current->kind) {
            case TokenKindIdentifier: {
                data_type = &*parse_decl->current->lit;

                next_token(parse_decl);

                if (parse_decl->current->lit == TokenKindColon) {
                    struct Location loc_data_type = NEW(Location);

                    start__Location(&loc_data_type,
                                    parse_decl->current->loc->s_line,
                                    parse_decl->current->loc->s_col);

                    struct DataType *restricted_data_type =
                      parse_data_type(self, parse_decl);

                    end__Location(&loc_data_type,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);

                    end__Location(&loc,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);

                    push__Vec(generic_params,
                              NEW(GenericRestrictedDataType,
                                  data_type,
                                  loc,
                                  NEW(Tuple,
                                      2,
                                      restricted_data_type,
                                      copy__Location(&loc_data_type))));
                } else {
                    end__Location(&loc,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);
                    push__Vec(generic_params,
                              NEW(GenericDataType, data_type, loc));
                }
            }
            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorUnexpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = token_kind_to_string__Token(*parse_decl->current);

                emit__Diagnostic(err);
            }
        }

        EXPECTED_TOKEN(parse_decl, TokenKindComma, {
            struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                         &self.parse_block,
                                         NEW(LilyError, LilyErrorExpectedToken),
                                         *parse_decl->current->loc,
                                         format(""),
                                         None());

            err->err->s = from__String("`,`");

            emit__Diagnostic(err);
        });
    }

    return generic_params;
}

static void
parse_fun_declaration(struct Parser *self)
{
}

static void
parse_declaration(struct Parser *self)
{
    switch (self->current->kind) {
        case ParseContextKindFun:
            parse_fun_declaration(self);
            break;
        case ParseContextKindClass:
            break;
        default:
            UNREACHABLE("unknown parse context kind");
    }
}

#define NEXT_BLOCK()                                                \
    self->pos += 1;                                                 \
    self->current = self->pos < len__Vec(*self->parse_block.blocks) \
                      ? ((struct ParseContext *)get__Vec(           \
                          *self->parse_block.blocks, self->pos))    \
                      : NULL

void
run__Parser(struct Parser *self)
{
    while (self->pos < len__Vec(*self->parse_block.blocks)) {
        parse_declaration(self);
        NEXT_BLOCK();
    }
}

void
__free__Parser(struct Parser self)
{
    FREE(ParseBlock, self.parse_block);
    FREE(Vec, self.decls);
}
