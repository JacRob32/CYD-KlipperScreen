#include "ui/screens/screen_extrude.h"
#include "ui/ui_manager.h"
#include <Arduino.h>

extern UIManager* g_uiManager;

ScreenExtrude::ScreenExtrude()
    : extruderTemp(25.0)
    , bedTemp(26.0)
    , selectedExtruder(1)
    , selectedDistance(0)
    , selectedSpeed(1) {
    
    setScreenName("Extrude");
}

void ScreenExtrude::load() {
    Serial.println(F("[ScreenExtrude] Loading..."));
    
    // Create main screen object
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    
    // Use vertical flex layout
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createLayout();
    
    Serial.println(F("[ScreenExtrude] Loaded successfully"));
}

void ScreenExtrude::createLayout() {
    createStatusbar();
    createExtruderRow();
    createActionRow();
    
    // Distance row
    const char* distOptions[] = {"5", "10", "15", "25"};
    createToggleRow(screen, "Distance (mm)", distOptions, 4, selectedDistance, lv_color_hex(0xFF6B00));
    
    // Speed row
    const char* speedOptions[] = {"1", "2", "5", "25"};
    createToggleRow(screen, "Speed (mm/s)", speedOptions, 4, selectedSpeed, lv_color_hex(0xFF6B00));
}

