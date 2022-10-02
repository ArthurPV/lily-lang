#include <base/color.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/option.h>
#include <base/str.h>
#include <lang/diagnostic/diagnostic.h>
#include <lang/scanner/token.h>
#include <string.h>

static inline const Str
diagnostic_kind_to_string(enum DiagnosticKind kind);
static inline const Str
apply_color(enum DiagnosticKind kind, const Str s);
static struct String *
detail_to_string(struct Detail self,
                 enum DiagnosticKind kind,
                 struct Location loc);
static inline struct String *
lily_error_to_string(struct LilyError err);
static inline struct String *
lily_warning_to_string(struct LilyWarning warn);
static inline const Str
get_code_of_lily_error(struct LilyError err);
static inline const Str
get_code_of_lily_warning(struct LilyWarning warn);
static inline struct String *
get_line(struct Diagnostic self, Usize line_number);
static struct String *
diagnostic_to_string(struct Diagnostic self);

const Str
diagnostic_kind_to_string(enum DiagnosticKind kind)
{
    switch (kind) {
        case DiagnosticKindError:
            return "error";
        case DiagnosticKindWarning:
            return "warning";
        case DiagnosticKindNote:
            return "note";
        default:
            UNREACHABLE("unknown diagnostic kind");
    }
}

const Str
apply_color(enum DiagnosticKind kind, const Str s)
{
    switch (kind) {
        case DiagnosticKindError:
            return RED(s);
        case DiagnosticKindWarning:
            return YELLOW(s);
        case DiagnosticKindNote:
            return CYAN(s);
        default:
            UNREACHABLE("unknown diagnostic kind");
    }
}

struct Detail *
__new__Detail(struct String *msg, struct Location loc, struct Vec *lines)
{
    struct Detail *self = malloc(sizeof(struct Detail));
    self->msg = msg;
    self->loc = loc;
    self->lines = lines;
    return self;
}

struct String *
detail_to_string(struct Detail self,
                 enum DiagnosticKind kind,
                 struct Location loc)
{
    Str string_color = NULL;
    switch (kind) {
        case DiagnosticKindError:
            string_color = RED("^");
            break;
        case DiagnosticKindWarning:
            string_color = YELLOW("^");
            break;
        case DiagnosticKindNote:
            string_color = CYAN("^");
            break;
        default:
            UNREACHABLE("unknown DiagnosticKind");
    }

    Str line_str = of_int__Str((int)loc.e_line);
    Usize line_str_length = strlen(line_str);
    const Str color_str =
      apply_color(kind, "^"); // TODO: rename this for improve variable's name.
    struct String *s = NULL;

    if (loc.s_line == loc.e_line) {
        struct String *repeat1 = repeat__String(" ", line_str_length - 1);
        struct String *repeat2 = repeat__String(" ", line_str_length);
        struct String *repeat3 = NEW(String);
        struct String *repeat4 = NULL;
        struct String *line = (struct String *)get__Vec(*self.lines, 0);

        for (Usize i = 0; i < loc.s_col - 1; i++)
            if (get__String(*line, i) == (char*)'\t')
                push_str__String(repeat3, "\t");
            else
                push_str__String(repeat3, " ");

        if (loc.e_col - loc.s_col < 1)
            repeat4 = repeat__String("^", loc.e_col - loc.s_col + 1);
        else
            repeat4 = repeat__String("^", loc.e_col - loc.s_col + 1);

        if (kind == DiagnosticKindError) {
            s = format("{S} |\n{d} | \x1b[31m{S}\x1b[0m\n{S} | {S}{S} {S}",
                       repeat1,
                       loc.s_line,
                       line,
                       repeat2,
                       repeat3,
                       repeat4,
                       self.msg);
        } else if (kind == DiagnosticKindWarning) {
            s = format("{S} |\n{d} | \x1b[33m{S}\x1b[0m\n{S} | {S}{S} {S}",
                       repeat1,
                       loc.s_line,
                       line,
                       repeat2,
                       repeat3,
                       repeat4,
                       self.msg);
        } else {
            s = format("{S} |\n{d} | \x1b[36m{S}\x1b[0m\n{S} | {S}{S} {S}",
                       repeat1,
                       loc.s_line,
                       line,
                       repeat2,
                       repeat3,
                       repeat4,
                       self.msg);
        }

        FREE(String, repeat1);
        FREE(String, repeat2);
        FREE(String, repeat3);
        FREE(String, repeat4);
        FREE(String, line);
    } else {
        TODO("diagnostic with more one line");
    }

