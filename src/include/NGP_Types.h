#pragma once

#include <stdint.h>

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

#define SDL_LIL_ENDIAN  1234
#define SDL_BIG_ENDIAN  4321

#ifndef NGP_BYTEORDER
#ifdef __linux__
#include <endian.h>
#define NGP_BYTEORDER  __BYTE_ORDER
#elif defined(__OpenBSD__)
#include <endian.h>
#define NGP_BYTEORDER  BYTE_ORDER
#elif defined(__FreeBSD__)
#include <sys/endian.h>
#define NGP_BYTEORDER  BYTE_ORDER
#else
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MIPSEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define NGP_BYTEORDER   SDL_BIG_ENDIAN
#else
#define NGP_BYTEORDER   SDL_LIL_ENDIAN
#endif
#endif
#endif

#if NGP_BYTEORDER == SDL_LIL_ENDIAN
#define NGP_SwapLE16(X)     (X)
#define NGP_SwapLE32(X)     (X)
#define NGP_SwapLE64(X)     (X)
#define NGP_SwapFloatLE(X)  (X)
#define NGP_SwapBE16(X)     SDL_Swap16(X)
#define NGP_SwapBE32(X)     SDL_Swap32(X)
#define NGP_SwapBE64(X)     SDL_Swap64(X)
#define NGP_SwapFloatBE(X)  SDL_SwapFloat(X)
#else
#define NGP_SwapLE16(X)     SDL_Swap16(X)
#define NGP_SwapLE32(X)     SDL_Swap32(X)
#define NGP_SwapLE64(X)     SDL_Swap64(X)
#define NGP_SwapFloatLE(X)  SDL_SwapFloat(X)
#define NGP_SwapBE16(X)     (X)
#define NGP_SwapBE32(X)     (X)
#define NGP_SwapBE64(X)     (X)
#define NGP_SwapFloatBE(X)  (X)
#endif

typedef enum {
  NGP_GamePadAxisTypeLeftX,
  NGP_GamePadAxisTypeLeftY,
  NGP_GamePadAxisTypeRightX,
  NGP_GamePadAxisTypeRightY,
  NGP_GamePadAxisTypeTriggerLeft,
  NGP_GamePadAxisTypeTriggerRight,
} NGP_GamePadAxisType;

/**
 *  The list of buttons available from a controller
 */
typedef enum
{
  NGP_GamePadButtonInvalid = -1,
  NGP_GamePadButtonA,
  NGP_GamePadButtonB,
  NGP_GamePadButtonX,
  NGP_GamePadButtonY,
  NGP_GamePadButtonBack,
  NGP_GamePadButtonGuide,
  NGP_GamePadButtonStart,
  NGP_GamePadButtonLeftStick,
  NGP_GamePadButtonRightStick,
  NGP_GamePadButtonLeftShoulder,
  NGP_GamePadButtonRightShoulder,
  NGP_GamePadButtonDPadUp,
  NGP_GamePadButtonDPadDown,
  NGP_GamePadButtonDPadLeft,
  NGP_GamePadButtonDPadRight,
  NGP_GamePadButtonMisc1,    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button */
  NGP_GamePadButtonPaddle1,  /* Xbox Elite paddle P1 */
  NGP_GamePadButtonPaddle2,  /* Xbox Elite paddle P3 */
  NGP_GamePadButtonPaddle3,  /* Xbox Elite paddle P2 */
  NGP_GamePadButtonPaddle4,  /* Xbox Elite paddle P4 */
  NGP_GamePadButtonTouchpad, /* PS4/PS5 touchpad button */
  NGP_GamePadButtonMax,
} NGP_GamePadButtonType;

typedef enum {
  NGP_GamePadSensorAccelerometer,
  NGP_GamePadSensorGyroscope,
} NGP_GamePadSensorType;

typedef int64_t NGP_GamePadID;
typedef int64_t NGP_Timestamp;