#include <base/color.h>
#include <base/format.h>
#include <base/new.h>
#include <base/print.h>
#include <base/string.h>
#include <lang/diagnostic/summary.h>

void
emit__Summary(Int32 count_error, Int32 count_warning, const Str msg)
{
    Str error = NULL;
    Str warning = NULL;

    if (count_error > 1)
        error = "errors";
    else
        error = "error";

    if (count_warning > 1)
        warning = "warnings";
    else
        warning = "warning";

    struct String *s = format("\n{sa}: {s} with {d} {sa} and {d} {sa}.",
                              CYAN("Summary"),
                              msg,
                              count_error,
                              RED(error),
                              count_warning,
                              YELLOW(warning));

    Println("{Sr}", s);
}