    free(string_color);
    free(color_str);
    free(line_str);

    return s;
}

struct String *
lily_error_to_string(struct LilyError err)
{
    switch (err.kind) {
        case LilyErrorUnclosedCommentMultiLine:
            return from__String("unclosed comment multi line");
        case LilyErrorUnclosedCharLiteral:
            return from__String("unclosed char literal");
        case LilyErrorInvalidCharLiteral:
            return from__String("invalid char literal");
        case LilyErrorUnclosedStringLiteral:
            return from__String("unclosed string literal");
        case LilyErrorInvalidHexLiteral:
            return from__String("invalid hexadecimal literal");
        case LilyErrorInvalidOctLiteral:
            return from__String("invalid octal literal");
        case LilyErrorInvalidBinLiteral:
            return from__String("invalid binary literal");
        case LilyErrorInvalidNumLiteral:
            return from__String("invalid number literal");
        case LilyErrorUnmatchedClosing:
            return from__String("unmatched closing");
        case LilyErrorMismatchedClosingDelimiter:
            return from__String("mismatched closing delimiter");
        case LilyErrorInvalidCharacter:
            return format("invalid character: `{S}`", err.s);
        case LilyErrorUnexpectedToken:
            return format("unexpected token: `{S}`", err.s);
        case LilyErrorMissFunctionName:
            return from__String("miss function name");
        case LilyErrorExpectedToken:
            return format("expected `{S}`", err.s);
        case LilyErrorMissClosingBlock:
            return format("miss closing block, expected: {S}", err.s);
        case LilyErrorMisuseOfSpecialClosingBlock:
            return from__String("misuse of special closing block");
        case LilyErrorBadUsageOfAsync:
            return from__String("bad usage of `async`");
        case LilyErrorBadUsageOfPub:
            return from__String("bad usage of `pub`");
        case LilyErrorInvalidItemInFunBody:
            return from__String("invalid item in `fun` body");
        case LilyErrorBadUsageOfType:
            return from__String("bad usage of `type`");
        case LilyErrorMissTypeName:
            return from__String("miss type name");
        case LilyErrorInvalidTokenInEnumVariant:
            return format("invalid token in enum variant");
        default:
            UNREACHABLE("unknown lily error kind");
    }
}

struct String *
lily_warning_to_string(struct LilyWarning warn)
{
    switch (warn.kind) {
        case LilyWarningUnusedParen:
            return from__String("unused paren");
        case LilyWarningIgnoredTags:
            return from__String("ignored tags");
        case LilyWarningIgnoredGenericParams:
            return from__String("ignored generic params");
        default:
            UNREACHABLE("unknown lily warning kind");
    }
}

const Str
get_code_of_lily_error(struct LilyError err)
{
    switch (err.kind) {
        case LilyErrorUnclosedCommentMultiLine:
            return "0001";
        case LilyErrorUnclosedCharLiteral:
            return "0002";
        case LilyErrorInvalidCharLiteral:
            return "0003";
        case LilyErrorUnclosedStringLiteral:
            return "0004";
        case LilyErrorInvalidHexLiteral:
            return "0005";
        case LilyErrorInvalidOctLiteral:
            return "0006";
        case LilyErrorInvalidBinLiteral:
            return "0007";
        case LilyErrorInvalidNumLiteral:
            return "0008";
        case LilyErrorUnmatchedClosing:
            return "0009";
        case LilyErrorMismatchedClosingDelimiter:
            return "0010";
        case LilyErrorInvalidCharacter:
            return "0011";
        case LilyErrorUnexpectedToken:
            return "0012";
        case LilyErrorMissFunctionName:
            return "0013";
        case LilyErrorExpectedToken:
            return "0014";
        case LilyErrorMissClosingBlock:
            return "0015";
        case LilyErrorMisuseOfSpecialClosingBlock:
            return "0016";
        case LilyErrorBadUsageOfAsync:
            return "0017";
        case LilyErrorBadUsageOfPub:
            return "0018";
        case LilyErrorInvalidItemInFunBody:
            return "0019";
        case LilyErrorBadUsageOfType:
            return "0020";
        case LilyErrorMissTypeName:
            return "0021";
        case LilyErrorInvalidTokenInEnumVariant:
            return "0022";
        default:
            UNREACHABLE("unknown lily error kind");
    }
}

