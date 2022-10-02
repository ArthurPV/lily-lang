#include <string.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/string.h>
#include <lang/scanner/token.h>

struct Location
__new__Location()
{
    struct Location self = {
        .s_line = 1,
        .s_col = 1,
        .e_line = 1,
        .e_col = 1
    };
    
    return self;
}

void
start__Location(struct Location *self, Usize line, Usize col)
{
    self->s_line = line;
    self->s_col = col;
}

void
end__Location(struct Location *self, Usize line, Usize col)
{
    self->e_line = line;
    self->e_col = col;
}

struct String *
to_string__Location(struct Location self)
{
    return format("{{ \n\t\ts_line: {d},\n\t\te_line: {d},\n\t\ts_col: "
                  "{d},\n\t\te_col: {d}\n\t}",
                  self.s_line,
                  self.e_line,
                  self.s_col,
                  self.e_col);
}

struct Location * copy__Location(struct Location *self) {
    struct Location *copy = malloc(sizeof(struct Location));

    memcpy(copy, self, sizeof(struct Location));

    return copy;
}

// void
// __free__Location(struct Location *self)
// {
//     // free(self);
// }

struct Doc *
__new__DocWithString(enum DocKind kind, struct String *s)
{
    struct Doc *self = malloc(sizeof(struct Doc));
    self->kind = kind;
    self->s = s;
    return self;
}

struct String *
to_string__Doc(struct Doc self)
{
    switch (self.kind) {
        case DocKindAuthor:
            return format("@author: {S}", self.s);
        case DocKindVersion:
            return format("@version: {{{S}}", self.s);
        default:
            UNREACHABLE("unknown doc kind");
    }
}

void __free__Doc(struct Doc *self) {
    if (self->s)
        FREE(String, self->s);

    free(self);
}

struct Token *
__new__Token(enum TokenKind kind, struct Location *loc)
{
    struct Token *self = malloc(sizeof(struct Token));
    self->kind = kind;
    self->loc = loc;
    return self;
}

struct Token *
__new__TokenLit(enum TokenKind kind, struct Location *loc, struct String *lit)
{
    struct Token *self = malloc(sizeof(struct Token));
    self->kind = kind;
    self->loc = loc;
    self->lit = lit;
    return self;
}

struct Token *
__new__TokenDoc(struct Location *loc, struct Doc *doc)
{
    struct Token *self = malloc(sizeof(struct Token));
    self->kind = TokenKindDocComment;
    self->loc = loc;
    self->doc = doc;
    return self;
}

