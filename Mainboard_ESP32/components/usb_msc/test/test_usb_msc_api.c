// Copyright 2023 PWr in Space, Krzysztof Gliwiński

#include "unity.h"
#include "usb_msc_api.h"

usb_msc_config_t usb_cfg = {
    .desc_device =
        {
            .bLength = sizeof(tusb_desc_device_t),
            .bDescriptorType = TUSB_DESC_DEVICE,
            .bcdUSB = 0x0200,
            .bDeviceClass = TUSB_CLASS_MSC,
            .bDeviceSubClass = MISC_SUBCLASS_COMMON,
            .bDeviceProtocol = MISC_PROTOCOL_IAD,
            .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
            .idVendor = 0x0483,
            .idProduct = 0x5750,
            .bcdDevice = 0x0100,
            .iManufacturer = 0x01,
            .iProduct = 0x02,
            .iSerialNumber = 0x03,
            .bNumConfigurations = 0x01,
        },
    .desc_cfg =
        {
            .bLength = sizeof(tusb_desc_configuration_t),
            .bDescriptorType = TUSB_DESC_CONFIGURATION,
            .wTotalLength = sizeof(tusb_desc_configuration_t),
            .bNumInterfaces = 0x01,
            .bConfigurationValue = 0x01,
            .iConfiguration = 0x00,
            .bmAttributes = TU_BIT(7) | TUSB_DESC_CONFIG_ATT_SELF_POWERED,
            .bMaxPower = TUSB_DESC_CONFIG_POWER_MA(100),
        },
};

TEST_CASE("USB MSC init test", "[LED]") {
  TEST_ASSERT_EQUAL(ESP_OK, usb_msc_init(&usb_cfg));
}
