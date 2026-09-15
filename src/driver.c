#include "../inc/driver.h"

int get_battery_value(libusb_device_handle *dev_handle, int *packet_id)
{
    int i = 1;
    while(1)
    {
        if(i == 0)
        {
            return 0;
        }
        unsigned char data[] = {0xc, 0x1, 0x20, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
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
            *packet_id = i - 1;
            if(buf[19] == 1)
            {
                return BATTERY_CHARGING;
            }
            return buf[18];
        }
    }
}
