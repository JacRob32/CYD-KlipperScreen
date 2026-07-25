#include "ui/screens/screen_base.h"

ScreenBase::ScreenBase() 
    : screen(nullptr)
    , screenName("Unnamed") {
}

ScreenBase::~ScreenBase() {
    if (screen) {
        lv_obj_del(screen);
        screen = nullptr;
    }
}

lv_obj_t* ScreenBase::createLabel(lv_obj_t* parent, const char* text, lv_coord_t x, lv_coord_t y) {
    lv_obj_t* label = lv_label_create(parent);
    lv_obj_set_style_text_color(label, lv_color_hex(COLOR_TEXT_LIGHT), LV_PART_MAIN);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_label_set_text(label, text);
    
    if (x != 0 || y != 0) {
        lv_obj_set_pos(label, x, y);
    } else {
        lv_obj_center(label);
    }
    
    return label;
}

lv_obj_t* ScreenBase::createButton(lv_obj_t* parent, const char* text, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, width, height);
    
    lv_obj_t* label = lv_label_create(btn);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_center(label);
    
    if (x != 0 || y != 0) {
        lv_obj_set_pos(btn, x, y);
    }
    
    return btn;
}

lv_obj_t* ScreenBase::createContainer(lv_obj_t* parent, lv_coord_t width, lv_coord_t height, lv_coord_t x, lv_coord_t y) {
    lv_obj_t* container = lv_obj_create(parent);
    lv_obj_set_size(container, width, height);
    lv_obj_set_style_bg_color(container, lv_color_hex(COLOR_BG_MEDIUM), LV_PART_MAIN);
    lv_obj_set_style_radius(container, 8, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(container, 10, LV_PART_MAIN);
    
    if (x != 0 || y != 0) {
        lv_obj_set_pos(container, x, y);
    }
    
    return container;
}

void ScreenBase::applyButtonStyle(lv_obj_t* btn, lv_color_t bgColor) {
    lv_obj_set_style_bg_color(btn, bgColor, LV_PART_MAIN);
    lv_obj_set_style_bg_color(btn, lv_color_darken(bgColor, LV_OPA_20), LV_PART_PRESSED);
    lv_obj_set_style_bg_color(btn, lv_color_lighten(bgColor, LV_OPA_10), LV_PART_CHECKED);
    lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
    lv_obj_set_style_text_color(btn, lv_color_hex(COLOR_TEXT_LIGHT), LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 3, LV_PART_MAIN);
    lv_obj_set_style_shadow_color(btn, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(btn, LV_OPA_30, LV_PART_MAIN);
}

void ScreenBase::applyLabelStyle(lv_obj_t* label, lv_color_t color, uint8_t fontSize) {
    lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
    
    if (fontSize <= 12) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_PART_MAIN);
    } else if (fontSize <= 14) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN);
    } else if (fontSize <= 16) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    } else if (fontSize <= 18) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_18, LV_PART_MAIN);
    } else if (fontSize <= 24) {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_24, LV_PART_MAIN);
    } else {
        lv_obj_set_style_text_font(label, &lv_font_montserrat_32, LV_PART_MAIN);
    }
}

void ScreenBase::applyContainerStyle(lv_obj_t* container, lv_color_t bgColor, uint8_t radius) {
    lv_obj_set_style_bg_color(container, bgColor, LV_PART_MAIN);
    lv_obj_set_style_radius(container, radius, LV_PART_MAIN);
    lv_obj_set_style_border_width(container, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(container, lv_color_hex(0x404040), LV_PART_MAIN);
    lv_obj_set_style_pad_all(container, 8, LV_PART_MAIN);
}

void ScreenBase::setupGridLayout(lv_obj_t* grid, lv_coord_t columnWidth, lv_coord_t rowHeight, 
                                 lv_coord_t colGap, lv_coord_t rowGap) {
    static lv_coord_t col_dsc[] = {columnWidth, columnWidth, columnWidth, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {rowHeight, rowHeight, rowHeight, rowHeight, LV_GRID_TEMPLATE_LAST};
    
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_set_grid_dsc_array(grid, col_dsc, row_dsc);
    lv_obj_set_grid_column_align(grid, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_row_align(grid, LV_GRID_ALIGN_CENTER);
    lv_obj_set_style_grid_column_gap(grid, colGap, 0);
    lv_obj_set_style_grid_row_gap(grid, rowGap, 0);
}
