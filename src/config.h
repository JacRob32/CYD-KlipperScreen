#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Display Configuration for ESP32-2432S028R (CYD)
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320
#define DISPLAY_ROTATION 1  // Portrait mode

// LVGL Configuration
#define LVGL_TICK_PERIOD 5
#define LVGL_TASK_PRIORITY 1
#define LVGL_TASK_STACK_SIZE 4096

// Touch Configuration (XPT2046)
#define TOUCH_CS    33
#define TOUCH_IRQ   36

// Button debounce time (ms)
#define BUTTON_DEBOUNCE_TIME 200

// Screen timeout (ms) - 0 to disable
#define SCREEN_TIMEOUT 300000

// Colors Theme (KlipperScreen inspired - Z-Bolt Dark)
#define COLOR_PRIMARY     0x2196F3  // Blue
#define COLOR_SECONDARY   0xFF5722  // Orange
#define COLOR_SUCCESS     0x4CAF50  // Green
#define COLOR_WARNING     0xFF9800  // Amber
#define COLOR_ERROR       0xF44336  // Red
#define COLOR_INFO        0x00BCD4  // Cyan

#define COLOR_BG_DARK     0x1A1A1A  // Dark background
#define COLOR_BG_MEDIUM   0x2D2D2D  // Medium background
#define COLOR_BG_LIGHT    0x3D3D3D  // Light background
#define COLOR_TEXT_DARK   0x000000
#define COLOR_TEXT_LIGHT  0xE0E0E0

// Screen IDs
enum ScreenID {
    SCREEN_HOME = 0,
    SCREEN_ACTIONS,
    SCREEN_EXTRUDE,
    SCREEN_MOVE,
    SCREEN_TEMPERATURE,
    SCREEN_PRINT_STATUS,
    SCREEN_SETTINGS,
    SCREEN_MAX
};

// Button color presets (matching KlipperScreen)
#define BTN_COLOR_1 0xD32F2F  // Red
#define BTN_COLOR_2 0x388E3C  // Green
#define BTN_COLOR_3 0x1976D2  // Blue
#define BTN_COLOR_4 0xF57C00  // Orange

#endif // CONFIG_H
