#ifndef DRIVER_H
#define DRIVER_H
#include <libusb-1.0/libusb.h>

#define M800_PRO_VID 0x248a
#define M800_PRO_PID 0x5b2f
#define M800_PRO_WIRED_PID 0x5b2e
#define INTERFACE 1

#define BATTERY_CHARGING 0x65

int get_battery_value(libusb_device_handle*, int*);

#endif
