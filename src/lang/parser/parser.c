#include <assert.h>
#include <base/format.h>
#include <base/macros.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/parser/parser.h>
#include <pthread.h>

#define EXPECTED_TOKEN_ERR(parse_block, expected)   \
    NEW(DiagnosticWithErrParser,                    \
        parse_block,                                \
        NEW(LilyError, LilyErrorExpectedToken),     \
        *parse_block->current->loc,                 \
        format(""),                                 \
        Some(format("expected `{c}`, found `{Sr}`", \
                    expected,                       \
                    token_kind_to_string__Token(*parse_block->current))));

#define CLOSE_BLOCK_NOTE(parse_block)                        \
    NEW(DiagnosticWithNoteParser,                            \
        parse_block,                                         \
        format("you may have forgotten to close the block"), \
        *parse_block->current->loc,                          \
        format(""),                                          \
        None());

#define PARSE_ENUM_DATA_TYPE(self, parse_block)                                \
    if (parse_block->current->kind == TokenKindLParen) {                       \
        next_token_pb(parse_block);                                            \
                                                                               \
        if (parse_block->current->kind == TokenKindErrorKw) {                  \
            self->is_error = true;                                             \
                                                                               \
            next_token_pb(parse_block);                                        \
                                                                               \
            if (parse_block->current->kind != TokenKindRParen) {               \
                struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, ')'); \
                                                                               \
                err->err->s = from__String(")");                               \
                                                                               \
                emit__Diagnostic(err);                                         \
            } else {                                                           \
                next_token_pb(parse_block);                                    \
            }                                                                  \
                                                                               \
            self->is_error = true;                                             \
        } else {                                                               \
            while (parse_block->current->kind != TokenKindRParen) {            \
                push__Vec(self->data_type, &*parse_block->current);            \
                next_token_pb(parse_block);                                    \
            }                                                                  \
                                                                               \
            self->has_data_type = true;                                        \
                                                                               \
            next_token_pb(parse_block);                                        \
        }                                                                      \
    }

#define PARSE_ENUM_VARIANTS(self, parse_block, bad_token)               \
    if (parse_block->current->kind != TokenKindEq) {                    \
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, '=');  \
                                                                        \
        err->err->s = from__String("=");                                \
                                                                        \
        emit__Diagnostic(err);                                          \
    } else                                                              \
        next_token_pb(parse_block);                                     \
                                                                        \
    while (parse_block->current->kind != TokenKindEndKw &&              \
           parse_block->current->kind != TokenKindEof) {                \
        if (valid_token_in_enum_variants(parse_block, bad_token)) {     \
            push__Vec(self->variants, &*parse_block->current);          \
            next_token_pb(parse_block);                                 \
        } else {                                                        \
            bad_token = true;                                           \
            next_token_pb(parse_block);                                 \
        }                                                               \
    }                                                                   \
                                                                        \
    if (parse_block->current->kind == TokenKindEof && !bad_token) {     \
        struct Diagnostic *err =                                        \
          NEW(DiagnosticWithErrParser,                                  \
              parse_block,                                              \
              NEW(LilyError, LilyErrorMissClosingBlock),                \
              *((struct Token *)get__Vec(*parse_block->scanner->tokens, \
                                         parse_block->pos - 1))         \
                 ->loc,                                                 \
              format("expected closing block here"),                    \
              None());                                                  \
                                                                        \
        err->err->s = from__String("`end`");                            \
                                                                        \
        emit__Diagnostic(err);                                          \
    } else                                                              \
        next_token_pb(parse_block);

static inline void
next_token_pb(struct ParseBlock *self);
static inline void
skip_to_next_block(struct ParseBlock *self);
static struct String *
get_type_name(struct ParseBlock *self);
static void
get_type_context(struct ParseBlock *self, bool is_pub);
static struct Vec *
get_generic_params_type(struct ParseBlock *self);
static inline bool
valid_fun_body_item(struct FunParseContext *self,
                    struct ParseBlock *parse_block,
                    bool already_invalid);
