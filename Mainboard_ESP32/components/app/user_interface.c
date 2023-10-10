// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "user_interface.h"

#define USER_INTERFACE_TAG "USER_INTERFACE"

#define I2C_MASTER_FREQ_HZ 400000

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
            [GREEN_INDEX] = {.ledc_mode = LEDC_HS_MODE,
                             .led_gpio_num = CONFIG_LED_K_G,
                             .ledc_channel_num = LEDC_CHANNEL_1,
                             .ledc_timer_num = LEDC_HS_TIMER,
                             .duty = 0,
                             .max_duty = MAX_DUTY,
                             .toggle = LED_OFF},
            [BLUE_INDEX] = {.ledc_mode = LEDC_HS_MODE,
                            .led_gpio_num = CONFIG_LED_K_B,
                            .ledc_channel_num = LEDC_CHANNEL_2,
                            .ledc_timer_num = LEDC_HS_TIMER,
                            .duty = 0,
                            .max_duty = MAX_DUTY,
                            .toggle = LED_OFF},
        },
    .max_duty = MAX_DUTY};

static ssd1306_t oled_display = {
    ._i2c_master_write_byte = _ssd1306_i2c_master_write_byte,
    ._i2c_master_write = _ssd1306_i2c_master_write,
    ._i2c_master_start = _ssd1306_i2c_master_start,
    ._i2c_master_stop = _ssd1306_i2c_master_stop,
    ._i2c_master_cmd_begin = _ssd1306_i2c_master_cmd_begin,
    ._i2c_cmd_link_create = _ssd1306_i2c_cmd_link_create,
    ._i2c_cmd_link_delete = _ssd1306_i2c_cmd_link_delete,
    ._delay = _ssd1306_delay,
    ._log = _ssd1306_log,
    .width = 128,
    .height = 64,
    .pages = 1,
    .i2c_address = 0x3c,
};

static mcu_i2c_config_t i2c = {
    .port = I2C_NUM_0,
    .sda = CONFIG_I2C_SDA,
    .scl = CONFIG_I2C_SCL,
    .clk_speed = I2C_MASTER_FREQ_HZ,
    .i2c_init_flag = false,
};

void user_interface_task(void* arg) {
  i2c_init(&i2c);
  ssd1306_esp32_config_mount_i2c_config(&i2c);
  ssd1306_init(&oled_display, 128, 64);
  ssd1306_clear_screen(&oled_display, false);
  ssd1306_set_contrast(&oled_display, 0xff);
  ssd1306_display_text_x3(&oled_display, 0, "Hello", 5, false);
  vTaskDelay(3000 / portTICK_PERIOD_MS);
  rgb_led_driver_init(&rgb_led_driver, LEDC_DUTY_RES, LEDC_FREQ_HZ);

  int center, top, bottom;
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
