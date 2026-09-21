#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../inc/driver.h"

#define APP_NAME "delux_driver"

int main(int argc, char* argv[])
{
    char* is_sudo_user = getenv("SUDO_USER");
    if (!is_sudo_user)
    {
        printf("INFO: Please, use sudo for current job.");
        return 0;
    }
    
    if (argc < 2)
    {
        printf("Write ./%s --help to see how it use.\n", APP_NAME);
        return 0;
    }
    int ret = 0;

    libusb_context* ctx = NULL;
    ret = libusb_init(&ctx);
    if (ret != LIBUSB_SUCCESS)
    {
        fprintf(stderr, "ERROR: Error initialization usb library. Returned value:  %i\n", ret);
        return -1;
    }

    libusb_device* needed_device = NULL;
    libusb_device** list = NULL;
    int count = libusb_get_device_list(ctx, &list);
    for (int i = 0; i < count; i++)
    {
        struct libusb_device_descriptor desc;
        libusb_get_device_descriptor(list[i], &desc);
        if(desc.idVendor == M800_PRO_VID && desc.idProduct == M800_PRO_PID || desc.idProduct == M800_PRO_WIRED_PID)
        {
            needed_device = list[i];
        }
    }

    if(needed_device == NULL)
    {
        printf("INFO: Device not found.\n");
        return 0;
    }

    libusb_device_handle* dev_handle = NULL;
    ret = libusb_open(needed_device, &dev_handle);
    if (ret != LIBUSB_SUCCESS)
    {
        fprintf(stderr, "ERROR: Mouse connection error.\n");
        return -1;
    }

    int has_driver = libusb_kernel_driver_active(dev_handle, INTERFACE);
    if (has_driver)
    {
        ret = libusb_detach_kernel_driver(dev_handle, INTERFACE);
        if (ret != LIBUSB_SUCCESS)
        {
            fprintf(stderr, "ERROR: Kernal detach error. Retured values: %i, %s\n", ret, libusb_error_name(ret));
            return -1;
        }
    }

    ret = libusb_claim_interface(dev_handle, INTERFACE);
    if (ret != LIBUSB_SUCCESS)
    {
        fprintf(stderr,"ERROR: Mouse interface binding error. Returned values: %i, %s\n", ret, libusb_error_name(ret));
        return -1;
    }


    int c;
    int what_battery = 0;
    int set_rate = 0;
    int mouse_rate = 1;

    while((c = getopt(argc, argv, ":br:")) != -1)
    {
        switch(c)
        {
        case 'b':
            what_battery = 1;
            break;
        case 'r':
            set_rate = 1;
            mouse_rate = atoi(optarg);
            break;
        }
    }
    
    if (what_battery)
    {
        int id = 0;
        int charge = get_battery_charging(dev_handle, &id);
        if(charge <= 0)
        {
            fprintf(stderr, "ERROR: Could not get battery value. %i %s\n", charge, libusb_error_name(charge));
            return -1;
        }
        if(charge == BATTERY_CHARGING)
        {
            printf("M800Pro: Mouse battery is charging now.\n");
        }
        else
        {
            printf("M800Pro battery value: %i%%\n", charge);
        }
    }

    if (set_rate)
    {
        int id = 0;
        ret = set_mouse_rate(mouse_rate, dev_handle, &id);
        if (ret == -1)
        {
            fprintf(stderr, "ERROR: Invalid argument for mouse rate.");
            return -1;
        }
    }
    
    
    return 0;
}
