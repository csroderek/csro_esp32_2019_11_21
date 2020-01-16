#include "csro_devices.h"

void csro_device_init(void)
{
#ifdef AIR_SYS
    csro_airsys_init();
#elif defined NLIGHT_TOUCH_3K3R
    csro_nlight_touch_3k3r_init();
#elif defined MOTOR_TOUCH_3K2R
    csro_motor_touch_3k2r_init();
#endif
}

void csro_device_on_connect(esp_mqtt_event_handle_t event)
{
#ifdef AIR_SYS
    csro_airsys_on_connect(event);
#elif defined NLIGHT_TOUCH_3K3R
    csro_nlight_touch_3k3r_on_connect(event);
#elif defined MOTOR_TOUCH_3K2R
    csro_motor_touch_3k2r_on_connect(event);
#endif
}

void csro_device_on_message(esp_mqtt_event_handle_t event)
{
#ifdef AIR_SYS
    csro_airsys_on_message(event);
#elif defined NLIGHT_TOUCH_3K3R
    csro_nlight_touch_3k3r_on_message(event);
#elif defined MOTOR_TOUCH_3K2R
    csro_motor_touch_3k2r_on_message(event);
#endif
}