static void
get_body_fun_parse_context(struct FunParseContext *self,
                           struct ParseBlock *parse_block);
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
static void
get_enum_object_parse_context(struct EnumObjectParseContext *self,
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

#include <base/print.h>
struct ParseBlock *
__new__ParseBlock(struct Scanner *scanner)
{
    struct ParseBlock *self = malloc(sizeof(struct ParseBlock));
    self->scanner = scanner;
    self->blocks = NEW(Vec, sizeof(struct Vec *));
    self->current = &*(struct Token *)get__Vec(*scanner->tokens, 0);
    self->disable_warning = NEW(Vec, sizeof(Str));
    self->pos = 0;
    self->count_error = 0;
    self->count_warning = 0;
    return self;
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
        switch (self->current->kind) {
            case TokenKindPubKw:
                next_token_pb(self);

                switch (self->current->kind) {
                    case TokenKindFunKw: {
                        struct FunParseContext *fun_parse_context =
                          NEW(FunParseContext);
                        fun_parse_context->is_pub = true;

                        next_token_pb(self);
                        get_fun_parse_context(fun_parse_context, self);

                        push__Vec(self->blocks,
                                  NEW(ParseContextFun, fun_parse_context));

                        break;
                    }
                    case TokenKindAsyncKw: {
                        next_token_pb(self);

                        if (self->current->kind != TokenKindFunKw) {
                            struct Diagnostic *err = NEW(
                              DiagnosticWithErrParser,
                              self,
                              NEW(LilyError, LilyErrorBadUsageOfAsync),
                              *self->current->loc,
                              format("expected `fun` after async declaration"),
                              None());

                            emit__Diagnostic(err);
                        } else
                            next_token_pb(self);

                        struct FunParseContext *fun_parse_context =
                          NEW(FunParseContext);
                        fun_parse_context->is_pub = true;
                        fun_parse_context->is_async = true;

                        get_fun_parse_context(fun_parse_context, self);

                        push__Vec(self->blocks,
                                  NEW(ParseContextFun, fun_parse_context));

                        break;
                    }
                    case TokenKindTypeKw:
                        get_type_context(self, true);
                        break;
                    case TokenKindObjectKw:
                        TODO("");
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
                struct FunParseContext *fun_parse_context =
                  NEW(FunParseContext);

                next_token_pb(self);
                get_fun_parse_context(fun_parse_context, self);

                push__Vec(self->blocks,
                          NEW(ParseContextFun, fun_parse_context));

                break;
            }
            case TokenKindTagKw:
                break;
            case TokenKindTypeKw:
                get_type_context(self, false);
                break;
            case TokenKindObjectKw:
                break;
            default:
                assert(0 && "error");
                skip_to_next_block(self);
                break;
        }
    }
}

