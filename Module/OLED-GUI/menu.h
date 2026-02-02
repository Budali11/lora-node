#include <stdint.h>
#include <stdbool.h>
#if !defined MENU_H
#define MENU_H
typedef struct _menu_t{
    struct _menu_t *parent;
    struct _menu_t *children;
    struct _menu_t *next;
    const char *title;
    char subtitle[17];
    void (*action)(void *);
    void *property;
    uint8_t cursor_pos;
    uint8_t oldsor_pos;
    uint8_t cursor_pos_page;
    uint8_t item_count;
    bool first_display;
} menu_t;
void menu_init(void);
#endif