void ScreenExtrude::createStatusbar() {
    // Statusbar container
    statusbar = lv_obj_create(screen);
    lv_obj_set_size(statusbar, DISPLAY_WIDTH, 30);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(statusbar, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(statusbar, 0);
    
    // Back button (curved arrow)
    lv_obj_t* btnBack = lv_btn_create(statusbar);
    lv_obj_set_size(btnBack, 24, 20);
    lv_obj_align(btnBack, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_opa(btnBack, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btnBack, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
        Serial.println(F("[ScreenExtrude] Back button clicked"));
        if (g_uiManager) {
            g_uiManager->goBack();
        }
    }, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* backIcon = lv_label_create(btnBack);
    lv_label_set_text(backIcon, "↩");
    lv_obj_center(backIcon);
    lv_obj_set_style_text_font(backIcon, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(backIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Temperature indicators (left side)
    lv_obj_t* tempContainer = lv_obj_create(statusbar);
    lv_obj_set_size(tempContainer, 80, 20);
    lv_obj_set_pos(tempContainer, 35, 5);
    lv_obj_set_style_bg_opa(tempContainer, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(tempContainer, 0, LV_PART_MAIN);
    lv_obj_set_flex_flow(tempContainer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tempContainer, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 
                         LV_FLEX_ALIGN_START);
    
    // Extruder temp
    lv_obj_t* icon1 = lv_label_create(tempContainer);
    lv_label_set_text(icon1, "🌡");
    lv_obj_set_style_text_font(icon1, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon1, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    lblExtruderTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblExtruderTemp, "%d°", (int)extruderTemp);
    lv_obj_set_style_text_font(lblExtruderTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblExtruderTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Bed temp
    lv_obj_t* spacer = lv_obj_create(tempContainer);
    lv_obj_set_size(spacer, 8, 1);
    lv_obj_set_style_bg_opa(spacer, LV_OPA_TRANSP, LV_PART_MAIN);
    
    lv_obj_t* icon2 = lv_label_create(tempContainer);
    lv_label_set_text(icon2, "🔥");
    lv_obj_set_style_text_font(icon2, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon2, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    lblBedTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblBedTemp, "%d°", (int)bedTemp);
    lv_obj_set_style_text_font(lblBedTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblBedTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Title (center)
    lblTitle = lv_label_create(statusbar);
    lv_label_set_text(lblTitle, "Printer | Extrude");
    lv_obj_align(lblTitle, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(lblTitle, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Time (right)
    lblTime = lv_label_create(statusbar);
    lv_label_set_text(lblTime, "09:12");
    lv_obj_align(lblTime, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(lblTime, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

void ScreenExtrude::createExtruderRow() {
    // Extruder selection row container
    extruderRow = lv_obj_create(screen);
    lv_obj_set_size(extruderRow, DISPLAY_WIDTH, 80);
    lv_obj_set_style_bg_opa(extruderRow, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(extruderRow, 5, LV_PART_MAIN);
    lv_obj_set_flex_grow(extruderRow, 0);
    
    // Horizontal flex layout
    lv_obj_set_flex_flow(extruderRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(extruderRow, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_CENTER);
    
    // Create extruder buttons
    createExtruderButton(extruderRow, "0", "24.7°C", 0, false);
    createExtruderButton(extruderRow, "1", "24.7°C", 1, true);  // Selected
    createExtruderButton(extruderRow, "2", "24.7°C", 2, false);
    createTempButton(extruderRow);
}

void ScreenExtrude::createExtruderButton(lv_obj_t* parent, const char* label, 
                                           const char* temp, lv_coord_t col, bool selected) {
    // Button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 55, 70);
    lv_obj_set_flex_grow(btn, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    
    if (selected) {
        // Pink dashed border for selected extruder
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(btn, lv_color_hex(0xE91E63), LV_PART_MAIN);
        lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_FULL, LV_PART_MAIN);
    } else {
        lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    }
    
    // White square with number
    lv_obj_t* square = lv_obj_create(btn);
    lv_obj_set_size(square, 30, 30);
    lv_obj_align(square, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_bg_color(square, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_radius(square, 4, LV_PART_MAIN);
    
    lv_obj_t* numLabel = lv_label_create(square);
    lv_label_set_text(numLabel, label);
    lv_obj_center(numLabel);
    lv_obj_set_style_text_font(numLabel, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(numLabel, lv_color_hex(0x000000), LV_PART_MAIN);
    
    // Temperature label
    lv_obj_t* tempLabel = lv_label_create(btn);
    lv_label_set_text(tempLabel, temp);
    lv_obj_align(tempLabel, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_text_font(tempLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(tempLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Event callback
    lv_obj_add_event_cb(btn, onExtruderClick, LV_EVENT_CLICKED, (void*)label);
}

void ScreenExtrude::createTempButton(lv_obj_t* parent) {
    // Temperature button (right side)
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 55, 70);
    lv_obj_set_flex_grow(btn, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    
    // Thermometer icon
    lv_obj_t* icon = lv_label_create(btn);
    lv_label_set_text(icon, "🌡");
    lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Label
    lv_obj_t* textLabel = lv_label_create(btn);
    lv_label_set_text(textLabel, "Temperature");
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, -5);
    lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Blue bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 50, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x20B2AA), LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback
    lv_obj_add_event_cb(btn, [](lv_event_t* e) {
        Serial.println(F("[ScreenExtrude] Temperature button clicked"));
    }, LV_EVENT_CLICKED, NULL);
}

void ScreenExtrude::createActionRow() {
    // Action buttons row container
    actionRow = lv_obj_create(screen);
    lv_obj_set_size(actionRow, DISPLAY_WIDTH, 90);
    lv_obj_set_style_bg_opa(actionRow, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(actionRow, 5, LV_PART_MAIN);
    lv_obj_set_flex_grow(actionRow, 0);
    
    // Horizontal flex layout
    lv_obj_set_flex_flow(actionRow, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(actionRow, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_CENTER);
    
    // Create action buttons
    createActionButton(actionRow, "▼", "Extrude", lv_color_hex(0x20B2AA), 0);
    createActionButton(actionRow, "↓", "Load", lv_color_hex(0xAEEA00), 1);
    createActionButton(actionRow, "↑", "Unload", lv_color_hex(0xE91E63), 2);
    createActionButton(actionRow, "", "Retract", lv_color_hex(0xFF6B00), 3);
}

void ScreenExtrude::createActionButton(lv_obj_t* parent, const char* icon, const char* label,
                                         lv_color_t barColor, lv_coord_t col) {
    // Button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 55, 80);
    lv_obj_set_flex_grow(btn, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
    
    // Icon
    lv_obj_t* iconLabel = lv_label_create(btn);
    lv_label_set_text(iconLabel, icon);
    lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 15);
    lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(iconLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Label
    lv_obj_t* textLabel = lv_label_create(btn);
    lv_label_set_text(textLabel, label);
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, 5);
    lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Colored bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 50, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback based on label
    if (strcmp(label, "Extrude") == 0) {
        lv_obj_add_event_cb(btn, onExtrudeClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Load") == 0) {
        lv_obj_add_event_cb(btn, onLoadClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Unload") == 0) {
        lv_obj_add_event_cb(btn, onUnloadClick, LV_EVENT_CLICKED, NULL);
    } else if (strcmp(label, "Retract") == 0) {
        lv_obj_add_event_cb(btn, onRetractClick, LV_EVENT_CLICKED, NULL);
    }
}

void ScreenExtrude::createToggleRow(lv_obj_t* parent, const char* title, const char** options,
                                      int count, int selected, lv_color_t selectedColor) {
    // Row container
    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_set_size(row, DISPLAY_WIDTH, 40);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(row, 5, LV_PART_MAIN);
    lv_obj_set_flex_grow(row, 0);
    
    // Title label
    lv_obj_t* titleLabel = lv_label_create(row);
    lv_label_set_text(titleLabel, title);
    lv_obj_set_pos(titleLabel, 10, -20);
    lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Horizontal flex layout for buttons
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_CENTER);
    
    // Create toggle buttons
    for (int i = 0; i < count; i++) {
        lv_obj_t* btn = lv_btn_create(row);
        lv_obj_set_size(btn, 45, 30);
        lv_obj_set_flex_grow(btn, 0);
        
        if (i == selected) {
            lv_obj_set_style_bg_color(btn, selectedColor, LV_PART_MAIN);
        } else {
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x2D2D2D), LV_PART_MAIN);
        }
        
        lv_obj_set_style_radius(btn, 4, LV_PART_MAIN);
        
        lv_obj_t* label = lv_label_create(btn);
        lv_label_set_text(label, options[i]);
        lv_obj_center(label);
        lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN);
        lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        
        // Event callback
        lv_obj_add_event_cb(btn, [](lv_event_t* e) {
            // Toggle button handling will be implemented when needed
            Serial.println(F("[ScreenExtrude] Toggle button clicked"));
        }, LV_EVENT_CLICKED, NULL);
    }
}

void ScreenExtrude::onExtruderClick(lv_event_t* e) {
    const char* extruder = (const char*)lv_event_get_user_data(e);
    Serial.printf("[ScreenExtrude] Extruder %s selected\n", extruder);
    // TODO: Send T{extruder} command
}

void ScreenExtrude::onExtrudeClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Extrude clicked"));
    // TODO: Send G1 E+5 F120 command
}

void ScreenExtrude::onLoadClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Load clicked"));
    // TODO: Send LOAD_FILAMENT command
}

void ScreenExtrude::onUnloadClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Unload clicked"));
    // TODO: Send UNLOAD_FILAMENT command
}

void ScreenExtrude::onRetractClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Retract clicked"));
    // TODO: Send G1 E-5 F120 command
}

void ScreenExtrude::onDistClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Distance button clicked"));
}

void ScreenExtrude::onSpeedClick(lv_event_t* e) {
    Serial.println(F("[ScreenExtrude] Speed button clicked"));
}
