// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "lora_test_config.h"

void lora_test_init() {
  _lora_spi_and_pins_init();
  lora_init(&lora);

  vTaskDelay(pdMS_TO_TICKS(100));

  lora_set_frequency(&lora, 867e6);
  lora_set_bandwidth(&lora, LORA_BW_250_kHz);
  lora_disable_crc(&lora);

  int16_t read_val_one = lora_read_reg(&lora, 0x0d);
  int16_t read_val_two = lora_read_reg(&lora, 0x0c);
  ESP_LOGI(TAG, "LORA_READ: %04x, %04x", read_val_one, read_val_two);

  ESP_ERROR_CHECK(
      uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
  esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
  esp_vfs_dev_uart_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM,
                                            ESP_LINE_ENDINGS_CR);
  esp_vfs_dev_uart_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM,
                                            ESP_LINE_ENDINGS_CRLF);
}

void task_lora_tx(void *param) {
  char buf[32];
  for (;;) {
    fgets(buf, 32, stdin);
    vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI(TAG, "sending packet: %s\n", buf);
    lora_send_packet(&lora, (uint8_t *)buf, strlen(buf));
    ESP_LOGI(TAG, "packet sent...\n");
  }
}

void task_lora_rx(void *param) {
  int x;
  uint8_t buf[32];
  for (;;) {
    lora_set_receive_mode(&lora);  // put into receive mode
    while (lora_received(&lora) == LORA_OK) {
      x = lora_receive_packet(&lora, buf, sizeof(buf));
      buf[x] = 0;
      printf("Received: %s\n", buf);
      lora_received(&lora);
    }
    vTaskDelay(10);
  }
}
