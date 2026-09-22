#include "../inc/driver.h"
#include <libusb-1.0/libusb.h>

int get_battery_charging(libusb_device_handle* dev_handle, int packet_id)
{
    int i = 1;
    while(1)
    {
        if(i == 0)
        {
            return 0;
        }
        unsigned char data[33] = {0xc, 0x1, 0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
        data[4] = i++;
        int n = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, data, 33, 0);
        unsigned char buf[33] = {0};
        n = libusb_control_transfer(dev_handle, 0xa1, 0x1, 0x30c, 1, buf, 33, 0);
        if(n < 0)
        {
            return n;
        }
        if(buf[18])
        {
            packet_id = i - 1;
            if(buf[19] == 1)
            {
                return BATTERY_CHARGING;
            }
            return buf[18];
        }
    }
}

int set_mouse_rate(int mouse_rate, libusb_device_handle* dev_handle, int packet_id)
{
    int current_mouse_rate = 0x1;
    if (mouse_rate == 1)
    {
        current_mouse_rate = Hz1000;
    }
    else if (mouse_rate == 2)
    {
        current_mouse_rate = Hz500;
    }
    else if (mouse_rate == 3)
    {
        current_mouse_rate = Hz250;
    }
    else if (mouse_rate == 4)
    {
        current_mouse_rate = Hz125;
    }
    else
    {
        return -1;
    }

    unsigned char data[33] = {0xc, 0x1, 0x7, 0x0, 0x32, 0x1, 0x1, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

    data[4] = packet_id + 1;
    data[7] = current_mouse_rate;

    int ret = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, data, 33, 0);

    return ret;
}

int set_mouse_timeout(int timeout, libusb_device_handle* dev_handle, int  packet_id)
{
    if (timeout < 0)
    {
        return -1;
    }

        unsigned char data[33] = {0xc, 0x1, 0xb, 0x0, 0x2, 0x1, 0x2, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    data[4] = packet_id + 1;
    data[7] = timeout;

    int ret = libusb_control_transfer(dev_handle, 0x21, 0x9, 0x30c, 1, data, 33, 0);
    
    return ret;
}
