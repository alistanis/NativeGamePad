/*
Native Game Pad
Copyright (C) 2021 Christopher Cooper <christopher.michael.cooper@gmail.com>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Many of the function signatures below were taken from SDL

Simple DirectMedia Layer
Copyright (C) 1997-2020 Sam Lantinga <slouken@libsdl.org>

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "NGP_Event.h"
#include "NGP_Types.h"

/**
 * Trivially copyable touchpad finger data struct
 */
typedef struct {
    int     Touchpad;
    int     Finger;
    int     ReturnValue;
    uint8_t State;
    float   X;
    float   Y;
    float   Pressure;
} NGP_TouchpadFinger;

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} NGP_Color;

const NGP_Color NGP_Red;
const NGP_Color NGP_Green;
const NGP_Color NGP_Blue;
const NGP_Color NGP_Purple;

typedef struct {
    double X;
    double Y;
} NGP_Vector2;

DECLSPEC double NGPCALL      clamp(double v, double low, double high);
DECLSPEC double NGPCALL      normalize_axis_double(double d);
DECLSPEC NGP_Vector2 NGPCALL NormalizeAxis(NGP_Vector2 v);

typedef struct NGP_GamePad NGP_GamePad;

extern DECLSPEC void NGPCALL NGP_Initialize(void);


extern DECLSPEC int NGPCALL NGP_NumGamePads();

/**
 * Returns a new NGP_GamePad* on success or a nullptr on failure
 * @param joystickIndex the SDL joystick index for this game pad
 * @return NGP_GamePad* or null
 */

extern DECLSPEC NGP_GamePad* NGPCALL NGP_GamePadOpen(int index);

/**
 * Frees a gamepad that was opened
 * @param p
 */
extern DECLSPEC void NGPCALL NGP_GamePadFree(NGP_GamePad* p);

/**
 * Is this game pad currently attached to the system
 * @param p
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadIsAttached(NGP_GamePad* p);

/**
 * Returns the name for this game pad, like PS5 Controller/Nintendo Switch Pro Controller/XBox One S
 * Controller
 * @param p
 * @return
 */
extern DECLSPEC const char* NGPCALL NGP_GamePadName(NGP_GamePad* p);

/**
 * Returns this game controller's serial number
 * @param p
 * @return
 */
extern DECLSPEC const char* NGPCALL NGP_GamePadSerial(NGP_GamePad* p);

/**
 * Returns the currently assigned player index (SDL) for this controller
 * @param p
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadPlayerIndex(NGP_GamePad* p);

/**
 * Returns how many touchpads this controller has
 * @param p
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadNumTouchpads(NGP_GamePad* p);

/**
 * Returns how many fingers are supported at once on a given touchpad
 * @param p
 * @param touchpad
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadNumTouchpadFingers(NGP_GamePad* p, int touchpad);

/**
 * Returns data for a finger on a touchpad
 * @param p
 * @param touchpad
 * @param finger
 * @return
 */
extern DECLSPEC NGP_TouchpadFinger NGPCALL NGP_GamePadTouchpadFingerData(NGP_GamePad* p,
                                                                         int          touchpad,
                                                                         int          finger);

/**
 * Returns the SDL JoystickID for this game pad
 * @param p
 * @return
 */
extern DECLSPEC int32_t NGPCALL NGP_GamePadJoystickID(NGP_GamePad* p);

/**
 * Returns the GamePadJoystick pointer for this gamepad
 * @param p
 * @return
 */
// SDL_Joystick* NGP_GamePadJoystick(NGP_GamePad* p);

/**
 * Returns the type of this gamepad
 * @param p
 * @return
 */
// SDL_GameControllerType NGP_GamePadType(NGP_GamePad* p);

/**
 * Returns the internal SDL_GameController*
 * @param p
 * @return
 */
// SDL_GameController* NGP_GamePadSDL_GameController(NGP_GamePad* p);

/**
 * Returns the value of the thumbstick left x axis
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisLeftX(NGP_GamePad* p);

/**
 * Returns the value of the thumbstick left y axis
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisLeftY(NGP_GamePad* p);

/**
 * Returns the value of the thumbstick right x axis
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisRightX(NGP_GamePad* p);

/**
 * Returns the value of the thumbstick right y axis
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisRightY(NGP_GamePad* p);

/*
 * These may need to be pointers
 */
