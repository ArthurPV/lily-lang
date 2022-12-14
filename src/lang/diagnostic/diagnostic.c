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
diagnostic_kind_to_String(enum DiagnosticKind kind);
static inline const Str
apply_color(enum DiagnosticKind kind, const Str s);
struct String *
detail_to_String(struct Detail self,
                 enum DiagnosticKind kind,
                 struct Location loc);
static inline struct String *
lily_error_to_String(struct LilyError err);
static inline struct String *
lily_warning_to_String(struct LilyWarning warn);
static inline const Str
get_code_of_lily_error(struct LilyError err);
static inline const Str
get_code_of_lily_warning(struct LilyWarning warn);
struct String *
get_line(struct Diagnostic self, Usize line_number);
struct String *
diagnostic_to_String(struct Diagnostic self);

static inline const Str
diagnostic_kind_to_String(enum DiagnosticKind kind)
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

static inline const Str
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
__new__Detail(struct String *msg, struct Vec *lines)
{
    struct Detail *self = malloc(sizeof(struct Detail));
    self->msg = msg;
    self->lines = lines;
    return self;
}

struct String *
detail_to_String(struct Detail self,
                 enum DiagnosticKind kind,
                 struct Location loc)
{
    Str line_str = of_int__Str((int)loc.e_line);
    Usize line_str_length = strlen(line_str);
    struct String *s = NULL;

    if (loc.s_line == loc.e_line) {
        struct String *repeat1 = repeat__String(" ", line_str_length - 1);
        struct String *repeat2 = repeat__String(" ", line_str_length);
        struct String *repeat3 = NEW(String);
        struct String *repeat4 = NULL;
        struct String *line = (struct String *)get__Vec(*self.lines, 0);

        for (Usize i = 0; i < loc.s_col - 1; i++)
            if (get__String(*line, i) == (char *)'\t')
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
    } else {
        TODO("diagnostic with more one line");
    }

    free(line_str);

    return s;
}

