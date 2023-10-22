# Wireless transmission diagrams and definitions

The wireless transmission data will be defined in the following document regarding the ROSALIA rev.2 PCB. It will be divided into two separate categories:
1. LoRa
2. BLE

## LoRa data transmission

The wireless communication with LoRa is based on RFM95w IC with SPI communication between the MCU. It will be two-sided with:
- The ROSALIA being a "slave" in the sense of it getting data with commands asynchronously from a foreign source (GS) and periodically sending data through a predefined data frame (?)
- GS being a "master" in the sense of it being in control of the ROSALIA device actions. It will be able to send commands to take the ROSALIA to different states, perform resets of its slave devices etc.
// (TODO add info what is a slave and what is a master in the system_overview doc)

### RFM95w overview

To fully understand how the communication will take place, we will first need to look at the RFM95w features and how it operates. For more closure look at the [RFM95w datasheet](../Related_IC_datasheets/RFM95_96_97_98W.pdf). The radio itself operates in the 868MHz band.

The packet consists of Preamble, (optional, but used) Header and Header CRC, data Payload and Payload CRC. Thankfully, due to the application provided directly in the IC, only the Payload will be sent through SPI communication to the MCU. Due to the built-in CRC calculation, it will also not be needed to calculate the Payload CRC by ROSALIA.

![LoRa packet](graphics/LoRa_packet.png)

### GS commands

We will start with a list of available commands that can be sent from GS to ROSALIA. The command data packet will have a predefined frame as shown below:

<span style="color:red; font-size:2em;">0x693B</span><span style="color:yellow; font-size:2em;"> SUDO </span><span style="color:red; font-size:2em;">0x3B</span><span style="color:green; font-size:2em;"> COMMAND AND (OPTIONAL) VALUES </span> <span style="color:red; font-size:2em;">0x693B</span>
<span style="font-size:2em;">"</span><span style="color:red; font-size:2em;">i;</span><span style="color:yellow; font-size:2em;"> SUDO </span><span style="color:red; font-size:2em;">;</span><span style="color:green; font-size:2em;"> COMMAND AND (OPTIONAL) VALUES </span><span style="font-size:2em;"></span><span style="color:red; font-size:2em;">i;</span><span style="font-size:2em;">"</span>

Where the top part shows the packet in HEX and bottom in string configuration defined in ASCII. 

__COMMAND__ is a 16 bit unsigned int from a list defined further in the document. __Optional values__ are some variables of unprecised length that can be sent with the command, and are also defined with commands in the table below

__SUDO__ is an optional argument that can force ROSALIA to take action, even if it should be prohibited by software

Parts in __RED__ are hardset and are supposed to filter out junk and wrong commands, if there are other transmitters nearby not supposed to interfere with the ROSALIA system.

#### Command definitions:

TODO add what is the range of proper commands in HEX

Normal mode:

| GS COMMAND | HEX definition | Ascii definition | Optional values |
| --- | --- | --- | --- |
| START TEST PROCEDURE | 0x40 | @ | None |
| TURN OFF ROSALIA | 0x41 | A | None |
| CHANGE STATE | 0x42 | B | STATE NUMBER (INT) |
| OPEN VALVE | 0x43 | C | VALVE NUMBER (INT) |
| CLOSE VALVE | 0x44 | D | VALVE NUMBER (INT) |
| TIMED OPEN VALVE | 0x45 | E | VALVE NUMBER (INT) TIME in ms (INT) |
| ENTER SETTINGS MODE | 0x46 | F | None |

Settings mode:

| GS COMMAND | HEX definition | Ascii definition | Optional values |
| --- | --- | --- | --- |
| START LOADCELL CALIBRATION | 0x47 | G | LOADCELL NUMBER (INT) |
| CHANGE LOADCELL CALFACTOR | 0x48 | H | LOADCELL NUMBER (INT) CALFACTOR (INT) |
| START SENSOR CALIBRATION | 0x47 | G | SENSOR NUMBER (INT) |
| CHANGE SENSOR CALFACTOR | 0x48 | H | SENSOR NUMBER (INT) CALFACTOR (INT) |
| CHANGE COUNTDOWN LENGTH | 0x49 | I | COUNTDOWN LENGTH TIME in ms (INT) |
| CHANGE FREQUENCY OF LORA FRAMES FROM ROSALIA | 0x4A | J | FREQUENCY in ms (INT) |
| SAVE SETTINGS AND EXIT | 0x4B | K | None |
| EXIT WITHOUT SAVING | 0x4C | L | None |

Available in all modes:
| GS COMMAND | HEX definition | Ascii definition | Optional values |
| --- | --- | --- | --- |
| SHOW ALL SELECTED SETTINGS | 0x4D | M | None |
| TOGGLE DATA DUMP THROUGH LORA | 0x4E | N | None |

Some of the commands will need to be confirmed after ROSALIA receives them. 

Confirmation commands:
| GS COMMAND | HEX definition | Ascii definition | Optional values |
| --- | --- | --- | --- |
| CONFIRM | 0x4F | O | None |
| DENY | 0x50 | P | None |

#### ROSALIA->GS frame definition

The frame will be constructed from structs presented below:
```C
typedef struct {
  struct rosalia_mainboard{
    uint16_t vbat_adc : 16;
    bool sd_save : 1;
    bool flash_save : 1;
    bool hardware_arm : 1;
    bool software_arm : 1;
    bool ble_state : 1;
    uint32_t t_minus : 32; // test time: until (negative) or after (positive) test start
    bool test_running : 1; // Is the test ongoing
    uint8_t rosalia_state : 8;
  };
  struct available_slaves {
    bool loadcell[2] : 2;
    bool vent : 1;
    bool main : 1;
    bool filling_station : 1;
  };
  struct vent_data {
    bool wakenUp : 1;
    uint8_t valveState : 2;
    uint32_t pressure;
    int8_t termistor1;
    int8_t termistor2;
    uint32_t batteryVoltage;
  };
  struct main_data {
    bool wakenUp : 1;
    uint8_t valveState : 2;
    uint32_t thermocouple1;
    uint32_t thermocouple2;
    uint32_t batteryVoltage;
  };
  struct loadcell_data[2] {
    uint16_t thrust : 16;
    uint16_t pressure : 16;
  };
  struct filling_data {
    uint8_t solenoid_state[3] : 2;
    uint16_t pressure[2] : 16;
    uint16_t thermistor[3] : 48;
    uint16_t thermocouple[3] : 48;
  };
}
lora_data_t;
```

## BLE data transmission

The BLE data transmission definitions will revolve mainly around GATT profiles (services) specifications. The Role of Rosalia in terms of GATT is __Server__ and be the device that stores and shares the data for other devices: clients (mainly smartphones and desktops with BLE capabilities). 

There will be data transmitted mostly for data acquisition and monitoring as well as writable attributes for some ability i.e. to change states, start the test procedure, etc.

In the characteristics category, we will define:
- properties (read, write, notify etc.)
- descriptors (user description, enabling notifications, presentation format etc.)

Each characteristic will have defined:
- Handle (4 byte), 
- UUID (up to 128 bit),
- Permissions, with the same notation as in ESP-IDF i.e. READ, WRITE, WRITE_NR, NOTIFY, BROADCAST, etc. (all defines on [documentation on gatt defines on ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_gatt_defs.html))
- value, where the focus will be on the value type and size

Below are some of the requirements:
- Each characteristic will have max. 1 descriptor defined
- There will be multiple attributes defined, similar to how data is handled in LoRa - grouped by the same categories. 
- the service and characteristic declarations must be read only