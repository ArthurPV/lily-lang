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
#include <base/util.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/parser/parser.h>
#include <string.h>

/*

  ____
 |  _ \    __ _   _ __   ___    ___   _ __
 | |_) |  / _` | | '__| / __|  / _ \ | '__|
 |  __/  | (_| | | |    \__ \ |  __/ | |
 |_|      \__,_| |_|    |___/  \___| |_|

The parser is the second major compilation phase in a traditional compiler. The
purpose of this step is to transform the set of tokens that was generated during
the scan into a node.

Ex:

                             fun add(x) = x;

                                    ↓

                    [fun, id(add), (, ), id(x), =, id(x), ;]

                                    ↓

        Function(name: add, Params([(name: x, data_type: NULL)]), Body([Id(x)]))
        ^ --> we are here

This parser goes through several sub-steps:

                           Analysis of the blocks

                                    ↓

                           The creation of nodes

                                    ↓

                                  Nodes

1. Analysis of the blocks:

During this step the parser will try to check if all the blocks (function,
module, record, class, enum...) are closed with the keyword end or the semicolon
token and to get as much information about these blocks as possible like
visibility, name, etc.

2. Creation of nodes:

This step will finish collecting all the information about the blocks like for
example parsing the body of a function. Then, it will transform the blocks into
a declaration (node).

3. Nodes:

This last step will just add the analyzer declarations into a vector (decls).

 */

#define EXPECTED_TOKEN_PB_ERR(parse_block, expected) \
    NEW(DiagnosticWithErrParser,                     \
        parse_block,                                 \
        NEW(LilyError, LilyErrorExpectedToken),      \
        *parse_block->current->loc,                  \
        format(""),                                  \
        Some(format("expected {s}, found `{Sr}`",    \
                    expected,                        \
                    token_kind_to_String__Token(*parse_block->current))));

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
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(self, "`:`");     \
                                                                         \
        err->err->s = from__String("`:`");                               \
                                                                         \
        emit__Diagnostic(err);                                           \
    } else if (self->current->kind == TokenKindColon && !is_object)      \
        next_token_pb(self);

#define EXPECTED_TOKEN_PB(self, token_kind, err) \
    if (self->current->kind != token_kind) {     \
        err;                                     \
    } else                                       \
        next_token_pb(self);

#define EXPECTED_TOKEN(self, token_kind, err)    \
    if (self->pos < len__Vec(*self->tokens)) {   \
        if (self->current->kind != token_kind) { \
            err;                                 \
        } else                                   \
            next_token(self);                    \
    }

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
            push__Vec(self->generic_params, parse_block->current);             \
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
            push__Vec(self->params, parse_block->current);      \
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
    while (parse_decl->current->kind != TokenKindRParen) { \
        block;                                             \
    }                                                      \
    next_token(parse_decl);

#define PARSE_HOOK(parse_decl, block)                     \
    while (parse_decl->current->kind != TokenKindRHook) { \
        block;                                            \
    }                                                     \
    next_token(parse_decl);

#define PARSE_BRACE(parse_decl, block)                     \
    while (parse_decl->current->kind != TokenKindRBrace) { \
        block;                                             \
    }                                                      \
    next_token(parse_decl);

#define PARSE_STMT(body)                                                       \
    struct Location loc = NEW(Location);                                       \
                                                                               \
    start__Location(&loc,                                                      \
                    parse_decl->previous->loc->s_line,                         \
                    parse_decl->previous->loc->s_col);                         \
                                                                               \
    switch (parse_decl->previous->kind) {                                      \
        case TokenKindReturnKw:                                                \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt, parse_return_stmt(self, parse_decl, loc))); \
            break;                                                             \
        case TokenKindAwaitKw:                                                 \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt, parse_await_stmt(self, parse_decl, loc)));  \
            break;                                                             \
        case TokenKindNextKw:                                                  \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt,                                             \
                  NEW(Stmt, StmtKindNext, *parse_decl->previous->loc)));       \
            break;                                                             \
        case TokenKindBreakKw:                                                 \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt,                                             \
                  NEW(Stmt, StmtKindBreak, *parse_decl->previous->loc)));      \
            break;                                                             \
        case TokenKindIfKw: {                                                  \
            struct IfCond *if_ = parse_if_stmt(self, parse_decl, &loc);        \
            push__Vec(body, NEW(FunBodyItemStmt, NEW(StmtIf, loc, if_)));      \
            break;                                                             \
        }                                                                      \
        case TokenKindForKw:                                                   \
            assert(0 && "todo");                                               \
        case TokenKindWhileKw:                                                 \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt, parse_while_stmt(self, parse_decl, loc)));  \
            break;                                                             \
        case TokenKindTryKw:                                                   \
            push__Vec(                                                         \
              body,                                                            \
              NEW(FunBodyItemStmt, parse_try_stmt(self, parse_decl, loc)));    \
            break;                                                             \
        case TokenKindMatchKw: {                                               \
            struct MatchStmt *match =                                          \
              parse_match_stmt(self, parse_decl, &loc);                        \
            push__Vec(body, NEW(FunBodyItemStmt, NEW(StmtMatch, loc, match))); \
            break;                                                             \
        }                                                                      \
        case TokenKindImportKw: {                                              \
            push__Vec(body, parse_import_stmt(self, parse_decl, loc));         \
            break;                                                             \
        }                                                                      \
        default:                                                               \
            UNREACHABLE("");                                                   \
    }

#define PARSE_BODY(body)                                                   \
    switch (parse_decl->current->kind) {                                   \
        case TokenKindReturnKw:                                            \
        case TokenKindAwaitKw:                                             \
        case TokenKindNextKw:                                              \
        case TokenKindBreakKw:                                             \
        case TokenKindIfKw:                                                \
        case TokenKindTryKw:                                               \
        case TokenKindForKw:                                               \
        case TokenKindWhileKw:                                             \
        case TokenKindMatchKw:                                             \
            next_token(parse_decl);                                        \
            PARSE_STMT(body);                                              \
                                                                           \
            break;                                                         \
                                                                           \
        default:                                                           \
            push__Vec(body,                                                \
                      NEW(FunBodyItemExpr, parse_expr(self, parse_decl))); \
            break;                                                         \
    }

#define VALID_TOKEN_FOR_DATA_TYPE \
    TokenKindEof:                 \
    case TokenKindLParen:         \
    case TokenKindRParen:         \
    case TokenKindLHook:          \
    case TokenKindRHook:          \
    case TokenKindComma:          \
    case TokenKindInterrogation:  \
    case TokenKindBang:           \
    case TokenKindDot:            \
    case TokenKindIdentifier:     \
    case TokenKindGlobalKw:       \
    case TokenKindMutKw:          \
    case TokenKindBar:            \
    case TokenKindArrow:          \
    case TokenKindStar

static Usize count_error = 0;
static Usize count_warning = 0;

static struct ParseContext *
get_block(struct ParseBlock *self, bool in_module);
static void
valid_name(struct ParseBlock *self,
           struct String *name,
           bool start_by_uppercase);
static inline void
next_token_pb(struct ParseBlock *self);
static inline void
skip_to_next_block(struct ParseBlock *self);
static struct String *
get_type_name(struct ParseBlock *self);
static struct ParseContext *
get_type_context(struct ParseBlock *self, bool is_pub);
static struct String *
get_object_name(struct ParseBlock *self);
static struct ParseContext *
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
static inline bool
valid_constant_data_type(struct ParseBlock *parse_block, bool already_invalid);
static inline bool
valid_constant_expr(struct ParseBlock *parse_block, bool already_invalid);
static void
get_constant_parse_context(struct ConstantParseContext *self,
                           struct ParseBlock *parse_block);
static void
get_error_parse_context(struct ErrorParseContext *self,
                        struct ParseBlock *parse_block);
static void
get_module_parse_context(struct ModuleParseContext *self,
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
parse_tags(struct Parser self, struct ParseDecl *parse_decl);
static struct Vec *
parse_generic_params(struct Parser self, struct ParseDecl *parse_decl);
static struct Expr *
parse_literal_expr(struct Parser self, struct ParseDecl *parse_decl);
static struct Expr *
parse_variable(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc,
               bool is_mut);
static struct Token *
peek_token(struct ParseDecl parse_decl, Usize n);
static void
skip_container(struct ParseDecl parse_decl, Usize *pos);
static bool
verify_if_has_comma(struct ParseDecl parse_decl, Usize add);
static struct Expr *
parse_expr_binary_op(struct Parser self,
                     struct ParseDecl *parse_decl,
                     struct Expr *left,
                     struct Location loc,
                     Usize prec);
static struct Expr *
parse_primary_expr(struct Parser self, struct ParseDecl *parse_decl);
static inline int *
parse_unary_op(enum TokenKind kind);
static inline int *
parse_binary_op(enum TokenKind kind);
static struct Expr *
parse_variant_expr(struct Parser self,
                   struct ParseDecl *parse_decl,
                   struct Expr *id,
                   struct Location loc);
static struct Expr *
parse_fun_call_expr(struct Parser self,
                    struct ParseDecl *parse_decl,
                    struct Expr *id,
                    struct Location loc);
static struct Expr *
parse_record_call_expr(struct Parser self,
                       struct ParseDecl *parse_decl,
                       struct Expr *id,
                       struct Location loc);
static struct Expr *
parse_array_access_expr(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Expr *id,
                        struct Location loc);
static struct Expr *
parse_tuple_access_expr(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Expr *id,
                        struct Location loc);
static struct Expr *
parse_identifier_access(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Location loc,
                        struct Vec *ids);
static struct Expr *
parse_expr(struct Parser self, struct ParseDecl *parse_decl);
static struct Stmt *
parse_return_stmt(struct Parser self,
                  struct ParseDecl *parse_decl,
                  struct Location loc);
static struct IfCond *
parse_if_stmt(struct Parser self,
              struct ParseDecl *parse_decl,
              struct Location *loc);
static struct Stmt *
parse_await_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location loc);
static struct Stmt *
parse_try_stmt(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc);
static struct MatchStmt *
parse_match_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location *loc);
static struct Stmt *
parse_while_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location loc);
static struct Stmt *
parse_for_stmt(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc);
static struct ImportStmt *
parse_import_value__parse_import_stmt(struct Parser self,
                                      struct String *buffer,
                                      struct String *as_value,
                                      struct Location buffer_loc,
                                      bool is_pub);
static struct String *
get_value__parse_import_stmt(struct String buffer, Usize *pos);
static struct String *
get_name__parse_import_stmt(struct Parser self,
                            struct String buffer,
                            struct Location buffer_loc,
                            Usize *pos);
static struct Vec *
get_value_in_selector__parse_import_stmt(struct Parser self,
                                         struct String buffer,
                                         struct Location buffer_loc,
                                         Usize *pos);
static struct Vec *
get_selector__parse_import_stmt(struct Parser self,
                                struct String buffer,
                                struct Location buffer_loc,
                                Usize *pos);
static void
next_char__parse_import_stmt(struct String buffer, char **current, Usize *pos);
static struct Stmt *
parse_import_stmt(struct Parser self,
                  struct ParseDecl *parse_decl,
                  struct Location loc);
static struct Vec *
parse_fun_body(struct Parser self, struct ParseDecl *parse_decl);
static struct Vec *
parse_fun_params(struct Parser self,
                 struct ParseDecl *parse_decl,
                 bool is_method);
static struct FunDecl *
parse_fun_declaration(struct Parser *self,
                      struct FunParseContext fun_parse_context);
static struct EnumDecl *
parse_enum_declaration(struct Parser *self,
                       struct EnumParseContext enum_parse_context,
                       bool is_object);
static struct RecordDecl *
parse_record_declaration(struct Parser *self,
                         struct RecordParseContext record_parse_context,
                         bool is_object);
static struct AliasDecl *
parse_alias_declaration(struct Parser *self,
                        struct AliasParseContext alias_parse_context);
static struct Vec *
parse_inheritance(struct Parser self, struct ParseDecl *parse_decl);
static struct TraitDecl *
parse_trait_declaration(struct Parser *self,
                        struct TraitParseContext trait_parse_context);
static struct Vec *
parse_impl(struct Parser self, struct ParseDecl *parse_decl);
static struct ClassDecl *
parse_class_declaration(struct Parser *self,
                        struct ClassParseContext class_parse_context);
static struct PropertyDecl *
parse_property_declaration(struct Parser *self,
                           struct ParseClassBody *parse_class);
static struct MethodDecl *
parse_method_declaration(struct Parser *self,
                         struct ParseClassBody *parse_class);
static struct ImportStmt *
parse_import_declaration(struct Parser *self,
                         struct ImportParseContext import_parse_context);
static struct ConstantDecl *
parse_constant_declaration(struct Parser *self,
                           struct ConstantParseContext constant_parse_context);
static struct ErrorDecl *
parse_error_declaration(struct Parser *self,
                        struct ErrorParseContext error_parse_context);
static struct ModuleDecl *
parse_module_declaration(struct Parser *self,
                         struct ModuleParseContext module_parse_context);
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
                                 (struct Token *)get__Vec(*scanner.tokens, 0),
                               .disable_warning = NEW(Vec, sizeof(Str)),
                               .pos = 0 };

    return self;
}

