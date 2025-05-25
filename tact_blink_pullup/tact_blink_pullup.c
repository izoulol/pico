/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#define INPUT_PIN 15    // 入力用のGPIOピン
#define LED_PIN 6      // LED用のGPIOピン

// Perform initialisation
int pico_led_init(void) {
    // LED用GPIOの初期化
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // 入力ピンの初期化
    gpio_init(INPUT_PIN);
    gpio_set_dir(INPUT_PIN, GPIO_IN);
    gpio_pull_up(INPUT_PIN);  // プルアップ抵抗を有効化
    return PICO_OK;
}

// Turn the led on or off based on input
void pico_set_led(bool input_state) {
    gpio_put(LED_PIN, !input_state);    // 入力がHIGHならLEDを点灯
}

int main() {
    // シリアル通信の初期化
    stdio_init_all();
    
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    
    // 起動時のLED初期状態設定
    pico_set_led(false);
    
    while (true) {
        bool input_state = gpio_get(INPUT_PIN);
        pico_set_led(input_state);
        sleep_ms(10);
    }
}
