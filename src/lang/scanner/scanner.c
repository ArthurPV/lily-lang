#include <string.h>

#include <base/file.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/option.h>
#include <base/result.h>
#include <base/types.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/diagnostic/summary.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

#define IS_DIGIT \
    '0'          \
      : case '1' \
      : case '2' \
      : case '3' \
      : case '4' : case '5' : case '6' : case '7' : case '8' : case '9'

#define IS_ID    \
    'a'          \
      : case 'b' \
      : case 'c' \
      : case 'd' \
      : case 'e' \
      : case 'f' \
      : case 'g' \
      : case 'h' \
      : case 'i' \
      : case 'j' \
      : case 'k' \
      : case 'l' \
      : case 'm' \
      : case 'n' \
      : case 'o' \
      : case 'p' \
      : case 'q' \
      : case 'r' \
      : case 's' \
      : case 't' \
      : case 'u' \
      : case 'v' \
      : case 'w' \
      : case 'x' \
      : case 'y' \
      : case 'z' \
      : case 'A' \
      : case 'B' \
      : case 'C' \
      : case 'D' \
      : case 'E' \
      : case 'F' \
      : case 'G' \
      : case 'H' \
      : case 'I' \
      : case 'J' \
      : case 'K' \
      : case 'L' \
      : case 'M' \
      : case 'N' \
      : case 'O' \
      : case 'P' \
      : case 'Q' \
      : case 'R' \
      : case 'S' \
      : case 'T' \
      : case 'U' \
      : case 'V' : case 'W' : case 'X' : case 'Y' : case 'Z' : case '_'

static enum TokenKind
get_keyword(const Str id);

static inline void
next_char(struct Scanner *self);
static inline void
skip_space(struct Scanner *self);
static inline void
jump(struct Scanner *self, Usize n);
static inline void
previous_char(struct Scanner *self);
static inline void
start_token(struct Scanner *self);
static inline void
end_token(struct Scanner *self);
static inline char *
peek_char(struct Scanner self, Usize n);
static void
next_char_by_token(struct Scanner *self, struct Token tok);
static inline void
push_token(struct Scanner *self, struct Token *tok);
static inline bool
is_digit(struct Scanner self);
static inline bool
is_ident(struct Scanner self);
static inline bool
is_hex(struct Scanner self);
static inline bool
is_oct(struct Scanner self);
static inline bool
is_bin(struct Scanner self);
static inline bool
is_num(struct Scanner self);
static struct Diagnostic *
__new__DiagnosticWithErrScanner(struct Scanner *self,
                                struct LilyError *err,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help);
static struct Diagnostic *
__new__DiagnosticWithWarnScanner(struct Scanner *self,
                                 struct LilyWarning *warn,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help);
static struct Diagnostic *
__new__DiagnosticWithNoteScanner(struct Scanner *self,
                                 struct String *note,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help);
static inline void
expected_char(struct Scanner *self, struct Diagnostic *dgn, char *expected);
static struct Result *
get_escape(struct Scanner *self, char *previous);
static enum TokenKind
scan_comment_one(struct Scanner *self);
static struct Result *
scan_comment_multi(struct Scanner *self);
static struct String *
scan_comment_doc(struct Scanner *self);
static struct String *
scan_identifier(struct Scanner *self);
static struct Result *
scan_char(struct Scanner *self, bool is_bit);
static struct Result *
scan_string(struct Scanner *self);
static struct Result *
scan_hex(struct Scanner *self);
static struct Result *
scan_oct(struct Scanner *self);
static struct Result *
scan_bin(struct Scanner *self);
static struct Result *
scan_num(struct Scanner *self);
static struct Result *
get_all_num(struct Scanner *self);
static inline bool
skip_and_verify(struct Scanner *self, const char *target);
static struct Result *
get_closing(struct Scanner *self, char *target);
static struct Result *
get_token(struct Scanner *self);

struct File
__new__File(Str name)
{
    struct Path *path = NEW(Path, name);

    struct File self = { .name = name, .content = read_file__Path(*path) };

    FREE(Path, path);

    return self;
}

void
__free__File(struct File self)
{
    FREE(String, self.content);
}

struct Source
__new__Source(struct File file)
{
    struct Source self = { .file = file, .pos = 0 };

    if (len__String(*file.content) > 0)
        self.c = get__String(*file.content, 0);
    else
        self.c = (char *)'\0';

    return self;
}

void
__free__Source(struct Source self)
{
    FREE(File, self.file);
}

struct Scanner
__new__Scanner(struct Source *src)
{
    struct Scanner self = { .src = src,
                            .line = 1,
                            .col = 1,
                            .loc = NEW(Location),
                            .tokens = NEW(Vec, sizeof(struct Token)),
                            .count_error = 0 };

    return self;
}