/**
 * Returns the X and Y data for the left thumbstick
 * @param p
 * @return
 */
extern DECLSPEC NGP_Vector2 NGPCALL NGP_GamePadLeftStick(NGP_GamePad* p);

/**
 * Returns the X and Y data for the right thumbstick
 * @param p
 * @return
 */
extern DECLSPEC NGP_Vector2 NGPCALL NGP_GamePadRightStick(NGP_GamePad* p);

/**
 * Returns the left rear trigger state
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisTriggerLeft(NGP_GamePad* p);

/**
 * Returns the right rear trigger state
 * @param p
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxisTriggerRight(NGP_GamePad* p);

/**
 * Returns the axis data for the specified SDL_GameControllerAxis for this game pad
 * @param p
 * @param axis
 * @return
 */
extern DECLSPEC int16_t NGPCALL NGP_GamePadAxis(NGP_GamePad* p, NGP_GamePadAxisType axis);

/**
 * Returns the game pad vendor ID, if there is one.
 * @param p
 * @return
 */
extern DECLSPEC uint16_t NGPCALL NGP_GamePadVendor(NGP_GamePad* p);

/**
 * Returns the game pad product ID, if there is one
 * @param p
 * @return
 */
extern DECLSPEC uint16_t NGPCALL NGP_GamePadProduct(NGP_GamePad* p);

/**
 * Returns the game pad product version, if there is one
 * @param p
 * @return
 */
extern DECLSPEC uint16_t NGPCALL NGP_GamePadProductVersion(NGP_GamePad* p);

/**
 * Start a rumble effect on the given game pad for the given duration in milliseconds.
 * Range is from 0 to 65535 for low_freq and high_freq
 * @param p
 * @param low_freq
 * @param high_freq
 * @param duration_ms
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadRumble(NGP_GamePad* p,
                                              uint16_t     low_freq,
                                              uint16_t     high_freq,
                                              uint32_t     duration_ms);

/**
 * Returns whether or not rumble is supported on this game pad
 * @param p
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadRumbleSupported(NGP_GamePad* p);

/**
 * Starts a rumble effect in the game pad triggers if supported
 * @param p
 * @param left
 * @param right
 * @param duration_ms
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadRumbleTriggers(NGP_GamePad* p,
                                                      uint16_t     left,
                                                      uint16_t     right,
                                                      uint32_t     duration_ms);

/**
 * Returns whether or not this game pad supports trigger rumble
 * @param p
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadRumbleTriggersSupported(NGP_GamePad* p);

/**
 * Start a rumble effect in the left trigger
 * @param p
 * @param left
 * @param duration_ms
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadRumbleLeftTrigger(NGP_GamePad* p,
                                                         uint16_t     left,
                                                         uint32_t     duration_ms);

/**
 * Start a rumble effect in the right trigger
 * @param p
 * @param right
 * @param duration_ms
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadRumbleRightTrigger(NGP_GamePad* p,
                                                          uint16_t     right,
                                                          uint32_t     duration_ms);

/**
 * Returns whether or not this game pad has a programmable LED
 * @param p
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadHasLED(NGP_GamePad* p);

/**
 * Set the LED to the given RGB values
 * @param p
 * @param red
 * @param green
 * @param blue
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadSetLED(NGP_GamePad* p, uint8_t red, uint8_t green, uint8_t blue);

/**
 * Set the LED to the given Color
 * @param p
 * @param c
 * @return
 */
extern DECLSPEC bool NGPCALL NGP_GamePadSetLEDColor(NGP_GamePad* p, NGP_Color c);

/**
 * Return the current status of the button on the given controller
 * @param p
 * @param button
 * @return
 */
extern DECLSPEC uint8_t NGPCALL NGP_GamePadButton(NGP_GamePad* p, NGP_GamePadButtonType button);

/**
 * Return the game pad button name for a given button
 * @param button
 * @return
 */
extern DECLSPEC const char* NGPCALL NGP_GamePadButtonName(NGP_GamePadButtonType button);
