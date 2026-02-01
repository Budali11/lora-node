#include "cmsis_os.h"
#include "event_groups.h"
#include "main.h"
#include "module_redirect.h"
#include "multi_button.h"
#include "portmacro.h"
#include "projdefs.h"
#include <stdint.h>
#include <stdio.h>
#include "menu.h"


#define EVENT_KEY_MENU 0x01
#define EVENT_KEY_UP 0x02
#define EVENT_KEY_DOWN 0x04
#define EVENT_KEY_CONFIRM 0x08

// static struct rt_event event_keyboard;
uint32_t tx_power;
uint32_t frequency;
EventGroupHandle_t event_keyboard;

void KEY_MENU_IRQ(Button *btn_handle) {
    // Key menu interrupt handler implementation goes here
    xEventGroupSetBits(event_keyboard, EVENT_KEY_MENU);
    // rt_event_send(&event_keyboard, EVENT_KEY_MENU);
}

void KEY_UP_IRQ(Button *btn_handle) {
    // Key up interrupt handler implementation goes here
    xEventGroupSetBits(event_keyboard, EVENT_KEY_UP);
    // rt_event_send(&event_keyboard, EVENT_KEY_UP);
}

void KEY_DOWN_IRQ(Button *btn_handle) {
    // Key down interrupt handler implementation goes here
    xEventGroupSetBits(event_keyboard, EVENT_KEY_DOWN);
    // rt_event_send(&event_keyboard, EVENT_KEY_DOWN);
}

void KEY_CONFIRM_IRQ(Button *btn_handle) {
    // Key confirm interrupt handler implementation goes here
    xEventGroupSetBits(event_keyboard, EVENT_KEY_CONFIRM);
    // rt_event_send(&event_keyboard, EVENT_KEY_CONFIRM);
}

void radio_thread_entry(void *parameter) {
    // Radio thread implementation goes here
    while(1) {
        vTaskDelay(1000);
    }
}

void button_timer_callback(TimerHandle_t xTimer) {
    // Button tick handler implementation goes here
    button_ticks();
}
void keyboard_thread_entry(void *parameter) {
    // Keyboard thread implementation goes here
    event_keyboard = xEventGroupCreate();
    Button btn_menu = {0}, btn_up = {0}, btn_down = {0}, btn_confirm = {0};
    Button btns[] = {btn_menu, btn_up, btn_down, btn_confirm};
    void *irq_handlers[] = {KEY_MENU_IRQ, KEY_UP_IRQ, KEY_DOWN_IRQ,
                            KEY_CONFIRM_IRQ};
    for (int i = 0; i < sizeof(btns) / sizeof(btns[0]); i++) {
        button_init(&btns[i], Button_Read, 0, i);
        button_start(&btns[i]);
        button_attach(&btns[i], BTN_SINGLE_CLICK, irq_handlers[i]);
    }
    TimerHandle_t keyboard_timer =
        xTimerCreate("keyboard timer", 5, pdTRUE, 0, button_timer_callback);
    xTimerStart(keyboard_timer, 0);
    while (1) {
        vTaskDelay(1000);
    }
}

#include "OLED_GUI.h"
#include "menu.h"
// rt_timer_t disp_fresh_timer;
extern uint8_t OLED_DisplayBuf[4][128];
extern menu_t main_menu;
menu_t *current_menu;
void disp_fresh_timeout(TimerHandle_t xTimer) {
    // Display refresh timeout handler implementation goes here
    static uint8_t old_pos = 3;
    if (old_pos != current_menu->cursor_pos) {
        OLED_ReverseArea(0, old_pos * 16, 128, 16);
        OLED_ReverseArea(0, current_menu->cursor_pos * 16, 128, 16);
    }
    old_pos = current_menu->cursor_pos;
    OLED_Update();
}

void main_menu_action(void *param) {
    // Main menu action implementation goes here
    current_menu = &main_menu;
    EventBits_t event_recv = 0;
    menu_t *temp_menu = current_menu->children;
    current_menu->cursor_pos = 0;
    for (;;) {
        for (int i = 0; temp_menu != NULL; i++) {
            char row[16] = {0};
            sprintf(row, "%-15s", temp_menu->title);
            OLED_ShowString(0, i * 16, row, OLED_8X16);
            if (temp_menu->next == NULL)
                break;
            temp_menu = temp_menu->next;
        }
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {

        } else if (event_recv & EVENT_KEY_UP) {
            if (current_menu->cursor_pos > 0) {
                current_menu->cursor_pos--;
            }
        } else if (event_recv & EVENT_KEY_DOWN) {
            if (current_menu->cursor_pos < 1) {
                current_menu->cursor_pos++;
            }
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            current_menu = current_menu->children;
            for (int i = 0; i < current_menu->parent->cursor_pos; i++) {
                current_menu = current_menu->next;
            }
            current_menu->action(NULL);
        }
    }
}

void freq_menu_action(void *param) {
    // Frequency menu action implementation goes here
    EventBits_t event_recv = 0;
    uint32_t freq_temp = *(uint32_t *)current_menu->property;
    for (;;) {
        char freq_str[16] = {0};
        sprintf(freq_str, "%-15s", "Frequency");
        OLED_ShowString(0, 0, freq_str, OLED_8X16);
        sprintf(freq_str, "%lu Hz", freq_temp);
        OLED_ClearArea(0, 16, 128, 8);
        OLED_ShowString(0, 16, freq_str, OLED_8X16);
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            return;
        } else if (event_recv & EVENT_KEY_UP) {
            freq_temp += 125000;
            sprintf(freq_str, "%lu Hz", freq_temp);
            OLED_ShowString(0, 16, freq_str, OLED_8X16);
        } else if (event_recv & EVENT_KEY_DOWN) {
            freq_temp -= 125000;
            sprintf(freq_str, "%lu Hz", freq_temp);
            OLED_ShowString(0, 16, freq_str, OLED_8X16);
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            *(uint32_t *)current_menu->property = freq_temp;
            return;
        }
    }
}

void power_menu_action(void *param) {
    // Power menu action implementation goes here
    EventBits_t event_recv = 0;
    uint32_t power_temp = *(uint32_t *)current_menu->property;
    for (;;) {
        char power_str[16] = {0};
        sprintf(power_str, "%-15s", current_menu->title);
        OLED_ShowString(0, 0, power_str, OLED_8X16);
        sprintf(power_str, "%lu dBm", power_temp);
        OLED_ShowString(0, 16, power_str, OLED_8X16);
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            return;
        } else if (event_recv & EVENT_KEY_UP) {
            power_temp += 1;
            sprintf(power_str, "%lu dBm", power_temp);
            OLED_ShowString(0, 16, power_str, OLED_8X16);
        } else if (event_recv & EVENT_KEY_DOWN) {
            power_temp -= 1;
            sprintf(power_str, "%lu dBm", power_temp);
            OLED_ShowString(0, 16, power_str, OLED_8X16);
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            *(uint32_t *)current_menu->property = power_temp;
            return;
        }
    }
}

void disp_thread_entry(void *parameter) {
    // Display thread implementation goes here
    menu_init();
    // OLED_0in91_Init();
    OLED_Init();
    TimerHandle_t disp_fresh_timer = xTimerCreate("OLED fresh", 100, pdTRUE, 0, disp_fresh_timeout);
    // rt_timer_init(disp_fresh_timer, "disp_fresh", disp_fresh_timeout, NULL,
    // 15,
    //               RT_TIMER_FLAG_PERIODIC);
    // rt_timer_start(disp_fresh_timer);
    xTimerStart(disp_fresh_timer, 0);
    main_menu.action(NULL);
}