static enum TokenKind
get_keyword(const Str id)
{
    if (!strcmp(id, "fun"))
        return TokenKindFunKw;
    if (!strcmp(id, "import"))
        return TokenKindImportKw;
    if (!strcmp(id, "as"))
        return TokenKindAsKw;
    if (!strcmp(id, "pub"))
        return TokenKindPubKw;
    if (!strcmp(id, "end"))
        return TokenKindEndKw;
    if (!strcmp(id, "test"))
        return TokenKindTestKw;
    if (!strcmp(id, "module"))
        return TokenKindModuleKw;
    if (!strcmp(id, "type"))
        return TokenKindTypeKw;
    if (!strcmp(id, "alias"))
        return TokenKindAliasKw;
    if (!strcmp(id, "record"))
        return TokenKindRecordKw;
    if (!strcmp(id, "tag"))
        return TokenKindTagKw;
    if (!strcmp(id, "enum"))
        return TokenKindEnumKw;
    if (!strcmp(id, "match"))
        return TokenKindMatchKw;
    if (!strcmp(id, "true"))
        return TokenKindTrueKw;
    if (!strcmp(id, "false"))
        return TokenKindFalseKw;
    if (!strcmp(id, "if"))
        return TokenKindIfKw;
    if (!strcmp(id, "elif"))
        return TokenKindElifKw;
    if (!strcmp(id, "else"))
        return TokenKindElseKw;
    if (!strcmp(id, "do"))
        return TokenKindDoKw;
    if (!strcmp(id, "while"))
        return TokenKindWhileKw;
    if (!strcmp(id, "for"))
        return TokenKindForKw;
    if (!strcmp(id, "error"))
        return TokenKindErrorKw;
    if (!strcmp(id, "class"))
        return TokenKindClassKw;
    if (!strcmp(id, "trait"))
        return TokenKindTraitKw;
    if (!strcmp(id, "async"))
        return TokenKindAsyncKw;
    if (!strcmp(id, "await"))
        return TokenKindAwaitKw;
    if (!strcmp(id, "and"))
        return TokenKindAndKw;
    if (!strcmp(id, "xor"))
        return TokenKindXorKw;
    if (!strcmp(id, "or"))
        return TokenKindOrKw;
    if (!strcmp(id, "not"))
        return TokenKindNotKw;
    if (!strcmp(id, "nil"))
        return TokenKindNilKw;
    if (!strcmp(id, "undef"))
        return TokenKindUndefKw;
    if (!strcmp(id, "object"))
        return TokenKindObjectKw;
    if (!strcmp(id, "self"))
        return TokenKindSelfKw;
    if (!strcmp(id, "try"))
        return TokenKindTryKw;
    if (!strcmp(id, "catch"))
        return TokenKindCatchKw;
    if (!strcmp(id, "return"))
        return TokenKindReturnKw;
    if (!strcmp(id, "next"))
        return TokenKindNextKw;
    if (!strcmp(id, "break"))
        return TokenKindBreakKw;
    if (!strcmp(id, "asm"))
        return TokenKindAsmKw;
    if (!strcmp(id, "macro"))
        return TokenKindMacroKw;
    if (!strcmp(id, "impl"))
        return TokenKindImplKw;
    if (!strcmp(id, "in"))
        return TokenKindInKw;
    if (!strcmp(id, "mut"))
        return TokenKindMutKw;
    if (!strcmp(id, "begin"))
        return TokenKindBeginKw;
    return TokenKindIdentifier;
}

static inline void
next_char(struct Scanner *self)
{
    if (self->src->pos < len__String(*self->src->file.content) - 1) {
        if (self->src->c == (char *)'\n') {
            self->line++;
            self->col = 1;
        } else {
            self->col++;
        }
        self->src->pos++;
        self->src->c = get__String(*self->src->file.content, self->src->pos);
    }
}

static inline void
skip_space(struct Scanner *self)
{
    while ((self->src->c == (char *)'\n' || self->src->c == (char *)'\t' ||
            self->src->c == (char *)'\r' || self->src->c == (char *)' ') &&
           self->src->pos < len__String(*self->src->file.content) - 1) {
        next_char(self);
    }
}

static inline void
jump(struct Scanner *self, Usize n)
{
    for (; n--;)
        next_char(self);
}

static inline void
previous_char(struct Scanner *self)
{
    self->col--;
    self->src->pos--;
    self->src->c = get__String(*self->src->file.content, self->src->pos);
}

static inline void
start_token(struct Scanner *self)
{
    self->loc.s_line = self->line;
    self->loc.s_col = self->col;
}

static inline void
end_token(struct Scanner *self)
{
    self->loc.e_line = self->line;
    self->loc.e_col = self->col;
}

static inline char *
peek_char(struct Scanner self, Usize n)
{
    if (self.src->pos + n < len__String(*self.src->file.content) - 1) {
        return get__String(*self.src->file.content, self.src->pos + n);
    }
    return NULL;
}