struct String *
token_kind_to_string__Token(struct Token self)
{
    switch (self.kind) {
        case TokenKindDot:
            return from__String(".");
        case TokenKindDotDotDot:
            return from__String("...");
        case TokenKindComma:
            return from__String(",");
        case TokenKindColon:
            return from__String(":");
        case TokenKindColonColon:
            return from__String("::");
        case TokenKindBar:
            return from__String("|");
        case TokenKindArrow:
            return from__String("->");
        case TokenKindInverseArrow:
            return from__String("<-");
        case TokenKindFatArrow:
            return from__String("=>");
        case TokenKindAt:
            return from__String("@");
        case TokenKindLParen:
            return from__String("(");
        case TokenKindRParen:
            return from__String(")");
        case TokenKindLBrace:
            return from__String("{");
        case TokenKindRBrace:
            return from__String("}");
        case TokenKindLHook:
            return from__String("[");
        case TokenKindRHook:
            return from__String("]");
        case TokenKindHashtag:
            return from__String("#");
        case TokenKindSemicolon:
            return from__String(";");
        case TokenKindDollar:
            return from__String("$");
        case TokenKindBacktrick:
            return from__String("`");
        case TokenKindEof:
            return from__String("Eof");
        case TokenKindPlus:
            return from__String("+");
        case TokenKindMinus:
            return from__String("-");
        case TokenKindStar:
            return from__String("*");
        case TokenKindStarStar:
            return from__String("**");
        case TokenKindStarStarEq:
            return from__String("**=");
        case TokenKindSlash:
            return from__String("/");
        case TokenKindPercentage:
            return from__String("%");
        case TokenKindHat:
            return from__String("^");
        case TokenKindPlusPlus:
            return from__String("++");
        case TokenKindMinusMinus:
            return from__String("--");
        case TokenKindPlusPlusEq:
            return from__String("++=");
        case TokenKindMinusMinusEq:
            return from__String("--=");
        case TokenKindWave:
            return from__String("~");
        case TokenKindWaveEq:
            return from__String("~=");
        case TokenKindColonEq:
            return from__String(":=");
        case TokenKindPlusEq:
            return from__String("+=");
        case TokenKindMinusEq:
            return from__String("-=");
        case TokenKindStarEq:
            return from__String("*=");
        case TokenKindSlashEq:
            return from__String("/=");
        case TokenKindPercentageEq:
            return from__String("%=");
        case TokenKindHatEq:
            return from__String("^=");
        case TokenKindEq:
            return from__String("=");
        case TokenKindEqEq:
            return from__String("==");
        case TokenKindDotDot:
            return from__String("..");
        case TokenKindLShift:
            return from__String("<");
        case TokenKindRShift:
            return from__String(">");
        case TokenKindLShiftLShift:
            return from__String("<<");
        case TokenKindRShiftRShift:
            return from__String(">>");
        case TokenKindLShiftLShiftEq:
            return from__String("<<=");
        case TokenKindRShiftRShiftEq:
            return from__String(">>=");
        case TokenKindLShiftEq:
            return from__String("<=");
        case TokenKindRShiftEq:
            return from__String(">=");
        case TokenKindBang:
            return from__String("!");
        case TokenKindNotEq:
            return from__String("not=");
        case TokenKindInterrogation:
            return from__String("?");
        case TokenKindAmpersand:
            return from__String("&");
        case TokenKindDotStar:
            return from__String(".*");
        case TokenKindDotInterrogation:
            return from__String(".?");
        case TokenKindBarRShift:
            return from__String("|>");
        case TokenKindXorEq:
            return from__String("xor=");
        case TokenKindFunKw:
            return from__String("fun");
        case TokenKindImportKw:
            return from__String("import");
        case TokenKindAsKw:
            return from__String("as");
        case TokenKindPubKw:
            return from__String("pub");
        case TokenKindEndKw:
            return from__String("end");
        case TokenKindTestKw:
            return from__String("test");
        case TokenKindModuleKw:
            return from__String("module");
        case TokenKindTypeKw:
            return from__String("type");
        case TokenKindAliasKw:
            return from__String("alias");
        case TokenKindRecordKw:
            return from__String("record");
        case TokenKindTagKw:
            return from__String("tag");
        case TokenKindEnumKw:
            return from__String("enum");
        case TokenKindMatchKw:
            return from__String("match");
        case TokenKindTrueKw:
            return from__String("true");
        case TokenKindFalseKw:
            return from__String("false");
        case TokenKindIfKw:
            return from__String("if");
        case TokenKindElifKw:
            return from__String("elif");
        case TokenKindElseKw:
            return from__String("else");
        case TokenKindDoKw:
            return from__String("do");
        case TokenKindWhileKw:
            return from__String("while");
        case TokenKindForKw:
            return from__String("for");
        case TokenKindErrorKw:
            return from__String("error");
        case TokenKindClassKw:
            return from__String("class");
        case TokenKindTraitKw:
            return from__String("trait");
        case TokenKindAsyncKw:
            return from__String("async");
        case TokenKindAwaitKw:
            return from__String("await");
        case TokenKindAndKw:
            return from__String("and");
        case TokenKindXorKw:
            return from__String("xor");
        case TokenKindOrKw:
            return from__String("or");
        case TokenKindNotKw:
            return from__String("not");
        case TokenKindNilKw:
            return from__String("nil");
        case TokenKindUndefKw:
            return from__String("undef");
        case TokenKindObjectKw:
            return from__String("object");
        case TokenKindSelfKw:
            return from__String("self");
        case TokenKindTryKw:
            return from__String("try");
        case TokenKindCatchKw:
            return from__String("catch");
        case TokenKindReturnKw:
            return from__String("return");
        case TokenKindNextKw:
            return from__String("next");
        case TokenKindBreakKw:
            return from__String("break");
        case TokenKindAsmKw:
            return from__String("asm");
        case TokenKindMacroKw:
            return from__String("macro");
        case TokenKindImplKw:
            return from__String("impl");
        case TokenKindInKw:
            return from__String("in");
        case TokenKindMutKw:
            return from__String("mut");
        case TokenKindBeginKw:
            return from__String("begin");
        case TokenKindStringLit:
            return format("\"{S}\"", self.lit);
        case TokenKindCharLit:
            return format("'{S}'", self.lit);
        case TokenKindBitStringLit:
            return format("b\"{S}\"", self.lit);
        case TokenKindBitCharLit:
            return format("b'{S}'", self.lit);
        case TokenKindFloatLit:
        case TokenKindIntLit:
            return format("{S}", self.lit);
        case TokenKindOneComment:
            return format("Comment One");
        case TokenKindMultiComment:
            return format("Comment Multi");
        case TokenKindDocComment:
            return format("Comment Doc");
        case TokenKindIdentifier:
            return format("{S}", self.lit);
        default:
            UNREACHABLE("token doesn't exists");
    }
}

struct String *
to_string__Token(struct Token self)
{
    return format("{{ \n\tkind: {Sr},\n\tloc: {Sr}\n}",
                  token_kind_to_string__Token(self),
                  to_string__Location(*self.loc));
}

struct Token *copy__Token(struct Token *self) {
    struct Token *copy = malloc(sizeof(struct Token));

    memcpy(copy, self, sizeof(struct Token));

    return copy;
}

void
__free__Token(struct Token *self) {
    free(self->loc);
    free(self);
}

void __free__TokenLit(struct Token *self) {
    FREE(String, self->lit);
    free(self->loc);
    free(self);
}

void __free__TokenDoc(struct Token *self) {
    FREE(Doc, self->doc);
    free(self->loc);
    free(self);
}

void __free__TokenAll(struct Token *self) {
    switch (self->kind) {
        case TokenKindIdentifier:
        case TokenKindIntLit:
        case TokenKindCharLit:
        case TokenKindStringLit:
        case TokenKindFloatLit:
        case TokenKindBitStringLit:
        case TokenKindBitCharLit:
            FREE(TokenLit, self);
            break;
        case TokenKindDocComment:
            FREE(TokenDoc, self);
            break;
        default:
            FREE(Token, self);
            break;
    }
}