const Str
get_code_of_lily_warning(struct LilyWarning warn)
{
    switch (warn.kind) {
        case LilyWarningUnusedParen:
            return "0001";
        case LilyWarningIgnoredTags:
            return "0002";
        case LilyWarningIgnoredGenericParams:
            return "0003";
        default:
            UNREACHABLE("unknown lily warning kind");
    }
}

void
__free__Detail(struct Detail *self)
{
    FREE(String, self->msg);
    FREE(Vec, self->lines);
    free(self);
}

struct LilyError *
__new__LilyError(enum LilyErrorKind kind)
{
    struct LilyError *self = malloc(sizeof(struct LilyError));
    self->kind = kind;
    return self;
}

struct LilyError *
__new__LilyErrorWithString(enum LilyErrorKind kind, struct String *s)
{
    struct LilyError *self = malloc(sizeof(struct LilyError));
    self->kind = kind;
    self->s = s;
    return self;
}

void
__free__LilyError(struct LilyError *self)
{
    if (self->s != NULL)
        FREE(String, self->s);

    free(self);
}

struct LilyWarning *
__new__LilyWarning(enum LilyWarningKind kind)
{
    struct LilyWarning *self = malloc(sizeof(struct LilyWarning));
    self->kind = kind;
    return self;
}

struct LilyWarning *
__new__LilyWarningWithString(enum LilyWarningKind kind, struct String *s)
{
    struct LilyWarning *self = malloc(sizeof(struct LilyWarning));
    self->kind = kind;
    self->s = s;
    return self;
}

void
__free__LilyWarning(struct LilyWarning *self)
{
    free(self);
}

struct Diagnostic *
__new__DiagnosticWithErr(struct LilyError *err,
                         struct Location loc,
                         struct File file,
                         struct String *detail_msg,
                         struct Option *help)
{
    struct Diagnostic *self = malloc(sizeof(struct Diagnostic));
    self->err = err;
    self->kind = DiagnosticKindError;
    self->loc = loc;
    self->file = file;
    self->help = help;

