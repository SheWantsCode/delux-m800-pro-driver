#ifndef DRIVER_H
#define DRIVER_H
#include <libusb-1.0/libusb.h>

#define M800_PRO_VID 0x248a
#define M800_PRO_PID 0x5b2f
#define M800_PRO_WIRED_PID 0x5b2e
#define INTERFACE 1

#define BATTERY_CHARGING 0x65

enum MouseRate {
    Hz1000 = 0x1,
    Hz500  = 0x2,
    Hz250  = 0x4,
    Hz125  = 0x8,
};

int get_battery_charging(libusb_device_handle*, int);
int set_mouse_rate(int, libusb_device_handle*, int);
int set_mouse_timeout(int, libusb_device_handle*, int);

#endif