static struct ParseContext *
get_block(struct ParseBlock *self, bool in_module)
{
    struct Location loc = NEW(Location);

    start__Location(
      &loc, self->current->loc->s_line, self->current->loc->s_col);

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
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(ParseContextFun, fun_parse_context, loc);
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
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(ParseContextFun, fun_parse_context, loc);
                }

                case TokenKindTypeKw:
                    return get_type_context(self, true);

                case TokenKindObjectKw:
                    return get_object_context(self, true);

                case TokenKindTagKw:
                    TODO("");
                    break;

                case TokenKindErrorKw: {
                    struct ErrorParseContext error_parse_context =
                      NEW(ErrorParseContext);

                    error_parse_context.is_pub = true;

                    get_error_parse_context(&error_parse_context, self);
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(ParseContextError, error_parse_context, loc);
                }

                case TokenKindModuleKw: {
                    struct ModuleParseContext module_parse_context =
                      NEW(ModuleParseContext);

                    module_parse_context.is_pub = true;

                    get_module_parse_context(&module_parse_context, self);
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(ParseContextModule, module_parse_context, loc);
                }

                case TokenKindImportKw: {
                    struct ImportParseContext import_parse_context =
                      NEW(ImportParseContext);

                    import_parse_context.is_pub = true;

                    get_import_parse_context(&import_parse_context, self);
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(ParseContextImport, import_parse_context, loc);
                }

                case TokenKindIdentifier: {
                    struct ConstantParseContext constant_parse_context =
                      NEW(ConstantParseContext);

                    valid_name(self, self->current->lit, true);

                    constant_parse_context.name = self->current->lit;

                    get_constant_parse_context(&constant_parse_context, self);
                    end__Location(&loc,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_line,
                                  ((struct Token *)get__Vec(
                                     *self->scanner.tokens, self->pos - 1))
                                    ->loc->s_col);

                    return NEW(
                      ParseContextConstant, constant_parse_context, loc);
                }

                default: {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          self,
                          NEW(LilyError, LilyErrorBadUsageOfPub),
                          *self->current->loc,
                          format("expected `fun`, `async`, `type`, tag`, "
                                 "`error`, `import`, `ID` or "
                                 "`object` after `pub` declaration"),
                          None());

                    emit__Diagnostic(err);
                    skip_to_next_block(self);

                    return NULL;
                }
            }

        case TokenKindFunKw: {
            struct FunParseContext fun_parse_context = NEW(FunParseContext);

            next_token_pb(self);
            get_fun_parse_context(&fun_parse_context, self);
            end__Location(
              &loc,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_line,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_col);

            return NEW(ParseContextFun, fun_parse_context, loc);
        }
        case TokenKindTagKw:
            break;

        case TokenKindTypeKw:
            return get_type_context(self, false);

        case TokenKindObjectKw:
            return get_object_context(self, false);

        case TokenKindErrorKw: {
            struct ErrorParseContext error_parse_context =
              NEW(ErrorParseContext);

            get_error_parse_context(&error_parse_context, self);
            end__Location(
              &loc,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_line,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_col);

            return NEW(ParseContextError, error_parse_context, loc);
        }

        case TokenKindModuleKw: {
            struct ModuleParseContext module_parse_context =
              NEW(ModuleParseContext);

            get_module_parse_context(&module_parse_context, self);
            end__Location(
              &loc,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_line,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_col);

            return NEW(ParseContextModule, module_parse_context, loc);
        }

        case TokenKindBang: {
            next_token_pb(self);

            if (in_module) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      self,
                      NEW(LilyError, LilyErrorInvalidAttributeInThisBlock),
                      *self->current->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
            }

            if (self->current->kind != TokenKindLHook) {
                EXPECTED_TOKEN_PB(self, TokenKindLHook, {
                    struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(self, "`[`");

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
                if (self->current->kind == TokenKindStringLit && !in_module) {
                    push__Vec(self->disable_warning, self->current->lit);
                    next_token_pb(self);
                } else if (self->current->kind != TokenKindStringLit) {
                    EXPECTED_TOKEN_PB(self, TokenKindStringLit, {
                        struct Diagnostic *err =
                          EXPECTED_TOKEN_PB_ERR(self, "string literal");

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
            return NULL;
        }
        }

        case TokenKindIdentifier: {
            struct ConstantParseContext constant_parse_context =
              NEW(ConstantParseContext);

            valid_name(self, self->current->lit, true);

            constant_parse_context.name = self->current->lit;

            get_constant_parse_context(&constant_parse_context, self);
            end__Location(
              &loc,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_line,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->s_col);

            return NEW(ParseContextConstant, constant_parse_context, loc);
        }

        case TokenKindImportKw: {
            struct ImportParseContext import_parse_context =
              NEW(ImportParseContext);

            get_import_parse_context(&import_parse_context, self);
            end__Location(
              &loc,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->e_line,
              ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
                ->loc->e_col);

            return NEW(ParseContextImport, import_parse_context, loc);
        }

        default: {
            struct Diagnostic *err = NEW(
              DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorUnexpectedTokenForBeginingInGlobal),
              *self->current->loc,
              from__String("expected `pub`, `fun`, `type`, `object`, `tag`, "
                           "`import`, `ID`, `error`, `!`"),
              None());

            err->err->s = token_kind_to_String__Token(*self->current);

            emit__Diagnostic(err);
            skip_to_next_block(self);

            return NULL;
        }
    }

    return NULL;
}

static void
valid_name(struct ParseBlock *self,
           struct String *name,
           bool start_by_uppercase)
{
    char *c = get__String(*name, 0);
    bool is_uppercase = c >= (char *)'A' && c <= (char *)'Z';

    if (is_uppercase && !start_by_uppercase) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorNameMustStartByLowercaseCharacter),
              *self->current->loc,
              from__String(""),
              None());

        emit__Diagnostic(err);
    } else if (!is_uppercase && start_by_uppercase) {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              self,
              NEW(LilyError, LilyErrorNameMustStartByUppercaseCharacter),
              *self->current->loc,
              from__String(""),
              None());

        emit__Diagnostic(err);
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
        struct ParseContext *block = get_block(self, false);

        if (block)
            push__Vec(self->blocks, block);
    }

    if (count_error > 0) {
        emit__Summary(
          count_error, count_warning, "the parse block phase has been failed");
        exit(1);
    }
}

static inline void
next_token_pb(struct ParseBlock *self)
{
    if (self->current->kind != TokenKindEof) {
        self->pos += 1;
        self->current =
          (struct Token *)get__Vec(*self->scanner.tokens, self->pos);
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
                          token_kind_to_String__Token(*self->current))));

        emit__Diagnostic(err);

        return NULL;
    }

    valid_name(self, self->current->lit, true);

    return self->current->lit;
}

static struct ParseContext *
get_type_context(struct ParseBlock *self, bool is_pub)
{
    struct String *name = get_type_name(self);
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));
    struct Location loc = NEW(Location);
    bool has_generic_params = false;
    bool is_object = false;

    start__Location(
      &loc, self->current->loc->s_line, self->current->loc->s_col);

    PARSE_GENERIC_TYPE_AND_OBJECT(self);

    switch (self->current->kind) {
        case TokenKindEnumKw: {
            struct EnumParseContext enum_parse_context = NEW(EnumParseContext);

            enum_parse_context.name = name;
            enum_parse_context.generic_params = generic_params;
            enum_parse_context.is_pub = is_pub;
            enum_parse_context.has_generic_params = has_generic_params;

            get_enum_parse_context(&enum_parse_context, self);
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextEnum, enum_parse_context, loc, false);
        }

        case TokenKindRecordKw: {
            struct RecordParseContext record_parse_context =
              NEW(RecordParseContext);

            record_parse_context.name = name;
            record_parse_context.generic_params = generic_params;
            record_parse_context.is_pub = is_pub;
            record_parse_context.has_generic_params = has_generic_params;

            get_record_parse_context(&record_parse_context, self);
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextRecord, record_parse_context, loc, false);
        }

        case TokenKindAliasKw: {
            struct AliasParseContext alias_parse_context =
              NEW(AliasParseContext);

            alias_parse_context.name = name;
            alias_parse_context.generic_params = generic_params;
            alias_parse_context.is_pub = is_pub;
            alias_parse_context.has_generic_params = has_generic_params;

            get_alias_parse_context(&alias_parse_context, self);
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextAlias, alias_parse_context, loc);
        }

        default: {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  self,
                  NEW(LilyError, LilyErrorBadUsageOfType),
                  *self->current->loc,
                  format(""),
                  Some(format("expected `enum` or `record`, found `{Sr}`",
                              token_kind_to_String__Token(*self->current))));

            emit__Diagnostic(err);
            skip_to_next_block(self);

            FREE(Vec, generic_params);

            return NULL;
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
                          token_kind_to_String__Token(*self->current))));

        emit__Diagnostic(err);

        return NULL;
    }

    valid_name(self, self->current->lit, true);

    return self->current->lit;
}

static struct ParseContext *
get_object_context(struct ParseBlock *self, bool is_pub)
{
    struct String *name = get_object_name(self);
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));
    struct Vec *impl = NEW(Vec, sizeof(struct Token));
    struct Vec *inh = NEW(Vec, sizeof(struct Token));
    struct Location loc = NEW(Location);
    bool has_generic_params = false;
    bool is_object = true;

    start__Location(
      &loc, self->current->loc->s_line, self->current->loc->s_col);

    PARSE_GENERIC_TYPE_AND_OBJECT(self);

    struct Location loc_impl = NEW(Location);

    start__Location(
      &loc_impl, self->current->loc->s_line, self->current->loc->s_col);

    if (self->current->kind == TokenKindImplKw) {
        next_token_pb(self);

        while (self->current->kind != TokenKindFatArrow &&
               self->current->kind != TokenKindColon &&
               self->current->kind != TokenKindEof) {
            push__Vec(impl, self->current);
            next_token_pb(self);
        }
    }

    end__Location(
      &loc_impl,
      ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
        ->loc->s_line,
      ((struct Token *)get__Vec(*self->scanner.tokens, self->pos - 1))
        ->loc->s_col);

    struct Location loc_inh = NEW(Location);

    start__Location(
      &loc_inh, self->current->loc->s_line, self->current->loc->s_col);

    if (self->current->kind == TokenKindFatArrow) {
        next_token_pb(self);

        EXPECTED_TOKEN_PB(self, TokenKindLHook, {
            struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(self, "`[`");

            err->err->s = from__String("`[`");

            emit__Diagnostic(err);
        });

        while (self->current->kind != TokenKindRHook &&
               self->current->kind != TokenKindEof) {
            push__Vec(inh, self->current);
            next_token_pb(self);
        }

        end__Location(
          &loc_inh, self->current->loc->s_line, self->current->loc->s_col);

        next_token_pb(self);
    }

    EXPECTED_TOKEN_PB(self, TokenKindColon, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(self, "`:`");

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
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextEnum, enum_parse_context, loc, true);
        }

        case TokenKindRecordKw: {
            struct RecordParseContext record_parse_context =
              NEW(RecordParseContext);

            record_parse_context.name = name;
            record_parse_context.generic_params = generic_params;
            record_parse_context.is_pub = is_pub;
            record_parse_context.has_generic_params = has_generic_params;

            get_record_parse_context(&record_parse_context, self);
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextRecord, record_parse_context, loc, true);
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
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextTrait, trait_parse_context, loc);
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
            end__Location(
              &loc, self->current->loc->s_line, self->current->loc->s_col);

            return NEW(ParseContextClass, class_parse_context, loc);
        }

        default: {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  self,
                  NEW(LilyError, LilyErrorBadUsageOfObject),
                  loc_inh,
                  format(""),
                  Some(format(
                    "expected `class`, `trait, `enum` or `record`, found {Sr}",
                    token_kind_to_String__Token(*self->current))));

            emit__Diagnostic(err);
            skip_to_next_block(self);

            FREE(Vec, impl);
            FREE(Vec, inh);
            FREE(Vec, generic_params);

            return NULL;
        }
    }
}

static struct Vec *
get_generic_params(struct ParseBlock *self)
{
    struct Vec *generic_params = NEW(Vec, sizeof(struct Token));

    if (self->current->kind == TokenKindLHook) {
        next_token_pb(self);

        while (self->current->kind != TokenKindRHook) {
            push__Vec(generic_params, self->current);
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
                                    .has_return_type = false,
                                    .is_operator = false,
                                    .name = NULL,
                                    .tags = NEW(Vec, sizeof(struct Token)),
                                    .generic_params =
                                      NEW(Vec, sizeof(struct Token)),
                                    .params = NEW(Vec, sizeof(struct Token)),
                                    .return_type =
                                      NEW(Vec, sizeof(struct Token)),
                                    .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_body_item(struct ParseBlock *parse_block,
                bool already_invalid,
                bool is_fun)
{
    if (!is_fun && (parse_block->current->kind == TokenKindSelfKw ||
                    parse_block->current->kind == TokenKindPubKw))
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
                  parse_block->current);                     \
    else                                                     \
        push__Vec(((struct MethodParseContext *)self)->body, \
                  parse_block->current);

static void
verify_stmt(void *self, struct ParseBlock *parse_block, bool is_fun)
{
    if (parse_block->current->kind == TokenKindDoKw ||
        parse_block->current->kind == TokenKindBeginKw) {
        Usize start_line = parse_block->current->loc->s_line;
        bool bad_item = false;

        PUSH_BODY();

        next_token_pb(parse_block);

        while (parse_block->current->kind != TokenKindEndKw &&
               parse_block->current->kind != TokenKindSemicolon &&
               parse_block->current->kind != TokenKindEof) {
            if (valid_body_item(parse_block, bad_item, is_fun)) {
                if (parse_block->current->kind == TokenKindElifKw ||
                    parse_block->current->kind ==
                      TokenKindCatchKw) { // Special case with catch and elif to
                                          // avoid unintended errors
                    while (parse_block->current->kind != TokenKindDoKw &&
                           parse_block->current->kind != TokenKindEof) {
                        PUSH_BODY();
                        next_token_pb(parse_block);
                    }

                    if (parse_block->current->kind == TokenKindEof) {
                        VERIFY_EOF(parse_block, false, "`do`");
                        break;
                    }

                    PUSH_BODY();
                    next_token_pb(parse_block);
                }

                if (parse_block->current->kind == TokenKindDoKw ||
                    parse_block->current->kind == TokenKindBeginKw)
                    verify_stmt(self, parse_block, is_fun);

                PUSH_BODY();

                next_token_pb(parse_block);
            } else {
                bad_item = true;
                next_token_pb(parse_block);
            }
        }

        PUSH_BODY();

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
                case TokenKindBeginKw:
                case TokenKindDoKw:
                    verify_stmt(self, parse_block, is_fun);
                    break;
                default:
                    if (is_fun)
                        push__Vec(((struct FunParseContext *)self)->body,
                                  parse_block->current);
                    else
                        push__Vec(((struct MethodParseContext *)self)->body,
                                  parse_block->current);

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
                          token_kind_to_String__Token(*parse_block->current))));

            err->err->s = token_kind_to_String__Token(*parse_block->current);

            emit__Diagnostic(err);
        }

        if (self->has_tags) {
            struct Location loc_warn = NEW(Location);

            start__Location(&loc_warn,
                            parse_block->current->loc->s_line,
                            parse_block->current->loc->s_col);

            next_token_pb(parse_block);

            // #(Example[T], ...)
            while (parse_block->current->kind != TokenKindRParen) {
                push__Vec(self->tags, parse_block->current);
                next_token_pb(parse_block);
            }

            if (parse_block->current->kind == TokenKindRParen &&
                len__Vec(*self->tags) == 0) {
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
            }

            if (len__Vec(*self->tags) == 0)
                self->has_tags = false;

            next_token_pb(parse_block);
        } else {
            push__Vec(self->tags, parse_block->current);
            next_token_pb(parse_block);
        }
    }

    // 2. Get name of function.
    if (parse_block->current->kind == TokenKindIdentifier) {
        valid_name(parse_block, parse_block->current->lit, false);

        self->name = parse_block->current->lit;
    } else if (parse_block->current->kind == TokenKindIdentifierOp) {
        valid_name(parse_block, parse_block->current->lit, false);

        self->name = parse_block->current->lit;
        self->is_operator = true;
    } else {
        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              parse_block,
              NEW(LilyError, LilyErrorMissFunctionName),
              *parse_block->current->loc,
              format(""),
              Some(format("add function's name, found: `{Sr}`",
                          token_kind_to_String__Token(*parse_block->current))));

        emit__Diagnostic(err);
    }

    next_token_pb(parse_block);

    // 3. Get generic params.
    PARSE_GENERIC_PARAMS(self);

    // 4. Get params.
    PARSE_PARAMS(self);

    // 5. Get return type
    while (parse_block->current->kind != TokenKindEq &&
           parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        push__Vec(self->return_type, parse_block->current);
        next_token_pb(parse_block);
    }

    if (len__Vec(*self->return_type) > 0)
        self->has_return_type = true;

    if (parse_block->current->kind == TokenKindEndKw ||
        parse_block->current->kind == TokenKindSemicolon ||
        parse_block->current->kind == TokenKindEof) {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorUnexpectedToken),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        switch (parse_block->current->kind) {
            case TokenKindEndKw:
                err->err->s = from__String("end");
                break;

            case TokenKindSemicolon:
                err->err->s = from__String(";");
                break;

            case TokenKindEof:
                err->err->s = from__String("Eof");
                break;

            default:
                break;
        }

        emit__Diagnostic(err);
    }

    // 6. Get body.
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

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
    FREE(Vec, self.return_type);
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

            EXPECTED_TOKEN_PB(parse_block, TokenKindRParen, {
                struct Diagnostic *err =
                  EXPECTED_TOKEN_PB_ERR(parse_block, "`)`");

                err->err->s = from__String("`)`");

                emit__Diagnostic(err);
            });

            self->is_error = true;
        } else {
            while (parse_block->current->kind != TokenKindRParen) {
                push__Vec(self->data_type, parse_block->current);
                next_token_pb(parse_block);
            }

            self->has_data_type = true;

            next_token_pb(parse_block);
        }
    }

    // 2. Variants
    bool bad_token = false;

    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_enum_variants(parse_block, bad_token)) {
            push__Vec(self->variants, parse_block->current);
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
        case VALID_TOKEN_FOR_DATA_TYPE:
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
        case VALID_TOKEN_FOR_DATA_TYPE:
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
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_record_fields(parse_block, bad_token)) {
            push__Vec(self->fields, parse_block->current);
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
        case VALID_TOKEN_FOR_DATA_TYPE:
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
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    })

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_alias_data_type(parse_block, bad_token)) {
            push__Vec(self->data_type, parse_block->current);
            next_token_pb(parse_block);
        } else {
            bad_token = true;
            next_token_pb(parse_block);
        }
    }

    if (len__Vec(*self->data_type) == 0) {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorMissDataType),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        emit__Diagnostic(err);
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
        case VALID_TOKEN_FOR_DATA_TYPE:
        case TokenKindAt:
        case TokenKindSelfKw:
        case TokenKindColonColon:
            return true;

        default: {
            if (!already_invalid) {
                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorInvalidTokenInRecordField),
                  *parse_block->current->loc,
                  format("this token is invalid inside the trait, expected: "
                         "`(`, `)`, `[`, `]`, `ID`, `?`, `!`, `,`, `.`, `->`, "
                         "`@` or `::`"),
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
get_trait_parse_context(struct TraitParseContext *self,
                        struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    // 1. Body
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_token_in_trait_body(parse_block, bad_token)) {
            push__Vec(self->body, parse_block->current);
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
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

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
        struct Location loc_item = NEW(Location);

        start__Location(&loc_item,
                        parse_block->current->loc->s_line,
                        parse_block->current->loc->s_col);

        if (valid_class_token_in_body(parse_block, bad_token)) {
            // push__Vec(self->body, parse_block->current);
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
                          token_kind_to_String__Token(*parse_block->current))));

            emit__Diagnostic(err);
        }

        struct String *name = parse_block->current->lit;

        valid_name(parse_block, name, false);
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
                  EXPECTED_TOKEN_PB_ERR(parse_block, "`(`, `[` or `=`");

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
        end__Location(&loc_item,
                      ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                                parse_block->pos - 1))
                        ->loc->s_line,
                      ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                                parse_block->pos - 1))
                        ->loc->s_col);
        push__Vec(self->body,
                  NEW(ParseContextMethod, method_parse_context, loc_item));

        goto exit;
    }

        import : {
            struct ImportParseContext impor_parse_context =
              NEW(ImportParseContext);

        get_import_parse_context(&impor_parse_context, parse_block);
        end__Location(&loc_item,
                      ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                                parse_block->pos - 1))
                        ->loc->s_line,
                      ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                                parse_block->pos - 1))
                        ->loc->s_col);
        push__Vec(self->body,
                  NEW(ParseContextImport, impor_parse_context, loc_item));

        goto exit;
    }

