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
#include <string.h>

/*
  ____
 / ___|    ___    __ _   _ __    _ __     ___   _ __
 \___ \   / __|  / _` | | '_ \  | '_ \   / _ \ | '__|
  ___) | | (__  | (_| | | | | | | | | | |  __/ | |
 |____/   \___|  \__,_| |_| |_| |_| |_|  \___| |_|

The scanner phase turns the characters into tokens.

Features:

1. The scanner handles other types of errors such as the failure
to close parentheses, brackets and braces.

Example: (

Output:
./Example.lily:1:1: error[0009]: unmatched closing
  |
1 | (
  | ^
help: consider add closing: `)`, `}` or `]`

2. The scanner manages to get escapes in string or in char.

Example: "\n"

Output: no errors

Example: "\w"

Output:
./Example.lily:1:3: error[0012]: invalid escape
  |
1 | "\w"
  |   ^

3. The scanner also manages the analysis of documentation comments.

4. The scanner also handles all syntactic errors in the literals.

    4.1 The scanner also handles the following syntax for floats.

    Example: 4.
    Output: no errors

Example:
"hello"
3.3
4.
3e+3
3E+3
22
0xff
0xFF
0o23
0b0101
'c'
'\n'
"\n"

Output: no errors
 */

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

// Convert Str id in TokenKind.
static enum TokenKind
get_keyword(const Str id);

// Advance one position in the file content.
static inline void
next_char(struct Scanner *self);

static inline void
skip_space(struct Scanner *self);

// next_char n times
static inline void
jump(struct Scanner *self, Usize n);

// Back one position in the file content.
static inline void
previous_char(struct Scanner *self);

// Assign start_line (s_line) and start_column (s_col) to the current line and
// column.
static inline void
start_token(struct Scanner *self);

// Assign end_line (e_line) and end_column (e_col) to the current line and
// column.
static inline void
end_token(struct Scanner *self);

// Peek to the next n char.
static inline char *
peek_char(struct Scanner self, Usize n);

// Advance in the file content according to the Token.
static void
next_char_by_token(struct Scanner *self, struct Token tok);

// Push token in self->tokens.
static inline void
push_token(struct Scanner *self, struct Token *tok);

// Valid if the current char corresponds to a digit.
static inline bool
is_digit(struct Scanner self);

// Valid if the current char corresponds to an identifier.
static inline bool
is_ident(struct Scanner self);

// Valid if the current char corresponds to a hexadecimal integer syntax.
static inline bool
is_hex(struct Scanner self);

// Valid if the current char corresponds to an octal integer syntax.
static inline bool
is_oct(struct Scanner self);

// Valid if the current char corresponds to a binary integer syntax.
static inline bool
is_bin(struct Scanner self);

// Valid if the current char corresponds to a number (float or other integer)
// syntax.
static inline bool
is_num(struct Scanner self);

// Constructs error Diagnostic for the scanner phase.
static struct Diagnostic *
__new__DiagnosticWithErrScanner(struct Scanner *self,
                                struct LilyError *err,
                                struct Location loc,
                                struct String *detail_msg,
                                struct Option *help);

// Constructs warning Diagnostic for the scanner phase.
static struct Diagnostic *
__new__DiagnosticWithWarnScanner(struct Scanner *self,
                                 struct LilyWarning *warn,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help);

// Constructs note Diagnostic for the scanner phase.
static struct Diagnostic *
__new__DiagnosticWithNoteScanner(struct Scanner *self,
                                 struct String *note,
                                 struct Location loc,
                                 struct String *detail_msg,
                                 struct Option *help);

// Emits an error if the expected char doesn't match to the current char.
// Not used for the moment.
static inline void
expected_char(struct Scanner *self,
              struct Scanner *scan_doc,
              struct Diagnostic *dgn,
              char *expected);

// Get escape in char or string literal
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

static void
align_location(struct Scanner *self);

static struct Doc *
scan_doc_author(struct Scanner *self);
static struct Result *
scan_doc_contract(struct Scanner *self);
static struct Doc *
scan_doc_description(struct Scanner *self);
static struct Doc *
scan_doc_file(struct Scanner *self);
static struct Result *
scan_doc_generic(struct Scanner *self);
static struct Result *
scan_doc_prototype(struct Scanner *self);
static struct Doc *
scan_doc_see(struct Scanner *self);
static struct Doc *
scan_doc_version(struct Scanner *self);
static void
get_doc(struct Scanner *self, struct Scanner *scan_doc);

// Scan all numbers (integer or float literal).
static struct Result *
get_all_nums(struct Scanner *self);

