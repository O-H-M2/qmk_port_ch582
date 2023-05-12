/*
Copyright 2022 OctopusZ <https://github.com/OctopusZ>

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
static bool LCD_state = 1;
static bool LCD_num_send = 0;
static bool LCD_layer_send = 0;
static bool LCD_bat_send = 0;
static bool LCD_mute_send = 0;

static uint16_t LCD_start_timer;
static uint8_t LCD_start_state = 0;

static uint8_t layer_num = 0;
static bool numlocks = 0;
static uint8_t bat_percentage = 0;

volatile uint8_t uart_start_timeout = 0;
void LCD_on()
{
    writePinHigh(LCD_EN);
    setPinOutput(LCD_EN);
    LCD_state = 1;
    LCD_start_timer = timer_read();
    LCD_start_state = 0;
}

void LCD_off()
{
    writePinLow(LCD_EN);
    setPinOutput(LCD_EN);
    LCD_state = 0;
    uart_stop();
}

bool enter_power_level_2_kb()
{
    if (!LCD_state) {
        return false;
    }
    LCD_off();
    return true;
}

void quit_power_level_2_kb()
{
    LCD_on();
}

static void USB2LCD()
{
    writePinHigh(USB_SET);
    setPinOutput(USB_SET);
}

static void USB2MCU()
{
    writePinLow(USB_SET);
    setPinOutput(USB_SET);
}

void bat_send(uint8_t bat_num)
{
    if (LCD_state) {
        uint8_t TX_date[] = { 0xfe, 0x02, 0x04, 0x0A, 0x01, 100 };
        TX_date[5] = bat_num;
        TX_date[3] = TX_date[5]; // sum
        uart_transmit(TX_date, sizeof(TX_date) + 1);
    }
}
void layer_send(uint8_t layer_num)
{
    if (LCD_state) {
        uint8_t TX_date[] = { 0xfe, 0x02, 0x03, 0x01, 0x01, 0x01 };
        TX_date[5] = layer_num;
        TX_date[3] = TX_date[5]; // sum
        uart_transmit(TX_date, sizeof(TX_date) + 1);
    }
}
static void indicators_send(uint8_t indi)
{
    if (LCD_state) {
        uint8_t TX_date[] = { 0xfe, 0x02, 0x02, 0x01, 0x01, 0x01 };
        TX_date[5] = indi;
        TX_date[3] = TX_date[5]; // sum
        uart_transmit(TX_date, sizeof(TX_date) + 1);
    }
}

static void mute_send(void)
{
    if (LCD_state) {
        uint8_t TX_date[] = { 0xfe, 0x02, 0x05, 0x01, 0x01, 0x01 };
        uart_transmit(TX_date, sizeof(TX_date) + 1);
    }
}

#ifdef RGB_MATRIX_ENABLE

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max)
{
    if (numlocks != host_keyboard_led_state().num_lock) {
        numlocks = host_keyboard_led_state().num_lock;
        LCD_num_send = 1;
    }

    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }
    if (led_min <= 0 && led_max >= 0 && host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(0, 0xFF, 0x00, 0x00);
    }
#if defined BATTERY_MEASURE_PIN || defined BLE_ENABLE
    extern void wireless_rgb_indicator_task(uint8_t led_min, uint8_t led_max);

    wireless_rgb_indicator_task(led_min, led_max);
#endif
    return true;
}

#endif

layer_state_t layer_state_set_kb(layer_state_t state)
{
    if (layer_num != get_highest_layer(state)) {
        layer_num = get_highest_layer(state);
        LCD_layer_send = 1;
    }
    return state;
}

void keyboard_post_init_kb()
{
    // setPinInput(B12);
    PRINT("init\n");
    uart_init(115200);
    // uart_start();

    LCD_on();

    writePinLow(USB_SET);
    setPinOutput(USB_SET);

    do {
        sys_safe_access_enable();
        R8_SLP_CLK_OFF0 &= ~RB_SLP_CLK_TMR0;
        sys_safe_access_disable();
    } while (R8_SLP_CLK_OFF0 & RB_SLP_CLK_TMR0);
}

//void keyboard_task_pre() // just for debug
void wireless_keyboard_pre_task()
{
    if (LCD_state && uart_start_timeout == 0) {
        uart_start();
        TMR1_TimerInit(FREQ_SYS / 1000);       // 设置定时时间 1ms
        TMR1_ITCfg(ENABLE, TMR0_3_IT_CYC_END); // 开启中断
        PFIC_EnableIRQ(TMR1_IRQn);

        uart_start_timeout = 1;

#ifdef BATTERY_MEASURE_PIN
        if (bat_percentage != battery_get_last_percentage()) {
            bat_percentage = battery_get_last_percentage();
            LCD_bat_send = 1;
        };
#endif
    }
}
void housekeeping_task_kb()
{
    if (LCD_state) {
        switch (LCD_start_state) {
            case 0:
                if (timer_elapsed(LCD_start_timer) > 50 && uart_start_timeout == 2) {
                    LCD_start_timer = timer_read();
                    LCD_start_state++;
                    layer_send(layer_num);
                    uart_start_timeout = 3;
                }
                break;
            case 1:
                if (timer_elapsed(LCD_start_timer) > 10 && uart_start_timeout == 2) {
                    LCD_start_timer = timer_read();
                    LCD_start_state++;
                    numlocks = host_keyboard_led_state().num_lock;
                    if (numlocks)
                        indicators_send(1);
                    else
                        indicators_send(0);
                    uart_start_timeout = 3;
                }
                break;
            case 2:
                if (timer_elapsed(LCD_start_timer) > 10 && uart_start_timeout == 2) {
                    LCD_start_timer = timer_read();
                    LCD_start_state++;
#ifdef BATTERY_MEASURE_PIN
                    bat_percentage = battery_get_last_percentage();
                    bat_send(bat_percentage);
#endif
                    uart_start_timeout = 3;
                }
                break;
            default:
                break;
        }

        if (LCD_layer_send) {
            if (uart_start_timeout == 2)
                layer_send(layer_num);
            LCD_layer_send = 0;
            uart_start_timeout = 3;
        }
        if (LCD_bat_send) {
            if (uart_start_timeout == 2)
                bat_send(bat_percentage);
            LCD_bat_send = 0;
            uart_start_timeout = 3;
        }
        if (LCD_num_send) {
            if (uart_start_timeout == 2) {
                if (numlocks)
                    indicators_send(1);
                else
                    indicators_send(0);
            }
            LCD_num_send = 0;
            uart_start_timeout = 3;
        }
        if (uart_start_timeout == 3) {
            uart_stop();
            uart_start_timeout = 0;
        }
        if (LCD_mute_send) {
            if (uart_start_timeout == 2)
                mute_send();
            LCD_mute_send = 0;
            uart_start_timeout = 3;
        }
    }
}

#define U2M 32277
#define U2E 32278

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
        case KC_MUTE:
            LCD_mute_send = 1;
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

__INTERRUPT __HIGH_CODE void TMR1_IRQHandler()
{
    // setPinInput(B12);
    
    uart_start_timeout = 2;
    TMR1_ITCfg(DISABLE, TMR0_3_IT_CYC_END); // 开启中断
    PFIC_DisableIRQ(TMR1_IRQn);
}