property : {
    get_property_parse_context(&property_parse_context, parse_block);
    end__Location(&loc_item,
                  ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                            parse_block->pos - 1))
                    ->loc->s_line,
                  ((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                            parse_block->pos - 1))
                    ->loc->s_col);
    push__Vec(self->body,
              NEW(ParseContextProperty, property_parse_context, loc_item));

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
    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

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
                                       .has_return_type = false,
                                       .name = NULL,
                                       .generic_params =
                                         NEW(Vec, sizeof(struct Token)),
                                       .params = NEW(Vec, sizeof(struct Token)),
                                       .return_type =
                                         NEW(Vec, sizeof(struct Token)),
                                       .body = NEW(Vec, sizeof(struct Token)) };

    return self;
}

static void
get_method_parse_context(struct MethodParseContext *self,
                         struct ParseBlock *parse_block)
{
    PARSE_GENERIC_PARAMS(self);
    PARSE_PARAMS(self);

    if (parse_block->current->kind != TokenKindEq) {
        self->has_return_type = true;

        while (parse_block->current->kind != TokenKindEq &&
               parse_block->current->kind != TokenKindEof) {
            push__Vec(self->return_type, parse_block->current);
            next_token_pb(parse_block);
        }
    }

    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = EXPECTED_TOKEN_PB_ERR(parse_block, "`=`");

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
    FREE(Vec, self.return_type);
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
        push__Vec(self->data_type, parse_block->current);
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
        self->value = parse_block->current->lit;
        self->value_loc = *parse_block->current->loc;
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
            self->as_value = parse_block->current->lit;
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

struct ConstantParseContext
__new__ConstantParseContext()
{
    struct ConstantParseContext self = { .is_pub = false,
                                         .name = NULL,
                                         .data_type =
                                           NEW(Vec, sizeof(struct Token)),
                                         .expr =
                                           NEW(Vec, sizeof(struct Token)) };

    return self;
}

static inline bool
valid_constant_data_type(struct ParseBlock *parse_block, bool already_invalid)
{
    switch (parse_block->current->kind) {
        case VALID_TOKEN_FOR_DATA_TYPE:
            return true;

        default: {
            if (!already_invalid) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_block->current->loc,
                      format("invalid token in constant data type"),
                      None());

                err->err->s = from__String("`:=`");

                emit__Diagnostic(err);
            }
            return false;
        }
    }
}

static inline bool
valid_constant_expr(struct ParseBlock *parse_block, bool already_invalid)
{
    switch (parse_block->current->kind) {
        case TokenKindObjectKw:
        case TokenKindFunKw:
        case TokenKindPubKw:
        case TokenKindErrorKw:
        case TokenKindTagKw:
        case TokenKindModuleKw:
        case TokenKindMacroKw:
        case TokenKindAsyncKw:
        case TokenKindClassKw:
        case TokenKindRecordKw:
        case TokenKindTypeKw:
        case TokenKindEnumKw:
        case TokenKindAliasKw:
        case TokenKindImplKw:
        case TokenKindTraitKw:
        case TokenKindReturnKw: {
            if (!already_invalid) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_block->current->loc,
                      format("invalid token in constant expression"),
                      None());

                err->err->s = from__String("`;`");

                emit__Diagnostic(err);
            }
            return false;
        }

        default:
            return true;
    }
}

static void
get_constant_parse_context(struct ConstantParseContext *self,
                           struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    if (parse_block->current->kind == TokenKindColonColon) {
        next_token_pb(parse_block);

        bool bad_token = false;

        while (parse_block->current->kind != TokenKindColonEq &&
               parse_block->current->kind != TokenKindEof) {
            if (valid_constant_data_type(parse_block, bad_token)) {
                push__Vec(self->data_type, parse_block->current);
                next_token_pb(parse_block);
            } else {
                bad_token = true;
                next_token_pb(parse_block);
            }
        }

        if (parse_block->current->kind == TokenKindEof) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  parse_block,
                  NEW(LilyError, LilyErrorExpectedToken),
                  *((struct Token *)get__Vec(*parse_block->scanner.tokens,
                                             parse_block->pos - 1))
                     ->loc,
                  format(""),
                  None());

            err->err->s = from__String("`:=`");

            emit__Diagnostic(err);
        } else
            next_token_pb(parse_block);
    } else
        next_token_pb(parse_block);

    bool bad_token = false;

    while (parse_block->current->kind != TokenKindSemicolon &&
           parse_block->current->kind != TokenKindEof) {
        if (valid_constant_expr(parse_block, bad_token)) {
            push__Vec(self->expr, parse_block->current);
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
__free__ConstantParseContext(struct ConstantParseContext self)
{
    FREE(Vec, self.data_type);
    FREE(Vec, self.expr);
}

struct ErrorParseContext
__new__ErrorParseContext()
{
    struct ErrorParseContext self = { .is_pub = false,
                                      .has_generic_params = false,
                                      .has_data_type = false,
                                      .name = NULL,
                                      .generic_params =
                                        NEW(Vec, sizeof(struct Token)),
                                      .data_type =
                                        NEW(Vec, sizeof(struct Token)) };

    return self;
}

static void
get_error_parse_context(struct ErrorParseContext *self,
                        struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    if (parse_block->current->kind == TokenKindLHook) {
        self->has_generic_params = true;

        next_token_pb(parse_block);

        while (parse_block->current->kind != TokenKindRHook) {
            push__Vec(self->generic_params, parse_block->current);
            next_token_pb(parse_block);
        }

        next_token_pb(parse_block);
    }

    if (parse_block->current->kind != TokenKindIdentifier) {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorMissErrorName),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        emit__Diagnostic(err);
    } else {
        valid_name(parse_block, parse_block->current->lit, true);

        self->name = parse_block->current->lit;
    }

    next_token_pb(parse_block);

    if (parse_block->current->kind != TokenKindSemicolon) {
        self->has_data_type = true;

        bool bad_token = false;

        while (parse_block->current->kind != TokenKindSemicolon &&
               parse_block->current->kind != TokenKindEof) {
            if (valid_token_in_enum_variants(
                  parse_block, bad_token)) { // change this function call
                push__Vec(self->data_type, parse_block->current);
                next_token_pb(parse_block);
            } else {
                bad_token = true;
                next_token_pb(parse_block);
            }
        }
    }

    EXPECTED_TOKEN_PB(parse_block, TokenKindSemicolon, {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorMissClosingBlock),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        err->err->s = from__String("`;`");

        emit__Diagnostic(err);
    });
}

void
__free__ErrorParseContext(struct ErrorParseContext self)
{
    FREE(Vec, self.generic_params);
    FREE(Vec, self.data_type);
}

struct ModuleParseContext
__new__ModuleParseContext()
{
    struct ModuleParseContext self = { .is_pub = false,
                                       .name = NULL,
                                       .body = NEW(
                                         Vec, sizeof(struct ParseContext)) };

    return self;
}

static void
get_module_parse_context(struct ModuleParseContext *self,
                         struct ParseBlock *parse_block)
{
    next_token_pb(parse_block);

    if (parse_block->current->kind != TokenKindIdentifier) {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorMissModuleName),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        emit__Diagnostic(err);
    } else {
        valid_name(parse_block, parse_block->current->lit, true);

        self->name = parse_block->current->lit;

        next_token_pb(parse_block);
    }

    EXPECTED_TOKEN_PB(parse_block, TokenKindEq, {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     parse_block,
                                     NEW(LilyError, LilyErrorExpectedToken),
                                     *parse_block->current->loc,
                                     format(""),
                                     None());

        err->err->s = from__String("`=`");

        emit__Diagnostic(err);
    });

    while (parse_block->current->kind != TokenKindEndKw &&
           parse_block->current->kind != TokenKindEof) {
        struct ParseContext *block = get_block(parse_block, true);

        if (block)
            push__Vec(self->body, block);
    }

    VERIFY_EOF(parse_block, false, "`end`");
}

void
__free__ModuleParseContext(struct ModuleParseContext self)
{
    for (Usize i = 0; i < len__Vec(*self.body); i++)
        FREE(ParseContextAll, get__Vec(*self.body, i));

    FREE(Vec, self.body);
}

static inline struct Diagnostic *
__new__DiagnosticWithErrParser(struct ParseBlock *self,
                               struct LilyError *err,
                               struct Location loc,
                               struct String *detail_msg,
                               struct Option *help)
{
    count_error += 1;
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
    count_warning += 1;
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
__new__ParseContextFun(struct FunParseContext fun, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindFun;
    self->loc = loc;
    self->value.fun = fun;
    return self;
}

struct ParseContext *
__new__ParseContextEnum(struct EnumParseContext enum_,
                        struct Location loc,
                        bool is_object)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));

    if (is_object)
        self->kind = ParseContextKindEnumObject;
    else
        self->kind = ParseContextKindEnum;

    self->loc = loc;

    self->value.enum_ = enum_;
    return self;
}

struct ParseContext *
__new__ParseContextRecord(struct RecordParseContext record,
                          struct Location loc,
                          bool is_object)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));

    if (is_object)
        self->kind = ParseContextKindRecordObject;
    else
        self->kind = ParseContextKindRecord;

    self->loc = loc;

    self->value.record = record;
    return self;
}

struct ParseContext *
__new__ParseContextAlias(struct AliasParseContext alias, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindAlias;
    self->loc = loc;
    self->value.alias = alias;
    return self;
}

struct ParseContext *
__new__ParseContextTrait(struct TraitParseContext trait, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindTrait;
    self->loc = loc;
    self->value.trait = trait;
    return self;
}

struct ParseContext *
__new__ParseContextClass(struct ClassParseContext class, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindClass;
    self->loc = loc;
    self->value.class = class;
    return self;
}

struct ParseContext *
__new__ParseContextMethod(struct MethodParseContext method, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindMethod;
    self->loc = loc;
    self->value.method = method;
    return self;
}

struct ParseContext *
__new__ParseContextProperty(struct PropertyParseContext property,
                            struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindProperty;
    self->loc = loc;
    self->value.property = property;
    return self;
}

struct ParseContext *
__new__ParseContextImport(struct ImportParseContext import, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindImport;
    self->loc = loc;
    self->value.import = import;
    return self;
}

struct ParseContext *
__new__ParseContextConstant(struct ConstantParseContext constant,
                            struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindConstant;
    self->loc = loc;
    self->value.constant = constant;
    return self;
}

struct ParseContext *
__new__ParseContextError(struct ErrorParseContext error, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindError;
    self->loc = loc;
    self->value.error = error;
    return self;
}

