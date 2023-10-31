// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "sdcard.h"
#include "sdkconfig.h"
#include "unity.h"

static sdmmc_card_t card;

const spi_host_device_t host = CONFIG_SPI_HOST;

static sd_card_t sd_card = {
    .card = &card,
    .spi_host = host,
    .mount_point = SDCARD_MOUNT_POINT,
    .cs_pin = CONFIG_SD_CS,
    .card_detect_pin = CONFIG_SD_CD,
    .mounted = false,
};

static sd_card_config_t sd_card_config = {
    .spi_host = host,
    .mount_point = SDCARD_MOUNT_POINT,
    .cs_pin = CONFIG_SD_CS,
    .cd_pin = CONFIG_SD_CD,
};

const char* path = SD_CREATE_FILE_PREFIX("test.txt");
/*
TEST_CASE("SD card init test", "[SD]") {
  TEST_ASSERT_EQUAL(true, SD_init(&sd_card, &sd_card_config));
}

TEST_CASE("SD card write test", "[SD]") {
  const char* data = "test";
  size_t length = strlen(data);
  TEST_ASSERT_EQUAL(true, SD_write(&sd_card, path, data, length));
}

TEST_CASE("SD file exists test", "[SD]") {
  TEST_ASSERT_EQUAL(true, SD_file_exists(path));
  TEST_ASSERT_EQUAL(false, SD_file_exists(SD_CREATE_FILE_PREFIX("test_2.txt")));
}

TEST_CASE("SD ok test", "[SD]") { TEST_ASSERT_EQUAL(true, SD_is_ok(&sd_card)); } */
