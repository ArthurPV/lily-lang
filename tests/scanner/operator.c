#include "operator.h"
#include <base/file.h>
#include <base/new.h>
#include <base/test.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

int
test_operator()
{
    struct File *file = NEW(File, "./tests/scanner/operator.lily");
    struct Source *src = NEW(Source, file);
    struct Scanner *scanner = NEW(Scanner, src);
    run__Scanner(scanner);

    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 0))->kind,
                   TokenKindPlus);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 1))->kind,
                   TokenKindMinus);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 2))->kind,
                   TokenKindStar);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 3))->kind,
                   TokenKindStarStar);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 4))->kind,
                   TokenKindStarStarEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 5))->kind,
                   TokenKindSlash);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 6))->kind,
                   TokenKindPercentage);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 7))->kind,
                   TokenKindHat);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 8))->kind,
                   TokenKindPlusPlus);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 9))->kind,
                   TokenKindMinusMinus);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 10))->kind,
                   TokenKindPlusPlusEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 11))->kind,
                   TokenKindMinusMinusEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 12))->kind,
                   TokenKindColonEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 13))->kind,
                   TokenKindPlusEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 14))->kind,
                   TokenKindMinusEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 15))->kind,
                   TokenKindStarEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 16))->kind,
                   TokenKindSlashEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 17))->kind,
                   TokenKindPercentageEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 18))->kind,
                   TokenKindHatEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 19))->kind,
                   TokenKindEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 20))->kind,
                   TokenKindEqEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 21))->kind,
                   TokenKindDotDot);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 22))->kind,
                   TokenKindLShift);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 23))->kind,
                   TokenKindRShift);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 24))->kind,
                   TokenKindLShiftLShift);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 25))->kind,
                   TokenKindRShiftRShift);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 26))->kind,
                   TokenKindLShiftLShiftEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 27))->kind,
                   TokenKindRShiftRShiftEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 28))->kind,
                   TokenKindLShiftEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 29))->kind,
                   TokenKindRShiftEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 30))->kind,
                   TokenKindNotEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 31))->kind,
                   TokenKindInterrogation);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 32))->kind,
                   TokenKindAmpersand);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 33))->kind,
                   TokenKindDotStar);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 34))->kind,
                   TokenKindDotInterrogation);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 35))->kind,
                   TokenKindBarRShift);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 36))->kind,
                   TokenKindWave);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 37))->kind,
                   TokenKindWaveEq);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 38))->kind,
                   TokenKindXorEq);

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}