struct ParseContext *
__new__ParseContextModule(struct ModuleParseContext module, struct Location loc)
{
    struct ParseContext *self = malloc(sizeof(struct ParseContext));
    self->kind = ParseContextKindModule;
    self->loc = loc;
    self->value.module = module;
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
__free__ParseContextConstant(struct ParseContext *self)
{
    FREE(ConstantParseContext, self->value.constant);
    free(self);
}

void
__free__ParseContextModule(struct ParseContext *self)
{
    FREE(ModuleParseContext, self->value.module);
    free(self);
}

void
__free__ParseContextError(struct ParseContext *self)
{
    FREE(ErrorParseContext, self->value.error);
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

        case ParseContextKindConstant:
            FREE(ParseContextConstant, self);
            break;

        case ParseContextKindError:
            FREE(ParseContextError, self);
            break;

        case ParseContextKindModule:
            FREE(ParseContextModule, self);
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
          ((struct ParseContext *)get__Vec(*parse_block.blocks, 0));
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

struct ParseClassBody
__new__ParseClassBody(struct Vec *blocks)
{
    struct ParseClassBody self = { .pos = 0,
                                   .current = len__Vec(*blocks) == 0
                                                ? NULL
                                                : get__Vec(*blocks, 0),
                                   .blocks = blocks };

    return self;
}

static inline void
next_token(struct ParseDecl *self)
{
    self->pos += 1;

    if (self->pos < len__Vec(*self->tokens)) {
        self->current = get__Vec(*self->tokens, self->pos);
        self->previous = get__Vec(*self->tokens, self->pos - 1);
    } else
        self->previous = self->current;
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
        case TokenKindStar:
        case TokenKindSelfKw:
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
        case TokenKindSelfKw:
            return NEW(DataType, DataTypeKindSelf);
        case TokenKindMutKw:
            return NEW(DataTypeMut, parse_data_type(self, parse_decl));
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

            else if (!strcmp(identifier_str, "BitChar"))
                data_type = NEW(DataType, DataTypeKindBitChar);

            else if (!strcmp(identifier_str, "Str"))
                data_type = NEW(DataType, DataTypeKindStr);

            else if (!strcmp(identifier_str, "BitStr"))
                data_type = NEW(DataType, DataTypeKindBitStr);

            else if (!strcmp(identifier_str, "Any"))
                data_type = NEW(DataType, DataTypeKindAny);

            else if (!strcmp(identifier_str, "Never"))
                data_type = NEW(DataType, DataTypeKindNever);

            else if (!strcmp(identifier_str, "Unit"))
                data_type = NEW(DataType, DataTypeKindUnit);

            else {
                struct Vec *names = NEW(Vec, sizeof(struct String));

                push__Vec(names, parse_decl->previous->lit);

                while (parse_decl->current->kind == TokenKindDot) {
                    next_token(parse_decl);

                    if (parse_decl->current->kind == TokenKindIdentifier)
                        push__Vec(names, parse_decl->current->lit);
                    else
                        assert(0 && "error");

                    next_token(parse_decl);
                }

                if (parse_decl->current->kind == TokenKindLHook) {
                    struct Vec *data_types = NEW(Vec, sizeof(struct DataType));

                    next_token(parse_decl);

                    PARSE_HOOK(parse_decl, {
                        push__Vec(data_types,
                                  parse_data_type(self, parse_decl));

                        if (parse_decl->current->kind != TokenKindRHook) {
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
                        }
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

                    data_type = NEW(DataTypeCustom, names, data_types);
                } else
                    data_type = NEW(DataTypeCustom, names, NULL);
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
                          token_kind_to_String__Token(*parse_decl->current);

                        emit__Diagnostic(err);

                        break;
                    }
                }
            }

            if (is_wildcard || size) {
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
            } else
                next_token(parse_decl);

            if (is_data_type(parse_decl)) {
                if (!size)
                    data_type = NEW(
                      DataTypeArray, parse_data_type(self, parse_decl), NULL);
                else
                    data_type = NEW(
                      DataTypeArray, parse_data_type(self, parse_decl), NULL);
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

                if (parse_decl->current->kind != TokenKindRParen) {
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
                }
            });

            data_type = NEW(DataTypeTuple, data_types);

            break;
        }

        case TokenKindStar:
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

            while (parse_decl->current->kind != TokenKindBar) {
                push__Vec(params, parse_data_type(self, parse_decl));

                if (parse_decl->current->kind == TokenKindArrow)
                    next_token(parse_decl);
                else if (parse_decl->current->kind == TokenKindBar) {
                    next_token(parse_decl);

                    break;
                } else {
                    assert(0 && "error");
                }
            }

            struct DataType *return_type = pop__Vec(params);

            if (len__Vec(*params) == 0) {
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

static struct Vec *
parse_tags(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Vec *tags = NEW(Vec, sizeof(struct Tuple));

    while (parse_decl->pos < len__Vec(*parse_decl->tokens)) {
        struct Location loc = NEW(Location);

        start__Location(&loc,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        switch (parse_decl->current->kind) {
            case TokenKindIdentifier: {
                struct DataType *data_type = parse_data_type(self, parse_decl);

                end__Location(&loc,
                              parse_decl->previous->loc->s_line,
                              parse_decl->previous->loc->s_col);

                push__Vec(tags, NEW(Tuple, 2, data_type, copy__Location(&loc)));

                break;
            }
            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`ID`");

                emit__Diagnostic(err);

                break;
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

    return tags;
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
                data_type = parse_decl->current->lit;

                next_token(parse_decl);

                if (parse_decl->current->kind == TokenKindColon) {
                    next_token(parse_decl);

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

                break;
            }

            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorUnexpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = token_kind_to_String__Token(*parse_decl->current);

                emit__Diagnostic(err);

                break;
            }
        }

        if (parse_decl->pos != len__Vec(*parse_decl->tokens)) {
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
        }
    }

    return generic_params;
}

static const Int128 Int32Min = -2147483648;
static const Int128 Int32Max = 2147483647;
static const Int128 Int64Min = -0x8000000000000000;
static const Int128 Int64Max = 0x7FFFFFFFFFFFFFFF;
// static const Int128 Int128Min = -0x80000000000000000000000000000000;
// static const Int128 Int128Max = 0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF;

static struct Expr *
parse_literal_expr(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Location loc = NEW(Location);
    struct Literal literal;

    start__Location(&loc,
                    parse_decl->previous->loc->s_line,
                    parse_decl->previous->loc->s_col);

    switch (parse_decl->previous->kind) {
        case TokenKindIntLit: {
            const Str int_str = to_Str__String(*parse_decl->previous->lit);
            Int128 res = atoi_i128(int_str);

            if (res <= Int32Max && res >= Int32Min)
                literal = NEW(LiteralInt32, (Int32)res);
            else if (res <= Int64Max && res >= Int64Min)
                literal = NEW(LiteralInt64, (Int64)res);
            else if (1) {
                TODO("Int128");
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorIntegerIsOutOfRange),
                      *parse_decl->previous->loc,
                      from__String(
                        "even the type Int128 does not support the integer"),
                      None());

                emit__Diagnostic(err);
                emit__Summary(
                  count_error, count_warning, "the parser has been failed");
                exit(1);
            }

            free(int_str);

            break;
        }

        case TokenKindCharLit: {
            const Str char_str = to_Str__String(*parse_decl->previous->lit);

            literal = NEW(LiteralChar, char_str[0]);

            free(char_str);

            break;
        }

        case TokenKindFloatLit: {
            const Str float_str = to_Str__String(*parse_decl->previous->lit);

            literal = NEW(LiteralFloat, strtod(float_str, NULL));

            free(float_str);

            break;
        }

        case TokenKindBitCharLit: {
            const Str bit_char_str = to_Str__String(*parse_decl->previous->lit);

            literal = NEW(LiteralBitChar, (UInt8)bit_char_str[0]);

            free(bit_char_str);

            break;
        }

        case TokenKindStringLit: {
            const Str str = to_Str__String(*parse_decl->previous->lit);

            literal = NEW(LiteralStr, str);

            break;
        }

        case TokenKindBitStringLit: {
            struct String bit_string = *parse_decl->previous->lit;
            UInt8 **bit_str = malloc(sizeof(UInt8 *) * len__String(bit_string));

            for (Usize i = len__String(bit_string); i--;)
                bit_str[i] = (UInt8 *)(UPtr)get__String(bit_string, i);

            bit_str[len__String(bit_string)] = (UInt8 *)'\0';

            literal = NEW(LiteralBitStr, bit_str);

            break;
        }

        case TokenKindTrueKw:
            literal = NEW(LiteralBool, true);
            break;

        case TokenKindFalseKw:
            literal = NEW(LiteralBool, false);
            break;

        default:
            UNREACHABLE("");
    }

    end__Location(&loc,
                  parse_decl->previous->loc->e_line,
                  parse_decl->previous->loc->e_col);

    return NEW(ExprLiteral, literal, loc);
}

static struct Expr *
parse_variable(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc,
               bool is_mut)
{
    struct String *name = parse_decl->previous->lit;
    struct DataType *data_type = NULL;

    if (parse_decl->current->kind == TokenKindColonColon) {
        next_token(parse_decl);

        data_type = parse_data_type(self, parse_decl);
    }

    if (!data_type)
        next_token(parse_decl);
    else {
        EXPECTED_TOKEN(parse_decl, TokenKindColonEq, {
            struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                         &self.parse_block,
                                         NEW(LilyError, LilyErrorExpectedToken),
                                         *parse_decl->current->loc,
                                         from__String(""),
                                         None());

            err->err->s = from__String("`:=`");

            emit__Diagnostic(err);
        });
    }

    struct Expr *expr = parse_expr(self, parse_decl);

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    if (!data_type)
        return NEW(
          ExprVariable, NEW(VariableDecl, name, NULL, expr, is_mut), loc);
    else
        return NEW(
          ExprVariable, NEW(VariableDecl, name, data_type, expr, is_mut), loc);
}

static struct Token *
peek_token(struct ParseDecl parse_decl, Usize n)
{
    if (parse_decl.pos + n < len__Vec(*parse_decl.tokens))
        return get__Vec(*parse_decl.tokens, parse_decl.pos + n);
    else
        return NULL;
}

static void
skip_container(struct ParseDecl parse_decl, Usize *pos)
{
    while (1) {
        struct Token *t = peek_token(parse_decl, *pos++);

        if (!t)
            break;

        if (t->kind == TokenKindRParen || t->kind == TokenKindRBrace ||
            t->kind == TokenKindRHook) {
            (*pos)++;
            break;
        }
    }
}

static bool
verify_if_has_comma(struct ParseDecl parse_decl, Usize add)
{
    Usize pos = 1 + add;

    while (1) {
        struct Token *t = peek_token(parse_decl, pos++);

        if (!t)
            break;
        else {
            switch (t->kind) {
                case TokenKindLParen:
                case TokenKindLBrace:
                case TokenKindLHook:
                    skip_container(parse_decl, &pos);
                    break;
                case TokenKindComma:
                    return true;
                case TokenKindRParen:
                case TokenKindRBrace:
                case TokenKindRHook:
                    return false;
                default:
                    break;
            }
        }
    }

    return false;
}

static struct Expr *
parse_expr_binary_op(struct Parser self,
                     struct ParseDecl *parse_decl,
                     struct Expr *left,
                     struct Location loc,
                     Usize prec)
{
    while (1) {
        int *binary_op_kind = parse_binary_op(parse_decl->current->kind);
        struct String *binary_op_string = NULL;
        binary_op_string = parse_decl->current->lit;

        if (!binary_op_kind)
            break;

        Usize next_prec =
          get_precedence__BinaryOpKind((enum BinaryOpKind)(UPtr)binary_op_kind);

        if (next_prec > prec)
            break;

        if (next_prec < prec)
            left = parse_expr_binary_op(self, parse_decl, left, loc, next_prec);
        else {
            next_token(parse_decl);
            struct Expr *right = parse_primary_expr(self, parse_decl);
            end__Location(&loc,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);
            left = NEW(ExprBinaryOp,
                       NEW(BinaryOp,
                           (enum BinaryOpKind)(UPtr)binary_op_kind,
                           left,
                           right,
                           binary_op_string),
                       loc);
        }
    }

    return left;
}

static struct Expr *
parse_primary_expr(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Expr *expr = NULL;
    struct Location loc = NEW(Location);

    start__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    int *unary_op = parse_unary_op(parse_decl->current->kind);
    struct String *unary_op_string = NULL;

    if (unary_op) {
        if (parse_decl->pos + 1 < len__Vec(*parse_decl->tokens))
            if (unary_op == (int *)UnaryOpKindCustom &&
                ((struct Token *)get__Vec(*parse_decl->tokens,
                                          parse_decl->pos + 1))
                    ->kind == TokenKindLParen) {
                if (verify_if_has_comma(*parse_decl, 1)) {
                    goto exit_unary;
                }
            }

        if (unary_op == (int *)UnaryOpKindCustom)
            unary_op_string = parse_decl->current->lit;

        next_token(parse_decl);

        if (parse_decl->pos == len__Vec(*parse_decl->tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorExpectedRightValue),
                  *parse_decl->current->loc,
                  from__String("unary operator must take right value"),
                  None());

            emit__Diagnostic(err);
            exit(1);
        }

        struct Expr *right = parse_primary_expr(self, parse_decl);

        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        return NEW(ExprUnaryOp,
                   NEW(UnaryOp,
                       (enum UnaryOpKind)(UPtr)(unary_op),
                       right,
                       unary_op_string),
                   loc);
    }

exit_unary : {
}

    next_token(parse_decl);

    switch (parse_decl->previous->kind) {
        case TokenKindIdentifierOp: {
            expr = parse_fun_call_expr(
              self,
              parse_decl,
              NEW(ExprIdentifier, unary_op_string, *parse_decl->previous->loc),
              loc);
            break;
        }
        case TokenKindIdentifier: {
            const Str id_str = to_Str__String(*parse_decl->previous->lit);

            if (!strcmp(id_str, "_")) {
                end__Location(&loc,
                              parse_decl->previous->loc->s_line,
                              parse_decl->previous->loc->s_col);

                expr = NEW(Expr, ExprKindWildcard, loc);
            } else {
                switch (parse_decl->current->kind) {
                    case TokenKindDot: {
                        struct Vec *ids = NEW(Vec, sizeof(struct Expr));

                        push__Vec(ids,
                                  NEW(ExprIdentifier,
                                      parse_decl->previous->lit,
                                      *parse_decl->previous->loc));

                        expr =
                          parse_identifier_access(self, parse_decl, loc, ids);

                        break;
                    }
                    case TokenKindColon:
                    case TokenKindColonDollar:
                        expr =
                          parse_variant_expr(self,
                                             parse_decl,
                                             NEW(ExprIdentifier,
                                                 parse_decl->previous->lit,
                                                 *parse_decl->previous->loc),
                                             loc);
                        break;
                    case TokenKindLParen:
                        expr =
                          parse_fun_call_expr(self,
                                              parse_decl,
                                              NEW(ExprIdentifier,
                                                  parse_decl->previous->lit,
                                                  *parse_decl->previous->loc),
                                              loc);
                        break;
                    case TokenKindLBrace:
                        expr = parse_record_call_expr(
                          self,
                          parse_decl,
                          NEW(ExprIdentifier,
                              parse_decl->previous->lit,
                              *parse_decl->previous->loc),
                          loc);
                        break;
                    case TokenKindLHook:
                        expr = parse_array_access_expr(
                          self,
                          parse_decl,
                          NEW(ExprIdentifier,
                              parse_decl->previous->lit,
                              *parse_decl->previous->loc),
                          loc);
                        break;
                    case TokenKindHashtag: {
                        expr = parse_tuple_access_expr(
                          self,
                          parse_decl,
                          NEW(ExprIdentifier,
                              parse_decl->previous->lit,
                              *parse_decl->previous->loc),
                          loc);
                        break;
                    }
                    case TokenKindColonEq:
                    case TokenKindColonColon:
                        free(id_str);
                        return parse_variable(self, parse_decl, loc, false);
                    default:
                        end__Location(&loc,
                                      parse_decl->previous->loc->e_line,
                                      parse_decl->previous->loc->e_col);

                        expr =
                          NEW(ExprIdentifier, parse_decl->previous->lit, loc);

                        break;
                }
            }

            free(id_str);

            break;
        }

        case TokenKindMutKw:
            next_token(parse_decl);
            return parse_variable(self, parse_decl, loc, true);

        case TokenKindSelfKw: {
            switch (parse_decl->current->kind) {
                case TokenKindDot: {
                    struct Vec *ids = NEW(Vec, sizeof(struct Expr));

                    push__Vec(
                      ids, NEW(Expr, ExprKindSelf, *parse_decl->previous->loc));

                    expr = parse_identifier_access(self, parse_decl, loc, ids);

                    break;
                }

                default:
                    expr = NEW(Expr, ExprKindSelf, *parse_decl->previous->loc);

                    break;
            }

            break;
        }

        case TokenKindGlobalKw: {
            struct Vec *ids = NEW(Vec, sizeof(struct Expr));

            expr = parse_identifier_access(self, parse_decl, loc, ids);

            break;
        }

        case TokenKindLParen: {
            if (verify_if_has_comma(*parse_decl, 0)) {
                if (parse_decl->current->kind == TokenKindRParen) {
                    next_token(parse_decl);
                    end__Location(&loc,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);

                    expr = NEW(ExprLiteral, NEW(LiteralUnit), loc);
                } else {

                    struct Vec *tuple = NEW(Vec, sizeof(struct Expr));

                    PARSE_PAREN(parse_decl, {
                        push__Vec(tuple, parse_expr(self, parse_decl));

                        if (parse_decl->current->kind != TokenKindRParen) {
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
                        }
                    });

                    end__Location(&loc,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);

                    expr = NEW(ExprTuple, tuple, loc);
                }
            } else {
                struct Expr *grouping = parse_expr(self, parse_decl);

                end__Location(&loc,
                              parse_decl->current->loc->s_line,
                              parse_decl->current->loc->s_col);
                next_token(parse_decl);

                expr = NEW(ExprGrouping, grouping, loc);
            }

            break;
        }

        case TokenKindLHook: {
            struct Vec *array = NEW(Vec, sizeof(struct Expr));

            PARSE_HOOK(parse_decl, {
                push__Vec(array, parse_expr(self, parse_decl));

                if (parse_decl->current->kind != TokenKindRHook) {
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
                }
            });

            end__Location(&loc,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            expr = NEW(ExprArray, array, loc);

            break;
        }

        case TokenKindTryKw: {
            next_token(parse_decl);

            struct Expr *expr_try = parse_expr(self, parse_decl);

            end__Location(&loc,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            expr = NEW(ExprTry, expr_try, loc);

            break;
        }

        case TokenKindIfKw: {
            next_token(parse_decl);

            struct IfCond *if_ = parse_if_stmt(self, parse_decl, &loc);

            expr = NEW(ExprIf, if_, loc);

            break;
        }

        case TokenKindBeginKw: {
            EXPECTED_TOKEN(parse_decl, TokenKindEq, {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`=`");

                emit__Diagnostic(err);
            });

            struct Vec *body = NEW(Vec, sizeof(struct FunBodyItem));

            while (parse_decl->current->kind != TokenKindEndKw &&
                   parse_decl->current->kind != TokenKindSemicolon) {
                PARSE_BODY(body);
            }

            next_token(parse_decl);

            end__Location(&loc,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            return NEW(ExprBlock, body, loc);
        }

        case TokenKindAmpersand: {
            next_token(parse_decl);

            struct Expr *expr_ref = parse_expr(self, parse_decl);

            end__Location(&loc,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            expr = NEW(ExprRef, expr_ref, loc);

            break;
        }

        case TokenKindUndefKw:
            end__Location(&loc,
                          parse_decl->previous->loc->e_line,
                          parse_decl->previous->loc->e_col);

            expr = NEW(Expr, ExprKindUndef, loc);

            break;

        case TokenKindNilKw:
            end__Location(&loc,
                          parse_decl->previous->loc->e_line,
                          parse_decl->previous->loc->e_col);

            expr = NEW(Expr, ExprKindNil, loc);

            // next_token(parse_decl);

            break;

        case TokenKindNoneKw:
            end__Location(&loc,
                          parse_decl->previous->loc->e_line,
                          parse_decl->previous->loc->e_col);

            expr = NEW(Expr, ExprKindNone, loc);

            // next_token(parse_decl);

            break;

        case TokenKindIntLit:
        case TokenKindCharLit:
        case TokenKindFloatLit:
        case TokenKindBitCharLit:
        case TokenKindStringLit:
        case TokenKindBitStringLit:
        case TokenKindTrueKw:
        case TokenKindFalseKw:
            expr = parse_literal_expr(self, parse_decl);
            break;
        default: {
            if (parse_binary_op(parse_decl->previous->kind)) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedRightValue),
                      *parse_decl->current->loc,
                      from__String(
                        "binary operator must take left and right value"),
                      None());

                emit__Diagnostic(err);
                exit(1);
            } else if (parse_unary_op(parse_decl->previous->kind)) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedRightValue),
                      *parse_decl->current->loc,
                      from__String("unary operator must take right value"),
                      None());

                emit__Diagnostic(err);
                exit(1);
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorUnexpectedExpression),
                      *parse_decl->current->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
                exit(1);
            }
        }
    }

    return expr;
}

