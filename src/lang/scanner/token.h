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

#ifndef LILY_TOKEN_H
#define LILY_TOKEN_H

#include <base/types.h>

enum TokenKind
{
    TokenKindDot,
    TokenKindDotDotDot,
    TokenKindComma,
    TokenKindColon,
    TokenKindColonColon,
    TokenKindBar,
    TokenKindBarEq,
    TokenKindArrow,
    TokenKindInverseArrow,
    TokenKindFatArrow,
    TokenKindAt,
    TokenKindLParen,
    TokenKindRParen,
    TokenKindLBrace,
    TokenKindRBrace,
    TokenKindLHook,
    TokenKindRHook,
    TokenKindHashtag,
    TokenKindSemicolon,
    TokenKindDollar,
    TokenKindEof,
    TokenKindPlus,
    TokenKindMinus,
    TokenKindStar,
    TokenKindStarStar,
    TokenKindStarStarEq,
    TokenKindSlash,
    TokenKindPercentage,
    TokenKindHat,
    TokenKindPlusPlus,
    TokenKindMinusMinus,
    TokenKindPlusPlusEq,
    TokenKindMinusMinusEq,
    TokenKindWave,
    TokenKindColonEq,
    TokenKindPlusEq,
    TokenKindMinusEq,
    TokenKindStarEq,
    TokenKindSlashEq,
    TokenKindPercentageEq,
    TokenKindHatEq,
    TokenKindEq,
    TokenKindEqEq,
    TokenKindDotDot,
    TokenKindLShift,
    TokenKindRShift,
    TokenKindLShiftLShift,
    TokenKindRShiftRShift,
    TokenKindLShiftLShiftEq,
    TokenKindRShiftRShiftEq,
    TokenKindLShiftEq,
    TokenKindRShiftEq,
    TokenKindBang,
    TokenKindNotEq,
    TokenKindInterrogation,
    TokenKindAmpersand,
    TokenKindAmpersandEq,
    TokenKindDotStar,
    TokenKindDotInterrogation,
    TokenKindBarRShift,
    TokenKindXorEq,
    TokenKindColonDollar,
    TokenKindFunKw,
    TokenKindImportKw,
    TokenKindAsKw,
    TokenKindPubKw,
    TokenKindEndKw,
    TokenKindTestKw,
    TokenKindModuleKw,
    TokenKindTypeKw,
    TokenKindAliasKw,
    TokenKindRecordKw,
    TokenKindTagKw,
    TokenKindEnumKw,
    TokenKindMatchKw,
    TokenKindTrueKw,
    TokenKindFalseKw,
    TokenKindIfKw,
    TokenKindElifKw,
    TokenKindElseKw,
    TokenKindDoKw,
    TokenKindWhileKw,
    TokenKindForKw,
    TokenKindErrorKw,
    TokenKindClassKw,
    TokenKindTraitKw,
    TokenKindAsyncKw,
    TokenKindAwaitKw,
    TokenKindAndKw,
    TokenKindXorKw,
    TokenKindOrKw,
    TokenKindNotKw,
    TokenKindNilKw,
    TokenKindNoneKw,
    TokenKindUndefKw,
    TokenKindObjectKw,
    TokenKindSelfKw,
    TokenKindTryKw,
    TokenKindCatchKw,
    TokenKindReturnKw,
    TokenKindNextKw,
    TokenKindBreakKw,
    TokenKindAsmKw,
    TokenKindMacroKw,
    TokenKindImplKw,
    TokenKindInKw,
    TokenKindMutKw,
    TokenKindBeginKw,
    TokenKindGlobalKw,
    TokenKindPackageKw,
    TokenKindStringLit,
    TokenKindCharLit,
    TokenKindBitStringLit,
    TokenKindBitCharLit,
    TokenKindFloatLit,
    TokenKindIntLit,
    TokenKindOneComment,
    TokenKindMultiComment,
    TokenKindDocComment,
    TokenKindIdentifier,
    TokenKindIdentifierOp
};