static inline struct String *
lily_error_to_String(struct LilyError err)
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
        case LilyErrorInvalidEscape:
            return from__String("invalid escape");
        case LilyErrorUnclosedChar:
            return from__String("unclosed char");
        case LilyErrorUnexpectedToken:
            return format("unexpected token: `{S}`", err.s);
        case LilyErrorMissFunctionName:
            return from__String("miss function name");
        case LilyErrorExpectedToken:
            return format("expected {S}", err.s);
        case LilyErrorMissClosingBlock:
            return format("miss closing block, expected: {S}", err.s);
        case LilyErrorMisuseOfSpecialClosingBlock:
            return from__String("misuse of special closing block");
        case LilyErrorBadUsageOfAsync:
            return from__String("bad usage of `async`");
        case LilyErrorBadUsageOfPub:
            return from__String("bad usage of `pub`");
        case LilyErrorInvalidItemInFunOrMethodBody:
            return from__String("invalid item in `fun` or method body");
        case LilyErrorBadUsageOfType:
            return from__String("bad usage of `type`");
        case LilyErrorMissTypeName:
            return from__String("miss type name");
        case LilyErrorInvalidTokenInEnumVariant:
            return from__String("invalid token in enum variant");
        case LilyErrorInvalidTokenInRecordField:
            return from__String("invalid token in record field");
        case LilyErrorMissObjectName:
            return from__String("miss object name");
        case LilyErrorInvalidTokenInAliasDataType:
            return from__String("invalid token in alias data type");
        case LilyErrorUnexpectedInheritance:
            return from__String("unexpected inheritance");
        case LilyErrorUnexpectedImplementation:
            return from__String("unexpected implementation");
        case LilyErrorInvalidTokenInTagBody:
            return from__String("invalid token in tag body");
        case LilyErrorMissNameOnPropertyOrMethod:
            return from__String("miss name on property or method");
        case LilyErrorInvalidClassItem:
            return from__String("invalid class item");
        case LilyErrorMissImportValue:
            return from__String(
              "miss `import` value (expected string literal)");
        case LilyErrorMissAsValue:
            return from__String("miss `as` value (expected identifier)");
        case LilyErrorUnexpectedTokenForBeginingInGlobal:
            return format("unexpected token for begining in global: `{S}`",
                          err.s);
        case LilyErrorUnknownAttribute:
            return from__String("unknown attribute");
        case LilyErrorExpectedAttribute:
            return from__String("expected attribute");
        case LilyErrorMissDataType:
            return from__String("miss data type");
        case LilyErrorIntegerIsOutOfRange:
            return from__String("the integer is out of range");
        case LilyErrorMissParamName:
            return from__String("miss param name");
        case LilyErrorOperatorIdentifierCannotBeAKeyword:
            return from__String("the operator identifier cannot be a keyword");
        case LilyErrorMissFieldCallName:
            return from__String("miss field call name");
        case LilyErrorMissErrorName:
            return from__String("miss error name");
        case LilyErrorMissModuleName:
            return from__String("miss module name");
        case LilyErrorExpectedRightValue:
            return from__String("expected right value");
        case LilyErrorUnexpectedExpression:
            return from__String("unexpected expression");
        case LilyErrorInvalidAttributeInThisBlock:
            return from__String("invalid attribute in this block");
        case LilyErrorBadUsageOfObject:
            return from__String("bad usage of object");
        case LilyErrorExpectedExpression:
            return from__String("expected expression");
        case LilyErrorMissVariantName:
            return from__String("miss variant name");
        case LilyErrorExpectedUserDefinedDataType:
            return from__String("expected user defined data type");
        case LilyErrorMissPrototypeName:
            return from__String("miss prototype name");
        case LilyErrorMissParamsPrototype:
            return from__String("miss params prototype");
        case LilyErrorUnexpectedSelfParamInFunction:
            return from__String("unexpected self param in function");
        case LilyErrorDuplicateSelfParam:
            return from__String("duplicate self param");
        case LilyErrorSelfParamMustBeTheFirstParam:
            return from__String("the self param must be in first");
        case LilyErrorUnknownDataType:
            return format("unknown data type: `{S}`", err.s);
        case LilyErrorExpectedEnumObjectOrRecordObject:
            return from__String("expected enum or record object");
        case LilyErrorDuplicateDeclaration:
            return from__String("duplicate declaration");
        case LilyErrorDuplicateVariant:
            return from__String("duplicate variant");
        case LilyErrorDuplicateField:
            return from__String("duplicate field");
        case LilyErrorBadImportValue:
            return from__String("bad import value");
        case LilyErrorUnexpectedCharacterInImportValue:
            return from__String("unexpected character in import value");
        case LilyErrorExpectedCharacter:
            return format("there is expected character, found: `{S}`", err.s);
        case LilyErrorUnknownDocFlag:
            return from__String("unknown doc flag");
        case LilyErrorExpectedCharacterInImportValue:
            return from__String("expected character in import value");
        case LilyErrorEmptyImportValue:
            return from__String("empty import value");
        case LilyErrorNotExpectedImportValue:
            return from__String("not expected import value");
        case LilyErrorDependencyCycleOnImportValue:
            return from__String("dependency cycle on import value");
        case LilyErrorExpectedFinalAccessInImportValue:
            return from__String("expected final access in import value");
        case LilyErrorImportValueAccessIsNotFound:
            return from__String(
              "import value access is not found in the specified path");
        case LilyErrorDuplicateGenericParam:
            return from__String("duplicate generic param");
        case LilyErrorNameMustStartByLowercaseCharacter:
            return from__String("name must start by lowercase character");
        case LilyErrorNameMustStartByUppercaseCharacter:
            return from__String("name must start by uppercase character");
        case LilyErrorExpectedIntegerDataType:
            return from__String("expected integer data type");
        case LilyErrorExpectedALargerIntegerDataType:
            return from__String("expected a larger integer data type");
        case LilyErrorFloatIsOutOfRange:
            return from__String("float is out of range");
        case LilyErrorUnmatchedDataType:
            return from__String("unmatched data type");
        default:
            UNREACHABLE("unknown lily error kind");
    }
}

