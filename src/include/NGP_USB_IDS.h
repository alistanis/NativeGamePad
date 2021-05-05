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

The Macro definitions below are from SDL:

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

/* Definitions of useful USB VID/PID values */

#define USB_VENDOR_APPLE 0x05ac
#define USB_VENDOR_GOOGLE 0x18d1
#define USB_VENDOR_HYPERKIN 0x2e24
#define USB_VENDOR_MICROSOFT 0x045e
#define USB_VENDOR_NINTENDO 0x057e
#define USB_VENDOR_NVIDIA 0x0955
#define USB_VENDOR_PDP 0x0e6f
#define USB_VENDOR_POWERA_ALT 0x20d6
#define USB_VENDOR_POWERA 0x24c6
#define USB_VENDOR_RAZER 0x1532
#define USB_VENDOR_SHENZHEN 0x0079
#define USB_VENDOR_SONY 0x054c
#define USB_VENDOR_VALVE 0x28de

#define USB_PRODUCT_GOOGLE_STADIA_CONTROLLER 0x9400
#define USB_PRODUCT_EVORETRO_GAMECUBE_ADAPTER 0x1846
#define USB_PRODUCT_NINTENDO_GAMECUBE_ADAPTER 0x0337
#define USB_PRODUCT_NINTENDO_SWITCH_PRO 0x2009
#define USB_PRODUCT_NINTENDO_SWITCH_JOY_CON_LEFT 0x2006
#define USB_PRODUCT_NINTENDO_SWITCH_JOY_CON_RIGHT 0x2007
#define USB_PRODUCT_RAZER_PANTHERA 0x0401
#define USB_PRODUCT_RAZER_PANTHERA_EVO 0x1008
#define USB_PRODUCT_RAZER_ATROX 0x0a00
#define USB_PRODUCT_SONY_DS4 0x05c4
#define USB_PRODUCT_SONY_DS4_DONGLE 0x0ba0
#define USB_PRODUCT_SONY_DS4_SLIM 0x09cc
#define USB_PRODUCT_SONY_DS5 0x0ce6
#define USB_PRODUCT_XBOX_ONE_ELITE_SERIES_1 0x02e3
#define USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2 0x0b00
#define USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLUETOOTH 0x0b05
#define USB_PRODUCT_XBOX_ONE_S 0x02ea
#define USB_PRODUCT_XBOX_ONE_S_REV1_BLUETOOTH 0x02e0
#define USB_PRODUCT_XBOX_ONE_S_REV2_BLUETOOTH 0x02fd
#define USB_PRODUCT_XBOX_ONE_SERIES_X 0x0b12
#define USB_PRODUCT_XBOX_ONE_SERIES_X_BLUETOOTH 0x0b13
#define USB_PRODUCT_XBOX_ONE_SERIES_X_POWERA 0x2001
#define USB_PRODUCT_XBOX_ONE_RAW_INPUT_CONTROLLER 0x02ff
#define USB_PRODUCT_XBOX_ONE_XINPUT_CONTROLLER 0x02fe /* Made up product ID for XInput */

typedef enum {
    NGP_USB_Vendor_Apple     = USB_VENDOR_APPLE,
    NGP_USB_Vendor_Google    = USB_VENDOR_GOOGLE,
    NGP_USB_Vendor_Hyperkin  = USB_VENDOR_HYPERKIN,
    NGP_USB_Vendor_Microsoft = USB_VENDOR_MICROSOFT,
    NGP_USB_Vendor_Nintendo  = USB_VENDOR_NINTENDO,
    NGP_USB_Vendor_Nvidia    = USB_VENDOR_NVIDIA,
    NGP_USB_Vendor_PDP       = USB_VENDOR_PDP,
    NGP_USB_Vendor_PoweraAlt = USB_VENDOR_POWERA_ALT,
    NGP_USB_Vendor_Powera    = USB_VENDOR_POWERA,
    NGP_USB_Vendor_Razer     = USB_VENDOR_RAZER,
    NGP_USB_Vendor_Shenzhen  = USB_VENDOR_SHENZHEN,
    NGP_USB_Vendor_Sony      = USB_VENDOR_SONY,
    NGP_USB_Vendor_Valve     = USB_VENDOR_VALVE,
} NGP_USB_VendorIDs;

