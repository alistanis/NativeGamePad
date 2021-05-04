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

The code below was not taken directly from SDL, but was heavily inspired by it.

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

#include "NGP_Types.h"

typedef enum {
  // NGP_DeviceEvent
  NGP_EventGamePadAttached,
  NGP_EventGamePadDetached,
  // NGP_ButtonEvent
  NGP_EventButtonDown,
  NGP_EventButtonUp,
  // NGP_AxisEvent
  NGP_EventAxis,
  // NGP_TouchpadEvent
  NGP_EventTouchpadDown,
  NGP_EventTouchpadUp,
  NGP_EventTouchpadMotion,
  // NGP_SensorEvent
  NGP_EventSensorData,
} NGP_EventType;

typedef struct {
  NGP_GamePadButtonType Button;
  uint8_t State;
} NGP_ButtonEvent;

typedef struct {
} NGP_DeviceEvent;

typedef struct {
  NGP_GamePadAxisType AxisType;
  int16_t Data;
} NGP_AxisEvent;

typedef struct {
  double X;
  double Y;
} NGP_TouchpadEvent;

typedef struct {

} NGP_SensorEvent;

typedef struct {
  union {
    NGP_DeviceEvent DeviceEvent;
    NGP_ButtonEvent ButtonEvent;
    NGP_AxisEvent AxisEvent;
    NGP_TouchpadEvent TouchpadEvent;
    NGP_SensorEvent SensorEvent;
  } Event;
  NGP_GamePadID GamePadID;
  NGP_Timestamp Timestamp;
  NGP_EventType Kind;
} NGP_Event;