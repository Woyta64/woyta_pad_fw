#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    // Use the native USB port on RP2040
#ifndef CFG_TUSB_RHPORT0_MODE
#define CFG_TUSB_RHPORT0_MODE   OPT_MODE_DEVICE
#endif

    // Enable Device Stack
#define CFG_TUD_ENABLED       1

    // Enable 1 HID Interface (Keyboard only)
#define CFG_TUD_HID           1
#define CFG_TUD_CDC           0
#define CFG_TUD_MSC           0
#define CFG_TUD_MIDI          0
#define CFG_TUD_VENDOR        0

    // Standard Buffer Size
#define CFG_TUD_HID_EP_BUFSIZE    64

#ifdef __cplusplus
}
#endif