static inline void
next_token_pb(struct ParseBlock *self)
{
    if (self->current->kind != TokenKindEof) {
        self->pos += 1;
        self->current =
          &*(struct Token *)get__Vec(*self->scanner->tokens, self->pos);
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

    if (((struct Token *)get__Vec(*self->scanner->tokens, self->pos + 1))
            ->kind == TokenKindLHook ||
        self->current->kind == TokenKindIdentifier)
        next_token_pb(self);

    if (self->current->kind == TokenKindLHook) {
        struct Vec *content = get_generic_params_type(self);

        concat__Vec(generic_params, content);

        FREE(Vec, content);
    }

    if (len__Vec(*generic_params) != 0)
        has_generic_params = true;

    if (self->current->kind != TokenKindColon) {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(self, ':');

        err->err->s = from__String(":");

        emit__Diagnostic(err);
    } else {
        next_token_pb(self);
    }

    switch (self->current->kind) {
        case TokenKindEnumKw: {
            struct EnumParseContext *enum_parse_context = NEW(EnumParseContext);

            enum_parse_context->name = name;
            enum_parse_context->generic_params = generic_params;
            enum_parse_context->is_pub = is_pub;
            enum_parse_context->has_generic_params = has_generic_params;

            get_enum_parse_context(enum_parse_context, self);

            push__Vec(self->blocks, NEW(ParseContextEnum, enum_parse_context));

            break;
        }
        case TokenKindRecordKw:
            struct RecordParseContext *record_parse_context =
              NEW(RecordParseContext);

            record_parse_context->name = name;
            record_parse_context->generic_params = generic_params;
            record_parse_context->is_pub = is_pub;
            record_parse_context->has_generic_params = has_generic_params;

            get_record_parse_context(record_parse_context, self);

            push__Vec(self->blocks,
                      NEW(ParseContextRecord, record_parse_context));

            break;
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

static struct Vec *
get_generic_params_type(struct ParseBlock *self)
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
__free__ParseBlock(struct ParseBlock *self)
{
    for (Usize i = 0; i < len__Vec(*self->blocks); i++)
        FREE(ParseContextAll, get__Vec(*self->blocks, i));

    for (Usize i = 0; i < len__Vec(*self->disable_warning); i++)
        free(get__Vec(*self->disable_warning, i));

    FREE(Vec, self->blocks);
    FREE(Vec, self->disable_warning);
    FREE(Scanner, self->scanner);
    free(self);
}

struct FunParseContext *
__new__FunParseContext()
{
    struct FunParseContext *self = malloc(sizeof(struct FunParseContext));
    self->is_pub = false;
    self->is_async = false;
    self->has_generic_params = false;
    self->has_tag = false;
    self->has_tags = false;
    self->has_params = false;
    self->name = NULL;
    self->tags = NEW(Vec, sizeof(struct Token));
    self->generic_params = NEW(Vec, sizeof(struct Token));
    self->params = NEW(Vec, sizeof(struct Token));
    self->body = NEW(Vec, sizeof(struct Token));
    return self;
}

static inline bool
valid_fun_body_item(struct FunParseContext *self,
                    struct ParseBlock *parse_block,
                    bool already_invalid)
{
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
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      parse_block,
                      NEW(LilyError, LilyErrorInvalidItemInFunBody),
                      *parse_block->current->loc,
                      format("this token is invalid inside the function"),
                      None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE(parse_block);

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
        default:
            return true;
    }
}

static void
get_body_fun_parse_context(struct FunParseContext *self,
                           struct ParseBlock *parse_block)
{
    Usize start_line = parse_block->current->loc->s_line;
    bool bad_item = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_fun_body_item(self, parse_block, bad_item)) {
            switch (parse_block->current->kind) {
                case TokenKindDoKw:
                case TokenKindIfKw:
                    get_body_fun_parse_context(self, parse_block);
                    break;
                default:
                    push__Vec(self->body, &*parse_block->current);
                    next_token_pb(parse_block);
                    break;
            }
        } else {
            bad_item = true;
            next_token_pb(parse_block);
        }
    }

    Usize end_line = parse_block->current->loc->e_line;

    if (parse_block->current->kind == TokenKindEof && !bad_item) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMissClosingBlock),
              *((struct Token *)get__Vec(*parse_block->scanner->tokens,
                                         parse_block->pos - 1))
                 ->loc,
              format("expected closing block here"),
              None());

        err->err->s = format("`;` or `end`");

        emit__Diagnostic(err);
    }

    if (parse_block->current->kind == TokenKindSemicolon &&
        start_line != end_line) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMisuseOfSpecialClosingBlock),
              *parse_block->current->loc,
              format("the `;` is used when a function is write one line"),
              Some(format("replace `;` closing, by `end` closing")));

        emit__Diagnostic(err);
    }

    next_token_pb(parse_block);
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
    if (parse_block->current->kind == TokenKindLHook) {
        next_token_pb(parse_block);

        while (parse_block->current->kind != TokenKindRHook) {
            push__Vec(self->generic_params, &*parse_block->current);
            next_token_pb(parse_block);
        }

        struct Diagnostic *warn =
          NEW(DiagnosticWithWarnParser,
              parse_block,
              NEW(LilyWarning, LilyWarningIgnoredGenericParams),
              *parse_block->current->loc,
              format("the generic params are ignored because they are empty"),
              None());

        emit_warning__Diagnostic(warn, parse_block->disable_warning);

        if (len__Vec(*self->generic_params) != 0)
            self->has_generic_params = true;

        next_token_pb(parse_block);
    }

    // 4. Get params.
    if (parse_block->current->kind == TokenKindLParen) {
        next_token_pb(parse_block);

        while (parse_block->current->kind != TokenKindRParen) {
            push__Vec(self->params, &*parse_block->current);
            next_token_pb(parse_block);
        }

        if (len__Vec(*self->params) != 0)
            self->has_params = true;

        next_token_pb(parse_block);
    }

    // 5. Get body.
    if (parse_block->current->kind == TokenKindEq)
        next_token_pb(parse_block);
    else {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, '=');

        err->err->s = from__String("=");

        emit__Diagnostic(err);
    }

    get_body_fun_parse_context(self, parse_block);
}

