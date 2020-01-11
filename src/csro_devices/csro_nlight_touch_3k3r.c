#include "csro_devices.h"

#ifdef NLIGHT_TOUCH_3K3R

#define TOUCH_01_NUM 0
#define TOUCH_02_NUM 2
#define TOUCH_03_NUM 3

#define RELAY_01_NUM GPIO_NUM_27
#define RELAY_02_NUM GPIO_NUM_14
#define RELAY_03_NUM GPIO_NUM_12

#define LED_01_NUM GPIO_NUM_19
#define LED_02_NUM GPIO_NUM_18
#define LED_03_NUM GPIO_NUM_5

static void csro_update_nlight_touch_3k3r_state(void)
{
}

static void nlight_touch_3k3r_relay_led_task(void *args)
{
}

static void nlight_touch_3k3r_key_task(void *args)
{
}

void csro_nlight_touch_3k3r_init(void)
{
}

void csro_nlight_touch_3k3r_on_connect(esp_mqtt_event_handle_t event)
{
}

void csro_nlight_touch_3k3r_on_message(esp_mqtt_event_handle_t event)
{
}

#endif
