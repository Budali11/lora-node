#include "FreeRTOSConfig.h"
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
#include "stm32l4xx_hal_conf.h"
#include <stdbool.h>

#define EVENT_KEY_MENU 0x01
#define EVENT_KEY_UP 0x02
#define EVENT_KEY_DOWN 0x04
#define EVENT_KEY_CONFIRM 0x08

// static struct rt_event event_keyboard;
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

uint32_t radio_frequency = 866000000;
int32_t radio_tx_power = 14;
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
extern uint8_t line_selected[MAX_LINES]; 
extern menu_t main_menu;
menu_t *current_menu;

void Update_Cursor(void) {
    // cursor
    configASSERT(current_menu != NULL);
    if (current_menu->cursor_pos > current_menu->oldsor_pos) {
        // moved down
        if (current_menu->cursor_pos_page < MAX_LINES - 1) {
            current_menu->cursor_pos_page++;
        }
    } else if (current_menu->cursor_pos < current_menu->oldsor_pos) {
        // moved up
        if (current_menu->cursor_pos_page > 0) {
            current_menu->cursor_pos_page--;
        }
    }
    if (line_selected[current_menu->cursor_pos_page] != 1) {
        for (int i = 0; i < MAX_LINES; i++) {
            if (line_selected[i] == 1) {
                OLED_ReverseArea(0, i*16, 128, 16);
                line_selected[i] = 0;
                break;
            }
        }
        OLED_ReverseArea(0, current_menu->cursor_pos_page*16, 128, 16);
        line_selected[current_menu->cursor_pos_page] = 1;
    }
}

void disp_fresh_timeout(TimerHandle_t xTimer) {
    // Display refresh timeout handler implementation goes here
    OLED_Update();
}

void Update_Menu_Display(menu_t *menu) {
    // Update menu display implementation goes here
    if (menu == NULL)
        return;
    menu_t *temp_menu = menu->children;
    if (temp_menu == NULL) {
        // this menu is a leaf node
        temp_menu = menu;
        char row[17] = {0};
        sprintf(row, "%-16s", temp_menu->title);
        OLED_ShowString(0, 0, row, OLED_8X16);
        OLED_ClearArea(0, 16, 128, 16);
        OLED_ShowString(0, 16, temp_menu->subtitle, OLED_8X16);
        line_selected[0] = 0;
        line_selected[1] = 0;
    }
    else {
        menu_t *menu_list[MAX_LINES] = {0};
        // gather menu items to display
        if (menu->cursor_pos > menu->oldsor_pos 
                && menu->cursor_pos_page < MAX_LINES - 1) {
            // no need to scroll down
            Update_Cursor();
            return;
        } else if (menu->cursor_pos < menu->oldsor_pos 
                && menu->cursor_pos_page > 0) {
            // no need to scroll up
            Update_Cursor();
            return;
        } else if (menu->cursor_pos < menu->oldsor_pos && menu->cursor_pos_page == 0) {
            // need to scroll up
            for (int i = 0; i < MAX_LINES; i++) {
                if (i + menu->cursor_pos < menu->item_count) {
                    menu_list[i] = menu->children;
                    for (int j = 0; j < i + menu->cursor_pos; j++) {
                        menu_list[i] = menu_list[i]->next;
                    }
                } else {
                    menu_list[i] = NULL;
                }
            }
        } else if (menu->cursor_pos > menu->oldsor_pos && menu->cursor_pos_page == MAX_LINES - 1) {
            // need to scroll down
            for (int i = 0; i < MAX_LINES; i++) {
                if (i + menu->cursor_pos - (MAX_LINES - 1) < menu->item_count) {
                    menu_list[i] = menu->children;
                    for (int j = 0; j < i + menu->cursor_pos - (MAX_LINES - 1); j++) {
                        menu_list[i] = menu_list[i]->next;
                    }
                } else {
                    menu_list[i] = NULL;
                }
            }
        } else {
            // first time display or no scrolling
            if (!menu->first_display) {
                return;
            }
            // cursor_pos should appear at cursor_pos_page
            for (int i = 0; i < MAX_LINES; i++) {
                if (i + menu->cursor_pos - menu->cursor_pos_page < menu->item_count) {
                    menu_list[i] = menu->children;
                    for (int j = 0; j < i + menu->cursor_pos - menu->cursor_pos_page; j++) {
                        menu_list[i] = menu_list[i]->next;
                    }
                } else {
                    menu_list[i] = NULL;
                }
            }
            // for (int i = 0; i < MAX_LINES; i++) {
            //     if (i < menu->item_count) {
            //         menu_list[i] = menu->children;
            //         for (int j = 0; j < i; j++) {
            //             menu_list[i] = menu_list[i]->next;
            //         }
            //     } else {
            //         menu_list[i] = NULL;
            //     }
            // }
            menu->first_display = false;
        }
        
        // display menu list
        for (int i = 0; i < MAX_LINES; i++) {
            char row[17] = {0};
            if (menu_list[i] != NULL) {
                sprintf(row, "%-16s", menu_list[i]->title);
            } else {
                sprintf(row, "                ");
            }
            OLED_ShowString(0, i * 16, row, OLED_8X16);
            line_selected[i] = 0;
        }
        Update_Cursor();
    }
}