void
__free__FunParseContext(struct FunParseContext *self)
{
    FREE(Vec, self->tags);
    FREE(Vec, self->generic_params);
    FREE(Vec, self->params);
    FREE(Vec, self->body);
    free(self);
}

struct EnumParseContext *
__new__EnumParseContext()
{
    struct EnumParseContext *self = malloc(sizeof(struct EnumParseContext));
    self->is_pub = false;
    self->has_generic_params = false;
    self->has_data_type = false;
    self->is_error = false;
    self->name = NULL;
    self->data_type = NEW(Vec, sizeof(struct Token));
    self->generic_params = NULL;
    self->variants = NEW(Vec, sizeof(struct Token));
    return self;
}

static void
get_enum_parse_context(struct EnumParseContext *self,
                       struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. DataType
    PARSE_ENUM_DATA_TYPE(self, parse_block);

    // 2. Variants
    bool bad_token = false;

    PARSE_ENUM_VARIANTS(self, parse_block, bad_token);
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
                             "`(`, `)`, `[`, `]`, `ID`, `?`, `!` or `,`"),
                      None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE(parse_block);

                emit__Diagnostic(err);
                emit__Diagnostic(note);
            }

            return false;
        }
    }
}

void
__free__EnumParseContext(struct EnumParseContext *self)
{
    FREE(Vec, self->data_type);
    FREE(Vec, self->generic_params);
    FREE(Vec, self->variants);
    free(self);
}

