/*
Copyright 2023 OctopusZ <https://github.com/OctopusZ>
Copyright 2023 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "gpio.h"
#include "uart.h"
#include "wait.h"
#include "battery_measure.h"

static union {
    uint32_t raw;
    struct {
        bool lcd_state : 1;
    };
} user_config = {};

#ifdef RGB_MATRIX_ENABLE

/* clang-format off */
led_config_t g_led_config = {
	{
        {0,  1,  2,  3}, 
        {7,  6,  5,  4}, 
        {8,  9,  10, NO_LED}, 
        {13, 12, 11, 16}, 
        {14, NO_LED,  15,   NO_LED}, 

	}, {
        {0,0},{75,0},{149,0},{224,0},
        {224,24},{149,16},{75,16},{0,16},
        {0,32},{75,32},{149,32},
        {149,48},{75,48},{0,48},
        {38,64},       {118,64}, {224,56},      

	}, {
		4, 4, 4, 4, 
        4, 4, 4, 4,
        4, 4, 4, 
        4, 4, 4, 4, 
        4, 4, 
	}
};
/* clang-format on */
#endif

static struct LCD_env_t {
    uint8_t state               : 1;
    uint8_t numlock             : 1;
    uint8_t layer               : 1;
    uint8_t battery             : 1;
    uint8_t mute                : 1;
    uint8_t initial_sequence    : 3;
    uint8_t last_numlock_record : 1;
    uint8_t last_battery_record : 7;
    uint8_t last_layer_record;
    uint16_t auxiliary_timer;
    uint32_t wake_tick;
} LCD_env = {};

void LCD_on()
{
    writePinHigh(LCD_EN);
    setPinOutput(LCD_EN);
    LCD_env.state = 1;
    LCD_env.initial_sequence = 0;
    LCD_env.auxiliary_timer = timer_read();

    if (LCD_env.state != user_config.lcd_state) {
        user_config.lcd_state = 1;
        eeconfig_update_user(user_config.raw); // write the setings to EEPROM
    }
}

void LCD_off()
{
    writePinLow(LCD_EN);
    setPinOutput(LCD_EN);
    LCD_env.state = 0;

    if (LCD_env.state != user_config.lcd_state) {
        user_config.lcd_state = 0;
        eeconfig_update_user(user_config.raw); // write the setings to EEPROM
    }
}

bool enter_power_level_2_kb()
{
    if (!LCD_env.state) {
        return false;
    }
    LCD_off();
    return true;
}

void quit_power_level_2_kb()
{
    LCD_on();
}

static inline void USB2LCD()
{
    writePinHigh(USB_SET);
    setPinOutput(USB_SET);
}

static inline void USB2MCU()
{
    writePinLow(USB_SET);
    setPinOutput(USB_SET);
}

static void bat_send(uint8_t bat_num)
{
    if (!LCD_env.state) {
        return;
    }

    uint8_t TX_data[] = { 0xfe, 0x02, 0x04, 0x0A, 0x01, 100 };

    TX_data[5] = bat_num;
    TX_data[3] = TX_data[5]; // sum
    uart_transmit(TX_data, sizeof(TX_data) + 1);
}

static void layer_send(uint8_t layer_num)
{
    if (!LCD_env.state) {
        return;
    }

    uint8_t TX_data[] = { 0xfe, 0x02, 0x03, 0x01, 0x01, 0x01 };

    TX_data[5] = layer_num;
    TX_data[3] = TX_data[5]; // sum
    uart_transmit(TX_data, sizeof(TX_data) + 1);
}

static void indicators_send(uint8_t indi)
{
    if (!LCD_env.state) {
        return;
    }

    uint8_t TX_data[] = { 0xfe, 0x02, 0x02, 0x01, 0x01, 0x01 };

    TX_data[5] = indi;
    TX_data[3] = TX_data[5]; // sum
    uart_transmit(TX_data, sizeof(TX_data) + 1);
}

static void mute_send(void)
{
    if (!LCD_env.state) {
        return;
    }

    uint8_t TX_data[] = { 0xfe, 0x02, 0x05, 0x01, 0x01, 0x01 };

    uart_transmit(TX_data, sizeof(TX_data) + 1);
}

#ifdef RGB_MATRIX_ENABLE

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max)
{
    if (led_min <= 0 && led_max >= 0 && host_keyboard_led_state().num_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0xFF, 0x00, 0x00);
    }

    return true;
}

#endif