static inline int *
parse_unary_op(enum TokenKind kind)
{
    switch (kind) {
        case TokenKindMinus:
            return (int *)UnaryOpKindNegative;

        case TokenKindNotKw:
            return (int *)UnaryOpKindNot;

        case TokenKindAmpersand:
            return (int *)UnaryOpKindReference;

        case TokenKindWave:
            return (int *)UnaryOpKindBitNot;

        case TokenKindIdentifierOp:
            return (int *)UnaryOpKindCustom;

        default:
            return NULL;
    }
}

static inline int *
parse_binary_op(enum TokenKind kind)
{
    switch (kind) {
        case TokenKindPlus:
            return (int *)BinaryOpKindAdd;

        case TokenKindMinus:
            return (int *)BinaryOpKindSub;

        case TokenKindStar:
            return (int *)BinaryOpKindMul;

        case TokenKindSlash:
            return (int *)BinaryOpKindDiv;

        case TokenKindPercentage:
            return (int *)BinaryOpKindMod;

        case TokenKindDotDot:
            return (int *)BinaryOpKindRange;

        case TokenKindLShift:
            return (int *)BinaryOpKindLt;

        case TokenKindRShift:
            return (int *)BinaryOpKindGt;

        case TokenKindLShiftEq:
            return (int *)BinaryOpKindLe;

        case TokenKindRShiftEq:
            return (int *)BinaryOpKindGe;

        case TokenKindEqEq:
            return (int *)BinaryOpKindEq;

        case TokenKindNotEq:
            return (int *)BinaryOpKindNe;

        case TokenKindAndKw:
            return (int *)BinaryOpKindAnd;

        case TokenKindOrKw:
            return (int *)BinaryOpKindOr;

        case TokenKindXorKw:
            return (int *)BinaryOpKindXor;

        case TokenKindEq:
            return (int *)BinaryOpKindAssign;

        case TokenKindPlusEq:
            return (int *)BinaryOpKindAddAssign;

        case TokenKindMinusEq:
            return (int *)BinaryOpKindSubAssign;

        case TokenKindStarEq:
            return (int *)BinaryOpKindMulAssign;

        case TokenKindSlashEq:
            return (int *)BinaryOpKindDivAssign;

        case TokenKindPercentageEq:
            return (int *)BinaryOpKindModAssign;

        case TokenKindHatEq:
            return (int *)BinaryOpKindConcatAssign;

        case TokenKindLShiftLShiftEq:
            return (int *)BinaryOpKindBitLShiftAssign;

        case TokenKindRShiftRShiftEq:
            return (int *)BinaryOpKindBitRShiftAssign;

        case TokenKindBarEq:
            return (int *)BinaryOpKindBitOrAssign;

        case TokenKindXorEq:
            return (int *)BinaryOpKindXorAssign;

        case TokenKindAmpersandEq:
            return (int *)BinaryOpKindBitAndAssign;

        case TokenKindPlusPlusEq:
            return (int *)BinaryOpKindMergeAssign;

        case TokenKindMinusMinusEq:
            return (int *)BinaryOpKindUnmergeAssign;

        case TokenKindStarStarEq:
            return (int *)BinaryOpKindExponentAssign;

        case TokenKindBarRShift:
            return (int *)BinaryOpKindChain;

        case TokenKindPlusPlus:
            return (int *)BinaryOpKindMerge;

        case TokenKindMinusMinus:
            return (int *)BinaryOpKindUnmerge;

        case TokenKindDollar:
            return (int *)BinaryOpKindRepeat;

        case TokenKindHat:
            return (int *)BinaryOpKindConcat;

        case TokenKindLShiftLShift:
            return (int *)BinaryOpKindBitLShift;

        case TokenKindRShiftRShift:
            return (int *)BinaryOpKindBitRShift;

        case TokenKindBar:
            return (int *)BinaryOpKindBitOr;

        case TokenKindAmpersand:
            return (int *)BinaryOpKindBitAnd;

        case TokenKindStarStar:
            return (int *)BinaryOpKindExponent;

        case TokenKindIdentifierOp:
            return (int *)BinaryOpKindCustom;

        default:
            return NULL;
    }
}

static struct Expr *
parse_variant_expr(struct Parser self,
                   struct ParseDecl *parse_decl,
                   struct Expr *id,
                   struct Location loc)
{
    if (parse_decl->current->kind == TokenKindColon) {
        next_token(parse_decl);
        struct Expr *expr = parse_expr(self, parse_decl);

        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        return NEW(ExprVariant, NEW(Variant, id, Some(expr)), loc);
    } else { // anyone else token is unreachable
        next_token(parse_decl);
        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        return NEW(ExprVariant, NEW(Variant, id, None()), loc);
    }
}

static struct Expr *
parse_fun_call_expr(struct Parser self,
                    struct ParseDecl *parse_decl,
                    struct Expr *id,
                    struct Location loc)
{
    struct Vec *params_call = NEW(Vec, sizeof(struct Tuple));

    next_token(parse_decl);

    PARSE_PAREN(parse_decl, {
        struct Location loc_param_call = NEW(Location);

        start__Location(&loc_param_call,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        switch (parse_decl->current->kind) {
            case TokenKindIdentifier: {
                if (((struct Token *)get__Vec(*parse_decl->tokens,
                                              parse_decl->pos + 1))
                      ->kind == TokenKindColonEq) {
                    struct String *name = parse_decl->current->lit;

                    next_token(parse_decl);
                    next_token(parse_decl);

                    struct Expr *expr = parse_expr(self, parse_decl);

                    end__Location(&loc_param_call,
                                  parse_decl->current->loc->s_line,
                                  parse_decl->current->loc->s_col);

                    push__Vec(params_call,
                              NEW(Tuple,
                                  2,
                                  NEW(FunParamCallDefault, expr, name),
                                  copy__Location(&loc_param_call)));
                } else
                    goto expr;

                break;
            }
            default: {
            expr : {
                struct Expr *expr = parse_expr(self, parse_decl);

                end__Location(&loc_param_call,
                              parse_decl->current->loc->s_line,
                              parse_decl->current->loc->s_col);

                push__Vec(params_call,
                          NEW(Tuple,
                              2,
                              NEW(FunParamCall, expr),
                              copy__Location(&loc_param_call)));
            }

            break;
            }
        }

        if (parse_decl->current->kind != TokenKindRParen) {
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
        }
    });

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(ExprFunCall, NEW(FunCall, id, params_call), loc);
}

static struct Expr *
parse_record_call_expr(struct Parser self,
                       struct ParseDecl *parse_decl,
                       struct Expr *id,
                       struct Location loc)
{
    struct Vec *fields = NEW(Vec, sizeof(struct Tuple));

    next_token(parse_decl);

    PARSE_BRACE(parse_decl, {
        struct Location loc_field = NEW(Location);
        struct String *name = NULL;

        start__Location(&loc_field,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        if (parse_decl->current->kind == TokenKindIdentifier) {
            name = parse_decl->current->lit;

            next_token(parse_decl);
        } else {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorMissFieldCallName),
                  *parse_decl->previous->loc,
                  from__String(""),
                  None());

            emit__Diagnostic(err);
        }

        if (parse_decl->current->kind == TokenKindColonEq) {
            next_token(parse_decl);

            struct Expr *expr = parse_expr(self, parse_decl);

            end__Location(&loc_field,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            push__Vec(fields,
                      NEW(Tuple,
                          2,
                          NEW(FieldCall, name, Some(expr)),
                          copy__Location(&loc_field)));
        } else {
            end__Location(&loc_field,
                          parse_decl->current->loc->s_line,
                          parse_decl->current->loc->s_col);

            push__Vec(fields,
                      NEW(Tuple,
                          2,
                          NEW(FieldCall, name, None()),
                          copy__Location(&loc_field)));
        }

        if (parse_decl->current->kind != TokenKindRBrace) {
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
        }
    });

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(ExprRecordCall, NEW(RecordCall, id, fields), loc);
}

static struct Expr *
parse_array_access_expr(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Expr *id,
                        struct Location loc)
{
    struct Vec *access = NEW(Vec, sizeof(struct Expr));

    while (parse_decl->current->kind == TokenKindLHook) {
        next_token(parse_decl);
        push__Vec(access, parse_expr(self, parse_decl));

        EXPECTED_TOKEN(parse_decl, TokenKindRHook, {
            struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                         &self.parse_block,
                                         NEW(LilyError, LilyErrorExpectedToken),
                                         *parse_decl->current->loc,
                                         format(""),
                                         None());

            err->err->s = from__String("`]`");

            emit__Diagnostic(err);
        });
    }

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(ExprArrayAccess, NEW(ArrayAccess, id, access), loc);
}

static struct Expr *
parse_tuple_access_expr(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Expr *id,
                        struct Location loc)
{
    struct Vec *access = NEW(Vec, sizeof(struct Expr));

    while (parse_decl->current->kind == TokenKindHashtag) {
        next_token(parse_decl);
        push__Vec(access, parse_expr(self, parse_decl));
    }

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(ExprTupleAccess, NEW(TupleAccess, id, access), loc);
}

static struct Expr *
parse_identifier_access(struct Parser self,
                        struct ParseDecl *parse_decl,
                        struct Location loc,
                        struct Vec *ids)
{
    while (parse_decl->current->kind == TokenKindDot &&
           parse_decl->pos < len__Vec(*parse_decl->tokens)) {
        next_token(parse_decl);

        switch (parse_decl->current->kind) {
            case TokenKindIdentifier:
                push__Vec(ids,
                          NEW(ExprIdentifier,
                              parse_decl->current->lit,
                              *parse_decl->current->loc));
                next_token(parse_decl);

                break;
            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`ID`");

                emit__Diagnostic(err);
            }
        }
    }

    {
        struct Location loc_call = NEW(Location);

        loc_call.s_line = loc.s_line;
        loc_call.s_col = loc.s_col;

        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        switch (parse_decl->current->kind) {
            case TokenKindLParen:
                return parse_fun_call_expr(self,
                                           parse_decl,
                                           NEW(ExprIdentifierAccess, ids, loc),
                                           loc_call);
            case TokenKindLBrace:
                return parse_record_call_expr(
                  self,
                  parse_decl,
                  NEW(ExprIdentifierAccess, ids, loc),
                  loc_call);
            case TokenKindLHook:
                return parse_array_access_expr(
                  self,
                  parse_decl,
                  NEW(ExprIdentifierAccess, ids, loc),
                  loc_call);
            case TokenKindHashtag:
                return parse_tuple_access_expr(
                  self,
                  parse_decl,
                  NEW(ExprIdentifierAccess, ids, loc),
                  loc_call);
            default:
                break;
        }
    }

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(ExprIdentifierAccess, ids, loc);
}

static struct Expr *
parse_expr(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Location loc = NEW(Location);

    start__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    struct Expr *left = parse_primary_expr(self, parse_decl);

    struct Expr *expr = parse_expr_binary_op(
      self, parse_decl, left, loc, get_precedence__Expr(left));

    return expr;
}

static struct Stmt *
parse_return_stmt(struct Parser self,
                  struct ParseDecl *parse_decl,
                  struct Location loc)
{
    struct Expr *expr = parse_expr(self, parse_decl);

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(StmtReturn, loc, expr);
}

static struct IfCond *
parse_if_stmt(struct Parser self,
              struct ParseDecl *parse_decl,
              struct Location *loc)
{
    struct Expr *if_cond = NULL;
    struct Vec *if_body = NEW(Vec, sizeof(struct FunBodyItem));

    if_cond = parse_expr(self, parse_decl);

