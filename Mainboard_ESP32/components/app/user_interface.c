// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "user_interface.h"

#define USER_INTERFACE_TAG "USER_INTERFACE"

static rgb_led_driver_t rgb_led_driver = {
    .led_drv =
        {
            [RED_INDEX] =
                {
                    .ledc_mode = LEDC_HS_MODE,
                    .led_gpio_num = CONFIG_LED_K_R,
                    .ledc_channel_num = LEDC_CHANNEL_0,
                    .ledc_timer_num = LEDC_HS_TIMER,
                    .duty = 0,
                    .max_duty = MAX_DUTY,
                    .toggle = LED_OFF,
                },
            [GREEN_INDEX] =
                {
                    .ledc_mode = LEDC_HS_MODE,
                    .led_gpio_num = CONFIG_LED_K_G,
                    .ledc_channel_num = LEDC_CHANNEL_1,
                    .ledc_timer_num = LEDC_HS_TIMER,
                    .duty = 0,
                    .max_duty = MAX_DUTY,
                    .toggle = LED_OFF,
                },
            [BLUE_INDEX] =
                {
                    .ledc_mode = LEDC_HS_MODE,
                    .led_gpio_num = CONFIG_LED_K_B,
                    .ledc_channel_num = LEDC_CHANNEL_2,
                    .ledc_timer_num = LEDC_HS_TIMER,
                    .duty = 0,
                    .max_duty = MAX_DUTY,
                    .toggle = LED_OFF,
                },
        },
    .max_duty = MAX_DUTY
};
void user_interface_task(void* arg) {
  rgb_led_driver_init(&rgb_led_driver, LEDC_DUTY_RES, LEDC_FREQ_HZ);
  ESP_LOGI(USER_INTERFACE_TAG, "RGB LED driver initialized");
  led_state_t toggle = LED_OFF;
  for (;;) {
    toggle = !toggle;
    rgb_led_toggle(&rgb_led_driver, toggle);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void init_user_interface_task(void* arg) {
  xTaskCreatePinnedToCore(user_interface_task, "user_interface_task", 4096,
                          NULL, 1, NULL, 1);
}
