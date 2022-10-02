#ifndef LILY_RUNTIME_NET_H
#define LILY_RUNTIME_NET_H

#include <lang/runtime/api.h>
#include <lang/runtime/runtime.h>
// #include <sys/socket.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    enum Shutdown
    {
        ShutdownRead,
        ShutdownWrite,
        ShutdownBoth
    };

#if defined(__cplusplus)
    extern "C"
}
#endif

#endif // LILY_RUNTIME_NET_H