    EXPECTED_TOKEN(parse_decl, TokenKindDoKw, {
        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     &self.parse_block,
                                     NEW(LilyError, LilyErrorExpectedToken),
                                     *parse_decl->current->loc,
                                     format(""),
                                     None());

        err->err->s = from__String("`do`");

        emit__Diagnostic(err);
    });

    while (parse_decl->current->kind != TokenKindEndKw &&
           parse_decl->current->kind != TokenKindSemicolon &&
           parse_decl->current->kind != TokenKindElifKw &&
           parse_decl->current->kind != TokenKindElseKw) {
        PARSE_BODY(if_body);
    }

    next_token(parse_decl);

    struct Vec *elif = NULL;

    switch (parse_decl->previous->kind) {
        case TokenKindElifKw: {
            elif = NEW(Vec, sizeof(struct IfBranch));

        elif : {
            struct Expr *elif_expr = parse_expr(self, parse_decl);
            struct Vec *elif_body = NEW(Vec, sizeof(struct FunBodyItem));

            EXPECTED_TOKEN(parse_decl, TokenKindDoKw, {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`do`");

                emit__Diagnostic(err);
            });

            while (parse_decl->current->kind != TokenKindEndKw &&
                   parse_decl->current->kind != TokenKindElifKw &&
                   parse_decl->current->kind != TokenKindElseKw) {
                PARSE_BODY(elif_body);
            }

            push__Vec(elif, NEW(IfBranch, elif_expr, elif_body));

            next_token(parse_decl);

            switch (parse_decl->previous->kind) {
                case TokenKindEndKw:
                    end__Location(loc,
                                  parse_decl->previous->loc->s_line,
                                  parse_decl->previous->loc->s_col);

                    return NEW(IfCond,
                               NEW(IfBranch, if_cond, if_body),
                               Some(elif),
                               None());
                case TokenKindElifKw:
                    goto elif;
                case TokenKindElseKw:
                    goto else_;
                default:
                    UNREACHABLE("");
            }
        };

          break;
        }
        case TokenKindElseKw: {
        else_ : {
            struct Vec *else_body = NEW(Vec, sizeof(struct FunBodyItem));

            while (parse_decl->current->kind != TokenKindEndKw) {
                PARSE_BODY(else_body);
            }

            next_token(parse_decl);
            end__Location(loc,
                          parse_decl->previous->loc->s_line,
                          parse_decl->previous->loc->s_col);

            if (!elif)
                return NEW(IfCond,
                           NEW(IfBranch, if_cond, if_body),
                           None(),
                           Some(else_body));
            else
                return NEW(IfCond,
                           NEW(IfBranch, if_cond, if_body),
                           Some(elif),
                           Some(else_body));
        }
        }
        case TokenKindSemicolon:
        case TokenKindEndKw:
            end__Location(loc,
                          parse_decl->previous->loc->s_line,
                          parse_decl->previous->loc->s_col);

            return NEW(IfCond, NEW(IfBranch, if_cond, if_body), None(), None());
        default:
            UNREACHABLE("");
    }
}

static struct Stmt *
parse_await_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location loc)
{
    struct Expr *expr = parse_expr(self, parse_decl);

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(StmtAwait, loc, expr);
}

static struct Stmt *
parse_try_stmt(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc)
{
    next_token(parse_decl);

    struct Vec *try_body = NEW(Vec, sizeof(struct FunBodyItem));
    struct Option *catch_expr = NULL;
    struct Option *catch_body = NULL;

    while (parse_decl->current->kind != TokenKindEndKw &&
           parse_decl->current->kind != TokenKindCatchKw) {
        PARSE_BODY(try_body);
    }

    if (parse_decl->current->kind == TokenKindCatchKw) {
        next_token(parse_decl);

        struct Expr *expr = parse_expr(self, parse_decl);

        if (expr->kind == ExprKindWildcard)
            catch_expr = None();
        else
            catch_expr = Some(expr);

        next_token(parse_decl);

        struct Vec *catch_body_vec = NEW(Vec, sizeof(struct FunBodyItem));

        while (parse_decl->current->kind != TokenKindEndKw) {
            PARSE_BODY(catch_body_vec);
        }

        catch_body = Some(catch_body_vec);

        next_token(parse_decl);
        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        return NEW(
          StmtTry, loc, NEW(TryStmt, try_body, catch_expr, catch_body));
    } else {
        next_token(parse_decl);

        catch_expr = None();
        catch_body = None();

        end__Location(&loc,
                      parse_decl->current->loc->s_line,
                      parse_decl->current->loc->s_col);

        return NEW(
          StmtTry, loc, NEW(TryStmt, try_body, catch_expr, catch_body));
    }
}

static struct MatchStmt *
parse_match_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location *loc)
{
    struct Expr *matching = parse_expr(self, parse_decl);
    struct Vec *patterns = NEW(Vec, sizeof(struct Tuple));

    next_token(parse_decl);

    while (parse_decl->current->kind != TokenKindEndKw) {
        struct Expr *pattern_match = parse_expr(self, parse_decl);
        struct Expr *cond = NULL;

        if (parse_decl->current->kind == TokenKindInterrogation) {
            next_token(parse_decl);

            cond = parse_expr(self, parse_decl);
        }

        if (parse_decl->current->kind != TokenKindFatArrow)
            assert(0 && "error");
        else
            next_token(parse_decl);

        struct Expr *pattern_expr = parse_expr(self, parse_decl);

        push__Vec(patterns, NEW(Tuple, 3, pattern_match, cond, pattern_expr));

        if (parse_decl->current->kind != TokenKindEndKw) {
            if (parse_decl->current->kind != TokenKindComma) {
                assert(0 && "error");
            } else
                next_token(parse_decl);
        }
    }

    next_token(parse_decl);
    end__Location(
      loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(MatchStmt, matching, patterns);
}

static struct Stmt *
parse_while_stmt(struct Parser self,
                 struct ParseDecl *parse_decl,
                 struct Location loc)
{
    struct Expr *while_expr = parse_expr(self, parse_decl);
    struct Vec *while_body = NEW(Vec, sizeof(struct FunBodyItem));

    next_token(parse_decl);

    while (parse_decl->current->kind != TokenKindEndKw) {
        PARSE_BODY(while_body);
    }

    next_token(parse_decl);
    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(StmtWhile, loc, NEW(WhileStmt, while_expr, while_body));
}

static struct Stmt *
parse_for_stmt(struct Parser self,
               struct ParseDecl *parse_decl,
               struct Location loc)
{
}

static struct Stmt *
parse_import_stmt(struct Parser self,
                  struct ParseDecl *parse_decl,
                  struct Location loc)
{
    struct String *import_value = NULL;
    struct Location import_value_loc;
    struct String *as_value = NULL;

    if (parse_decl->current->kind == TokenKindStringLit) {
        import_value = parse_decl->current->lit;
        import_value_loc = *parse_decl->current->loc;

        next_token(parse_decl);
    } else {
        import_value_loc = *parse_decl->current->loc;

        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     &self.parse_block,
                                     NEW(LilyError, LilyErrorMissImportValue),
                                     *parse_decl->previous->loc,
                                     from__String("miss import value"),
                                     None());

        emit__Diagnostic(err);
    }

    if (parse_decl->current->kind == TokenKindAsKw) {
        next_token(parse_decl);

        if (parse_decl->current->kind == TokenKindIdentifier) {
            as_value = parse_decl->current->lit;

            next_token(parse_decl);
        } else {
            struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                         &self.parse_block,
                                         NEW(LilyError, LilyErrorMissAsValue),
                                         *parse_decl->previous->loc,
                                         from__String("miss as value"),
                                         None());

            emit__Diagnostic(err);
        }
    }

    end__Location(
      &loc, parse_decl->current->loc->s_line, parse_decl->current->loc->s_col);

    return NEW(StmtImport,
               loc,
               parse_import_value__parse_import_stmt(
                 self, import_value, as_value, import_value_loc, false));
}

#define UPDATE_CURRENT() \
    current = i < len__String(*buffer) ? get__String(*buffer, i) : NULL;

static struct ImportStmt *
parse_import_value__parse_import_stmt(struct Parser self,
                                      struct String *buffer,
                                      struct String *as_value,
                                      struct Location buffer_loc,
                                      bool is_pub)
{
    struct Vec *import_value = NEW(Vec, sizeof(struct ImportStmtValue));
    char *current;
    Usize i = 0;

    if (buffer) {
        if (len__String(*buffer) == 0) {
            current = NULL;

            {
                struct Location loc_err = buffer_loc;

                loc_err.s_col += i + 1;
                loc_err.e_col = loc_err.e_col;

                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorEmptyImportValue),
                      loc_err,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
            }

            goto exit;
        } else
            current = get__String(*buffer, 0);

        if (current == (char *)'@') {
            if (++i < len__String(*buffer)) {
                const struct String *name =
                  get_name__parse_import_stmt(self, *buffer, buffer_loc, &i);
                const Str name_str = to_Str__String(*name);

                UPDATE_CURRENT();

#define IMPORT_VALUE_ERR(c, v)                                     \
    struct Location loc_err = buffer_loc;                          \
                                                                   \
    loc_err.s_col += i + 1;                                        \
    loc_err.e_col = loc_err.s_col;                                 \
                                                                   \
    struct Diagnostic *err =                                       \
      NEW(DiagnosticWithErrParser,                                 \
          &self.parse_block,                                       \
          NEW(LilyError, LilyErrorExpectedCharacterInImportValue), \
          loc_err,                                                 \
          from__String(""),                                        \
          Some(format("expected `{c}`, to close "                  \
                      "`@{s}` import value",                       \
                      c,                                           \
                      v)));                                        \
                                                                   \
    emit__Diagnostic(err);

                if (!strcmp(name_str, "std"))
                    push__Vec(import_value, NEW(ImportStmtValueStd));
                else if (!strcmp(name_str, "core"))
                    push__Vec(import_value, NEW(ImportStmtValueCore));
                else if (!strcmp(name_str, "builtin"))
                    push__Vec(import_value, NEW(ImportStmtValueBuiltin));
                else if (!strcmp(name_str, "file")) {
                    if (current == (char *)'(') {
                        push__Vec(
                          import_value,
                          NEW(ImportStmtValueFile,
                              get_value__parse_import_stmt(*buffer, &i)));

                        UPDATE_CURRENT();

                        if (current != (char *)')') {
                            IMPORT_VALUE_ERR(')', "file");
                        }

                        next_char__parse_import_stmt(*buffer, &current, &i);
                    } else {
                        IMPORT_VALUE_ERR('(', "file");
                    }
                } else if (!strcmp(name_str, "url")) {
                    if (current == (char *)'(') {
                        push__Vec(
                          import_value,
                          NEW(ImportStmtValueUrl,
                              get_value__parse_import_stmt(*buffer, &i)));

                        UPDATE_CURRENT();

                        if (current != (char *)')') {
                            IMPORT_VALUE_ERR(')', "url");
                        }

                        next_char__parse_import_stmt(*buffer, &current, &i);
                    } else {
                        IMPORT_VALUE_ERR('(', "url");
                    }
                } else {
                    struct Location loc_err = buffer_loc;

                    loc_err.e_line = loc_err.s_line;
                    loc_err.s_col += 1;
                    loc_err.e_col = loc_err.s_col;

                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self.parse_block,
                          NEW(LilyError, LilyErrorBadImportValue),
                          loc_err,
                          format("bad `@` import value, replace `{s}` by "
                                 "`std`, builtin`, `file` or `url`",
                                 name_str),
                          Some(from__String(
                            "an import value starting with `@` can take the "
                            "names: `std`, `builtin`, `file` or `url`")));

                    emit__Diagnostic(err);

                    goto exit;
                }

                bool is_exit =
                  (current >= (char *)'a' && current <= (char *)'z') ||
                  (current >= (char *)'A' && current <= (char *)'Z') ||
                  current == (char *)'_' || current == NULL;

                if (name_str)
                    free(name_str);

                if (name)
                    FREE(String, (struct String *)name);

                UPDATE_CURRENT();

                if (is_exit) {
                    goto exit;
                } else if (current == (char *)'.') {
                    next_char__parse_import_stmt(*buffer, &current, &i);
                } else {
                    struct Location loc_err = buffer_loc;

                    loc_err.e_line = loc_err.s_line;
                    loc_err.s_col += i + 1;
                    loc_err.e_col = loc_err.s_col;

                    struct Diagnostic *err = NEW(
                      DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorUnexpectedCharacterInImportValue),
                      loc_err,
                      from__String(""),
                      Some(format("found `{c}`, expected `.`",
                                  (char)(UPtr)current)));

                    emit__Diagnostic(err);

                    goto exit;
                }
            } else {
                assert(0 && "error");

                goto exit;
            }
        }

        for (; current != NULL;
             next_char__parse_import_stmt(*buffer, &current, &i)) {
            if ((current >= (char *)'a' && current <= (char *)'z') ||
                (current >= (char *)'A' && current <= (char *)'Z') ||
                current == (char *)'_') {
                push__Vec(import_value,
                          NEW(ImportStmtValueAccess,
                              get_name__parse_import_stmt(
                                self, *buffer, buffer_loc, &i)));

                UPDATE_CURRENT();

                if (!((current >= (char *)'a' && current <= (char *)'z') ||
                      (current >= (char *)'A' && current <= (char *)'Z') ||
                      (current >= (char *)'0' && current <= (char *)'9') ||
                      current == (char *)'_' || current == (char *)'.' ||
                      current == NULL)) {
                    struct Location loc_err = buffer_loc;

                    loc_err.e_line = loc_err.s_line;
                    loc_err.s_col += i + 1;
                    loc_err.e_col = loc_err.s_col;

                    struct Diagnostic *err = NEW(
                      DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorUnexpectedCharacterInImportValue),
                      loc_err,
                      from__String(""),
                      Some(format("found `{c}`, expected `.`",
                                  (char)(UPtr)current)));

                    emit__Diagnostic(err);

                    goto exit;
                }
            } else if (current == (char *)'{') {
                push__Vec(import_value,
                          get_selector__parse_import_stmt(
                            self, *buffer, buffer_loc, &i));
                UPDATE_CURRENT();

                if (i >= len__String(*buffer) - 1)
                    break;

                if (current == (char *)'.')
                    next_char__parse_import_stmt(*buffer, &current, &i);
            } else if (current == (char *)'*') {
                push__Vec(import_value, NEW(ImportStmtValueWildcard));
                break;
            } else {
                struct Location loc_err = buffer_loc;

                loc_err.e_line = loc_err.s_line;
                loc_err.s_col += i + 1;
                loc_err.e_col = loc_err.s_col;

                struct Diagnostic *err = NEW(
                  DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorUnexpectedCharacterInImportValue),
                  loc_err,
                  from__String(""),
                  Some(format("found `{c}`, expected `ID`, `{{`, `}` or `*`",
                              (char)(UPtr)current)));

                emit__Diagnostic(err);

                break;
            }
        }

    exit : {
    }
    } else {
        struct Location loc_err = buffer_loc;

        loc_err.s_col += i + 1;
        loc_err.e_col = loc_err.s_col;

        struct Diagnostic *err = NEW(DiagnosticWithErrParser,
                                     &self.parse_block,
                                     NEW(LilyError, LilyErrorMissImportValue),
                                     loc_err,
                                     from__String("miss import value"),
                                     None());

        emit__Diagnostic(err);
    }

    if (!as_value)
        return NEW(ImportStmt, import_value, is_pub, None());

    return NEW(ImportStmt, import_value, is_pub, Some(as_value));
}

static struct String *
get_value__parse_import_stmt(struct String buffer, Usize *pos)
{
    struct String *s = NEW(String);
    char *current = get__String(buffer, *pos);

    while (current != (char *)')') {
        push__String(s, get__String(buffer, *pos));
        next_char__parse_import_stmt(buffer, &current, pos);
    }

    return s;
}

