#include <stdint.h>
#if !defined MENU_H
#define MENU_H
typedef struct _menu_t{
    struct _menu_t *parent;
    struct _menu_t *children;
    struct _menu_t *next;
    const char *title;
    char *subtitle;
    void (*action)(void *);
    void *property;
    uint8_t cursor_pos;
    uint8_t oldsor_pos;
} menu_t;
void menu_init(void);
#endif