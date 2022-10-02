#include <base/format.h>
#include <lang/generate/generate_c.h>

void
write_main_function(struct Generate *self)
{
    append__String(self->output, from__String("int main() {\n"), true);
    append__String(self->output, from__String("\treturn 0;\n"), true);
    append__String(self->output, from__String("}"), true);
}

void
run__GenerateC(struct Generate self)
{
    write_on_file__Generate(self);
}
