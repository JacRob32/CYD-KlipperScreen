#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

#include <Arduino.h>
#include <lvgl.h>
#include "../../config.h"

class ScreenBase {
public:
    ScreenBase();
    virtual ~ScreenBase();
    
    virtual void load() = 0;
    virtual void update() {}
    virtual void onActivate() {}
    virtual void onDeactivate() {}
    
    lv_obj_t* getScreen() const { return screen; }
    void setScreenName(const char* name) { screenName = name; }
    const char* getScreenName() const { return screenName; }
    
protected:
    lv_obj_t* screen;
    const char* screenName;
    
    // Helper methods for common LVGL operations
    lv_obj_t* createLabel(lv_obj_t* parent, const char* text, lv_coord_t x = 0, lv_coord_t y = 0);
    lv_obj_t* createButton(lv_obj_t* parent, const char* text, lv_coord_t width, lv_coord_t height, lv_coord_t x = 0, lv_coord_t y = 0);
    lv_obj_t* createContainer(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, lv_coord_t x = 0, lv_coord_t y = 0);
    
    void applyButtonStyle(lv_obj_t* btn, lv_color_t bgColor);
    void applyLabelStyle(lv_obj_t* label, lv_color_t color, uint8_t fontSize = 16);
    void applyContainerStyle(lv_obj_t* container, lv_color_t bgColor, uint8_t radius = 8);
    
    // Grid layout helpers
    void setupGridLayout(lv_obj_t* grid, lv_coord_t columnWidth, lv_coord_t rowHeight, 
                        lv_coord_t colGap = 5, lv_coord_t rowGap = 5);
};

#endif // SCREEN_BASE_H
