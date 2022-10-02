#define _GNU_SOURCE

#include <assert.h>
#include <base/file.h>
#include <base/new.h>
#include <base/platform.h>
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

struct Path *
__new__Path(Str path)
{
    struct Path *self = malloc(sizeof(struct Path));
    self->path = from__String(path);
    return self;
}

bool
is_directory__Path(struct Path self)
{
    Str path_str = to_Str__String(*self.path);
    DIR *dir = opendir(path_str);

    if (ENOENT == errno || dir == NULL) {
        free(path_str);
        return false;
    }

    free(path_str);
    closedir(dir);

    return true;
}

Str
get_extension__Path(struct Path self)
{
    bool extension_started = false;
    struct String *extension = NEW(String);
    Usize len = len__String(*self.path);

    for (Usize i = 0; i < len; i++) {
        if (get__String(*self.path, i) == (char *)'.' &&
            get__String(*self.path, i + 1 < len ? i + 1 : i) != (char *)'.' &&
            get__String(*self.path, i + 1 < len ? i + 1 : i) != (char *)'/' &&
            get__String(*self.path, i + 1 < len ? i + 1 : i) != (char *)'\\')
            extension_started = true;

        if (extension_started)
            push__String(extension, get__String(*self.path, i));
    }

    Str extension_str = to_Str__String(*extension);

    FREE(String, extension);

    return extension_str;
}

struct String *
read_file__Path(struct Path self)
{
    assert(is_directory__Path(self) == false && "The path is a directory");

    Str path = to_Str__String(*self.path);
    FILE *file = fopen(path, "rb");
    struct String *file_content = NEW(String);

    if (file == NULL)
        assert(0 && "failed to read file");

#ifdef LILY_WINDOWS_OS
    char c;

    c = (char)fgetc(file);
    while (c != EOF) {
        push__String(file_content, (char *) (UPtr)c);
        c = (char)fgetc(file);
    }
#else
    Str line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1)
        push_str__String(file_content, line);

    free(line);
#endif

    fclose(file);
    free(path);

    push__String(file_content, (char*)'\n');

    return file_content;
}

void
write_file__Path(struct Path self, const Str content)
{
    assert(is_directory__Path(self) == false && "The path is a directory");

    Str path = to_Str__String(*self.path);
    FILE *file = fopen(path, "w");

    if (file == NULL)
        assert(0 && "write error");

    fprintf(file, "%s", content);
    fclose(file);
    free(path);
}

void
__free__Path(struct Path *self)
{
    FREE(String, self->path);
    free(self);
}
