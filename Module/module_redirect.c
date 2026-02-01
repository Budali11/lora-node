#include "main.h"
#include "multi_button.h"
#include "module_redirect.h"
uint8_t Button_Read(uint8_t button_id) {
    if (button_id == BUTTON_MENU_ID) {
        return HAL_GPIO_ReadPin(KEY_MENU_GPIO_Port, KEY_MENU_Pin);
    } else if (button_id == BUTTON_UP_ID) {
        return HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin);
    } else if (button_id == BUTTON_DOWN_ID) {
        return HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin);
    } else if (button_id == BUTTON_CONFIRM_ID) {
        return HAL_GPIO_ReadPin(KEY_CONFIRM_GPIO_Port, KEY_CONFIRM_Pin);
    }
    return 0xff;
}