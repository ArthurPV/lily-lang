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

#ifndef LILY_DIAGNOSTIC_H
#define LILY_DIAGNOSTIC_H

#include <base/string.h>
#include <base/tuple.h>
#include <base/vec.h>
#include <lang/scanner/scanner.h>

#define DIAGNOSTIC_ENUM(dgn) dgn(Error) dgn(Warning) dgn(Note)

#define DIAGNOSTIC_KIND(name) DiagnosticKind##name,

enum DiagnosticKind
{
    DIAGNOSTIC_ENUM(DIAGNOSTIC_KIND)
};

typedef struct Detail
{
    struct String *msg;
    struct Vec *lines;
} Detail;

/**
 *
 * @brief Construct Detail type.
 */
struct Detail *
__new__Detail(struct String *msg, struct Vec *lines);

/**
 *
 * @brief Convert Detail to String.
 */
struct String *
to_String__Detail(struct Detail self,
                  enum DiagnosticKind kind,
                  struct Location loc);

/**
 *
 * @brief Free Detail type.
 */
void
__free__Detail(struct Detail *self);

enum LilyErrorKind
{
    LilyErrorUnclosedCommentMultiLine,
    LilyErrorUnclosedCharLiteral,
    LilyErrorInvalidCharLiteral,
    LilyErrorUnclosedStringLiteral,
    LilyErrorInvalidHexLiteral,
    LilyErrorInvalidOctLiteral,
    LilyErrorInvalidBinLiteral,
    LilyErrorInvalidNumLiteral,
    LilyErrorUnmatchedClosing,
    LilyErrorMismatchedClosingDelimiter,
    LilyErrorInvalidCharacter,
    LilyErrorInvalidEscape,
    LilyErrorUnclosedChar,
    LilyErrorUnexpectedToken,
    LilyErrorMissFunctionName,
    LilyErrorExpectedToken,
    LilyErrorMissClosingBlock,
    LilyErrorMisuseOfSpecialClosingBlock,
    LilyErrorBadUsageOfAsync,
    LilyErrorBadUsageOfPub,
    LilyErrorInvalidItemInFunOrMethodBody,
    LilyErrorBadUsageOfType,
    LilyErrorMissTypeName,
    LilyErrorInvalidTokenInEnumVariant,
    LilyErrorInvalidTokenInRecordField,
    LilyErrorMissObjectName,
    LilyErrorInvalidTokenInAliasDataType,
    LilyErrorUnexpectedInheritance,
    LilyErrorUnexpectedImplementation,
    LilyErrorInvalidTokenInTagBody,
    LilyErrorMissNameOnPropertyOrMethod,
    LilyErrorInvalidClassItem,
    LilyErrorMissImportValue,
    LilyErrorMissAsValue,
    LilyErrorUnexpectedTokenForBeginingInGlobal,
    LilyErrorUnknownAttribute,
    LilyErrorExpectedAttribute,
    LilyErrorMissDataType,
    LilyErrorIntegerIsOutOfRange,
    LilyErrorMissParamName,
    LilyErrorOperatorIdentifierCannotBeAKeyword,
    LilyErrorMissFieldCallName,
    LilyErrorMissErrorName,
    LilyErrorMissModuleName,
    LilyErrorExpectedRightValue,
    LilyErrorUnexpectedExpression,
    LilyErrorInvalidAttributeInThisBlock,
    LilyErrorBadUsageOfObject,
    LilyErrorExpectedExpression,
    LilyErrorMissVariantName,
    LilyErrorMissFieldName,
    LilyErrorExpectedUserDefinedDataType,
    LilyErrorMissPrototypeName,
    LilyErrorMissParamsPrototype,
    LilyErrorUnexpectedSelfParamInFunction,
    LilyErrorDuplicateSelfParam,
    LilyErrorSelfParamMustBeTheFirstParam,
    LilyErrorUnknownDataType,
    LilyErrorExpectedEnumObjectOrRecordObject,
    LilyErrorDuplicateDeclaration,
    LilyErrorDuplicateVariant,
    LilyErrorDuplicateField,
    LilyErrorBadImportValue,
    LilyErrorUnexpectedCharacterInImportValue,
    LilyErrorExpectedCharacter,
    LilyErrorUnknownDocFlag,
    LilyErrorExpectedCharacterInImportValue,
    LilyErrorEmptyImportValue,
    LilyErrorNotExpectedImportValue,
    LilyErrorDependencyCycleOnImportValue,
    LilyErrorExpectedFinalAccessInImportValue,
    LilyErrorImportValueAccessIsNotFound,
    LilyErrorExpectedFinalImportValue,
    LilyErrorDuplicateGenericParam,
    LilyErrorNameMustStartByLowercaseCharacter,
    LilyErrorNameMustStartByUppercaseCharacter,
    LilyErrorExpectedIntegerDataType,
    LilyErrorExpectedALargerIntegerDataType,
    LilyErrorFloatIsOutOfRange,
    LilyErrorUnmatchedDataType
};

