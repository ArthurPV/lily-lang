#include <lang/generate/generate_c.h>
#include <base/format.h>

void
write_main_function(struct Generate *self)
{
    append__String(self->ouput, from__String("int main() {\n"), true);
    append__String(self->ouput, from__String("\treturn 0;\n"), true);
    append__String(self->ouput, from__String("}"), true);
}