static void
next_char_by_token(struct Scanner *self, struct Token tok)
{
    switch (tok.kind) {
        case TokenKindOneComment:
        case TokenKindMultiComment:
        case TokenKindDocComment:
            return;
        case TokenKindNotEq:
        case TokenKindXorEq:
        case TokenKindLParen:
        case TokenKindLBrace:
        case TokenKindLHook:
        case TokenKindRParen:
        case TokenKindRBrace:
        case TokenKindRHook:
        case TokenKindFunKw:
        case TokenKindImportKw:
        case TokenKindAsKw:
        case TokenKindPubKw:
        case TokenKindEndKw:
        case TokenKindTestKw:
        case TokenKindModuleKw:
        case TokenKindTypeKw:
        case TokenKindAliasKw:
        case TokenKindRecordKw:
        case TokenKindTagKw:
        case TokenKindEnumKw:
        case TokenKindMatchKw:
        case TokenKindTrueKw:
        case TokenKindFalseKw:
        case TokenKindIfKw:
        case TokenKindElifKw:
        case TokenKindElseKw:
        case TokenKindDoKw:
        case TokenKindWhileKw:
        case TokenKindForKw:
        case TokenKindErrorKw:
        case TokenKindClassKw:
        case TokenKindTraitKw:
        case TokenKindAsyncKw:
        case TokenKindAwaitKw:
        case TokenKindAndKw:
        case TokenKindXorKw:
        case TokenKindOrKw:
        case TokenKindNotKw:
        case TokenKindNilKw:
        case TokenKindUndefKw:
        case TokenKindObjectKw:
        case TokenKindSelfKw:
        case TokenKindTryKw:
        case TokenKindCatchKw:
        case TokenKindReturnKw:
        case TokenKindNextKw:
        case TokenKindBreakKw:
        case TokenKindAsmKw:
        case TokenKindMacroKw:
        case TokenKindImplKw:
        case TokenKindInKw:
        case TokenKindMutKw:
        case TokenKindBeginKw:
        case TokenKindStringLit:
        case TokenKindCharLit:
        case TokenKindBitStringLit:
        case TokenKindBitCharLit:
        case TokenKindIntLit:
        case TokenKindFloatLit:
        case TokenKindIdentifier:
        case TokenKindIdentifierOp:
            next_char(self);
            return;

        default: {
            struct String *tok_string = token_kind_to_string__Token(tok);

            jump(self, len__String(*tok_string));

            FREE(String, tok_string);

            return;
        }
    }
}

static inline void
push_token(struct Scanner *self, struct Token *tok)
{
    push__Vec(self->tokens, tok);
}

static inline bool
is_digit(struct Scanner self)
{
    return (self.src->c >= (char *)'0' && self.src->c <= (char *)'9') ||
           self.src->c == (char *)'_';
}

static inline bool
is_ident(struct Scanner self)
{
    return (self.src->c >= (char *)'a' && self.src->c <= (char *)'z') ||
           (self.src->c >= (char *)'A' && self.src->c <= (char *)'Z') ||
           self.src->c == (char *)'_' || is_digit(self);
}

static inline bool
is_hex(struct Scanner self)
{
    return is_digit(self) ||
           (self.src->c >= (char *)'a' && self.src->c <= (char *)'f') ||
           (self.src->c >= (char *)'A' && self.src->c <= (char *)'F');
}

static inline bool
is_oct(struct Scanner self)
{
    return (self.src->c >= (char *)'0' && self.src->c <= (char *)'7') ||
           self.src->c == (char *)'_';
}

static inline bool
is_bin(struct Scanner self)
{
    return (self.src->c >= (char *)'0' && self.src->c <= (char *)'1') ||
           self.src->c == (char *)'_';
}

static inline bool
is_num(struct Scanner self)
{
    return is_digit(self) ||
           (self.src->c == (char *)'.' && peek_char(self, 1) != (char *)'.') ||
           self.src->c == (char *)'e' || self.src->c == (char *)'E';
}

static struct Diagnostic *
__new__DiagnosticWithErrScanner(struct Scanner *self,
                                struct LilyError *err,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help)
{
    self->count_error += 1;
    return NEW(DiagnosticWithErr, err, loc, self->src->file, detail_msg, help);
}

static struct Diagnostic *
__new__DiagnosticWithWarnScanner(struct Scanner *self,
                                 struct LilyWarning *warn,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help)
{
    return NEW(
      DiagnosticWithWarn, warn, loc, self->src->file, detail_msg, help);
}

static struct Diagnostic *
__new__DiagnosticWithNoteScanner(struct Scanner *self,
                                 struct String *note,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help)
{
    return NEW(
      DiagnosticWithNote, note, loc, self->src->file, detail_msg, help);
}

static inline void
expected_char(struct Scanner *self, struct Diagnostic *dgn, char *expected)
{
    if (expected == (char *)'\\' && self->src->c == (char *)'n')
        next_char(self);
    else
        emit__Diagnostic(dgn);
}

