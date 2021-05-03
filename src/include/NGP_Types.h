#pragma once


/* Some compilers use a special export keyword */
#ifndef DECLSPEC
# if defined(__WIN32__) || defined(__WINRT__) || defined(__CYGWIN__)
#  ifdef DLL_EXPORT
#   define DECLSPEC __declspec(dllexport)
#  else
#   define DECLSPEC
#  endif
# elif defined(__OS2__)
#   ifdef BUILD_SDL
#    define DECLSPEC    __declspec(dllexport)
#   else
#    define DECLSPEC
#   endif
# else
#  if defined(__GNUC__) && __GNUC__ >= 4
#   define DECLSPEC __attribute__ ((visibility("default")))
#  else
#   define DECLSPEC
#  endif
# endif
#endif

/* By default NGP uses the C calling convention */
#ifndef NGPCALL
#if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
#define NGPCALL __cdecl
#elif defined(__OS2__) || defined(__EMX__)
#define NGPCALL _System
# if defined (__GNUC__) && !defined(_System)
#  define _System /* for old EMX/GCC compat.  */
# endif
#else
#define NGPCALL
#endif
#endif /* NGPCALL */

#define NGP_THUMBSTICK_AXIS_MIN   (-32768)
#define NGP_THUMBSTICK_AXIS_MAX   32767