layer_state_t layer_state_set_kb(layer_state_t state)
{
    if (LCD_env.last_layer_record != get_highest_layer(state)) {
        LCD_env.last_layer_record = get_highest_layer(state);
        LCD_env.layer = 1;
    }

    return layer_state_set_user(state);
}

void eeconfig_init_user()
{
    // EEPROM is getting reset!
    user_config.lcd_state = 1;
    eeconfig_update_user(user_config.raw); // Write default value to EEPROM now
}

void keyboard_post_init_kb()
{
    uart_init(115200);
    uart_start();
    user_config.raw = eeconfig_read_user();
    LCD_env.state = user_config.lcd_state;
    if (LCD_env.state == 1) {
        LCD_on();
    }
    USB2MCU();
}

void wireless_keyboard_pre_task()
{
    if (!LCD_env.state) {
        return;
    }

    SysTick_Config(SysTick_LOAD_RELOAD_Msk);
    PFIC_DisableIRQ(SysTick_IRQn);
    uart_start();
    LCD_env.wake_tick = (SysTick->CNT) & 0xffffffff;
}

void housekeeping_task_kb()
{
#ifdef BATTERY_MEASURE_PIN
    if (LCD_env.last_battery_record != battery_get_last_percentage()) {
        LCD_env.last_battery_record = battery_get_last_percentage();
        LCD_env.battery = 1;
    };
#endif
    if (LCD_env.last_numlock_record != host_keyboard_led_state().num_lock) {
        LCD_env.last_numlock_record = host_keyboard_led_state().num_lock;
        LCD_env.numlock = 1;
    }

    if (!LCD_env.state ||
        (LCD_env.initial_sequence >= 3 && !LCD_env.numlock && !LCD_env.layer && !LCD_env.battery && !LCD_env.mute)) {
        return;
    }

    uint32_t target_tick = LCD_env.wake_tick + (uint32_t)FREQ_SYS / 1000;
    uint32_t current_tick;

    do {
        current_tick = (SysTick->CNT) & 0xffffffff;
    } while (current_tick < target_tick);

    switch (LCD_env.initial_sequence) {
        case 0:
            if (timer_elapsed(LCD_env.auxiliary_timer) > 50) {
                LCD_env.auxiliary_timer = timer_read();
                LCD_env.initial_sequence++;
                layer_send(LCD_env.last_layer_record);
            }
            return;
        case 1:
            if (timer_elapsed(LCD_env.auxiliary_timer) > 10) {
                LCD_env.auxiliary_timer = timer_read();
                LCD_env.initial_sequence++;
                indicators_send(LCD_env.last_numlock_record);
            }
            return;
        case 2:
#ifdef BATTERY_MEASURE_PIN
            if (timer_elapsed(LCD_env.auxiliary_timer) > 10) {
                LCD_env.auxiliary_timer = timer_read();
                LCD_env.initial_sequence++;
                bat_send(LCD_env.last_battery_record);
            }
#else
            LCD_env.initial_sequence++;
#endif
            return;
        default:
            break;
    }

    if (LCD_env.layer) {
        layer_send(LCD_env.last_layer_record);
        LCD_env.layer = 0;
    }
    if (LCD_env.battery) {
        bat_send(LCD_env.last_battery_record);
        LCD_env.battery = 0;
    }
    if (LCD_env.numlock) {
        indicators_send(LCD_env.last_numlock_record);
        LCD_env.numlock = 0;
    }
    if (LCD_env.mute) {
        mute_send();
        LCD_env.mute = 0;
    }
}

void wireless_keyboard_post_task()
{
    uart_stop();
}

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
    switch (keycode) {
        case U2M:
            if (record->event.pressed) {
                USB2MCU();
            }
            return false;
        case U2E:
            if (record->event.pressed) {
                USB2LCD();
            }
            return false;
        case KC_LCD_ON:
            if (record->event.pressed) {
                LCD_on();
            }
            return false;
        case KC_LCD_OFF:
            if (record->event.pressed) {
                LCD_off();
            }
            return false;

        case KC_MUTE:
            LCD_env.mute = 1;
            return true;
        default:
            return true;
    }
}

int main()
{
    extern void protocol_setup();
    extern void protocol_init();
    extern void protocol_task();

    platform_setup();

    protocol_setup();
#if !defined ESB_ENABLE || ESB_ENABLE != 2
    keyboard_setup();
#endif

    protocol_init();

    /* Main loop */
    for (;;) {
        protocol_task();
        //! housekeeping_task() is handled by platform
    }
}
