#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "OLED_SSD1306/oled.h"

int main() {
    stdio_init_all();

    // Small delay to allow USB CDC to enumerate
    sleep_ms(500);

    // Initialize OLED (configures I2C1 on pins defined in oled.h)
    oled_init();

    // Draw some primitives
    oled_clear();
    oled_draw_string(0, 0, "SSD1306 Ready");
    oled_draw_line(0, 10, 127, 10, true);
    oled_draw_line(0, 63, 127, 63, true);

    // Example text buffer rendering
    oled_set_text_line(1, "Hello, BitDogLab!", OLED_ALIGN_CENTER);
    oled_set_text_line(3, "Rapport Demo", OLED_ALIGN_CENTER);
    oled_set_text_line(5, "RP2040 + I2C", OLED_ALIGN_CENTER);

    oled_render();
    sleep_ms(1000);

    // Render buffered text layout
    oled_render_text();

    // Blink a simple marker in a loop
    bool on = true;
    while (true) {
        oled_set_pixel(120, 56, on);
        oled_render();
        on = !on;
        sleep_ms(500);
    }
}
