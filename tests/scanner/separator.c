#include "separator.h"
#include <base/file.h>
#include <base/new.h>
#include <base/test.h>
#include <base/print.h>
#include <lang/scanner/scanner.h>
#include <lang/scanner/token.h>

int test_separator() {
    struct File *file = NEW(File, "./tests/scanner/separator.lily");
    struct Source *src = NEW(Source, file);
    struct Scanner *scanner = NEW(Scanner, src);
    run__Scanner(scanner);

    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 0))->kind,
                   TokenKindDot);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 1))->kind,
                   TokenKindDotDotDot);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 2))->kind,
                   TokenKindComma);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 3))->kind,
                   TokenKindColon);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 4))->kind,
                   TokenKindColonColon);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 5))->kind,
                   TokenKindBar);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 6))->kind,
                   TokenKindArrow);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 7))->kind,
                   TokenKindInverseArrow);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 8))->kind,
                   TokenKindFatArrow);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 9))->kind,
                   TokenKindAt);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 10))->kind,
                   TokenKindLParen);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 11))->kind,
                   TokenKindRParen);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 12))->kind,
                   TokenKindLBrace);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 13))->kind,
                   TokenKindRBrace);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 14))->kind,
                   TokenKindLHook);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 15))->kind,
                   TokenKindRHook);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 16))->kind,
                   TokenKindHashtag);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 17))->kind,
                   TokenKindSemicolon);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 18))->kind,
                   TokenKindDollar);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 19))->kind,
                   TokenKindBacktrick);
    TEST_ASSERT_EQ(((struct Token *)get__Vec(*scanner->tokens, 20))->kind,
                   TokenKindEof);

    FREE(Scanner, scanner);

    return TEST_SUCCESS;
}