#ifndef LILY_TOKEN_H
#define LILY_TOKEN_H

#include <base/types.h>

enum TokenKind {
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
    TokenKindWaveEq,
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
to_string__Location(struct Location self);

/**
 *
 * @brief Copy location pointer.
 */
struct Location * copy__Location(struct Location *self);

/**
 *
 * @brief Free Location type.
 */
// void
// __free__Location(struct Location *self);

enum DocKind
{
    DocKindAuthor,
    DocKindVersion
};

typedef struct Doc
{
    enum DocKind kind;
    union
    {
        struct String *s;
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
 * @brief Convert to doc to string.
 */
struct String *
to_string__Doc(struct Doc self);

/**
 *
 * @brief Free Doc type.
 */
void __free__Doc(struct Doc *self);

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
token_kind_to_string__Token(struct Token self);

/**
 *
 * @brief Convert Token to String.
 */
struct String *
to_string__Token(struct Token self);

/**
 *
 * @brief Copy the Token type.
 */
struct Token *copy__Token(struct Token *self);

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
void __free__TokenLit(struct Token *self);

/**
 *
 * @brief Free Token type with Doc.
 */
void __free__TokenDoc(struct Token *self);

/**
 *
 * @brief Free Token (all variants).
 */
void __free__TokenAll(struct Token *self);

#endif // LILY_TOKEN_H