// Verify if the targeted char match to the current char and skips (skip:
// call next_char) if is different.
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
    if (!strcmp(id, "None"))
        return TokenKindNoneKw;
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
    if (!strcmp(id, "global"))
        return TokenKindGlobalKw;
    if (!strcmp(id, "package"))
        return TokenKindPackageKw;
    return TokenKindIdentifier;
}

static inline void
next_char(struct Scanner *self)
{
    if (self->src->pos < len__String(*self->src->file.content) - 1) {
        if (self->src->c == (char *)'\n') {
            self->line++;
            self->col = 1;
        } else
            self->col++;
        self->src->pos++;
        self->src->c = get__String(*self->src->file.content, self->src->pos);
    }
}

static inline void
skip_space(struct Scanner *self)
{
    while ((self->src->c == (char *)'\n' || self->src->c == (char *)'\t' ||
            self->src->c == (char *)'\r' || self->src->c == (char *)' ') &&
           self->src->pos < len__String(*self->src->file.content) - 1)
        next_char(self);
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
    if (self.src->pos + n < len__String(*self.src->file.content) - 1)
        return get__String(*self.src->file.content, self.src->pos + n);

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
        case TokenKindNoneKw:
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
        case TokenKindGlobalKw:
        case TokenKindPackageKw:
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
            struct String *tok_string = token_kind_to_String__Token(tok);

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
expected_char(struct Scanner *self,
              struct Scanner *scan_doc,
              struct Diagnostic *dgn,
              char *expected)
{
    skip_space(scan_doc);

    if (expected == scan_doc->src->c) {
        self->count_error -= 1;

        next_char(scan_doc);

        FREE(Diagnostic, dgn);
    } else {
        dgn->loc.s_col += 3;
        dgn->loc.e_col += 3;

        emit__Diagnostic(dgn);
    }
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
    struct Location loc_error = NEW(Location);

    start__Location(&loc_error, self->line, self->col);

    while (self->src->c != (char *)'*' || peek_char(*self, 1) != (char *)'.') {
        if (self->src->pos >= len__String(*self->src->file.content) - 2) {
            end__Location(&loc_error, self->line, self->col);

            return Err(NEW(DiagnosticWithErrScanner,
                           self,
                           NEW(LilyError, LilyErrorUnclosedCommentMultiLine),
                           loc_error,
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

static void
align_location(struct Scanner *self)
{
    self->loc.s_col += 3;
    self->loc.e_col += 3;
}

#define GET_TOKENS()                                                     \
    struct Vec *tokens = NEW(Vec, sizeof(struct Token));                 \
    while (self->src->pos < len__String(*self->src->file.content) &&     \
           self->src->c != (char *)'}') {                                \
        skip_space(self);                                                \
                                                                         \
        if (self->src->pos >= len__String(*self->src->file.content) - 1) \
            break;                                                       \
                                                                         \
        if (self->src->c == (char *)'{') {                               \
            assert(0 && "error");                                        \
            break;                                                       \
        }                                                                \
                                                                         \
        struct Result *tok = get_token(self);                            \
                                                                         \
        if (is_err__Result(*tok))                                        \
            return tok;                                                  \
                                                                         \
        struct Token *token_ok = get_ok__Result(*tok);                   \
                                                                         \
        end_token(self);                                                 \
        next_char_by_token(self, *token_ok);                             \
                                                                         \
        if (token_ok->loc == NULL) {                                     \
            struct Location *copy = copy__Location(&self->loc);          \
                                                                         \
            token_ok->loc = copy;                                        \
        }                                                                \
                                                                         \
        if (token_ok->kind == TokenKindRParen ||                         \
            token_ok->kind == TokenKindRHook) {                          \
            for (Usize i = 0;                                            \
                 i < len__Vec(*self->tokens) &&                          \
                 ((struct Token *)get__Vec(*self->tokens, i))->kind !=   \
                   TokenKindDocComment;) {                               \
                push__Vec(tokens, shift__Vec(self->tokens));             \
            }                                                            \
        }                                                                \
                                                                         \
        push__Vec(tokens, token_ok);                                     \
        FREE(Result, tok);                                               \
    }

#define GET_STRING(s)                                                    \
    skip_space(self);                                                    \
                                                                         \
    while (self->src->c != (char *)'}' &&                                \
           self->src->pos < len__String(*self->src->file.content) - 1) { \
        push__String(s, self->src->c);                                   \
        next_char(self);                                                 \
    }

static struct Doc *
scan_doc_author(struct Scanner *self)
{
    struct String *author = NEW(String);

    GET_STRING(author);

    return NEW(DocWithString, DocKindAuthor, author);
}

static struct Result *
scan_doc_contract(struct Scanner *self)
{
    GET_TOKENS();

    return Ok(NEW(DocContract, tokens));
}

static struct Doc *
scan_doc_description(struct Scanner *self)
{
    struct String *desc = NEW(String);

    GET_STRING(desc);

    return NEW(DocWithString, DocKindDescription, desc);
}

static struct Doc *
scan_doc_file(struct Scanner *self)
{
    struct String *file = NEW(String);

    GET_STRING(file);

    return NEW(DocWithString, DocKindFile, file);
}

static struct Result *
scan_doc_generic(struct Scanner *self)
{
    GET_TOKENS();

    return Ok(NEW(DocGeneric, tokens));
}

static struct Result *
scan_doc_prototype(struct Scanner *self)
{
    GET_TOKENS();

    return Ok(NEW(DocPrototype, tokens));
}

static struct Doc *
scan_doc_see(struct Scanner *self)
{
    struct String *see = NEW(String);

    GET_STRING(see);

    return NEW(DocWithString, DocKindSee, see);
}

static struct Doc *
scan_doc_version(struct Scanner *self)
{
    struct String *version = NEW(String);

    GET_STRING(version);

    return NEW(DocWithString, DocKindVersion, version);
}

static void
get_doc(struct Scanner *self, struct Scanner *scan_doc)
{
    for (Usize i = 0; i < len__String(*scan_doc->src->file.content) - 2; i++) {
        skip_space(scan_doc);

        if (i >= len__String(*scan_doc->src->file.content) - 2)
            break;

        Usize s_line = scan_doc->line;
        Usize s_col = scan_doc->col;

        start_token(scan_doc);

        switch ((char)(UPtr)scan_doc->src->c) {
            case '@': {
                next_char(scan_doc);

                const struct String *id = scan_identifier(scan_doc);
                const Str id_str = to_Str__String(*id);

                int *doc_kind = NULL;

                if (!strcmp(id_str, "")) {
                    assert(0 && "error");
                } else if (!strcmp(id_str, "author"))
                    doc_kind = (int *)DocKindAuthor;
                else if (!strcmp(id_str, "contract"))
                    doc_kind = (int *)DocKindContract;
                else if (!strcmp(id_str, "desc"))
                    doc_kind = (int *)DocKindDescription;
                else if (!strcmp(id_str, "file"))
                    doc_kind = (int *)DocKindFile;
                else if (!strcmp(id_str, "generic"))
                    doc_kind = (int *)DocKindGeneric;
                else if (!strcmp(id_str, "global"))
                    doc_kind = (int *)DocKindGlobal;
                else if (!strcmp(id_str, "prot"))
                    doc_kind = (int *)DocKindPrototype;
                else if (!strcmp(id_str, "see"))
                    doc_kind = (int *)DocKindSee;
                else if (!strcmp(id_str, "version"))
                    doc_kind = (int *)DocKindVersion;
                else {
                    end_token(scan_doc);

                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrScanner,
                          self,
                          NEW(LilyError, LilyErrorUnknownDocFlag),
                          scan_doc->loc,
                          from__String(""),
                          None());

                    emit__Diagnostic(err);
                }

                if (id)
                    FREE(String, (struct String *)id);

                if (id_str)
                    free(id_str);

                next_char(scan_doc);
                skip_space(scan_doc);

                {
                    end_token(scan_doc);

                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrScanner,
                          self,
                          NEW(LilyError, LilyErrorExpectedCharacter),
                          scan_doc->loc,
                          from__String(""),
                          Some(from__String("add `{` this, after doc flag")));

                    err->err->s = format("{c}", (char)(UPtr)scan_doc->src->c);

                    expected_char(self, scan_doc, err, (char *)'{');
                }

                if (doc_kind) {
                    switch ((int)(UPtr)doc_kind) {
                        case DocKindAuthor: {
                            struct Doc *author = scan_doc_author(scan_doc);

                            end_token(scan_doc);
                            align_location(scan_doc);
                            push__Vec(scan_doc->tokens,
                                      NEW(TokenDoc,
                                          copy__Location(&scan_doc->loc),
                                          author));

                            break;
                        }
                        case DocKindContract: {
                            struct Result *contract =
                              scan_doc_contract(scan_doc);

                            scan_doc->loc.s_line = s_line;
                            scan_doc->loc.s_col = s_col;

                            end_token(scan_doc);
                            align_location(scan_doc);

                            if (is_err__Result(*contract))
                                emit__Diagnostic(contract->err);
                            else
                                push__Vec(scan_doc->tokens,
                                          NEW(TokenDoc,
                                              copy__Location(&scan_doc->loc),
                                              get_ok__Result(*contract)));

                            FREE(Result, contract);

                            break;
                        }
                        case DocKindDescription: {
                            struct Doc *desc = scan_doc_description(scan_doc);

                            end_token(scan_doc);
                            align_location(scan_doc);
                            push__Vec(scan_doc->tokens,
                                      NEW(TokenDoc,
                                          copy__Location(&scan_doc->loc),
                                          desc));

                            break;
                        }
                        case DocKindText:
                            TODO("@text");
                            break;
                        case DocKindFile: {
                            struct Doc *file = scan_doc_file(scan_doc);

                            end_token(scan_doc);
                            align_location(scan_doc);
                            push__Vec(scan_doc->tokens,
                                      NEW(TokenDoc,
                                          copy__Location(&scan_doc->loc),
                                          file));

                            break;
                        }
                        case DocKindGeneric: {
                            struct Result *generic = scan_doc_generic(scan_doc);

                            scan_doc->loc.s_line = s_line;
                            scan_doc->loc.s_col = s_col;

                            end_token(scan_doc);
                            align_location(scan_doc);

                            if (is_err__Result(*generic))
                                emit__Diagnostic(generic->err);
                            else
                                push__Vec(scan_doc->tokens,
                                          NEW(TokenDoc,
                                              copy__Location(&scan_doc->loc),
                                              get_ok__Result(*generic)));

                            FREE(Result, generic);

                            break;
                        }
                        case DocKindGlobal:
                            TODO("@global");
                            break;
                        case DocKindPrototype: {
                            struct Result *prot = scan_doc_prototype(scan_doc);

                            scan_doc->loc.s_line = s_line;
                            scan_doc->loc.s_col = s_col;

                            end_token(scan_doc);
                            align_location(scan_doc);

                            if (is_err__Result(*prot))
                                emit__Diagnostic(prot->err);
                            else
                                push__Vec(scan_doc->tokens,
                                          NEW(TokenDoc,
                                              copy__Location(&scan_doc->loc),
                                              get_ok__Result(*prot)));

                            FREE(Result, prot);

                            break;
                        }
                        case DocKindSee: {
                            struct Doc *see = scan_doc_see(scan_doc);

                            end_token(scan_doc);
                            align_location(scan_doc);
                            push__Vec(scan_doc->tokens,
                                      NEW(TokenDoc,
                                          copy__Location(&scan_doc->loc),
                                          see));

                            break;
                        }
                        case DocKindVersion: {
                            struct Doc *version = scan_doc_version(scan_doc);

                            end_token(scan_doc);
                            align_location(scan_doc);
                            push__Vec(scan_doc->tokens,
                                      NEW(TokenDoc,
                                          copy__Location(&scan_doc->loc),
                                          version));

                            break;
                        }
                    }
                    end_token(scan_doc);
                    align_location(scan_doc);

                    struct Diagnostic *err =
                      NEW(DiagnosticWithErrScanner,
                          self,
                          NEW(LilyError, LilyErrorExpectedCharacter),
                          scan_doc->loc,
                          from__String(""),
                          Some(from__String("add `}` after the end of "
                                            "doc flag declaration")));

                    err->err->s = format("{c}", (char)(UPtr)scan_doc->src->c);

                    expected_char(self, scan_doc, err, (char *)'}');
                }

                break;
            }
            default: {
                break;
            }
        }
    }
}

static struct Result *
get_all_nums(struct Scanner *self)
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
            else if (c2 == (char *)'$')
                kind = TokenKindColonDollar;
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
                Usize start_line = self->line;
                Usize start_col = self->col;

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

                struct Source scan_doc_src =
                  NEW(Source,
                      (struct File){ .content = doc,
                                     .name = self->src->file.name });
                struct Scanner scan_doc = NEW(Scanner, &scan_doc_src);

                scan_doc.line = start_line;
                scan_doc.col = start_col;

                get_doc(self, &scan_doc);

                if (len__Vec(*scan_doc.tokens) > 2) {
                    for (Usize i = 0; i < len__Vec(*scan_doc.tokens) - 2; i++)
                        push__Vec(self->tokens, get__Vec(*scan_doc.tokens, i));
                }

                struct Result *last_doc = Ok(
                  get__Vec(*scan_doc.tokens, len__Vec(*scan_doc.tokens) - 1));

                if (doc)
                    FREE(String, doc);

                FREE(Vec, scan_doc.tokens);

                return last_doc;
            } else if (c2 == (char *)'/')
                kind = scan_comment_one(self);
            else if (c2 == (char *)'=')
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
            return get_all_nums(self);

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
                          token_kind_to_String__Token(*token_ok);

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
                to_String__Token(*(struct Token *)get__Vec(*self->tokens, i)));
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
