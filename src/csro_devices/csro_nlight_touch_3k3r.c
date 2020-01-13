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
#define GPIO_OUTPUT_PIN_SEL ((1ULL << GPIO_OUTPUT_IO_0) | (1ULL << GPIO_OUTPUT_IO_1))

static void csro_update_nlight_touch_3k3r_state(void)
{
}

static void nlight_touch_3k3r_relay_led_task(void *args)
{
}

static void nlight_touch_3k3r_key_task(void *args)
{
    touch_pad_init();
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    for (int i = 0; i < TOUCH_PAD_MAX; i++)
    {
        touch_pad_config(i, 0);
    }
    while (true)
    {

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void csro_nlight_touch_3k3r_init(void)
{
    xTaskCreate(nlight_touch_3k3r_relay_led_task, "nlight_touch_3k3r_relay_led_task", 2048, NULL, configMAX_PRIORITIES - 8, NULL);
    xTaskCreate(nlight_touch_3k3r_key_task, "nlight_touch_3k3r_key_task", 2048, NULL, configMAX_PRIORITIES - 6, NULL);
}

void csro_nlight_touch_3k3r_on_connect(esp_mqtt_event_handle_t event)
{
}

void csro_nlight_touch_3k3r_on_message(esp_mqtt_event_handle_t event)
{
}

#endif
