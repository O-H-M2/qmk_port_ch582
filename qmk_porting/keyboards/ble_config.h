#pragma once

// #define CH58xBLE_ROM
#define BLE_TX_NUM_EVENT          4
#define PERIPHERAL_MAX_CONNECTION 2
#define CENTRAL_MAX_CONNECTION    0
#define BLE_BUFF_MAX_LEN          48
// #define BLE_MEMHEAP_SIZE          (1024 * 7)
// #define BLE_BUFF_NUM              10

#define BLE_SLOT_NUM          8
#define QMK_TASK_INTERVAL_MAX SYS_TICK_MS(15)

// Battery measurement period in (625us)
#define DEFAULT_BATT_PERIOD SYS_TICK_MS(9375)

// Advertising intervals (units of 625us, 160=100ms)
#define HID_INITIAL_ADV_INT_MIN SYS_TICK_MS(30)
#define HID_INITIAL_ADV_INT_MAX SYS_TICK_MS(50)
#define HID_HIGH_ADV_INT_MIN    SYS_TICK_MS(20)
#define HID_HIGH_ADV_INT_MAX    SYS_TICK_MS(20)
#define HID_LOW_ADV_INT_MIN     SYS_TICK_MS(211.25)
#define HID_LOW_ADV_INT_MAX     SYS_TICK_MS(211.25)

// Advertising timeouts in sec
#define HID_INITIAL_ADV_TIMEOUT 60
#define HID_HIGH_ADV_TIMEOUT    5
#define HID_LOW_ADV_TIMEOUT     30

// Param update delay
#define START_PARAM_UPDATE_EVT_DELAY SYS_TICK_MS(2000)

// Param update delay
#define START_PHY_UPDATE_DELAY SYS_TICK_MS(10000)

// HID idle timeout in msec; set to zero to disable timeout
#define DEFAULT_HID_IDLE_TIMEOUT 0

// Minimum connection interval (units of 1.25ms)
#define DEFAULT_DESIRED_MIN_CONN_INTERVAL MS_UNIT_1_25(15)

// Maximum connection interval (units of 1.25ms)
#define DEFAULT_DESIRED_MAX_CONN_INTERVAL MS_UNIT_1_25(45)

// Slave latency to use if parameter update request
#define DEFAULT_DESIRED_SLAVE_LATENCY 6

// Supervision timeout value (units of 10ms)
#define DEFAULT_DESIRED_CONN_TIMEOUT 500

// Default passcode
#define DEFAULT_PASSCODE 0

// Default GAP pairing mode
#define DEFAULT_PAIRING_MODE GAPBOND_PAIRING_MODE_WAIT_FOR_REQ

// Default MITM mode (TRUE to require passcode or OOB when pairing)
#define DEFAULT_MITM_MODE TRUE

// Default bonding mode, TRUE to bond
#define DEFAULT_BONDING_MODE TRUE

// Default GAP bonding I/O capabilities
// #define DEFAULT_IO_CAPABILITIES GAPBOND_IO_CAP_KEYBOARD_ONLY
#define DEFAULT_IO_CAPABILITIES GAPBOND_IO_CAP_NO_INPUT_NO_OUTPUT

// Battery level is critical when it is less than this %
#define DEFAULT_BATT_CRITICAL_LEVEL 6