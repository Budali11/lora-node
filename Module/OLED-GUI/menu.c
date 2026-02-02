#include "menu.h"
#include "stdlib.h"
#include <stdint.h>

menu_t main_menu;
void main_menu_action(void *param);
/* sub 1 menu */
menu_t freq_menu;
extern uint32_t radio_frequency;
void freq_menu_action(void *param);
menu_t power_menu;
extern uint32_t radio_tx_power;
void power_menu_action(void *param);
menu_t radio_menu;
void radio_menu_action(void *param);
/* sub 2 menu */
void menu_init(void) {
    main_menu.title = "Main Menu";
    main_menu.parent = NULL;
    main_menu.children = &freq_menu;
    main_menu.next = NULL;
    main_menu.action = main_menu_action;
    main_menu.item_count = 3;
    main_menu.first_display = true;
    freq_menu.title = "Freq Setting";
    freq_menu.parent = &main_menu;
    freq_menu.children = NULL;
    freq_menu.next = &power_menu;
    freq_menu.action = freq_menu_action;
    freq_menu.property = &radio_frequency;
    power_menu.title = "Power Setting";
    power_menu.parent = &main_menu;
    power_menu.children = NULL;
    power_menu.next = &radio_menu;
    power_menu.action = power_menu_action;
    power_menu.property = &radio_tx_power;
    radio_menu.title = "Radio Emit menu";
    radio_menu.parent = &main_menu;
    radio_menu.children = NULL;
    radio_menu.next = NULL;
    radio_menu.action = radio_menu_action;
    radio_menu.property = NULL;
}
