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