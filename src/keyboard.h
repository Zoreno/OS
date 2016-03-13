#ifndef KEYBOARD_H
#define KEYBOARD_H 

#include "common.h"

typedef unsigned char scan_code;
typedef void (*key_pressed_t)(scan_code);
typedef void (*key_typed_t)(scan_code);
typedef void (*key_released_t)(scan_code);

void register_key_pressed_handler(key_pressed_t* handler);
void register_key_typed_handler(key_typed_t* handler);
void register_key_released_handler(key_released_t* handler);

void init_keyboard();

#endif