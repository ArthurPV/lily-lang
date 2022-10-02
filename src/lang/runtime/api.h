#ifndef LILY_API_H
#define LILY_API_H

#ifdef _WIN32
#define LILY_EXPORT __declspec(dllexport)
#define LILY_EXPORT __declspec(dllimport)
#else
#define LILY_EXPORT __attribute__((visibility("default")))
#define LILY_IMPORT __attribute__((visibility("default")))
#endif

#ifdef LILY_BUILD
#define LILY_API LILY_EXPORT
#else
#define LILY_API LILY_IMPORT
#endif

#endif // LILY_API_H