static inline struct String *
lily_warning_to_String(struct LilyWarning warn)
{
    switch (warn.kind) {
        case LilyWarningUnusedParen:
            return from__String("unused paren");
        case LilyWarningIgnoredTags:
            return from__String("ignored tags");
        case LilyWarningIgnoredGenericParams:
            return from__String("ignored generic params");
        case LilyWarningIgnoredLambdaDataType:
            return from__String("ignored lambda data type");
        default:
            UNREACHABLE("unknown lily warning kind");
    }
}

static inline const Str
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
        case LilyErrorInvalidEscape:
            return "0012";
        case LilyErrorUnclosedChar:
            return "0013";
        case LilyErrorUnexpectedToken:
            return "0014";
        case LilyErrorMissFunctionName:
            return "0015";
        case LilyErrorExpectedToken:
            return "0016";
        case LilyErrorMissClosingBlock:
            return "0017";
        case LilyErrorMisuseOfSpecialClosingBlock:
            return "0018";
        case LilyErrorBadUsageOfAsync:
            return "0019";
        case LilyErrorBadUsageOfPub:
            return "0020";
        case LilyErrorInvalidItemInFunOrMethodBody:
            return "0021";
        case LilyErrorBadUsageOfType:
            return "0022";
        case LilyErrorMissTypeName:
            return "0023";
        case LilyErrorInvalidTokenInEnumVariant:
            return "0024";
        case LilyErrorInvalidTokenInRecordField:
            return "0025";
        case LilyErrorMissObjectName:
            return "0026";
        case LilyErrorInvalidTokenInAliasDataType:
            return "0027";
        case LilyErrorUnexpectedInheritance:
            return "0028";
        case LilyErrorUnexpectedImplementation:
            return "0029";
        case LilyErrorInvalidTokenInTagBody:
            return "0030";
        case LilyErrorMissNameOnPropertyOrMethod:
            return "0031";
        case LilyErrorInvalidClassItem:
            return "0032";
        case LilyErrorMissImportValue:
            return "0033";
        case LilyErrorMissAsValue:
            return "0034";
        case LilyErrorUnexpectedTokenForBeginingInGlobal:
            return "0035";
        case LilyErrorUnknownAttribute:
            return "0036";
        case LilyErrorExpectedAttribute:
            return "0037";
        case LilyErrorMissDataType:
            return "0038";
        case LilyErrorIntegerIsOutOfRange:
            return "0039";
        case LilyErrorMissParamName:
            return "0040";
        case LilyErrorOperatorIdentifierCannotBeAKeyword:
            return "0041";
        case LilyErrorMissFieldCallName:
            return "0042";
        case LilyErrorMissErrorName:
            return "0043";
        case LilyErrorMissModuleName:
            return "0044";
        case LilyErrorExpectedRightValue:
            return "0045";
        case LilyErrorUnexpectedExpression:
            return "0046";
        case LilyErrorInvalidAttributeInThisBlock:
            return "0047";
        case LilyErrorBadUsageOfObject:
            return "0048";
        case LilyErrorExpectedExpression:
            return "0049";
        case LilyErrorMissVariantName:
            return "0050";
        case LilyErrorMissFieldName:
            return "0051";
        case LilyErrorExpectedUserDefinedDataType:
            return "0052";
        case LilyErrorMissPrototypeName:
            return "0053";
        case LilyErrorMissParamsPrototype:
            return "0054";
        case LilyErrorUnexpectedSelfParamInFunction:
            return "0055";
        case LilyErrorDuplicateSelfParam:
            return "0056";
        case LilyErrorSelfParamMustBeTheFirstParam:
            return "0057";
        case LilyErrorUnknownDataType:
            return "0058";
        case LilyErrorExpectedEnumObjectOrRecordObject:
            return "0059";
        case LilyErrorDuplicateDeclaration:
            return "0060";
        case LilyErrorDuplicateVariant:
            return "0061";
        case LilyErrorDuplicateField:
            return "0062";
        case LilyErrorBadImportValue:
            return "0063";
        case LilyErrorUnexpectedCharacterInImportValue:
            return "0064";
        case LilyErrorExpectedCharacter:
            return "0065";
        case LilyErrorUnknownDocFlag:
            return "0066";
        case LilyErrorExpectedCharacterInImportValue:
            return "0067";
        case LilyErrorEmptyImportValue:
            return "0068";
        case LilyErrorNotExpectedImportValue:
            return "0069";
        case LilyErrorDependencyCycleOnImportValue:
            return "0070";
        case LilyErrorExpectedFinalAccessInImportValue:
            return "0071";
        case LilyErrorImportValueAccessIsNotFound:
            return "0072";
        case LilyErrorDuplicateGenericParam:
            return "0073";
        case LilyErrorNameMustStartByLowercaseCharacter:
            return "0074";
        case LilyErrorNameMustStartByUppercaseCharacter:
            return "0075";
        case LilyErrorExpectedIntegerDataType:
            return "0076";
        case LilyErrorExpectedALargerIntegerDataType:
            return "0077";
        case LilyErrorFloatIsOutOfRange:
            return "0078";
        case LilyErrorUnmatchedDataType:
            return "0079";
        default:
            UNREACHABLE("unknown lily error kind");
    }
}

