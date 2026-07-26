#include "screen_home_v2.h"
#include "ui/ui_manager.h"
#include <Arduino.h>

extern UIManager* g_uiManager;

ScreenHomeV2::ScreenHomeV2()
    : extruderTemp(21.0)
    , bedTemp(25.0)
    , piTemp(44.0) {
    
    setScreenName("Home");
}

void ScreenHomeV2::load() {
    Serial.println(F("[ScreenHomeV2] Loading..."));
    
    // Create main screen object
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    
    // Use flex layout for left/right panels
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createLeftPanel();
    createRightPanel();
    
    Serial.println(F("[ScreenHomeV2] Loaded successfully"));
}

void ScreenHomeV2::createLeftPanel() {
    // Left panel takes 60% of width
    leftPanel = lv_obj_create(screen);
    lv_obj_set_size(leftPanel, 145, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(leftPanel, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(leftPanel, 0, LV_PART_MAIN);
    lv_obj_set_flex_grow(leftPanel, 0);
    
    // Vertical flex layout
    lv_obj_set_flex_flow(leftPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(leftPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createTemperatureList();
    createTemperatureGraph();
}

void ScreenHomeV2::createTemperatureList() {
    // Container for temperature list
    lv_obj_t* tempList = lv_obj_create(leftPanel);
    lv_obj_set_size(tempList, 145, 80);
    lv_obj_set_style_bg_opa(tempList, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(tempList, 5, LV_PART_MAIN);
    lv_obj_set_style_pad_right(tempList, 0, LV_PART_MAIN);
    
    // Title "Temp (°C)"
    lv_obj_t* titleLabel = lv_label_create(tempList);
    lv_label_set_text(titleLabel, "Temp (°C)");
    lv_obj_center(titleLabel);
    lv_obj_set_y(titleLabel, 0);
    applyLabelStyle(titleLabel, lv_color_hex(0xE0E0E0), 16);
    
    // Temperature rows container
    lv_obj_t* tempRows = lv_obj_create(tempList);
    lv_obj_set_size(tempRows, 145, 60);
    lv_obj_set_pos(tempRows, 0, 20);
    lv_obj_set_style_bg_opa(tempRows, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(tempRows, 0, LV_PART_MAIN);
    
    // Row 1: Extruder (orange indicator)
    lv_obj_t* row1 = lv_obj_create(tempRows);
    lv_obj_set_size(row1, 145, 18);
    lv_obj_set_y(row1, 0);
    lv_obj_set_style_bg_opa(row1, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row1, 0, LV_PART_MAIN);
    
    // Orange indicator bar
    lv_obj_t* bar1 = lv_obj_create(row1);
    lv_obj_set_size(bar1, 4, 16);
    lv_obj_set_pos(bar1, 0, 1);
    lv_obj_set_style_bg_color(bar1, lv_color_hex(0xFF6B00), LV_PART_MAIN);
    lv_obj_set_style_radius(bar1, 2, LV_PART_MAIN);
    
    // Extruder label with icon
    lv_obj_t* extruderLabel = lv_label_create(row1);
    lv_label_set_text(extruderLabel, "Extruder");
    lv_obj_set_pos(extruderLabel, 12, 2);
    applyLabelStyle(extruderLabel, lv_color_hex(0xE0E0E0), 14);
    
    // Temperature value
    extruderTempLabel = lv_label_create(row1);
    lv_label_set_text_fmt(extruderTempLabel, "%d", (int)extruderTemp);
    lv_obj_align(extruderTempLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    applyLabelStyle(extruderTempLabel, lv_color_hex(0xFFFFFF), 16);
    
    // Row 2: Heater bed (teal indicator)
    lv_obj_t* row2 = lv_obj_create(tempRows);
    lv_obj_set_size(row2, 145, 18);
    lv_obj_set_y(row2, 19);
    lv_obj_set_style_bg_opa(row2, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row2, 0, LV_PART_MAIN);
    
    // Teal indicator bar
    lv_obj_t* bar2 = lv_obj_create(row2);
    lv_obj_set_size(bar2, 4, 16);
    lv_obj_set_pos(bar2, 0, 1);
    lv_obj_set_style_bg_color(bar2, lv_color_hex(0x20B2AA), LV_PART_MAIN);
    lv_obj_set_style_radius(bar2, 2, LV_PART_MAIN);
    
    // Bed label with icon
    lv_obj_t* bedLabel = lv_label_create(row2);
    lv_label_set_text(bedLabel, "Heater bed");
    lv_obj_set_pos(bedLabel, 12, 2);
    applyLabelStyle(bedLabel, lv_color_hex(0xE0E0E0), 14);
    
    // Temperature value
    bedTempLabel = lv_label_create(row2);
    lv_label_set_text_fmt(bedTempLabel, "%d", (int)bedTemp);
    lv_obj_align(bedTempLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    applyLabelStyle(bedTempLabel, lv_color_hex(0xFFFFFF), 16);
    
    // Row 3: Pi (magenta indicator)
    lv_obj_t* row3 = lv_obj_create(tempRows);
    lv_obj_set_size(row3, 145, 18);
    lv_obj_set_y(row3, 38);
    lv_obj_set_style_bg_opa(row3, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row3, 0, LV_PART_MAIN);
    
    // Magenta indicator bar
    lv_obj_t* bar3 = lv_obj_create(row3);
    lv_obj_set_size(bar3, 4, 16);
    lv_obj_set_pos(bar3, 0, 1);
    lv_obj_set_style_bg_color(bar3, lv_color_hex(0xE91E63), LV_PART_MAIN);
    lv_obj_set_style_radius(bar3, 2, LV_PART_MAIN);
    
    // Pi label with icon
    lv_obj_t* piLabel = lv_label_create(row3);
    lv_label_set_text(piLabel, "Pi");
    lv_obj_set_pos(piLabel, 12, 2);
    applyLabelStyle(piLabel, lv_color_hex(0xE0E0E0), 14);
    
    // Temperature value
    piTempLabel = lv_label_create(row3);
    lv_label_set_text_fmt(piTempLabel, "%d", (int)piTemp);
    lv_obj_align(piTempLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    applyLabelStyle(piTempLabel, lv_color_hex(0xFFFFFF), 16);
}

void ScreenHomeV2::createTemperatureGraph() {
    // Graph container
    lv_obj_t* graphContainer = lv_obj_create(leftPanel);
    lv_obj_set_size(graphContainer, 145, 140);
    lv_obj_set_style_bg_color(graphContainer, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(graphContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(graphContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(graphContainer, 5, LV_PART_MAIN);
    
    // Grid lines background
    lv_obj_t* gridBg = lv_obj_create(graphContainer);
    lv_obj_set_size(gridBg, 130, 100);
    lv_obj_set_pos(gridBg, 5, 5);
    lv_obj_set_style_bg_color(gridBg, lv_color_hex(0x0D0D0D), LV_PART_MAIN);
    lv_obj_set_style_border_color(gridBg, lv_color_hex(0x2A2A2A), LV_PART_MAIN);
    lv_obj_set_style_border_width(gridBg, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(gridBg, 0, LV_PART_MAIN);
    
    // Y-axis labels
    for (int i = 0; i <= 5; i++) {
        lv_obj_t* yLabel = lv_label_create(gridBg);
        lv_label_set_text_fmt(yLabel, "%d", (5 - i) * 10);
        lv_obj_set_y(yLabel, i * 20 - 6);
        lv_obj_set_x(yLabel, 0);
        applyLabelStyle(yLabel, lv_color_hex(0x888888), 10);
    }
    
    // Time axis labels
    lv_obj_t* time1 = lv_label_create(graphContainer);
    lv_label_set_text(time1, "21:54");
    lv_obj_set_pos(time1, 5, 108);
    applyLabelStyle(time1, lv_color_hex(0x888888), 9);
    
    lv_obj_t* time2 = lv_label_create(graphContainer);
    lv_label_set_text(time2, "22:10");
    lv_obj_align(time2, LV_ALIGN_BOTTOM_RIGHT, -5, 0);
    applyLabelStyle(time2, lv_color_hex(0x888888), 9);
}

void ScreenHomeV2::createRightPanel() {
    // Right panel takes 40% of width
    rightPanel = lv_obj_create(screen);
    lv_obj_set_size(rightPanel, 95, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(rightPanel, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(rightPanel, 5, LV_PART_MAIN);
    lv_obj_set_style_pad_left(rightPanel, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(rightPanel, 0);
    
    // Vertical layout for buttons
    lv_obj_set_flex_flow(rightPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(rightPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, 
                         LV_FLEX_ALIGN_START);
    
    createMenuButtons();
}

void ScreenHomeV2::createMenuButtons() {
    // Button 1: Homing (orange)
    createIconWithBar(rightPanel, "H", "Homing", 0, 0, lv_color_hex(0xFF6B00));
    
    // Spacer
    lv_obj_t* spacer1 = lv_obj_create(rightPanel);
    lv_obj_set_size(spacer1, 85, 8);
    lv_obj_set_style_bg_opa(spacer1, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Button 2: Temperature (pink)
    createIconWithBar(rightPanel, "T", "Temperature", 0, 0, lv_color_hex(0xE91E63));
    
    // Spacer
    lv_obj_t* spacer2 = lv_obj_create(rightPanel);
    lv_obj_set_size(spacer2, 85, 8);
    lv_obj_set_style_bg_opa(spacer2, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Button 3: Actions (teal)
    createIconWithBar(rightPanel, "A", "Actions", 0, 0, lv_color_hex(0x20B2AA));
    
    // Spacer
    lv_obj_t* spacer3 = lv_obj_create(rightPanel);
    lv_obj_set_size(spacer3, 85, 8);
    lv_obj_set_style_bg_opa(spacer3, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Button 4: Configuration (green)
    createIconWithBar(rightPanel, "C", "Configuration", 0, 0, lv_color_hex(0x76FF03));
    
    // Spacer
    lv_obj_t* spacer4 = lv_obj_create(rightPanel);
    lv_obj_set_size(spacer4, 85, 8);
    lv_obj_set_style_bg_opa(spacer4, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Button 5: Print (orange)
    createIconWithBar(rightPanel, "P", "Print", 0, 0, lv_color_hex(0xFF6B00));
}

void ScreenHomeV2::createIconWithBar(lv_obj_t* parent, const char* icon, const char* label,
                                       lv_coord_t x, lv_coord_t y, lv_color_t barColor) {
    // Main button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 85, 55);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    
    // Icon placeholder (large character until we have SVG support)
    lv_obj_t* iconLabel = lv_label_create(btn);
    lv_label_set_text(iconLabel, icon);
    lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 2);
    lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(iconLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Label
    lv_obj_t* textLabel = lv_label_create(btn);
    lv_label_set_text(textLabel, label);
    lv_obj_align(textLabel, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Colored bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 75, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -8);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback
    lv_obj_add_event_cb(btn, onButtonClick, LV_EVENT_CLICKED, (void*)label);
}

void ScreenHomeV2::onButtonClick(lv_event_t* e) {
    const char* btnName = (const char*)lv_event_get_user_data(e);
    Serial.printf("[ScreenHomeV2] Button clicked: %s\n", btnName);
    
    if (strcmp(btnName, "Homing") == 0) {
        Serial.println(F("[ScreenHomeV2] Navigate to Homing screen"));
        // TODO: Navigate to homing screen
    } else if (strcmp(btnName, "Temperature") == 0) {
        Serial.println(F("[ScreenHomeV2] Navigate to Temperature screen"));
        if (g_uiManager) {
            g_uiManager->navigateTo(SCREEN_TEMPERATURE);
        }
    } else if (strcmp(btnName, "Actions") == 0) {
        Serial.println(F("[ScreenHomeV2] Navigate to Actions screen"));
        if (g_uiManager) {
            g_uiManager->navigateTo(SCREEN_ACTIONS);
        }
    } else if (strcmp(btnName, "Configuration") == 0) {
        Serial.println(F("[ScreenHomeV2] Navigate to Configuration screen"));
        // TODO: Navigate to configuration screen
    } else if (strcmp(btnName, "Print") == 0) {
        Serial.println(F("[ScreenHomeV2] Navigate to Print screen"));
        // TODO: Navigate to print/files screen
    }
}

void ScreenHomeV2::onHomingClick(lv_event_t* e) {
    Serial.println(F("[ScreenHomeV2] Homing clicked"));
}

void ScreenHomeV2::onTemperatureClick(lv_event_t* e) {
    Serial.println(F("[ScreenHomeV2] Temperature clicked"));
}

void ScreenHomeV2::onActionsClick(lv_event_t* e) {
    Serial.println(F("[ScreenHomeV2] Actions clicked"));
}

void ScreenHomeV2::onConfigurationClick(lv_event_t* e) {
    Serial.println(F("[ScreenHomeV2] Configuration clicked"));
}
