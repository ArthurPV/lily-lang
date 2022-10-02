#include <base/print.h>
#include <base/string.h>
#include <base/new.h>
#include <base/format.h>

int Print(const Str format_, ...) {
    struct String *s = format(format_);
    Str str = to_Str__String(*s);

    print("%s", str);

    FREE(String, s);
    free(str);
}

int Println(const Str format_, ...) {
    Print(format_);
    printf("\n");
}