typedef struct LilyError
{
    enum LilyErrorKind kind;
    struct String *s;
} LilyError;

/**
 *
 * @return a new instance of LilyError.
 */
struct LilyError *
__new__LilyError(enum LilyErrorKind kind);

/**
 *
 * @return a new instance of LilyError with initialization of s.
 */
struct LilyError *
__new__LilyErrorWithString(enum LilyErrorKind kind, struct String *s);

/**
 *
 * @brief Free LilyError struct type.
 */
void
__free__LilyError(struct LilyError *self);

enum LilyWarningKind
{
    LilyWarningUnusedParen,
    LilyWarningIgnoredTags,
    LilyWarningIgnoredGenericParams,
    LilyWarningIgnoredLambdaDataType
};

typedef struct LilyWarning
{
    enum LilyWarningKind kind;
    union
    {
        struct String *s;
    };
} LilyWarning;

/**
 *
 * @return a new instance of LilyWarning.
 */
struct LilyWarning *
__new__LilyWarning(enum LilyWarningKind kind);

/**
 * @return a new instance of LilyWarning with initialization of s.
 */
struct LilyWarning *
__new__LilyWarningWithString(enum LilyWarningKind kind, struct String *s);

/**
 *
 * @brief Free LilyWarning type.
 */
void
__free__LilyWarning(struct LilyWarning *self);

typedef struct Diagnostic
{
    union
    {
        struct LilyError *err;
        struct LilyWarning *warn;
        struct String *note;
    };
    enum DiagnosticKind kind;
    struct Location loc;
    struct File file;
    struct Detail *detail;
    struct Option *help;
} Diagnostic;

/**
 *
 * @return a new instance of Diagnostic with LilyError.
 */
struct Diagnostic *
__new__DiagnosticWithErr(struct LilyError *err,
                         struct Location loc,
                         struct File file,
                         struct String *detail_msg,
                         struct Option *help);

/**
 *
 * @return a new instance of Diagnostic with LilyWarning.
 */
struct Diagnostic *
__new__DiagnosticWithWarn(struct LilyWarning *warn,
                          struct Location loc,
                          struct File file,
                          struct String *detail_msg,
                          struct Option *help);

/**
 *
 * @return a new instance of Diagnostic with note.
 */
struct Diagnostic *
__new__DiagnosticWithNote(struct String *note,
                          struct Location loc,
                          struct File file,
                          struct String *detail_msg,
                          struct Option *help);

/**
 *
 * @brief Print diagnostic.
 */
void
emit__Diagnostic(struct Diagnostic *self);

/**
 *
 * @brief Print warning diagnostic.
 */
void
emit_warning__Diagnostic(struct Diagnostic *self,
                         struct Vec *warning_disable_codes);

/**
 *
 * @brief Free Diagnostic type.
 */
void
__free__Diagnostic(struct Diagnostic *self);

#endif // LILY_DIAGNOSTIC_H
