#include <assert.h>
#include <base/new.h>
#include <base/string.h>
#include <base/types.h>
#include <stdlib.h>
#include <string.h>

struct String *
__new__String()
{
    struct String *self = malloc(sizeof(struct String));
    self->content = NEW(Vec, sizeof(char *));
    push__Vec(self->content, (char *)'\0');
    return self;
}

struct String *
from__String(const Str s)
{
    struct String *self = NEW(String);

    for (Usize i = 0; i < strlen(s); i++)
        push__String(self, (char *)(UPtr)s[i]);

    return self;
}

void
push__String(struct String *self, const char *c)
{
    modify_item__Vec(self->content, (char *)c, self->content->len - 1);
    push__Vec(self->content, (char *)'\0');
}

void
append__String(struct String *self, struct String *self2, bool drop)
{
    for (Usize i = 0; i < len__String(*self2); i++)
        push__String(self, get__String(*self2, i));

    if (drop)
        FREE(String, self2);
}

void
push_str__String(struct String *self, const Str s)
{
    for (Usize i = 0; i < strlen(s); i++)
        push__String(self, (char *)(UPtr)s[i]);
}

char *
get__String(struct String self, Usize idx)
{
    assert(idx < self.content->len - 1 || idx < 0 && "index out of bounds");
    return (char *)get__Vec(*self.content, idx);
}

char *
get_code__String(struct String self, Usize idx)
{
    return (char *)get__String(self, idx);
}

struct String
concat__String(struct String self, struct String self2)
{
    for (Usize i = 0; i < self2.content->len - 1; i++)
        push__String(&self, get__String(self2, i));

    return self;
}

bool
ends_with__String(struct String self,
                  struct String search_string,
                  const Usize *len)
{
    assert(search_string.content->len < self.content->len &&
           "index out of bounds");
    Usize count = 0;

    if (len == NULL) {
        for (Usize i = self.content->len - search_string.content->len;
             i < self.content->len - 1;
             i++)
            if (get__String(self, i) != get__String(search_string, count++))
                return false;
        return true;
    } else {
        for (Usize i = self.content->len - *len; i < self.content->len - 1; i++)
            if (get__String(self, i) != get__String(search_string, count++))
                return false;
        return true;
    }
}

#include <stdio.h>

Str
to_Str__String(struct String self)
{
    Str s = malloc(self.content->len);
    s[0] = '\0';

    for (Usize i = 0; i < self.content->len - 1; i++) {
        char c = (char)(UPtr)get__String(self, i);
        s[i] = c;
        s[i + 1] = '\0';
    }

    return s;
}

struct String *
repeat__String(const Str s, Usize count)
{
    assert(count >= 0 && "failed count < 0");
    struct String *self = NEW(String);

    for (Usize i = 0; i < count; i++) {
        push_str__String(self, s);
    }

    return self;
}

struct String *
insert__String(struct String self, struct String self2, Usize idx)
{
    assert(idx + self2.content->len - 1 < self.content->len - 1 ||
           idx < 0 && "index out of bounds");
    struct String *s = NEW(String);
    Usize count = 0;

    for (Usize i = 0; i < idx; i++)
        push__String(s, get__String(self, i));

    for (Usize i = idx;
         i < self.content->len - 1 && count < self2.content->len - 1;
         i++)
        push__String(s, get__String(self2, count++));

    for (Usize i = idx; i < self.content->len - 1; i++)
        push__String(s, get__String(self, i));

    return s;
}

void
replace__String(struct String *self, const char *c, Usize idx)
{
    modify_item__Vec(self->content, (void *)c, idx);
}

void
remove__String(struct String *self, Usize idx)
{
    assert(idx < self->content->len - 1 && "index out of bounds");
    remove__Vec(self->content, idx);
}

char *
pop__String(struct String *self)
{
    return pop__Vec(self->content);
}

bool
is_empty__String(struct String self)
{
    return self.content->len == 0;
}

struct Vec *
split__String(struct String self, char c)
{
    struct Vec *v = NEW(Vec, sizeof(struct String));

    for (Usize i = 0; i < len__String(self); i += 1) {
        struct String *splitted = NEW(String);

        if ((UPtr)get__String(self, i) != (UPtr)c) {
            while (i < len__String(self) &&
                   (UPtr)get__String(self, i) != (UPtr)c) {
                push__String(splitted, get__String(self, i));
                i += 1;
            }

            push__Vec(v, splitted);
        } else
            push__Vec(v, splitted);
    }

    return v;
}

bool
eq__String(struct String *self, struct String *self2, bool drop)
{
    bool is_equal = true;

    if (len__String(*self) < len__String(*self2) ||
        len__String(*self2) < len__String(*self))
        return false;

    for (Usize i = 0; i < len__String(*self); i++)
        if (get__String(*self, i) != get__String(*self2, i)) {
            is_equal = false;
            break;
        }

    if (drop)
        FREE(String, self2);

    return is_equal;
}

struct String *
copy__String(struct String *self)
{
    struct String *copy = malloc(sizeof(struct String));

    copy = memcpy(copy, self, sizeof(struct String));

    return copy;
}

Usize
len__String(struct String self)
{
    return self.content->len - 1;
}

Usize
capacity__String(struct String self)
{
    return self.content->capacity;
}

void
__free__String(struct String *self)
{
    FREE(Vec, self->content);
    free(self);
}