static struct String *
get_name__parse_import_stmt(struct Parser self,
                            struct String buffer,
                            struct Location buffer_loc,
                            Usize *pos)
{
    struct String *s = NEW(String);
    char *current =
      *pos < len__String(buffer) ? get__String(buffer, *pos) : NULL;

    while (((current >= (char *)'a' && current <= (char *)'z') ||
            (current >= (char *)'A' && current <= (char *)'Z') ||
            (current >= (char *)'0' && current <= (char *)'9') ||
            current == (char *)'_')) {
        push__String(s, get__String(buffer, *pos));
        next_char__parse_import_stmt(buffer, &current, pos);
    }

    if (len__String(*s) == 0) {
        struct Location loc_err = buffer_loc;

        loc_err.e_line = loc_err.s_line;
        loc_err.s_col += *pos + 1;
        loc_err.e_col = loc_err.s_col;

        struct Diagnostic *err =
          NEW(DiagnosticWithErrParser,
              &self.parse_block,
              NEW(LilyError, LilyErrorUnexpectedCharacterInImportValue),
              loc_err,
              from__String(""),
              Some(format("found `{c}`, expected `ID`", (char)(UPtr)current)));

        emit__Diagnostic(err);

        goto exit;
    }

exit : {
}

    return s;
}

static struct Vec *
get_value_in_selector__parse_import_stmt(struct Parser self,
                                         struct String buffer,
                                         struct Location buffer_loc,
                                         Usize *pos)
{
    struct Vec *value = NEW(Vec, sizeof(struct ImportStmtValue));
    char *current = get__String(buffer, *pos);

    do {
        if ((current >= (char *)'a' && current <= (char *)'z') ||
            (current >= (char *)'A' && current <= (char *)'Z') ||
            current == (char *)'_') {
            push__Vec(
              value,
              NEW(ImportStmtValueAccess,
                  get_name__parse_import_stmt(self, buffer, buffer_loc, pos)));
            next_char__parse_import_stmt(buffer, &current, pos);
        } else if (current == (char *)'*') {
            push__Vec(value, NEW(ImportStmtValueWildcard));
            next_char__parse_import_stmt(buffer, &current, pos);
            next_char__parse_import_stmt(buffer, &current, pos);

            break;
        } else if (current == (char *)'{') {
            next_char__parse_import_stmt(buffer, &current, pos);

            push__Vec(value,
                      NEW(ImportStmtValueSelector,
                          get_selector__parse_import_stmt(
                            self, buffer, buffer_loc, pos)));
        } else if (current == (char *)'}') {
            next_char__parse_import_stmt(buffer, &current, pos);

            break;
        } else {
            struct Location loc_err = buffer_loc;

            loc_err.e_line = loc_err.s_line;
            loc_err.s_col += *pos + 1;
            loc_err.e_col = loc_err.s_col;

            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorUnexpectedCharacterInImportValue),
                  loc_err,
                  from__String(""),
                  Some(format("found `{c}`, expected `ID`, `{{`, `}` or `*`",
                              (char)(UPtr)current)));

            emit__Diagnostic(err);

            goto exit;
        }
    } while (current != NULL && current == (char *)'.');

exit : {
}

    return value;
}

static struct Vec *
get_selector__parse_import_stmt(struct Parser self,
                                struct String buffer,
                                struct Location buffer_loc,
                                Usize *pos)
{
    struct Vec *selector = NEW(Vec, sizeof(struct Vec));
    char *current = get__String(buffer, ++(*(pos)));

    while (current != (char *)'}') {
        push__Vec(selector,
                  get_value_in_selector__parse_import_stmt(
                    self, buffer, buffer_loc, pos));

        current = get__String(buffer, *pos);

        if (get__String(buffer, (*(pos)-1)) == (char *)',')
            next_char__parse_import_stmt(buffer, &current, pos);

        while (current == (char *)' ' || current == (char *)'\t')
            next_char__parse_import_stmt(buffer, &current, pos);
    }

    if (current == NULL)
        assert(0 && "error");
    else
        next_char__parse_import_stmt(buffer, &current, pos);

    if (current == (char *)'.')
        next_char__parse_import_stmt(buffer, &current, pos);

    return selector;
}

static void
next_char__parse_import_stmt(struct String buffer, char **current, Usize *pos)
{
    if (*pos + 1 < len__String(buffer)) {
        ++(*pos);
        *current = get__String(buffer, *pos);
    } else
        *current = NULL;
}

// struct Vec<struct FunParam*>*
static struct Vec *
parse_fun_params(struct Parser self,
                 struct ParseDecl *parse_decl,
                 bool is_method)
{
    struct Vec *params = NEW(Vec, sizeof(struct FunParam));
    bool has_self_param = false;

    while (parse_decl->pos < len__Vec(*parse_decl->tokens)) {
        struct Location loc = NEW(Location);
        struct Location loc_data_type = NEW(Location);
        struct String *name = NULL;
        struct DataType *data_type = NULL;
        struct Expr *default_value = NULL;

        start__Location(&loc,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        if (parse_decl->current->kind == TokenKindSelfKw && is_method &&
            !has_self_param && parse_decl->pos == 0) {
            has_self_param = true;

            next_token(parse_decl);

            end__Location(&loc,
                          parse_decl->previous->loc->e_line,
                          parse_decl->previous->loc->e_col);

            EXPECTED_TOKEN(parse_decl, TokenKindComma, {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`,` or `:=`");

                emit__Diagnostic(err);
            });

            push__Vec(params, NEW(FunParamSelf, loc));
        } else if (parse_decl->current->kind == TokenKindSelfKw && !is_method) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorUnexpectedSelfParamInFunction),
                  *parse_decl->current->loc,
                  format(""),
                  None());

            emit__Diagnostic(err);
        } else if (parse_decl->current->kind == TokenKindSelfKw &&
                   has_self_param) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorDupliateSelfParam),
                  *parse_decl->current->loc,
                  format(""),
                  None());

            emit__Diagnostic(err);
        } else if (parse_decl->current->kind == TokenKindSelfKw &&
                   parse_decl->pos != 0) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self.parse_block,
                  NEW(LilyError, LilyErrorSelfParamMustBeTheFirstParam),
                  *parse_decl->current->loc,
                  format(""),
                  None());

            emit__Diagnostic(err);
        } else {
            if (parse_decl->current->kind != TokenKindIdentifier) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorMissParamName),
                      *parse_decl->previous->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);

                if (parse_decl->pos + 1 == len__Vec(*parse_decl->tokens))
                    next_token(parse_decl);
            } else {
                name = parse_decl->current->lit;

                next_token(parse_decl);
            }

            if (is_data_type(parse_decl)) {
                start__Location(&loc_data_type,
                                parse_decl->current->loc->s_line,
                                parse_decl->current->loc->s_col);

                data_type = parse_data_type(self, parse_decl);

                end__Location(&loc_data_type,
                              parse_decl->current->loc->s_line,
                              parse_decl->current->loc->s_col);
            }

            switch (parse_decl->current->kind) {
                case TokenKindColonEq:
                    next_token(parse_decl);

                    default_value = parse_expr(self, parse_decl);

                    break;
                default:
                    break;
            }

            if (!data_type && !default_value)
                end__Location(&loc,
                              parse_decl->current->loc->e_line,
                              parse_decl->current->loc->e_col);
            else
                end__Location(&loc,
                              parse_decl->current->loc->s_line,
                              parse_decl->current->loc->s_col);

            EXPECTED_TOKEN(parse_decl, TokenKindComma, {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`,` or `:=`");

                emit__Diagnostic(err);
            });

            if (!default_value && !data_type)
                push__Vec(params, NEW(FunParamNormal, name, NULL, loc));
            else if (!default_value)
                push__Vec(
                  params,
                  NEW(FunParamNormal,
                      name,
                      NEW(Tuple, 2, data_type, copy__Location(&loc_data_type)),
                      loc));
            else if (default_value && !data_type)
                push__Vec(params,
                          NEW(FunParamDefault, name, NULL, loc, default_value));
            else
                push__Vec(
                  params,
                  NEW(FunParamDefault,
                      name,
                      NEW(Tuple, 2, data_type, copy__Location(&loc_data_type)),
                      loc,
                      default_value));
        }
    }

    return params;
}

// struct Vec<struct FunBodyItem*>*
static struct Vec *
parse_fun_body(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Vec *body = NEW(Vec, sizeof(struct FunBodyItem));

    while (parse_decl->pos + 1 < len__Vec(*parse_decl->tokens)) {
        PARSE_BODY(body);
    }

    return body;
}

static struct FunDecl *
parse_fun_declaration(struct Parser *self,
                      struct FunParseContext fun_parse_context)
{
    struct Vec *tags = NULL;
    struct Vec *generic_params = NULL;
    struct Vec *params = NULL;
    struct Tuple *return_type = NULL;
    struct Vec *body = NULL;

    if (fun_parse_context.has_tags) {
        struct ParseDecl parse = NEW(ParseDecl, fun_parse_context.tags);

        tags = parse_tags(*self, &parse);
    }

    if (fun_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, fun_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (fun_parse_context.has_params) {
        struct ParseDecl parse = NEW(ParseDecl, fun_parse_context.params);

        params = parse_fun_params(*self, &parse, false);
    }

    if (fun_parse_context.has_return_type) {
        struct ParseDecl parse = NEW(ParseDecl, fun_parse_context.return_type);
        struct Location loc = NEW(Location);
        struct DataType *dt = NULL;

        start__Location(
          &loc, parse.current->loc->s_line, parse.current->loc->s_col);

        dt = parse_data_type(*self, &parse);

        if (len__Vec(*parse.tokens) == 1)
            end__Location(
              &loc, parse.current->loc->e_line, parse.current->loc->e_col);
        else
            end__Location(
              &loc, parse.current->loc->s_line, parse.current->loc->s_col);

        return_type = NEW(Tuple, 2, dt, copy__Location(&loc));

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s =
              format("`{S}`", token_kind_to_String__Token(*parse.current));

            emit__Diagnostic(err);
        }
    }

    if (len__Vec(*fun_parse_context.body) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, fun_parse_context.body);

        body = parse_fun_body(*self, &parse);
    }

    return NEW(FunDecl,
               fun_parse_context.name,
               tags,
               generic_params,
               params,
               return_type,
               body,
               fun_parse_context.is_pub,
               fun_parse_context.is_async);
}

static struct EnumDecl *
parse_enum_declaration(struct Parser *self,
                       struct EnumParseContext enum_parse_context,
                       bool is_object)
{
    struct Vec *generic_params = NULL;
    struct DataType *type_value = NULL;
    struct Vec *variants = NULL;

    if (enum_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, enum_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (enum_parse_context.has_data_type) {
        struct ParseDecl parse = NEW(ParseDecl, enum_parse_context.data_type);

        type_value = parse_data_type(*self, &parse);

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  from__String(""),
                  None());

            err->err->s = token_kind_to_String__Token(*parse.current);

            emit__Diagnostic(err);
        }
    }

    if (len__Vec(*enum_parse_context.variants) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, enum_parse_context.variants);
        variants = NEW(Vec, sizeof(struct VariantEnum));

        while (parse.pos < len__Vec(*parse.tokens)) {
            struct Location loc = NEW(Location);
            struct String *variant_name = NULL;
            struct DataType *data_type = NULL;

            start__Location(
              &loc, parse.current->loc->s_line, parse.current->loc->s_col);

            if (parse.current->kind == TokenKindIdentifier) {
                variant_name = parse.current->lit;
                next_token(&parse);
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self->parse_block,
                      NEW(LilyError, LilyErrorMissVariantName),
                      *parse.current->loc,
                      from__String(""),
                      None());

                emit__Diagnostic(err);
            }

            if (parse.current->kind != TokenKindComma &&
                parse.pos != len__Vec(*parse.tokens)) {
                data_type = parse_data_type(*self, &parse);

                if (parse.current->kind != TokenKindComma &&
                    parse.pos != len__Vec(*parse.tokens)) {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorExpectedToken),
                          *parse.current->loc,
                          format(""),
                          None());

                    err->err->s = from__String("`,`");

                    emit__Diagnostic(err);
                } else
                    next_token(&parse);
            } else
                next_token(&parse);

            end__Location(
              &loc, parse.current->loc->s_line, parse.current->loc->s_col);

            push__Vec(variants, NEW(VariantEnum, variant_name, data_type, loc));
        }
    }

    return NEW(EnumDecl,
               enum_parse_context.name,
               generic_params,
               variants,
               type_value,
               enum_parse_context.is_pub,
               is_object,
               enum_parse_context.is_error);
}

static struct RecordDecl *
parse_record_declaration(struct Parser *self,
                         struct RecordParseContext record_parse_context,
                         bool is_object)
{
    struct Vec *generic_params = NULL;
    struct Vec *fields = NULL;

    if (record_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, record_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (len__Vec(*record_parse_context.fields) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, record_parse_context.fields);
        fields = NEW(Vec, sizeof(struct FieldRecord));

        while (parse.pos < len__Vec(*parse.tokens)) {
            struct String *field_name = NULL;
            struct DataType *data_type = NULL;
            struct Expr *value = NULL;
            bool is_pub = false;
            struct Location loc = NEW(Location);

            start__Location(
              &loc, parse.current->loc->s_line, parse.current->loc->s_col);

            if (parse.current->kind == TokenKindPubKw) {
                is_pub = true;
                next_token(&parse);
            }

            if (parse.current->kind == TokenKindIdentifier) {
                field_name = parse.current->lit;
                next_token(&parse);
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self->parse_block,
                      NEW(LilyError, LilyErrorMissFieldName),
                      *parse.current->loc,
                      format(""),
                      None());

                emit__Diagnostic(err);
            }

            if (is_data_type(&parse)) {
                data_type = parse_data_type(*self, &parse);
            } else {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self->parse_block,
                      NEW(LilyError, LilyErrorMissDataType),
                      *parse.current->loc,
                      format(""),
                      None());

                emit__Diagnostic(err);
            }

            if (parse.current->kind == TokenKindColonEq)
                value = parse_expr(*self, &parse);

            if (parse.current->kind != TokenKindComma &&
                parse.pos != len__Vec(*parse.tokens)) {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self->parse_block,
                      NEW(LilyError, LilyErrorExpectedToken),
                      *parse.current->loc,
                      format(""),
                      None());

                err->err->s = from__String("`,`");

                emit__Diagnostic(err);
            } else
                next_token(&parse);

            end__Location(
              &loc, parse.previous->loc->s_line, parse.previous->loc->s_col);

            push__Vec(
              fields,
              NEW(FieldRecord, field_name, data_type, value, is_pub, loc));
        }
    }

    return NEW(RecordDecl,
               record_parse_context.name,
               generic_params,
               fields,
               record_parse_context.is_pub,
               is_object);
}

static struct AliasDecl *
parse_alias_declaration(struct Parser *self,
                        struct AliasParseContext alias_parse_context)
{
    struct Vec *generic_params = NULL;
    struct DataType *data_type = NULL;

    if (alias_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, alias_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    {
        struct ParseDecl parse = NEW(ParseDecl, alias_parse_context.data_type);

        data_type = parse_data_type(*self, &parse);

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s = token_kind_to_String__Token(*parse.current);

            emit__Diagnostic(err);
        }
    }

    return NEW(AliasDecl,
               alias_parse_context.name,
               generic_params,
               data_type,
               alias_parse_context.is_pub);
}

static struct Vec *
parse_inheritance(struct Parser self, struct ParseDecl *parse_decl)
{
    struct Vec *inh = NEW(Vec, sizeof(struct Tuple));

    while (parse_decl->pos < len__Vec(*parse_decl->tokens)) {
        struct Location loc = NEW(Location);
        struct DataType *dt = parse_data_type(self, parse_decl);

        start__Location(&loc,
                        parse_decl->current->loc->s_line,
                        parse_decl->current->loc->s_col);

        switch (dt->kind) {
            case DataTypeKindCustom:
                push__Vec(inh, NEW(Tuple, 2, dt, copy__Location(&loc)));
                break;
            default: {
                struct Diagnostic *err =
                  NEW(DiagnosticWithErrParser,
                      &self.parse_block,
                      NEW(LilyError, LilyErrorExpectedUserDefinedDataType),
                      *parse_decl->current->loc,
                      format(""),
                      None());

                emit__Diagnostic(err);

                FREE(DataTypeAll, dt);
            }
        }

        if (parse_decl->pos != len__Vec(*parse_decl->tokens)) {
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
        }
    }

    return inh;
}