    if (loc.s_line == loc.e_line) {
        void *lines_temp[1] = { get_line(*self, loc.s_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 1);

        self->detail = NEW(Detail, detail_msg, loc, lines);
    } else {
        void *lines_temp[2] = { get_line(*self, loc.s_line),
                                get_line(*self, loc.e_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 2);

        self->detail = NEW(Detail, detail_msg, loc, lines);
    }

    return self;
}

struct Diagnostic *
__new__DiagnosticWithWarn(struct LilyWarning *warn,
                          struct Location loc,
                          struct File file,
                          struct String *detail_msg,
                          struct Option *help)
{
    struct Diagnostic *self = malloc(sizeof(struct Diagnostic));
    self->warn = warn;
    self->kind = DiagnosticKindWarning;
    self->loc = loc;
    self->file = file;
    self->help = help;

    if (loc.s_line == loc.e_line) {
        void *lines_temp[1] = { get_line(*self, loc.s_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 1);
        self->detail = NEW(Detail, detail_msg, loc, lines);
    } else {
        void *lines_temp[2] = { get_line(*self, loc.s_line),
                                get_line(*self, loc.e_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 2);
        self->detail = NEW(Detail, detail_msg, loc, lines);
    }

    return self;
}

struct Diagnostic *
__new__DiagnosticWithNote(struct String *note,
                          struct Location loc,
                          struct File file,
                          struct String *detail_msg,
                          struct Option *help)
{
    struct Diagnostic *self = malloc(sizeof(struct Diagnostic));
    self->note = note;
    self->kind = DiagnosticKindNote;
    self->loc = loc;
    self->file = file;
    self->help = help;

    if (loc.s_line == loc.e_line) {
        void *lines_temp[1] = { get_line(*self, loc.s_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 1);
        self->detail = NEW(Detail, detail_msg, loc, lines);
    } else {
        void *lines_temp[2] = { get_line(*self, loc.s_line),
                                get_line(*self, loc.e_line) }; // VERIFY LEAK
        struct Vec *lines = from__Vec(lines_temp, sizeof(struct String *), 2);
        self->detail = NEW(Detail, detail_msg, loc, lines);
    }

    return self;
}

struct String *
get_line(struct Diagnostic self, Usize line_number)
{
    struct Vec *splitted_content = split__String(*self.file.content, '\n');
    struct String *line = get__Vec(*splitted_content, line_number - 1);

    for (Usize i = 0; i < len__Vec(*splitted_content); i++) {
        if (line_number - 1 != i)
            FREE(String, (struct String *)get__Vec(*splitted_content, i));
    }

    FREE(Vec, splitted_content);

    return line;
}

struct String *
diagnostic_to_string(struct Diagnostic self)
{
    Str diagnostic_kind_str = diagnostic_kind_to_string(self.kind);
    struct String *detail_string =
      detail_to_string(*self.detail, self.kind, self.loc);
    struct String *help_str = NULL;
    struct String *msg = NULL;
    Str code = NULL;
    Str kind_str = NULL;

    switch (self.kind) {
        case DiagnosticKindError:
            kind_str = RED(diagnostic_kind_str);
            break;
        case DiagnosticKindWarning:
            kind_str = YELLOW(diagnostic_kind_str);
            break;
        case DiagnosticKindNote:
            kind_str = CYAN(diagnostic_kind_str);
            break;
        default:
            UNREACHABLE("unknown diagnostic kind");
    }

    if (is_Some__Option(self.help))
        help_str = (struct String *)get__Option(self.help);

    if (self.kind == DiagnosticKindError) {
        msg = lily_error_to_string(*self.err);
        code = get_code_of_lily_error(*self.err);
    } else if (self.kind == DiagnosticKindWarning) {
        msg = lily_warning_to_string(*self.warn);
        code = get_code_of_lily_warning(*self.warn);
    } else if (self.kind == DiagnosticKindNote) {
        msg = self.note;
    } else
        UNREACHABLE("unknown diagnostic kind");

    if (self.kind == DiagnosticKindError ||
        self.kind == DiagnosticKindWarning) {
        if (help_str == NULL) {
            return format("{s}:{d}:{d}: {sa}[{s}]: {Sr}\n {Sr}",
                          self.file.name,
                          self.loc.s_line,
                          self.loc.s_col,
                          kind_str,
                          code,
                          msg,
                          detail_string);
        }

        return format(
          "{s}:{d}:{d}: {sa}[{s}]: {Sr}\n {Sr}\n\x1b[32mhelp\x1b[0m: {Sr}",
          self.file.name,
          self.loc.s_line,
          self.loc.s_col,
          kind_str,
          code,
          msg,
          detail_string,
          help_str);
    }

    if (help_str == NULL) {
        return format("{s}:{d}:{d}: {sa}: {S}\n {Sr}",
              self.file.name,
              self.loc.s_line,
              self.loc.s_col,
              kind_str,
              msg,
              detail_string);
    }

    return format(
      "{s}:{d}:{d}: {sa}: {S}\n {Sr}\n\x1b[32mhelp\x1b[0m: {Sr}",
      self.file.name,
      self.loc.s_line,
      self.loc.s_col,
      kind_str,
      msg,
      detail_string,
      help_str);
}

void
emit__Diagnostic(struct Diagnostic *self)
{
    struct String *s = diagnostic_to_string(*self);

    Println("{Sr}", s);

    FREE(Diagnostic, self);
}

void
emit_warning__Diagnostic(struct Diagnostic *self,
                         struct Vec *warning_disable_codes)
{
    assert(self->kind == DiagnosticKindWarning &&
           "only DiagnosticKindWarning variant is accepted");

    Str code = get_code_of_lily_warning(*self->warn);

    bool same_code = false;
    for (Usize i = 0; i < len__Vec(*warning_disable_codes); i++) {
        if (code == (Str)get__Vec(*warning_disable_codes, i)) {
            same_code = true;
            break;
        }
    }

    if (!same_code) {
        emit__Diagnostic(self);
    }
}

void
__free__Diagnostic(struct Diagnostic *self)
{
    FREE(Detail, self->detail);
    FREE(Option, self->help);

    if (self->kind == DiagnosticKindError)
        FREE(LilyError, self->err);
    else if (self->kind == DiagnosticKindWarning)
        FREE(LilyWarning, self->warn);
    else if (self->kind == DiagnosticKindNote)
        FREE(String, self->note);

    free(self);
}