typedef enum {
    NGP_USB_Product_StadiaController                   = USB_PRODUCT_GOOGLE_STADIA_CONTROLLER,
    NGP_USB_Product_EvoRetroGamecubeAdapter            = USB_PRODUCT_EVORETRO_GAMECUBE_ADAPTER,
    NGP_USB_Product_NintendoGamecubeAdapter            = USB_PRODUCT_NINTENDO_GAMECUBE_ADAPTER,
    NGP_USB_Product_NintendoSwitchProController        = USB_PRODUCT_NINTENDO_SWITCH_PRO,
    NGP_USB_Product_NintendoSwitchLeftJoycon           = USB_PRODUCT_NINTENDO_SWITCH_JOY_CON_LEFT,
    NGP_USB_Product_NintendoSwitchRightJoycon          = USB_PRODUCT_NINTENDO_SWITCH_JOY_CON_RIGHT,
    NGP_USB_Product_RazerPanthera                      = USB_PRODUCT_RAZER_PANTHERA,
    NGP_USB_Product_RazerPantheraEvo                   = USB_PRODUCT_RAZER_PANTHERA_EVO,
    NGP_USB_Product_RazerAtrox                         = USB_PRODUCT_RAZER_ATROX,
    NGP_USB_Product_SonyDS4                            = USB_PRODUCT_SONY_DS4,
    NGP_USB_Product_SonyDS4Dongle                      = USB_PRODUCT_SONY_DS4_DONGLE,
    NGP_USB_Product_SonyDS4Slim                        = USB_PRODUCT_SONY_DS4_SLIM,
    NGP_USB_Product_SonyDS5                            = USB_PRODUCT_SONY_DS5,
    NGP_USB_Product_MicrosoftXboxEliteSeries1          = USB_PRODUCT_XBOX_ONE_ELITE_SERIES_1,
    NGP_USB_Product_MicrosoftXboxEliteSeries2          = USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2,
    NGP_USB_Product_MicrosoftXboxEliteSeries2Bluetooth = USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLUETOOTH,
    NGP_USB_Product_MicrosoftXboxOneS                  = USB_PRODUCT_XBOX_ONE_S,
    NGP_USB_Product_MicrosoftXboxOneSRev1Bluetooth     = USB_PRODUCT_XBOX_ONE_S_REV1_BLUETOOTH,
    NGP_USB_Product_MicrosoftXboxOneSRev2Bluetooth     = USB_PRODUCT_XBOX_ONE_S_REV2_BLUETOOTH,
    NGP_USB_Product_MicrosoftXboxSeriesX               = USB_PRODUCT_XBOX_ONE_SERIES_X,
    NGP_USB_Product_MicrosoftXboxSeriesXBluetooth      = USB_PRODUCT_XBOX_ONE_SERIES_X_BLUETOOTH,
    NGP_USB_Product_XboxSeriesXPowera                  = USB_PRODUCT_XBOX_ONE_SERIES_X_POWERA,
    NGP_USB_Product_XboxOneRawInput                    = USB_PRODUCT_XBOX_ONE_RAW_INPUT_CONTROLLER,
    NGP_USB_Product_XboxXInput                         = USB_PRODUCT_XBOX_ONE_XINPUT_CONTROLLER,
} NGP_USB_ProductIDs;

typedef enum {
    NGP_USB_PS4_SerialRequestKey = 0x12,
    NGP_USB_PS5_SerialRequestKey = 0x09,
} NGP_USB_SerialRequestKeys;