struct RecordParseContext *
__new__RecordParseContext()
{
    struct RecordParseContext *self = malloc(sizeof(struct RecordParseContext));
    self->is_pub = false;
    self->has_generic_params = false;
    self->has_data_type = false;
    self->name = NULL;
    self->generic_params = NULL;
    self->fields = NEW(Vec, sizeof(struct Token));
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
        case TokenKindColon:
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
                  NEW(LilyError, LilyErrorInvalidTokenInRecordField),
                  *parse_block->current->loc,
                  format("this token is invalid inside the record, expected: "
                         "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, `,` or `:`"),
                  None());

                struct Diagnostic *note = CLOSE_BLOCK_NOTE(parse_block);

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
    if (parse_block->current->kind != TokenKindEq) {
        struct Diagnostic *err = EXPECTED_TOKEN_ERR(parse_block, '=');

        err->err->s = from__String("=");

        emit__Diagnostic(err);
    } else
        next_token_pb(parse_block);

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

    if (parse_block->current->kind == TokenKindEof && !bad_token) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMissClosingBlock),
              *((struct Token *)get__Vec(*parse_block->scanner->tokens,
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
__free__RecordParseContext(struct RecordParseContext *self)
{
    FREE(Vec, self->generic_params);
    FREE(Vec, self->fields);
    free(self);
}

struct EnumObjectParseContext *
__new__EnumObjectParseContext()
{
    struct EnumObjectParseContext *self =
      malloc(sizeof(struct EnumObjectParseContext));
    self->is_pub = false;
    self->has_generic_params = false;
    self->has_data_type = false;
    self->is_error = false;
    self->name = NULL;
    self->data_type = NULL;
    self->generic_params = NULL;
    self->variants = NEW(Vec, sizeof(struct Token));
    return self;
}

static void
get_enum_object_parse_context(struct EnumObjectParseContext *self,
                              struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. DataType
    PARSE_ENUM_DATA_TYPE(self, parse_block);

    // 2. Variants
    bool bad_token = false;

    PARSE_ENUM_VARIANTS(self, parse_block, bad_token);
}

void
__free__EnumObjectParseContext(struct EnumObjectParseContext *self)
{
    FREE(Vec, self->data_type);
    FREE(Vec, self->generic_params);
    FREE(Vec, self->variants);
    free(self);
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
      DiagnosticWithErr, err, loc, *self->scanner->src->file, detail_msg, help);
}

static inline struct Diagnostic *
__new__DiagnosticWithWarnParser(struct ParseBlock *self,
                                struct LilyWarning *warn,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help)
{
    self->count_warning += 1;
    return NEW(DiagnosticWithWarn,
               warn,
               loc,
               *self->scanner->src->file,
               detail_msg,
               help);
}

static inline struct Diagnostic *
__new__DiagnosticWithNoteParser(struct ParseBlock *self,
                                struct String *note,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help)
{
    return NEW(DiagnosticWithNote,
               note,
               loc,
               *self->scanner->src->file,
               detail_msg,
               help);
}

struct ParseContext *
__new__ParseContextFun(struct FunParseContext *fun)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindFun;
    self->value.fun = fun;
    return self;
}

struct ParseContext *
__new__ParseContextEnum(struct EnumParseContext *enum_)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindEnum;
    self->value.enum_ = enum_;
    return self;
}

struct ParseContext *
__new__ParseContextRecord(struct RecordParseContext *record)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindRecord;
    self->value.record = record;
    return self;
}

struct ParseContext *
__new__ParseContextEnumObject(struct EnumObjectParseContext *enum_object)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindEnumObject;
    self->value.enum_object = enum_object;
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
__free__ParseContextEnumObject(struct ParseContext *self)
{
    FREE(EnumObjectParseContext, self->value.enum_object);
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
            FREE(ParseContextEnum, self);
            break;
        case ParseContextKindRecord:
            FREE(ParseContextRecord, self);
            break;
        case ParseContextKindEnumObject:
            FREE(ParseContextEnumObject, self);
            break;
        default:
            UNREACHABLE("unknown parse context kind");
    }
}

struct Parser *
__new__Parser(struct ParseBlock *parse_block)
{
    struct Parser *self = malloc(sizeof(struct Parser));
    self->parse_block = parse_block;
    return self;
}

#include <base/print.h>

void *
printHello(void *count)
{
    Println("Hello {d}", (UPtr)count);
    pthread_exit(NULL);
}

void
run__Parser(struct Parser *self)
{
    pthread_t threads[len__Vec(*self->parse_block->blocks)];
    int res;

    for (Usize i = 0; i < len__Vec(*self->parse_block->blocks); i++) {
        res = pthread_create(&threads[i], NULL, printHello, (void *)i);

        assert(res == 0 && "internal error in thread");
        pthread_join(threads[i], NULL);
        pthread_detach(threads[i]);
    }
}

void
printHelloWT(void *count)
{
    Println("Hello {d}", (UPtr)count);
}

void
run_without_multi_thread__Parser(struct Parser *self)
{
    for (Usize i = 0; i < len__Vec(*self->parse_block->blocks); i++) {
        printHelloWT((Usize *)i);
    }
}

void
__free__Parser(struct Parser *self)
{
    FREE(ParseBlock, self->parse_block);
    free(self);
}
