#include <string.h>
#include <stdlib.h>

#include <base/assert.h>
#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>
#include <base/tuple.h>
#include <base/map.h>
#include <base/file.h>
#include <base/format.h>
#include <base/mem.h>
#include <base/print.h>
#include <lang/scanner/token.h>
#include <lang/scanner/scanner.h>
#include <lang/parser/parser.h>
#include <lang/analysis/typecheck.h>

int main() {
//    struct Vec *v = NEW(Vec, sizeof(int*));
//
//    push__Vec(v, (int*)1);
//    push__Vec(v, (int*)2);
//    push__Vec(v, (int*)3);
//
//    ASSERT_EQ((int*)1, (int*)get__Vec(*v, 0));
//    ASSERT_EQ((int*)2, (int*)get__Vec(*v, 1));
//    ASSERT_EQ((int*)3, (int*)get__Vec(*v, 2));
//    ASSERT_EQ(3, len__Vec(*v));
//
//    remove__Vec(v, 0);
//
//    ASSERT_EQ((int*)2, (int*)get__Vec(*v, 0));
//    ASSERT_EQ((int*)3, (int*)get__Vec(*v, 1));
//    ASSERT_EQ(2, len__Vec(*v));
//
//    struct Vec *v2 = NEW(Vec, sizeof(int*));
//
//    push__Vec(v2, (int*)4);
//    push__Vec(v2, (int*)5);
//    push__Vec(v2, (int*)6);
//
//    struct Vec v3 = concat__Vec(*v, *v2);
//    ASSERT_EQ(5, len__Vec(v3));
//
//    struct Vec v4 = fill__Vec(v3, (int*)0, NULL, NULL);
//    ASSERT_EQ((int*)0, get__Vec(v4, 0));
//    ASSERT_EQ(5, len__Vec(v4));
//
//    modify_item__Vec(&v4, (int*)4, 0);
//    ASSERT_EQ((int*)4, get__Vec(v4, 0));
//
//    void *items[2] = { (int*)2, (int*)10 };
//    struct Vec *v5 = from__Vec(items, sizeof(int*), 2);
//
//    ASSERT_EQ((int*)2, get__Vec(*v5, 0));
//    ASSERT_EQ((int*)10, get__Vec(*v5, 1));
//
//    bool is_include = includes__Vec(*v5, (int*)2);
//    ASSERT_EQ(is_include, true);
//
//    struct String *s = NEW(String);
//    push__String(s, (char*)'h');
//    push__String(s, (char*)'e');
//    push__String(s, (char*)'l');
//    push__String(s, (char*)'l');
//    push__String(s, (char*)'o');
//
//    ASSERT_EQ((char*)'h', get__String(*s, 0));
//    ASSERT_EQ((char*)'e', get__String(*s, 1));
//    ASSERT_EQ((char*)'l', get__String(*s, 2));
//    ASSERT_EQ((char*)'l', get__String(*s, 3));
//    ASSERT_EQ((char*)'o', get__String(*s, 4));
//
//    Str str = to_Str__String(*s);
//    ASSERT_EQ((char)'h', str[0]);
//    ASSERT_EQ((char)'e', str[1]);
//    ASSERT_EQ((char)'l', str[2]);
//    ASSERT_EQ((char)'l', str[3]);
//    ASSERT_EQ((char)'o', str[4]);
//
//    struct Vec *v6 = NEW(Vec, sizeof(int*));
//
//    push__Vec(v6, (int*)1);
//    push__Vec(v6, (int*)2);
//    push__Vec(v6, (int*)4);
//    insert__Vec(v6, (int*)3, 2);
//
//    ASSERT_EQ((int*)1, (int*)get__Vec(*v6, 0));
//    ASSERT_EQ((int*)2, (int*)get__Vec(*v6, 1));
//    ASSERT_EQ((int*)3, (int*)get__Vec(*v6, 2));
//    ASSERT_EQ((int*)4, (int*)get__Vec(*v6, 3));
//
//    struct Vec *v7 = NEW(Vec, sizeof(char*));
//
//    push__Vec(v7, (char*)"slacturyx");
//    push__Vec(v7, (char*)"winryx");
//    char *v7_join = join__Vec(v7, "@");
//
//    assert(!strcmp("slacturyx@winryx", v7_join) && "Bad Str assertion");
//
//    struct Vec *v8 = NEW(Vec, sizeof(int*));
//    push__Vec(v8, (int*)1);
//    push__Vec(v8, (int*)2);
//    push__Vec(v8, (int*)3);
//    push__Vec(v8, (int*)4);
//    struct Vec *v8_reverse = reverse__Vec(*v8);
//
//    ASSERT_EQ(4, len__Vec(*v8_reverse));
//
//    ASSERT_EQ((int*)4, get__Vec(*v8_reverse, 0));
//    ASSERT_EQ((int*)3, get__Vec(*v8_reverse, 1));
//    ASSERT_EQ((int*)2, get__Vec(*v8_reverse, 2));
//    ASSERT_EQ((int*)1, get__Vec(*v8_reverse, 3));
//
//    struct Vec *v8_sort = sort__Vec(*v8, false);
//
//    ASSERT_EQ((int*)1, get__Vec(*v8_sort, 0));
//    ASSERT_EQ((int*)2, get__Vec(*v8_sort, 1));
//    ASSERT_EQ((int*)3, get__Vec(*v8_sort, 2));
//    ASSERT_EQ((int*)4, get__Vec(*v8_sort, 3));
//
//    struct Vec *v9 = NEW(Vec, sizeof(int*));
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//    push__Vec(v9, (int*)10);
//
//
//    struct String *s2 = NEW(String);
//    push__String(s2, (char*)'h');
//    push__String(s2, (char*)'e');
//    push__String(s2, (char*)'l');
//    push__String(s2, (char*)'l');
//    push__String(s2, (char*)'o');
//
//    struct String *s3 = NEW(String);
//    push__String(s3, (char*)'w');
//    push__String(s3, (char*)'o');
//    push__String(s3, (char*)'r');
//    push__String(s3, (char*)'l');
//    push__String(s3, (char*)'d');
//
//    struct String s4 = concat__String(*s2, *s3);
//
//    ASSERT_EQ((char*)'h', get__String(s4, 0));
//    ASSERT_EQ((char*)'e', get__String(s4, 1));
//    ASSERT_EQ((char*)'l', get__String(s4, 2));
//    ASSERT_EQ((char*)'l', get__String(s4, 3));
//    ASSERT_EQ((char*)'o', get__String(s4, 4));
//    ASSERT_EQ((char*)'w', get__String(s4, 5));
//    ASSERT_EQ((char*)'o', get__String(s4, 6));
//    ASSERT_EQ((char*)'r', get__String(s4, 7));
//    ASSERT_EQ((char*)'l', get__String(s4, 8));
//    ASSERT_EQ((char*)'d', get__String(s4, 9));
//
//    bool ends_with_world = ends_with__String(s4, *s3, NULL);
//
//    struct String *s5 = from__String("Hello");
//
//    ASSERT_EQ((char*)'H', get__String(*s5, 0));
//    ASSERT_EQ((char*)'e', get__String(*s5, 1));
//    ASSERT_EQ((char*)'l', get__String(*s5, 2));
//    ASSERT_EQ((char*)'l', get__String(*s5, 3));
//    ASSERT_EQ((char*)'o', get__String(*s5, 4));
//
//    push_str__String(s5, " World");
//
//    ASSERT_EQ((char*)'H', get__String(*s5, 0));
//    ASSERT_EQ((char*)'e', get__String(*s5, 1));
//    ASSERT_EQ((char*)'l', get__String(*s5, 2));
//    ASSERT_EQ((char*)'l', get__String(*s5, 3));
//    ASSERT_EQ((char*)'o', get__String(*s5, 4));
//    ASSERT_EQ((char*)' ', get__String(*s5, 5));
//    ASSERT_EQ((char*)'W', get__String(*s5, 6));
//    ASSERT_EQ((char*)'o', get__String(*s5, 7));
//    ASSERT_EQ((char*)'r', get__String(*s5, 8));
//    ASSERT_EQ((char*)'l', get__String(*s5, 9));
//    ASSERT_EQ((char*)'d', get__String(*s5, 10));
//
//    struct String *s6 = repeat__String("^", 10);
//    Str s6_str = to_Str__String(*s6);
//
//    ASSERT_EQ((char*)'^', get__String(*s6, 0));
//    ASSERT_EQ((char*)'^', get__String(*s6, 1));
//    ASSERT_EQ((char*)'^', get__String(*s6, 2));
//    ASSERT_EQ((char*)'^', get__String(*s6, 3));
//    ASSERT_EQ((char*)'^', get__String(*s6, 4));
//    ASSERT_EQ((char*)'^', get__String(*s6, 5));
//    ASSERT_EQ((char*)'^', get__String(*s6, 6));
//    ASSERT_EQ((char*)'^', get__String(*s6, 7));
//    ASSERT_EQ((char*)'^', get__String(*s6, 8));
//    ASSERT_EQ((char*)'^', get__String(*s6, 9));
//
//    struct String *s7 = from__String("Hello");
//
//    ASSERT_EQ((char*)'H', get__String(*s7, 0));
//    ASSERT_EQ((char*)'e', get__String(*s7, 1));
//    ASSERT_EQ((char*)'l', get__String(*s7, 2));
//    ASSERT_EQ((char*)'l', get__String(*s7, 3));
//    ASSERT_EQ((char*)'o', get__String(*s7, 4));
//
//    struct String *s8 = insert__String(*s6, *s7, 2);
//
//    ASSERT_EQ((char*)'^', get__String(*s8, 0));
//    ASSERT_EQ((char*)'^', get__String(*s8, 1));
//    ASSERT_EQ((char*)'H', get__String(*s8, 2));
//    ASSERT_EQ((char*)'e', get__String(*s8, 3));
//    ASSERT_EQ((char*)'l', get__String(*s8, 4));
//    ASSERT_EQ((char*)'l', get__String(*s8, 5));
//    ASSERT_EQ((char*)'o', get__String(*s8, 6));
//    ASSERT_EQ((char*)'^', get__String(*s8, 7));
//    ASSERT_EQ((char*)'^', get__String(*s8, 8));
//    ASSERT_EQ((char*)'^', get__String(*s8, 9));
//    ASSERT_EQ((char*)'^', get__String(*s8, 10));
//    ASSERT_EQ((char*)'^', get__String(*s8, 11));
//    ASSERT_EQ((char*)'^', get__String(*s8, 12));
//    ASSERT_EQ((char*)'^', get__String(*s8, 13));
//
//    void *items2[2] = {(int*)0, (int*)1};
//    struct Vec *v12 = from__Vec(items2, sizeof(int*), 2);
//
//    ASSERT_EQ((int*)0, get__Vec(*v12, 0));
//    ASSERT_EQ((int*)1, get__Vec(*v12, 1));
//
//    truncate__Vec(v12, 0);
//    ASSERT_EQ(0, len__Vec(*v12));
//
//    bool is_clear = is_clear__Vec(*v12);
//    ASSERT_EQ(is_clear, true);
//
//    void *items3[8] = {(int*)0, (int*)1, (int*)2, (int*)3, (int*)4, (int*)5, (int*)6, (int*)7};
//    struct Vec *v13 = from__Vec(items3, sizeof(int*), 8);
//    ASSERT_EQ(v13->capacity, 16);
//    ASSERT_EQ(v13->len, 9);
//    pop__Vec(v13);
//    pop__Vec(v13);
//    ASSERT_EQ(v13->len, 7);
//    ASSERT_EQ(v13->capacity, 8);
//
//    struct Tuple *tp = NEW(Tuple, 2, (int*)3, (char*)'h');
//    ASSERT_EQ((int*)3, get__Tuple(*tp, 0));
//    ASSERT_EQ((char*)'h', get__Tuple(*tp, 1));
//
//    struct HashMap *hm = NEW(HashMap, sizeof(int*), sizeof(bool*));
//    insert__HashMap(hm, (int*)1, (bool*)false);
//    insert__HashMap(hm, (int*)2, (bool*)true);
//    insert__HashMap(hm, (int*)3, (bool*)false);
//    insert__HashMap(hm, (int*)4, (bool*)false);
//    insert__HashMap(hm, (int*)5, (bool*)true);
//    insert__HashMap(hm, (int*)6, (bool*)true);
//    insert__HashMap(hm, (int*)7, (bool*)true);
//
//    ASSERT_EQ(get__HashMap(*hm, (int*)1), (bool*)false);
//    ASSERT_EQ(get__HashMap(*hm, (int*)6), (bool*)true);
//
//    struct Tuple* tp_hm = get_key_value__HashMap(*hm, (int*)3);
//
//    ASSERT_EQ(get__Tuple(*tp_hm, 0), (int*)3);
//    ASSERT_EQ(get__Tuple(*tp_hm, 1), (bool*)false);
//
//    struct Path *path = NEW(Path, "./app.lily");
//    Str path_extension = get_extension__Path(*path);
//    struct String *content = read_file__Path(*path);
//    Str content_str = to_Str__String(*content);
//    ASSERT_EQ(!strcmp(path_extension, ".lily"), true);
//
//    struct String *f = format("Hello {s}", "Arthur");
//    Str f_str = to_Str__String(*f);
//    // printf("%s\n", f_str);
//
//    struct Location *loc = NEW(Location);
//    loc->s_line = 10;
//    loc->e_line = 11;
//    struct Token *tok = NEW(Token, TokenKindAliasKw, loc);
//    struct String *tok_string = to_string__Token(*tok);
//    Str tok_str = to_Str__String(*tok_string);
//    printf("%s\n", tok_str);
//
//    struct String *hello = format("Hello, {s}", "World");
//    Str str6 = to_Str__String(*hello);
//
//    struct String *s15 = format("Hello@Hi@Bonjour");
//    struct Vec *s15_splited = split__String(*s15, '@');
//
//    ASSERT_EQ(3, len__Vec(*s15_splited));
//
//    struct String *s16 = format("{Sr} world", from__String("hello"));
//
//    struct Vec *st = NEW(Vec, sizeof(int*));
//
//    for (Usize i = 0; i < 1000000; i++) {
//        push__Vec(st, (int*)i);
//    }

    struct File file = NEW(File, "./app.lily");
    struct Source src = NEW(Source, file);
    struct Scanner scanner = NEW(Scanner, &src);

    run__Scanner(&scanner);

    struct ParseBlock parse_block = NEW(ParseBlock, scanner);

    run__ParseBlock(&parse_block);

    struct Parser parser = NEW(Parser, parse_block);

    run__Parser(&parser);

    struct Typecheck tc = NEW(Typecheck, parser);
    // run_without_multi_thread__Parser(&parser);

//    int *v11_temp[9] = {(int*)1, (int*)2, (int*)3, (int*)4, (int*)5, (int*)6, (int*)7, (int*)8};
//    struct Vec *v11 = from__Vec((void**)v11_temp, sizeof(int*), 8);
//    remove__Vec(v11, 3);
//    pop__Vec(v11);

//    FREE(Vec, v);
//    FREE(Vec, v2);
//    FREE(Vec, v5);
//    free(str);
//    FREE(String, s);
//    FREE(Vec, v6);
//    FREE(Vec, v7);
//    free(v7_join);
//    FREE(Vec, v8);
//    FREE(Vec, v8_reverse);
//    FREE(Vec, v8_sort);
//    FREE(Vec, v9);
//    FREE(String, s2);
//    FREE(String, s3);
//    FREE(String, s5);
//    FREE(String, s6);
//    free(s6_str);
//    FREE(String, s7);
//    FREE(String, s8);
//    FREE(Vec, v12);
//    FREE(Vec, v13);
//    FREE(Tuple, tp);
//    FREE(HashMap, hm);
//    FREE(Tuple, tp_hm);
//    FREE(Path, path);
//    free(path_extension);
//    FREE(String, content);
//    free(content_str);
//    FREE(String, f);
//    free(f_str);
//    FREE(String, tok_string);
//    free(tok_str);
//    FREE(Token, tok);
//    FREE(String, hello);
//    free(str6);
//    FREE(String, s15);
//    FREE(Vec, s15_splited);
//    FREE(String, s16);
//    FREE(Vec, st);
    FREE(Typecheck, tc);
//    FREE(Vec, v11);
}
