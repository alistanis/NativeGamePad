

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