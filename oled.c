#include "oled/oled.h"
#include "ssd1306.h"
#include <string.h>
#include "magnectostriccao.h"

// Internal buffer and render area
static uint8_t ssd_buffer[ssd1306_buffer_length];
static struct render_area area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1,
    .buffer_length = 0
};

/* Internal text buffer for facade text lines */
static char text_buffer[max_text_lines][max_text_columns];

void oled_init(void) {
    // Initialize I2C pins and bus is done by ssd1306_i2c.h includes
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    calculate_render_area_buffer_length(&area);
    ssd1306_init();
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(ssd_buffer, &area);
}

void oled_clear(void) {
    memset(ssd_buffer, 0, ssd1306_buffer_length);
    render_on_display(ssd_buffer, &area);
}

void oled_render(void) {
    render_on_display(ssd_buffer, &area);
}

void oled_set_pixel(int x, int y, bool on) {
    ssd1306_set_pixel(ssd_buffer, x, y, on);
}

void oled_draw_line(int x0, int y0, int x1, int y1, bool on) {
    ssd1306_draw_line(ssd_buffer, x0, y0, x1, y1, on);
}

void oled_draw_char(int x, int y, char c) {
    ssd1306_draw_char(ssd_buffer, x, y, (uint8_t)c);
}

void oled_draw_string(int x, int y, const char *str) {
    ssd1306_draw_string(ssd_buffer, x, y, (char *)str);
}

/**
 * Set a single line of text in the internal text buffer
 */
void oled_set_text_line(uint8_t line, const char *text) {
    strncpy(text_buffer[line], text, max_text_columns);
}

/**
 * Render the internal text buffer lines to the display
 */
void oled_render_text(void) {
    for (uint8_t line = 0; line < max_text_lines; line++) {
        oled_draw_string(5, line * ssd1306_line_height, text_buffer[line]);
    }
    oled_render();
}