typedef struct Location
{
    Usize s_line;
    Usize s_col;
    Usize e_line;
    Usize e_col;
} Location;

/**
 *
 * @return new instance of Location.
 */
struct Location
__new__Location();

/**
 *
 * @brief Assign s_line and s_col to line and col.
 */
void
start__Location(struct Location *self, Usize line, Usize col);

/**
 *
 * @brief Assign e_line and e_col to line and col.
 */
void
end__Location(struct Location *self, Usize line, Usize col);

/**
 *
 * @brief Convert Location type to String.
 */
struct String *
to_String__Location(struct Location self);

/**
 *
 * @brief Copy location pointer.
 */
struct Location *
copy__Location(struct Location *self);

/**
 *
 * @brief Free Location type.
 */
// void
// __free__Location(struct Location *self);

enum DocKind
{
    DocKindAuthor = 1,
    DocKindContract,
    DocKindDescription,
    DocKindGlobal,
    DocKindText,
    DocKindFile,
    DocKindGeneric,
    DocKindPrototype,
    DocKindSee,
    DocKindVersion,
};

typedef struct Doc
{
    enum DocKind kind;
    union
    {
        struct String *s;
        struct Vec *contract; // struct Vec<struct Token*>*
        struct Vec *generic;  // struct Vec<struct Token*>*
        struct Vec *prot;     // struct Vec<struct Token*>*
    };
} Doc;

/**
 *
 * @return new instance of Doc with String.
 */
struct Doc *
__new__DocWithString(enum DocKind kind, struct String *s);

/**
 *
 * @return new instance of Doc (Contract variant).
 */
struct Doc *
__new__DocContract(struct Vec *contract);

/**
 *
 * @return new instance of Doc (Generic variant).
 */
struct Doc *
__new__DocGeneric(struct Vec *generic);

/**
 *
 * @return new instance of Doc (Prototype variant).
 */
struct Doc *
__new__DocPrototype(struct Vec *prot);

/**
 *
 * @brief Convert to doc to string.
 */
struct String *
to_String__Doc(struct Doc self);

/**
 *
 * @brief Free Doc type (String variant).
 */
void
__free__DocString(struct Doc *self);

/**
 *
 * @brief Free Doc type (Contract variant).
 */
void
__free__DocContract(struct Doc *self);

/**
 *
 * @brief Free Doc type (Generics variant).
 */
void
__free__DocGenerics(struct Doc *self);

/**
 *
 * @brief Free Doc type (Prototype variant).
 */
void
__free__DocPrototype(struct Doc *self);

/**
 *
 * @brief Free Doc type.
 */
void
__free__DocAll(struct Doc *self);

typedef struct Token
{
    enum TokenKind kind;
    union
    {
        struct String *lit;
        struct Doc *doc;
    };
    struct Location *loc;
} Token;

/**
 *
 * @return new instance of Token.
 */
struct Token *
__new__Token(enum TokenKind kind, struct Location *loc);

/**
 *
 * @return new instance of Token with literal union.
 */
struct Token *
__new__TokenLit(enum TokenKind kind, struct Location *loc, struct String *lit);

/**
 *
 * @return new instance of Token with doc union.
 */
struct Token *
__new__TokenDoc(struct Location *loc, struct Doc *doc);

/**
 *
 * @return token kind converted in String.
 */
struct String *
token_kind_to_String__Token(struct Token self);

/**
 *
 * @brief Convert Token to String.
 */
struct String *
to_String__Token(struct Token self);

/**
 *
 * @brief Copy the Token type.
 */
struct Token *
copy__Token(struct Token *self);

/**
 *
 * @brief Free Token type.
 */
void
__free__Token(struct Token *self);

/**
 *
 * @brief Free Token type with Literal.
 */
void
__free__TokenLit(struct Token *self);

/**
 *
 * @brief Free Token type with Doc.
 */
void
__free__TokenDoc(struct Token *self);

/**
 *
 * @brief Free Token (all variants).
 */
void
__free__TokenAll(struct Token *self);

#endif // LILY_TOKEN_H