void main_menu_action(void *param) {
    // Main menu action implementation goes here
    current_menu = &main_menu;
    EventBits_t event_recv = 0;
    current_menu->cursor_pos = 0;
    current_menu->cursor_pos_page = 0;
    Update_Menu_Display(current_menu);
    for (;;) {
        current_menu->oldsor_pos = current_menu->cursor_pos;
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            current_menu->first_display = true;
        } else if (event_recv & EVENT_KEY_UP) {
            if (current_menu->cursor_pos > 0) {
                current_menu->cursor_pos--;
            }
        } else if (event_recv & EVENT_KEY_DOWN) {
            if (current_menu->cursor_pos < current_menu->item_count - 1) {
                current_menu->cursor_pos++;
            }
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            current_menu = current_menu->children;
            for (int i = 0; i < current_menu->parent->cursor_pos; i++) {
                current_menu = current_menu->next;
            }
            current_menu->action(NULL);
            current_menu = current_menu->parent;
            current_menu->first_display = true;
        }
        Update_Menu_Display(current_menu);
    }
}

void freq_menu_action(void *param) {
    // Frequency menu action implementation goes here
    EventBits_t event_recv = 0;
    uint32_t freq_temp = *(uint32_t *)current_menu->property;
    for (;;) {
        sprintf(current_menu->subtitle, "%lu Hz", freq_temp);
        Update_Menu_Display(current_menu);
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            return;
        } else if (event_recv & EVENT_KEY_UP) {
            freq_temp += 125000;
        } else if (event_recv & EVENT_KEY_DOWN) {
            freq_temp -= 125000;
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            *(uint32_t *)current_menu->property = freq_temp;
            return;
        }
    }
}

void power_menu_action(void *param) {
    // Power menu action implementation goes here
    EventBits_t event_recv = 0;
    int32_t power_temp = *(int32_t *)current_menu->property;
    for (;;) {
        sprintf(current_menu->subtitle, "%ld dBm", power_temp);
        Update_Menu_Display(current_menu);
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            return;
        } else if (event_recv & EVENT_KEY_UP) {
            power_temp += 1;
        } else if (event_recv & EVENT_KEY_DOWN) {
            power_temp -= 1;
        } else if (event_recv & EVENT_KEY_CONFIRM) {
            *(int32_t *)current_menu->property = power_temp;
            return;
        }
    }
}

void radio_menu_action(void *param) {
    // Radio menu action implementation goes here
    EventBits_t event_recv = 0;
    for (;;) {
        sprintf(current_menu->subtitle, "Press confirm");
        Update_Menu_Display(current_menu);
        event_recv = xEventGroupWaitBits(event_keyboard,
                                         EVENT_KEY_MENU | EVENT_KEY_UP |
                                             EVENT_KEY_DOWN | EVENT_KEY_CONFIRM,
                                         pdTRUE, pdFALSE, portMAX_DELAY);
        if (event_recv & EVENT_KEY_MENU) {
            return;
        } else if (event_recv & EVENT_KEY_UP) {

        } else if (event_recv & EVENT_KEY_DOWN) {

        } else if (event_recv & EVENT_KEY_CONFIRM) {
            // emit a radio signal
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
