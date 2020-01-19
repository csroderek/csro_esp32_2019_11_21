#include "csro_devices.h"

#ifdef MOTOR_TOUCH_3K2R

#define THRESHOLD 10

#define TOUCH_01_NUM 0
#define TOUCH_02_NUM 2
#define TOUCH_03_NUM 3

#define RELAY_OPEN_NUM GPIO_NUM_27
#define RELAY_CLOSE_NUM GPIO_NUM_14

#define LED_01_NUM GPIO_NUM_19
#define LED_02_NUM GPIO_NUM_18
#define LED_03_NUM GPIO_NUM_5
#define GPIO_OUTPUT_PIN_SEL ((1ULL << RELAY_OPEN_NUM) | (1ULL << RELAY_CLOSE_NUM) | (1ULL << LED_01_NUM) | (1ULL << LED_02_NUM) | (1ULL << LED_03_NUM))

#define OPEN_POSITION 99
#define CLOSE_POSITION 1

typedef enum
{
    STOP = 0,
    OPEN = 1,
    CLOSE = 2,
    STOP_TO_OPEN = 3,
    STOP_TO_CLOSE = 4,
} motor_state;

motor_state motor = STOP;

static void csro_update_motor_touch_3k2r_state(void)
{
    if (mqttclient != NULL)
    {
        cJSON *state_json = cJSON_CreateObject();
        cJSON *airsys_json;

        cJSON_AddStringToObject(state_json, "time", sysinfo.time_str);
        cJSON_AddNumberToObject(state_json, "run", (int)(sysinfo.time_now - sysinfo.time_start));
        cJSON_AddItemToObject(state_json, "state", airsys_json = cJSON_CreateObject());
        cJSON_AddStringToObject(airsys_json, "mode", work_mode[csro_airsys.mode]);
        cJSON_AddNumberToObject(airsys_json, "temp", csro_airsys.temp);
        cJSON_AddStringToObject(airsys_json, "fan", fan_mode[csro_airsys.fan]);
        char *out = cJSON_PrintUnformatted(state_json);
        strcpy(mqttinfo.content, out);
        free(out);
        cJSON_Delete(state_json);
        sprintf(mqttinfo.pub_topic, "csro/%s/%s/state", sysinfo.mac_str, sysinfo.dev_type);
        esp_mqtt_client_publish(mqttclient, mqttinfo.pub_topic, mqttinfo.content, 0, 0, 1);
    }
}

static void motor_touch_3k2r_relay_led_task(void *args)
{
    static motor_state last_state = STOP;
    static uint16_t count_100ms = 0;
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    while (true)
    {
        bool update = false;
        if (last_state != motor)
        {
            last_state = motor;
            update = true;

            if (motor == STOP)
            {
                gpio_set_level(RELAY_OPEN_NUM, 0);
                gpio_set_level(RELAY_CLOSE_NUM, 0);
            }
            else if (motor == OPEN)
            {
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

static void motor_touch_3k2r_key_task(void *args)
{
    static uint16_t first_touch_value[4];
    static uint16_t single_holdtime[3];
    static uint16_t all_holdtime;
    touch_pad_init();
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    vTaskDelay(200 / portTICK_PERIOD_MS);
    for (int i = 0; i < 4; i++)
    {
        touch_pad_config(i, 0);
        touch_pad_read(i, &first_touch_value[i]);
    }

    while (true)
    {
        uint8_t touch_state[3];
        uint16_t touchvalue;
        for (uint8_t i = 0; i < 4; i++)
        {
            touch_pad_read(i, &touchvalue);
            if (i != 1)
            {
                touch_state[i == 0 ? 0 : i == 2 ? 1 : 2] = (first_touch_value[i] > touchvalue && (first_touch_value[i] - touchvalue >= THRESHOLD));
            }
        }
        if (touch_state[0] + touch_state[1] + touch_state[2] == 1)
        {
            all_holdtime = 0;
            single_holdtime[0] = (single_holdtime[0] + 1) * touch_state[0];
            single_holdtime[1] = (single_holdtime[1] + 1) * touch_state[1];
            single_holdtime[2] = (single_holdtime[2] + 1) * touch_state[2];
            if (single_holdtime[0] == 4)
            {
                if (motor == STOP || motor == STOP_TO_CLOSE)
                {
                    motor = OPEN;
                }
                else if (motor == CLOSE)
                {
                    motor = STOP_TO_OPEN;
                }
            }
            else if (single_holdtime[1] == 4)
            {
                motor = STOP;
            }
            else if (single_holdtime[2] == 4)
            {
                if (motor == STOP || motor == STOP_TO_OPEN)
                {
                    motor = CLOSE;
                }
                else if (motor == OPEN)
                {
                    motor = STOP_TO_CLOSE;
                }
            }
        }
        else if (touch_state[0] + touch_state[1] + touch_state[2] == 3)
        {
            all_holdtime++;
            single_holdtime[0] = 0;
            single_holdtime[1] = 0;
            single_holdtime[2] = 0;
            if (all_holdtime > 3000)
            {
                esp_restart();
            }
        }
        else
        {
            all_holdtime = 0;
            single_holdtime[0] = 0;
            single_holdtime[1] = 0;
            single_holdtime[2] = 0;
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void csro_motor_touch_3k2r_init(void)
{
    xTaskCreate(motor_touch_3k2r_relay_led_task, "motor_touch_3k2r_relay_led_task", 2048, NULL, configMAX_PRIORITIES - 8, NULL);
    xTaskCreate(motor_touch_3k2r_key_task, "motor_touch_3k2r_key_task", 2048, NULL, configMAX_PRIORITIES - 6, NULL);
}

void csro_motor_touch_3k2r_on_connect(esp_mqtt_event_handle_t event)
{
}

void csro_motor_touch_3k2r_on_message(esp_mqtt_event_handle_t event)
{
}

#endif
