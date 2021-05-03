#pragma once

#include "NGP_Types.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * Trivially copyable touchpad finger data struct
 */
typedef struct {
  int Touchpad;
  int Finger;
  int ReturnValue;
  uint8_t State;
  float X;
  float Y;
  float Pressure;
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

DECLSPEC double NGPCALL clamp(double v, double low, double high);
DECLSPEC double NGPCALL normalize_axis_double(double d);
DECLSPEC NGP_Vector2 NGPCALL NormalizeAxis(NGP_Vector2 v);

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

typedef struct NGP_GamePad NGP_GamePad;

extern DECLSPEC void NGPCALL NGP_Initialize(void);

/**
 * Returns a new NGP_GamePad* on success or a nullptr on failure
 * @param joystickIndex the SDL joystick index for this game pad
 * @return NGP_GamePad* or null
 */

extern DECLSPEC NGP_GamePad* NGPCALL NGP_GamePadOpen(int joystickIndex);

/**
 * Frees a gamepad that was opened and closes it on the SDL side
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
extern DECLSPEC NGP_TouchpadFinger NGPCALL NGP_GamePadTouchpadFingerData(NGP_GamePad* p, int touchpad, int finger);

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
//SDL_Joystick* NGP_GamePadJoystick(NGP_GamePad* p);

/**
 * Returns the type of this gamepad
 * @param p
 * @return
 */
//SDL_GameControllerType NGP_GamePadType(NGP_GamePad* p);

/**
 * Returns the internal SDL_GameController*
 * @param p
 * @return
 */
//SDL_GameController* NGP_GamePadSDL_GameController(NGP_GamePad* p);

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
extern DECLSPEC int NGPCALL NGP_GamePadRumble(NGP_GamePad* p, uint16_t low_freq, uint16_t high_freq, uint32_t duration_ms);

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
extern DECLSPEC int NGPCALL NGP_GamePadRumbleTriggers(NGP_GamePad* p, uint16_t left, uint16_t right, uint32_t duration_ms);

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
extern DECLSPEC int NGPCALL NGP_GamePadRumbleLeftTrigger(NGP_GamePad* p, uint16_t left, uint32_t duration_ms);

/**
 * Start a rumble effect in the right trigger
 * @param p
 * @param right
 * @param duration_ms
 * @return
 */
extern DECLSPEC int NGPCALL NGP_GamePadRumbleRightTrigger(NGP_GamePad* p, uint16_t right, uint32_t duration_ms);

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