static struct TraitDecl *
parse_trait_declaration(struct Parser *self,
                        struct TraitParseContext trait_parse_context)
{
    struct Vec *generic_params = NULL;
    struct Vec *inh = NULL;
    struct Vec *body = NULL;

    if (trait_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, trait_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (trait_parse_context.has_inheritance) {
        struct ParseDecl parse =
          NEW(ParseDecl, trait_parse_context.generic_params);

        inh = parse_inheritance(*self, &parse);
    }

    if (len__Vec(*trait_parse_context.body) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, trait_parse_context.body);
        body = NEW(Vec, sizeof(struct Prototype));

        while (parse.pos < len__Vec(*parse.tokens)) {
            if (parse.current->kind == TokenKindAsyncKw ||
                parse.current->kind == TokenKindAt) {
                struct String *name = NULL;
                struct Vec *params_type = NULL;
                struct DataType *return_type = NULL;
                bool is_async = false;
                bool has_first_self_param = false;
                struct Location loc = NEW(Location);

                start__Location(
                  &loc, parse.current->loc->s_line, parse.current->loc->s_col);

                if (parse.current->kind == TokenKindAsyncKw) {
                    is_async = true;
                    next_token(&parse);
                }

                if (parse.current->kind == TokenKindAt)
                    next_token(&parse);
                else {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorExpectedToken),
                          *parse.current->loc,
                          format(""),
                          None());

                    err->err->s = from__String("`@`");

                    emit__Diagnostic(err);
                }

                if (parse.current->kind == TokenKindIdentifier) {
                    name = parse.current->lit;
                    next_token(&parse);
                } else {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorMissPrototypeName),
                          *parse.current->loc,
                          format(""),
                          None());

                    emit__Diagnostic(err);
                }

                if (parse.current->kind == TokenKindColonColon)
                    next_token(&parse);
                else {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorExpectedToken),
                          *parse.current->loc,
                          format(""),
                          None());

                    err->err->s = from__String("`::`");

                    emit__Diagnostic(err);
                }

                if (is_data_type(&parse)) {
                    params_type = NEW(Vec, sizeof(struct DataType));
                    struct DataType *f_param = parse_data_type(*self, &parse);

                    if (f_param->kind == DataTypeKindSelf)
                        has_first_self_param = true;

                    push__Vec(params_type, f_param);
                } else {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorMissDataType),
                          *parse.current->loc,
                          format(""),
                          None());

                    emit__Diagnostic(err);
                }

                while (parse.current->kind == TokenKindArrow &&
                       parse.pos < len__Vec(*parse.tokens)) {
                    next_token(&parse);
                    push__Vec(params_type, parse_data_type(*self, &parse));
                }

                return_type = pop__Vec(params_type);

                if (len__Vec(*params_type) == 0) {
                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrParser,
                          &self->parse_block,
                          NEW(LilyError, LilyErrorMissParamsPrototype),
                          *parse.current->loc,
                          format(""),
                          None());

                    emit__Diagnostic(err);
                }

                end__Location(
                  &loc, parse.current->loc->s_line, parse.current->loc->s_col);

                push__Vec(body,
                          NEW(TraitBodyItemPrototype,
                              loc,
                              NEW(Prototype,
                                  name,
                                  params_type,
                                  return_type,
                                  is_async,
                                  has_first_self_param)));
            }
        }
    }

    return NEW(TraitDecl,
               trait_parse_context.name,
               generic_params,
               inh,
               body,
               trait_parse_context.is_pub);
}

static struct Vec *
parse_impl(struct Parser self, struct ParseDecl *parse_decl)
{
    return parse_inheritance(self, parse_decl);
}

#define NEXT_CLASS_BLOCK()                                            \
    parse.pos += 1;                                                   \
    parse.current =                                                   \
      parse.pos < len__Vec(*parse.blocks)                             \
        ? ((struct ParseContext *)get__Vec(*parse.blocks, parse.pos)) \
        : NULL

static struct ClassDecl *
parse_class_declaration(struct Parser *self,
                        struct ClassParseContext class_parse_context)
{
    struct Vec *generic_params = NULL;
    struct Vec *inheritance = NULL;
    struct Vec *impl = NULL;
    struct Vec *body = NULL;

    if (class_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, class_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (class_parse_context.has_inheritance) {
        struct ParseDecl parse =
          NEW(ParseDecl, class_parse_context.inheritance);

        inheritance = parse_inheritance(*self, &parse);
    }

    if (class_parse_context.has_impl) {
        struct ParseDecl parse = NEW(ParseDecl, class_parse_context.impl);

        impl = parse_impl(*self, &parse);
    }

    if (len__Vec(*class_parse_context.body) > 0) {
        body = NEW(Vec, sizeof(struct ClassBodyItem));
        struct ParseClassBody parse =
          NEW(ParseClassBody, class_parse_context.body);

        while (parse.pos < len__Vec(*parse.blocks)) {
            switch (parse.current->kind) {
                case ParseContextKindProperty: {
                    struct PropertyDecl *prop =
                      parse_property_declaration(self, &parse);

                    push__Vec(
                      body,
                      NEW(ClassBodyItemProperty, prop, parse.current->loc));

                    break;
                }
                case ParseContextKindMethod: {
                    struct MethodDecl *method =
                      parse_method_declaration(self, &parse);

                    push__Vec(
                      body,
                      NEW(ClassBodyItemMethod, method, parse.current->loc));

                    break;
                }
                case ParseContextKindImport:
                    break;
                default:
                    UNREACHABLE("");
            }

            NEXT_CLASS_BLOCK();
        }
    }

    return NEW(ClassDecl,
               class_parse_context.name,
               generic_params,
               inheritance,
               impl,
               body,
               class_parse_context.is_pub);
}

static struct PropertyDecl *
parse_property_declaration(struct Parser *self,
                           struct ParseClassBody *parse_class)
{
    struct PropertyParseContext property_parse_context =
      parse_class->current->value.property;
    struct DataType *data_type = NULL;

    {
        struct ParseDecl parse =
          NEW(ParseDecl, property_parse_context.data_type);

        data_type = parse_data_type(*self, &parse);
    }

    return NEW(PropertyDecl,
               property_parse_context.name,
               data_type,
               property_parse_context.is_pub);
}

static struct MethodDecl *
parse_method_declaration(struct Parser *self,
                         struct ParseClassBody *parse_class)
{
    struct MethodParseContext method_parse_context =
      parse_class->current->value.method;
    struct Vec *generic_params = NULL;
    struct Vec *params = NULL;
    struct DataType *return_type = NULL;
    struct Vec *body = NULL;
    bool has_first_self_param = false;

    if (method_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, method_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (method_parse_context.has_params) {
        struct ParseDecl parse = NEW(ParseDecl, method_parse_context.params);

        params = parse_fun_params(*self, &parse, true);

        has_first_self_param =
          ((struct FunParam *)get__Vec(*params, 0))->kind == FunParamKindSelf
            ? true
            : false;
    }

    if (method_parse_context.has_return_type) {
        struct ParseDecl parse =
          NEW(ParseDecl, method_parse_context.return_type);

        return_type = parse_data_type(*self, &parse);

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s =
              format("`{S}`", token_kind_to_String__Token(*parse.current));

            emit__Diagnostic(err);
        }
    }

    if (len__Vec(*method_parse_context.body) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, method_parse_context.body);

        body = parse_fun_body(*self, &parse);
    }

    return NEW(MethodDecl,
               method_parse_context.name,
               generic_params,
               params,
               return_type,
               body,
               has_first_self_param,
               method_parse_context.is_async,
               method_parse_context.is_pub);
}

static struct ImportStmt *
parse_import_declaration(struct Parser *self,
                         struct ImportParseContext import_parse_context)
{
    return parse_import_value__parse_import_stmt(*self,
                                                 import_parse_context.value,
                                                 import_parse_context.as_value,
                                                 import_parse_context.value_loc,
                                                 import_parse_context.is_pub);
}

static struct ConstantDecl *
parse_constant_declaration(struct Parser *self,
                           struct ConstantParseContext constant_parse_context)
{
    struct DataType *data_type = NULL;
    struct Expr *expr = NULL;

    if (len__Vec(*constant_parse_context.data_type) > 0) {
        struct ParseDecl parse =
          NEW(ParseDecl, constant_parse_context.data_type);

        data_type = parse_data_type(*self, &parse);

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s = token_kind_to_String__Token(*parse.current);

            emit__Diagnostic(err);
        }
    }

    if (len__Vec(*constant_parse_context.expr) > 0) {
        struct ParseDecl parse = NEW(ParseDecl, constant_parse_context.expr);

        expr = parse_expr(*self, &parse);

        if (parse.pos + 1 < len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s = token_kind_to_String__Token(*parse.current);

            emit__Diagnostic(err);
        }
    } else {
    }

    return NEW(ConstantDecl,
               constant_parse_context.name,
               data_type,
               expr,
               constant_parse_context.is_pub);
}

static struct ErrorDecl *
parse_error_declaration(struct Parser *self,
                        struct ErrorParseContext error_parse_context)
{
    struct Vec *generic_params = NULL;
    struct DataType *data_type = NULL;

    if (error_parse_context.has_generic_params) {
        struct ParseDecl parse =
          NEW(ParseDecl, error_parse_context.generic_params);

        generic_params = parse_generic_params(*self, &parse);
    }

    if (error_parse_context.has_data_type) {
        struct ParseDecl parse = NEW(ParseDecl, error_parse_context.data_type);

        data_type = parse_data_type(*self, &parse);

        if (parse.pos != len__Vec(*parse.tokens)) {
            struct Diagnostic *err =
              NEW(DiagnosticWithErrParser,
                  &self->parse_block,
                  NEW(LilyError, LilyErrorUnexpectedToken),
                  *parse.current->loc,
                  format(""),
                  None());

            err->err->s = token_kind_to_String__Token(*parse.current);

            emit__Diagnostic(err);
        }
    }

    return NEW(ErrorDecl,
               error_parse_context.name,
               generic_params,
               data_type,
               error_parse_context.is_pub);
}

static struct ModuleDecl *
parse_module_declaration(struct Parser *self,
                         struct ModuleParseContext module_parse_context)
{
    struct Vec *body = NULL;

    if (len__Vec(*module_parse_context.body) > 0) {
        body = NEW(Vec, sizeof(struct ModuleBodyItem));
        Usize pos = 0;
        struct ParseContext *current = get__Vec(*module_parse_context.body, 0);

        while (pos < len__Vec(*module_parse_context.body)) {
            switch (current->kind) {
                case ParseContextKindFun:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclFun,
                                      current->loc,
                                      parse_fun_declaration(
                                        self, current->value.fun))));
                    break;

                case ParseContextKindEnumObject:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclEnum,
                                      current->loc,
                                      parse_enum_declaration(
                                        self, current->value.enum_, true))));
                    break;
                case ParseContextKindEnum:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclEnum,
                                      current->loc,
                                      parse_enum_declaration(
                                        self, current->value.enum_, false))));
                    break;

                case ParseContextKindRecordObject:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclRecord,
                                      current->loc,
                                      parse_record_declaration(
                                        self, current->value.record, true))));
                    break;
                case ParseContextKindRecord:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclRecord,
                                      current->loc,
                                      parse_record_declaration(
                                        self, current->value.record, false))));
                    break;

                case ParseContextKindAlias:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclAlias,
                                      current->loc,
                                      parse_alias_declaration(
                                        self, current->value.alias))));
                    break;

                case ParseContextKindTrait:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclTrait,
                                      current->loc,
                                      parse_trait_declaration(
                                        self, current->value.trait))));
                    break;

                case ParseContextKindClass:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclClass,
                                      current->loc,
                                      parse_class_declaration(
                                        self, current->value.class))));
                    break;

                case ParseContextKindImport:
                    break;

                case ParseContextKindConstant:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclConstant,
                                      current->loc,
                                      parse_constant_declaration(
                                        self, current->value.constant))));
                    break;

                case ParseContextKindError:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclError,
                                      current->loc,
                                      parse_error_declaration(
                                        self, current->value.error))));
                    break;

                case ParseContextKindModule:
                    push__Vec(body,
                              NEW(ModuleBodyItemDecl,
                                  NEW(DeclModule,
                                      current->loc,
                                      parse_module_declaration(
                                        self, current->value.module))));
                    break;

                default:
                    break;
            }

            pos += 1;
            current = pos < len__Vec(*module_parse_context.body)
                        ? get__Vec(*module_parse_context.body, 1)
                        : NULL;
        }
    }

    return NEW(
      ModuleDecl, module_parse_context.name, body, module_parse_context.is_pub);
}

static void
parse_declaration(struct Parser *self)
{
    switch (self->current->kind) {
        case ParseContextKindFun:
            push__Vec(
              self->decls,
              NEW(DeclFun,
                  self->current->loc,
                  parse_fun_declaration(self, self->current->value.fun)));
            break;

        case ParseContextKindEnumObject:
            push__Vec(self->decls,
                      NEW(DeclEnum,
                          self->current->loc,
                          parse_enum_declaration(
                            self, self->current->value.enum_, true)));
            break;
        case ParseContextKindEnum:
            push__Vec(self->decls,
                      NEW(DeclEnum,
                          self->current->loc,
                          parse_enum_declaration(
                            self, self->current->value.enum_, false)));
            break;

        case ParseContextKindRecordObject:
            push__Vec(self->decls,
                      NEW(DeclRecord,
                          self->current->loc,
                          parse_record_declaration(
                            self, self->current->value.record, true)));
            break;
        case ParseContextKindRecord:
            push__Vec(self->decls,
                      NEW(DeclRecord,
                          self->current->loc,
                          parse_record_declaration(
                            self, self->current->value.record, false)));
            break;

        case ParseContextKindAlias:
            push__Vec(
              self->decls,
              NEW(DeclAlias,
                  self->current->loc,
                  parse_alias_declaration(self, self->current->value.alias)));
            break;

        case ParseContextKindTrait:
            push__Vec(
              self->decls,
              NEW(DeclTrait,
                  self->current->loc,
                  parse_trait_declaration(self, self->current->value.trait)));
            break;

        case ParseContextKindClass:
            push__Vec(
              self->decls,
              NEW(DeclClass,
                  self->current->loc,
                  parse_class_declaration(self, self->current->value.class)));
            break;

        case ParseContextKindImport:
            push__Vec(
              self->decls,
              NEW(DeclImport,
                  self->current->loc,
                  parse_import_declaration(self, self->current->value.import)));
            break;

        case ParseContextKindConstant:
            push__Vec(self->decls,
                      NEW(DeclConstant,
                          self->current->loc,
                          parse_constant_declaration(
                            self, self->current->value.constant)));
            break;

        case ParseContextKindError:
            push__Vec(
              self->decls,
              NEW(DeclError,
                  self->current->loc,
                  parse_error_declaration(self, self->current->value.error)));
            break;

        case ParseContextKindModule:
            push__Vec(
              self->decls,
              NEW(DeclModule,
                  self->current->loc,
                  parse_module_declaration(self, self->current->value.module)));
            break;

        default:
            break;
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

    if (count_error > 0) {
        emit__Summary(
          count_error, count_warning, "the parser phase has been failed");
        exit(1);
    }
}

void
__free__Parser(struct Parser self)
{
    FREE(ParseBlock, self.parse_block);

    for (Usize i = len__Vec(*self.decls); i--;)
        FREE(DeclAll, get__Vec(*self.decls, i));

    FREE(Vec, self.decls);
}
