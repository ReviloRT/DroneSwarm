
#pragma once

#include <Arduino.h>

#define PIN_LED GPIO_NUM_2
#define PIN_RGB_R GPIO_NUM_8
#define PIN_RGB_G GPIO_NUM_9
#define PIN_RGB_B GPIO_NUM_10

void blink(uint8_t pin, uint32_t millis);
void init_leds();
void set_RGB_LED(bool R_on, bool G_on, bool B_on);