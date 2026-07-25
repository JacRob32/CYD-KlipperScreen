#include "hardware/display_driver.h"
#include <XPT2046_Touchscreen.h>

// Touchscreen instance
static XPT2046_Touchscreen ts(TOUCH_CS);

DisplayDriver::DisplayDriver() 
    : tft()
    , touchInitialized(false) {
}

bool DisplayDriver::init() {
    Serial.println(F("[DisplayDriver] Initializing..."));
    
    // Initialize TFT
    tft.init();
    tft.setRotation(DISPLAY_ROTATION);
    tft.fillScreen(TFT_BLACK);
    
    #if defined(TFT_BL)
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, TFT_BACKLIGHT_ON);
    Serial.println(F("[DisplayDriver] Backlight enabled"));
    #endif
    
    Serial.println(F("[DisplayDriver] TFT initialized"));
    
    // Initialize Touchscreen
    ts.begin();
    ts.setRotation(DISPLAY_ROTATION);
    touchInitialized = true;
    Serial.println(F("[DisplayDriver] Touchscreen initialized"));
    
    // Initialize LVGL
    lv_init();
    Serial.println(F("[DisplayDriver] LVGL core initialized"));
    
    // Create display buffer (double buffering for smoother updates)
    static lv_color_t buf1[DISPLAY_WIDTH * 40];
    static lv_color_t buf2[DISPLAY_WIDTH * 40];
    
    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISPLAY_WIDTH * 40);
    
    // Initialize display driver
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = DISPLAY_WIDTH;
    disp_drv.ver_res = DISPLAY_HEIGHT;
    disp_drv.flush_cb = lvglFlushCallback;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);
    
    // Initialize input device driver (touch)
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = lvglTouchRead;
    lv_indev_drv_register(&indev_drv);
    
    Serial.println(F("[DisplayDriver] LVGL display and input registered"));
    Serial.println(F("[DisplayDriver] Initialization complete"));
    
    return true;
}

void DisplayDriver::taskHandler() {
    lv_task_handler();
    delay(LVGL_TICK_PERIOD);
}

void DisplayDriver::lvglFlushCallback(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
    // Get TFT instance - using a static approach since we can't easily pass instance
    static TFT_eSPI* tft_ptr = nullptr;
    if (!tft_ptr) {
        tft_ptr = new TFT_eSPI();
        tft_ptr->init();
    }
    
    int32_t w = (area->x2 - area->x1 + 1);
    int32_t h = (area->y2 - area->y1 + 1);
    
    tft_ptr->startWrite();
    tft_ptr->setAddrWindow(area->x1, area->y1, w, h);
    tft_ptr->pushColors((uint16_t*)&color_p->full, w * h, true);
    tft_ptr->endWrite();
    
    lv_disp_flush_ready(disp_drv);
}

bool DisplayDriver::lvglTouchRead(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
    if (ts.touched()) {
        TS_Point p = ts.getPoint();
        data->state = LV_INDEV_STATE_PRESSED;
        data->point.x = p.x;
        data->point.y = p.y;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
    return false; // No buffering needed
}
