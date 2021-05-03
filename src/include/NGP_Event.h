#pragma once

#include "NGP_Types.h"

typedef enum {
  NGP_ControllerAttached,
  NGP_ControllerDetached,
  NGP_ButtonDown,
  NGP_ButtonUp,
  NGP_Axis,
  NGP_TouchpadDown,
  NGP_TouchpadUp,
  NGP_TouchpadMotion,
} NGP_EventType;

typedef struct NGP_Event NGP_Event;