static inline const Str
get_code_of_lily_warning(struct LilyWarning warn)
{
    switch (warn.kind) {
        case LilyWarningUnusedParen:
            return "0001";
        case LilyWarningIgnoredTags:
            return "0002";
        case LilyWarningIgnoredGenericParams:
            return "0003";
        case LilyWarningIgnoredLambdaDataType:
            return "0004";
        default:
            UNREACHABLE("unknown lily warning kind");
    }
}

void
__free__Detail(struct Detail *self)
{
    FREE(String, self->msg);

    for (Usize i = 0; i < len__Vec(*self->lines); i++)
        FREE(String, get__Vec(*self->lines, i));

    FREE(Vec, self->lines);
    free(self);
}

struct LilyError *
__new__LilyError(enum LilyErrorKind kind)
{
    struct LilyError *self = malloc(sizeof(struct LilyError));
    self->kind = kind;
    self->s = NULL;
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
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));

        self->detail = NEW(Detail, detail_msg, lines);
    } else {
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));
        push__Vec(lines, get_line(*self, loc.e_line));

        self->detail = NEW(Detail, detail_msg, lines);
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
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));

        self->detail = NEW(Detail, detail_msg, lines);
    } else {
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));
        push__Vec(lines, get_line(*self, loc.e_line));

        self->detail = NEW(Detail, detail_msg, lines);
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
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));

        self->detail = NEW(Detail, detail_msg, lines);
    } else {
        struct Vec *lines = NEW(Vec, sizeof(struct String));
        push__Vec(lines, get_line(*self, loc.s_line));
        push__Vec(lines, get_line(*self, loc.e_line));

        self->detail = NEW(Detail, detail_msg, lines);
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
diagnostic_to_String(struct Diagnostic self)
{
    Str diagnostic_kind_str = diagnostic_kind_to_String(self.kind);
    struct String *detail_string =
      detail_to_String(*self.detail, self.kind, self.loc);
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
        msg = lily_error_to_String(*self.err);
        code = get_code_of_lily_error(*self.err);
    } else if (self.kind == DiagnosticKindWarning) {
        msg = lily_warning_to_String(*self.warn);
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

    return format("{s}:{d}:{d}: {sa}: {S}\n {Sr}\n\x1b[32mhelp\x1b[0m: {Sr}",
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
    struct String *s = diagnostic_to_String(*self);

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
        Str disable_code =
          to_Str__String(*(struct String *)get__Vec(*warning_disable_codes, i));

        if (!strcmp(code, disable_code)) {
            same_code = true;
            free(disable_code);

            break;
        }

        free(disable_code);
    }

    if (!same_code)
        emit__Diagnostic(self);
    else {
        // for (Usize i = 0; i < len__Vec(*self->detail->lines); i++)
        //     FREE(String, get__Vec(*self->detail->lines, i));
        FREE(Diagnostic, self);
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
