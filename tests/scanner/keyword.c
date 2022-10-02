#include "keyword.h"
#include <base/file.h>
#include <base/new.h>
#include <base/test.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

int test_keyword() {
    struct File file = NEW(File, "./tests/scanner/keyword.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);
    run__Scanner(&scanner);

    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 0))->kind,
                   TokenKindFunKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 1))->kind,
                   TokenKindImportKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 2))->kind,
                   TokenKindAsKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 3))->kind,
                   TokenKindPubKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 4))->kind,
                   TokenKindEndKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 5))->kind,
                   TokenKindTestKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 6))->kind,
                   TokenKindModuleKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 7))->kind,
                   TokenKindTypeKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 8))->kind,
                   TokenKindAliasKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 9))->kind,
                   TokenKindRecordKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 10))->kind,
                   TokenKindTagKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 11))->kind,
                   TokenKindEnumKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 12))->kind,
                    TokenKindMatchKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 13))->kind,
                   TokenKindTrueKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 14))->kind,
                   TokenKindFalseKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 15))->kind,
                   TokenKindIfKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 16))->kind,
                   TokenKindElifKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 17))->kind,
                   TokenKindElseKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 18))->kind,
                   TokenKindDoKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 19))->kind,
                   TokenKindWhileKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 20))->kind,
                   TokenKindForKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 21))->kind,
                   TokenKindErrorKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 22))->kind,
                   TokenKindClassKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 23))->kind,
                   TokenKindTraitKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 24))->kind,
                   TokenKindAsyncKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 25))->kind,
                   TokenKindAwaitKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 26))->kind,
                   TokenKindAndKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 27))->kind,
                   TokenKindXorKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 28))->kind,
                   TokenKindOrKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 29))->kind,
                   TokenKindNotKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 30))->kind,
                   TokenKindNilKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 31))->kind,
                   TokenKindUndefKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 32))->kind,
                   TokenKindObjectKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 33))->kind,
                   TokenKindSelfKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 34))->kind,
                   TokenKindTryKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 35))->kind,
                   TokenKindCatchKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 36))->kind,
                   TokenKindReturnKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 37))->kind,
                   TokenKindNextKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 38))->kind,
                   TokenKindBreakKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 39))->kind,
                   TokenKindAsmKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 40))->kind,
                   TokenKindMacroKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 41))->kind,
                   TokenKindImplKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 42))->kind,
                   TokenKindInKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 43))->kind,
                   TokenKindMutKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 44))->kind,
                   TokenKindBeginKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 45))->kind,
                   TokenKindGlobalKw);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner.tokens, 46))->kind,
                   TokenKindPackageKw);

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}
