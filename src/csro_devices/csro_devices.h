#ifndef CSRO_DEVICES_H_
#define CSRO_DEVICES_H_

#include "./csro_common/csro_common.h"

typedef struct
{
    uint16_t mode;
    uint16_t fan;
    double temp;
} csro_airsystem;

extern csro_airsystem csro_airsys;

//csro_devices.c
void csro_device_init(void);
void csro_device_on_connect(esp_mqtt_event_handle_t event);
void csro_device_on_message(esp_mqtt_event_handle_t event);

//csro_airsys.c
void csro_airsys_init(void);
void csro_airsys_on_connect(esp_mqtt_event_handle_t event);
void csro_airsys_on_message(esp_mqtt_event_handle_t event);

#endif