static struct Result *
get_escape(struct Scanner *self, char *previous)
{
    struct Result *res = NULL;
    struct Location loc_error = NEW(Location);

    start__Location(&loc_error, self->line, self->col);

    if (previous == (char *)'\\' && self->src->c == (char *)'n')
        res = Ok(from__String("\\n"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'t')
        res = Ok(from__String("\\t"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'r')
        res = Ok(from__String("\\r"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'b')
        res = Ok(from__String("\\b"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'\\')
        res = Ok(from__String("\\"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'\'')
        res = Ok(from__String("\'"));
    else if (previous == (char *)'\\' && self->src->c == (char *)'\"')
        res = Ok(from__String("\""));
    else if (previous == (char *)'\\' &&
             len__String(*self->src->file.content) < self->src->pos - 1) {
        end__Location(&loc_error, self->line, self->col);

        res = Err(NEW(DiagnosticWithErrScanner,
                      self,
                      NEW(LilyError, LilyErrorUnclosedChar),
                      loc_error,
                      format(""),
                      None()));
    } else if (previous == (char *)'\\') {
        end__Location(&loc_error, self->line, self->col);

        res = Err(NEW(DiagnosticWithErrScanner,
                      self,
                      NEW(LilyErrorWithString,
                          LilyErrorInvalidEscape,
                          format("{c}", (UPtr)self->src->c)),
                      loc_error,
                      format(""),
                      None()));
    } else
        res = Ok(format("{c}", (UPtr)previous));

    if (previous == (char *)'\\')
        next_char(self);

    return res;
}

static enum TokenKind
scan_comment_one(struct Scanner *self)
{
    while (self->src->c != (char *)'\n') {
        next_char(self);
    }

    return TokenKindOneComment;
}

static struct Result *
scan_comment_multi(struct Scanner *self)
{
    struct Location location_error = NEW(Location);

    start__Location(&location_error, self->line, self->col);

    while (self->src->c != (char *)'*' || peek_char(*self, 1) != (char *)'.') {
        if (self->src->pos >= len__String(*self->src->file.content) - 2) {
            end__Location(&location_error, self->line, self->col);

            return Err(NEW(DiagnosticWithErrScanner,
                           self,
                           NEW(LilyError, LilyErrorUnclosedCommentMultiLine),
                           location_error,
                           format(""),
                           None()));
        }
        next_char(self);
    }

    jump(self, 2);
    return Ok((int *)TokenKindMultiComment);
}

static struct String *
scan_comment_doc(struct Scanner *self)
{
    struct String *doc = NEW(String);

    if (self->src->c == (char *)' ') {
        next_char(self);
    }

    while (self->src->c != (char *)'\n') {
        next_char(self);

        if (self->src->pos >= len__String(*self->src->file.content) - 1)
            break;

        push__String(
          doc,
          (char *)get__String(*self->src->file.content, self->src->pos - 1));
    }

    previous_char(self);

    return doc;
}

static struct String *
scan_identifier(struct Scanner *self)
{
    struct String *id = NEW(String);

    while (is_ident(*self)) {
        next_char(self);
        push__String(id,
                     get__String(*self->src->file.content, self->src->pos - 1));
    }

    previous_char(self);

    return id;
}

static struct Result *
scan_char(struct Scanner *self, bool is_bit)
{
    struct Location loc_error = NEW(Location);

    start__Location(&loc_error, self->line, self->col);
    next_char(self);

    if (self->src->c != (char *)'\'') {
        next_char(self);

        char *target = self->src->c;
        struct Result *escape = get_escape(
          self, get__String(*self->src->file.content, self->src->pos - 1));

        end__Location(&loc_error, self->line, self->col);

        if (target != (char *)'\'' && self->src->c != (char *)'\'') {
            return Err(
              NEW(DiagnosticWithErrScanner,
                  self,
                  NEW(LilyError, LilyErrorUnclosedCharLiteral),
                  loc_error,
                  format("this token is unexpected: `{c}`", (UPtr)self->src->c),
                  Some(format("replace `{c}` by `'`", (UPtr)self->src->c))));
        }

        struct String *escape_ok = NULL;

        if (is_ok__Result(*escape))
            escape_ok = (struct String *)get_ok__Result(*escape);
        else
            return escape;

        FREE(Result, escape);

        if (is_bit)
            return Ok(NEW(TokenLit, TokenKindBitCharLit, NULL, escape_ok));

        return Ok(NEW(TokenLit, TokenKindCharLit, NULL, escape_ok));
    } else {
        end__Location(&loc_error, self->line, self->col);

        return Err(NEW(DiagnosticWithErrScanner,
                       self,
                       NEW(LilyError, LilyErrorInvalidCharLiteral),
                       loc_error,
                       format("unexpected token here: `'`"),
                       None()));
    }
}

static struct Result *
scan_string(struct Scanner *self)
{
    struct Location location_error = NEW(Location);
    struct String *s = NEW(String);

    start__Location(&location_error, self->line, self->col);
    next_char(self);

    while ((UPtr)self->src->c != '\"') {
        if (self->src->pos > len__String(*self->src->file.content) - 2) {
            end__Location(&location_error, self->line, self->col);

            return Err(
              NEW(DiagnosticWithErrScanner,
                  self,
                  NEW(LilyError, LilyErrorUnclosedStringLiteral),
                  location_error,
                  format(""),
                  Some(format("add `\"` at the end of string literal"))));
        }

        next_char(self);

        struct Result *escape = get_escape(
          self, get__String(*self->src->file.content, self->src->pos - 1));

        if (is_err__Result(*escape))
            return escape;

        append__String(s, get_ok__Result(*escape), true);

        FREE(Result, escape);
    }

    return Ok(s);
}

static struct Result *
scan_hex(struct Scanner *self)
{
    struct Location loc_error = NEW(Location);
    struct String *hex = from__String("0x");

    start__Location(&loc_error, self->line, self->col);
    jump(self, 2);

    while (is_hex(*self)) {
        if ((UPtr)self->src->c != '_')
            push__String(hex, self->src->c);

        next_char(self);
    }

    Str hex_str = to_Str__String(*hex);

    if (!strcmp(hex_str, "0x")) {
        end__Location(&loc_error, self->line, self->col - 1);

        free(hex_str);

        return Err(NEW(DiagnosticWithErrScanner,
                       self,
                       NEW(LilyError, LilyErrorInvalidHexLiteral),
                       loc_error,
                       format(""),
                       Some(format("add character between `a` (`A`) and `f` "
                                   "(`F`) or `0` and `9` (ex: `0xff`)"))));
    }

    previous_char(self);

    free(hex_str);

    return Ok(NEW(TokenLit, TokenKindIntLit, NULL, hex));
}

static struct Result *
scan_oct(struct Scanner *self)
{
    struct Location loc_error = NEW(Location);
    struct String *oct = from__String("0o");

    start__Location(&loc_error, self->line, self->col);
    jump(self, 2);

    while (is_oct(*self)) {
        if ((UPtr)self->src->c != '_')
            push__String(oct, self->src->c);

        next_char(self);
    }

    Str oct_str = to_Str__String(*oct);

    if (!strcmp(oct_str, "0o")) {
        end__Location(&loc_error, self->line, self->col);

        free(oct_str);

        return Err(
          NEW(DiagnosticWithErrScanner,
              self,
              NEW(LilyError, LilyErrorInvalidOctLiteral),
              loc_error,
              format(""),
              Some(format("add character between `0` and `7` (ex: `0o773`)"))));
    }

    previous_char(self);

    free(oct_str);

    return Ok(NEW(TokenLit, TokenKindIntLit, NULL, oct));
}

static struct Result *
scan_bin(struct Scanner *self)
{

    struct Location loc_error = NEW(Location);
    struct String *bin = from__String("0b");

    start__Location(&loc_error, self->line, self->col);
    jump(self, 2);

    while (is_bin(*self)) {
        if ((UPtr)self->src->c != '_')
            push__String(bin, self->src->c);

        next_char(self);
    }

    Str bin_str = to_Str__String(*bin);

    if (!strcmp(bin_str, "0b")) {
        end__Location(&loc_error, self->line, self->col);

        free(bin_str);

        return Err(NEW(
          DiagnosticWithErrScanner,
          self,
          NEW(LilyError, LilyErrorInvalidBinLiteral),
          loc_error,
          format(""),
          Some(format("add character between `0` and `1` (ex: `0b0011`)"))));
    }

    previous_char(self);

    free(bin_str);

    return Ok(NEW(TokenLit, TokenKindIntLit, NULL, bin));
}

static struct Result *
scan_num(struct Scanner *self)
{
    struct Location num_location = NEW(Location);
    struct String *num = NEW(String);
    bool is_float = false;

    start__Location(&num_location, self->line, self->col);

    while (is_num(*self)) {
        if (self->src->c == (char *)'e' || self->src->c == (char *)'E') {
            push__String(num, self->src->c);
            next_char(self);

            if (self->src->c == (char *)'-' || self->src->c == (char *)'+' ||
                is_digit(*self)) {
                push__String(num, self->src->c);
                next_char(self);

                is_float = true;
            } else {
                end__Location(&num_location, self->line, self->col);

                return Err(
                  NEW(DiagnosticWithErrScanner,
                      self,
                      NEW(LilyError, LilyErrorInvalidNumLiteral),
                      num_location,
                      format(""),
                      Some(format("add digit or `-` or `+` after `e` or `E` "
                                  "scientific number indicator"))));
            }
        } else if (self->src->c == (char *)'.' && is_float) {
            end__Location(&num_location, self->line, self->col);

            return Err(NEW(
              DiagnosticWithErrScanner,
              self,
              NEW(LilyError, LilyErrorInvalidNumLiteral),
              num_location,
              format("you can't have more than one point on a float literal"),
              Some(format("remove `.`"))));
        } else if (self->src->c == (char *)'.' && !is_float) {
            is_float = true;

            push__String(num, self->src->c);
            next_char(self);
        } else {
            if (self->src->c != (char *)'_')
                push__String(num, self->src->c);
            next_char(self);
        }
    }

    end__Location(&num_location, self->line, self->col);
    previous_char(self);

    if (is_float) {
        if (self->src->c == (char *)'-' || self->src->c == (char *)'+') {
            return Err(NEW(
              DiagnosticWithErrScanner,
              self,
              NEW(LilyError, LilyErrorInvalidNumLiteral),
              num_location,
              format(""),
              Some(format("add digit after `-` or `+` in scientific number"))));
        }

        return Ok(NEW(TokenLit, TokenKindFloatLit, NULL, num));
    }

    return Ok(NEW(TokenLit, TokenKindIntLit, NULL, num));
}

static struct Result *
get_all_num(struct Scanner *self)
{
    struct Result *res = NULL;

    if (self->src->c == (char *)'0' && peek_char(*self, 1) == (char *)'x')
        res = scan_hex(self);
    else if (self->src->c == (char *)'0' && peek_char(*self, 1) == (char *)'o')
        res = scan_oct(self);
    else if (self->src->c == (char *)'0' && peek_char(*self, 1) == (char *)'b')
        res = scan_bin(self);
    else
        res = scan_num(self);

    return res;
}

static inline bool
skip_and_verify(struct Scanner *self, const char *target)
{
    skip_space(self);
    return self->src->c != target;
}

static struct Result *
get_closing(struct Scanner *self, char *target)
{
    skip_space(self);

    struct Location loc = self->loc;

    while (skip_and_verify(self, target)) {
        if (self->src->pos >= len__String(*self->src->file.content) - 1) {
            return Err(
              NEW(DiagnosticWithErrScanner,
                  self,
                  NEW(LilyError, LilyErrorUnmatchedClosing),
                  loc,
                  format(""),
                  Some(format("consider add closing: `)`, `}` or `]`"))));
        }

        struct Result *tok = get_token(self);

        if (is_err__Result(*tok))
            return tok;

        struct Token *token_ok = get_ok__Result(*tok);

        end_token(self);
        next_char_by_token(self, *token_ok);

        if (token_ok->loc == NULL) {
            struct Location *copy = copy__Location(&self->loc);

            token_ok->loc = copy;
        }

        push_token(self, token_ok);

        FREE(Result, tok);
    }

    start_token(self);

    switch ((UPtr)target) {
        case ')':
            return Ok(NEW(Token, TokenKindRParen, NULL));

        case '}':
            return Ok(NEW(Token, TokenKindRBrace, NULL));

        case ']':
            return Ok(NEW(Token, TokenKindRHook, NULL));

        default:
            UNREACHABLE("unknown target");
    }
}

static struct Result *
get_token(struct Scanner *self)
{
    char *c2 = peek_char(*self, 1);
    char *c3 = peek_char(*self, 2);
    enum TokenKind kind = TokenKindEof;

    start_token(self);

    switch ((UPtr)self->src->c) {
        case '.':
            if (c2 == (char *)'.' && c3 == (char *)'.')
                kind = TokenKindDotDotDot;
            else if (c2 == (char *)'.')
                kind = TokenKindDotDot;
            else if (c2 == (char *)'*')
                kind = TokenKindDotStar;
            else if (c2 == (char *)'?')
                kind = TokenKindDotInterrogation;
            else
                kind = TokenKindDot;
            break;

        case ',':
            kind = TokenKindComma;
            break;

        case ':':
            if (c2 == (char *)'=')
                kind = TokenKindColonEq;
            else if (c2 == (char *)':')
                kind = TokenKindColonColon;
            else
                kind = TokenKindColon;
            break;

        case '|':
            if (c2 == (char *)'>')
                kind = TokenKindBarRShift;
            else if (c2 == (char *)'=')
                kind = TokenKindBarEq;
            else
                kind = TokenKindBar;
            break;

        case '@':
            kind = TokenKindAt;
            break;

        case '(': {
            struct Token *tok = NEW(Token, TokenKindLParen, NULL);

            end_token(self);
            tok->loc = copy__Location(&self->loc);
            next_char_by_token(self, *tok);
            push_token(self, tok);

            return get_closing(self, (char *)')');
        }

        case ')': {
            end_token(self);

            return Err(NEW(DiagnosticWithErrScanner,
                           self,
                           NEW(LilyError, LilyErrorMismatchedClosingDelimiter),
                           self->loc,
                           format(""),
                           Some(format("remove this `)`"))));
        }

        case '{': {
            struct Token *tok = NEW(Token, TokenKindLBrace, NULL);

            end_token(self);
            tok->loc = copy__Location(&self->loc);
            next_char_by_token(self, *tok);
            push_token(self, tok);

            return get_closing(self, (char *)'}');
        }

        case '}': {
            end_token(self);

            return Err(NEW(DiagnosticWithErrScanner,
                           self,
                           NEW(LilyError, LilyErrorMismatchedClosingDelimiter),
                           self->loc,
                           format(""),
                           Some(format("remove this `}`"))));
        }

        case '[': {
            struct Token *tok = NEW(Token, TokenKindLHook, NULL);

            end_token(self);
            tok->loc = copy__Location(&self->loc);
            next_char_by_token(self, *tok);
            push_token(self, tok);

            return get_closing(self, (char *)']');
        }

        case ']': {
            end_token(self);

            return Err(NEW(DiagnosticWithErrScanner,
                           self,
                           NEW(LilyError, LilyErrorMismatchedClosingDelimiter),
                           self->loc,
                           format(""),
                           Some(format("remove this `]`"))));
        }

        case '#':
            kind = TokenKindHashtag;
            break;

        case ';':
            kind = TokenKindSemicolon;
            break;

        case '$':
            kind = TokenKindDollar;
            break;

        case '`': {
            struct String *id = NEW(String);

            next_char(self);

            while (self->src->c != (char *)'`' &&
                   self->src->pos !=
                     len__String(*self->src->file.content) - 1) {
                push__String(id, self->src->c);
                next_char(self);
            }

            if (self->src->pos == len__String(*self->src->file.content) - 1) {
                assert(0 && "error");
            }

            Str id_str = to_Str__String(*id);
            enum TokenKind tok_kw = get_keyword(id_str);

            end_token(self);

            free(id_str);

            switch (tok_kw) {
                case TokenKindIdentifier: {
                    struct Location *copy = copy__Location(&self->loc);

                    return Ok(NEW(TokenLit, TokenKindIdentifierOp, copy, id));
                }
                default: {
                    end_token(self);

                    struct Diagnostic *dgn =
                      NEW(DiagnosticWithErrScanner,
                          self,
                          NEW(LilyError,
                              LilyErrorOperatorIdentifierCannotBeAKeyword),
                          self->loc,
                          format(""),
                          None());

                    return Err(dgn);
                }
            }

            break;
        }

        case '+':
            if (c2 == (char *)'+' && c3 == (char *)'=')
                kind = TokenKindPlusPlusEq;
            else if (c2 == (char *)'=')
                kind = TokenKindPlusEq;
            else if (c2 == (char *)'+')
                kind = TokenKindPlusPlus;
            else
                kind = TokenKindPlus;
            break;

        case '~':
            if (c2 == (char *)'=')
                kind = TokenKindWaveEq;
            else
                kind = TokenKindWave;
            break;

        case '-':
            if (c2 == (char *)'-' && c3 == (char *)'=')
                kind = TokenKindMinusMinusEq;
            else if (c2 == (char *)'=')
                kind = TokenKindMinusEq;
            else if (c2 == (char *)'-')
                kind = TokenKindMinusMinus;
            else if (c2 == (char *)'>')
                kind = TokenKindArrow;
            else
                kind = TokenKindMinus;
            break;

        case '*':
            if (c2 == (char *)'*' && c3 == (char *)'=')
                kind = TokenKindStarStarEq;
            else if (c2 == (char *)'=')
                kind = TokenKindStarEq;
            else if (c2 == (char *)'*')
                kind = TokenKindStarStar;
            else
                kind = TokenKindStar;
            break;

        case '/':
            if (c2 == (char *)'/' && c3 == (char *)'/') {
                jump(self, 3);

                struct String *doc = scan_comment_doc(self);

                push__String(doc, (char *)'\n');
                next_char(self);
                skip_space(self);

                while (true) {
                    char *c2_doc = peek_char(*self, 1);
                    char *c3_doc = peek_char(*self, 2);

                    if (self->src->c == (char *)'/' && c2_doc == (char *)'/' &&
                        c3_doc == (char *)'/') {
                        skip_space(self);
                        jump(self, 3);

                        struct String *doc2 = scan_comment_doc(self);
                        char *next_one = peek_char(*self, 1);

                        push__String(doc2, (char *)'\n');

                        if (next_one == NULL) {
                            append__String(doc, doc2, true);
                            next_char(self);
                            break;
                        }

                        append__String(doc, doc2, true);
                        next_char(self);
                        skip_space(self);
                    } else {
                        break;
                    }
                }

                bool is_eof = false;

                // GET FLAGS
                // ...

                skip_space(self);

                return get_token(self);

            } else if (c2 == (char *)'/') {
                kind = scan_comment_one(self);
                next_char(self);
            } else if (c2 == (char *)'=')
                kind = TokenKindSlashEq;
            else if (c2 == (char *)'*')
                return scan_comment_multi(self);
            else
                kind = TokenKindSlash;
            break;

        case '%':
            if (c2 == (char *)'=')
                kind = TokenKindPercentageEq;
            else
                kind = TokenKindPercentage;
            break;

        case '^':
            if (c2 == (char *)'=')
                kind = TokenKindHatEq;
            else
                kind = TokenKindHat;
            break;

        case '=':
            if (c2 == (char *)'=')
                kind = TokenKindEqEq;
            else if (c2 == (char *)'>')
                kind = TokenKindFatArrow;
            else
                kind = TokenKindEq;
            break;

        case '<':
            if (c2 == (char *)'<' && c3 == (char *)'=')
                kind = TokenKindLShiftLShiftEq;
            else if (c2 == (char *)'<')
                kind = TokenKindLShiftLShift;
            else if (c2 == (char *)'=')
                kind = TokenKindLShiftEq;
            else if (c2 == (char *)'-')
                kind = TokenKindInverseArrow;
            else
                kind = TokenKindLShift;
            break;

        case '>':
            if (c2 == (char *)'>' && c3 == (char *)'=')
                kind = TokenKindRShiftRShiftEq;
            else if (c2 == (char *)'>')
                kind = TokenKindRShiftRShift;
            else if (c2 == (char *)'=')
                kind = TokenKindRShiftEq;
            else
                kind = TokenKindRShift;
            break;

        case '!':
            kind = TokenKindBang;
            break;

        case '?':
            kind = TokenKindInterrogation;
            break;

        case '&':
            if (c2 == (char *)'=')
                kind = TokenKindAmpersandEq;
            else
                kind = TokenKindAmpersand;
            break;

        case IS_DIGIT:
            return get_all_num(self);

        case '\"': {
            struct Result *string = scan_string(self);
            struct String *string_ok = NULL;

            if (is_err__Result(*string))
                return string;
            else {
                end_token(self);

                string_ok = get_ok__Result(*string);

                FREE(Result, string);

                struct Location *copy = copy__Location(&self->loc);

                return Ok(NEW(TokenLit, TokenKindStringLit, copy, string_ok));
            }
        }

        case '\'':
            return scan_char(self, false);

        case IS_ID: {
            if (self->src->c == (char *)'b' && c2 == (char *)'\"') {
                next_char(self);

                struct Result *string = scan_string(self);
                struct String *string_ok = NULL;

                if (is_err__Result(*string)) {
                    return string;
                } else {
                    end_token(self);

                    string_ok = get_ok__Result(*string);

                    FREE(Result, string);

                    struct Location *copy = copy__Location(&self->loc);

                    return Ok(
                      NEW(TokenLit, TokenKindBitStringLit, copy, string_ok));
                }
            } else if (self->src->c == (char *)'b' && c2 == (char *)'\'') {
                next_char(self);

                return scan_char(self, true);
            }

            struct String *id = scan_identifier(self);
            Str id_str = to_Str__String(*id);
            enum TokenKind tok_kw = get_keyword(id_str);

            end_token(self);

            free(id_str);

            switch (tok_kw) {
                case TokenKindIdentifier: {
                    struct Location *copy = copy__Location(&self->loc);

                    return Ok(NEW(TokenLit, tok_kw, copy, id));
                }
                default: {
                    FREE(String, id);

                    char *next_one = peek_char(*self, 1);

                    if (tok_kw == TokenKindXorKw && next_one == (char *)'=') {
                        next_char(self);

                        return Ok(NEW(Token, TokenKindXorEq, NULL));
                    } else if (tok_kw == TokenKindNotKw &&
                               next_one == (char *)'=') {
                        next_char(self);

                        return Ok(NEW(Token, TokenKindNotEq, NULL));
                    }

                    struct Location *copy = copy__Location(&self->loc);

                    return Ok(NEW(Token, tok_kw, copy));
                }
            }
        }

        default: {
            end_token(self);

            struct Diagnostic *dgn = NEW(
              DiagnosticWithErrScanner,
              self,
              NEW(LilyError, LilyErrorInvalidCharacter),
              self->loc,
              format("unexpected character"),
              Some(format("remove this character: `{c}`", (UPtr)self->src->c)));

            dgn->err->s = format("{c}", (UPtr)self->src->c);

            return Err(dgn);
        }
    }

    return Ok(NEW(Token, kind, NULL));
}

void
run__Scanner(struct Scanner *self)
{
    if (len__String(*self->src->file.content) > 1) {
        while (self->src->pos < len__String(*self->src->file.content) - 1) {
            skip_space(self);

            if (self->src->pos >= len__String(*self->src->file.content) - 1)
                break;

            struct Result *token = get_token(self);
            struct Token *token_ok = NULL;
            struct Diagnostic *token_err = NULL;

            if (is_err__Result(*token)) {
                token_err = get_err__Result(*token);

                emit__Diagnostic(token_err);
                next_char(self);
            } else {
                token_ok = get_ok__Result(*token);

                switch (token_ok->kind) {
                    case TokenKindDot:
                    case TokenKindComma:
                    case TokenKindColon:
                    case TokenKindBar:
                    case TokenKindArrow:
                    case TokenKindAt:
                    case TokenKindRParen:
                    case TokenKindRBrace:
                    case TokenKindRHook:
                    case TokenKindHashtag:
                    case TokenKindSemicolon:
                    case TokenKindDollar:
                    case TokenKindPlus:
                    case TokenKindMinus:
                    case TokenKindStar:
                    case TokenKindSlash:
                    case TokenKindPercentage:
                    case TokenKindHat:
                    case TokenKindWave:
                    case TokenKindEq:
                    case TokenKindLShift:
                    case TokenKindRShift:
                    case TokenKindBang:
                    case TokenKindInterrogation:
                    case TokenKindAmpersand: {
                    token_with_len_1 : {
                        end_token(self);

                        if (token_ok->loc == NULL) {
                            struct Location *copy = copy__Location(&self->loc);

                            token_ok->loc = copy;
                        }

                        next_char_by_token(self, *token_ok);
                    }

                    break;
                    }
                    case TokenKindIntLit:
                    case TokenKindIdentifier:
                    case TokenKindIdentifierOp: {
                        struct String *token_string =
                          token_kind_to_string__Token(*token_ok);

                        if (len__String(*token_string) == 1) {
                            FREE(String, token_string);
                            goto token_with_len_1;
                        } else {
                            FREE(String, token_string);
                            goto token_with_len_over_1;
                        }

                        break;
                    }
                    default: {
                    token_with_len_over_1 : {
                        next_char_by_token(self, *token_ok);
                        previous_char(self);
                        end_token(self);
                        next_char(self);

                        if (token_ok->loc == NULL) {
                            struct Location *copy = copy__Location(&self->loc);

                            token_ok->loc = copy;
                        }
                    } break;
                    }
                }

#ifdef HIDDEN_UNUSED_COMMENT
                if (token_ok->kind != TokenKindOneComment &&
                    token_ok->kind != TokenKindMultiComment)
                    push_token(self, token_ok);
                else
                    FREE(Token, token_ok);
#endif

#ifndef HIDDEN_UNUSED_COMMENT
                push_token(self, token_ok);
#endif

                FREE(Result, token);

                if (self->src->pos >= len__String(*self->src->file.content) - 1)
                    break;
            }
        }

        start_token(self);
        end_token(self);

        struct Location *copy = copy__Location(&self->loc);

        push_token(self, NEW(Token, TokenKindEof, copy));

        if (self->count_error > 0) {
            emit__Summary(
              (Int32)self->count_error, 0, "the scanner phase has been failed");
            exit(1);
        }
    } else {
        start_token(self);
        end_token(self);

        struct Location *copy = copy__Location(&self->loc);

        push_token(self, NEW(Token, TokenKindEof, copy));
    }

#ifdef DEBUG
    for (Usize i = 0; i < len__Vec(*self->tokens); i++)
        Println("{Sr}",
                to_string__Token(*(struct Token *)get__Vec(*self->tokens, i)));
#endif
}

void
__free__Scanner(struct Scanner self)
{
    for (Usize i = len__Vec(*self.tokens); i--;)
        FREE(TokenAll, get__Vec(*self.tokens, i));

    FREE(Vec, self.tokens);
    FREE(Source, *self.src);
}
