#include <stdio.h>
#include "pico/stdlib.h"

#define LED_PIN 6
#define BUTTON_PIN 15
#define DEBOUNCE_DELAY_MS 50

// LEDを初期化する関数
void init_led() {
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

// LEDの状態を設定する関数
void set_led_state(bool on) {
    gpio_put(LED_PIN, on);
}

// tactileスイッチの初期化する関数
void init_button() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // プルアップ抵抗を有効にする
}

int main()
{
    stdio_init_all();

    init_led(); // LEDの初期化関数を呼び出し

    init_button(); // tactileスイッチの初期化関数を呼び出し
    
    bool led_on = false;
    bool last_button_state = true; // ボタンの初期状態は離されている（HIGH）と仮定
    uint32_t last_debounce_time = 0;

    while (true) {
        bool current_button_state = gpio_get(BUTTON_PIN);

        // ボタンの状態が変化し、かつ前回のチャタリングから十分な時間が経過した場合
        if (current_button_state != last_button_state && (to_ms_since_boot(get_absolute_time()) - last_debounce_time) > DEBOUNCE_DELAY_MS) {
            last_debounce_time = to_ms_since_boot(get_absolute_time());

            // ボタンが押された場合（プルアップなのでLOWになった時）
            if (current_button_state == false) {
                led_on = !led_on; // LEDの状態をトグル
                set_led_state(led_on); // LEDの状態を設定する関数を呼び出し
                printf("LED is now %s\n", led_on ? "ON" : "OFF");
            }
        }
        last_button_state = current_button_state;
        sleep_ms(10); // CPU負荷を軽減するため短い待機を入れる
    }
}
