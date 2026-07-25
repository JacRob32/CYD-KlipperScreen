#include "ui/screens/screen_temperature.h"
#include "ui/ui_manager.h"
#include <Arduino.h>

extern UIManager* g_uiManager;

ScreenTemperature::ScreenTemperature()
    : extruderTemp(20.4)
    , bedTemp(19.6)
    , mcuTemp(19.6)
    , piTemp(33.8) {
    
    setScreenName("Temperature");
}

void ScreenTemperature::load() {
    Serial.println(F("[ScreenTemperature] Loading..."));
    
    // Create main screen object
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    
    // Use horizontal flex layout for left/right panels
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(screen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createLayout();
    
    Serial.println(F("[ScreenTemperature] Loaded successfully"));
}

void ScreenTemperature::createLayout() {
    createStatusbar();
    createLeftPanel();
    createRightPanel();
}

void ScreenTemperature::createStatusbar() {
    // Statusbar container (spans full width at top)
    statusbar = lv_obj_create(nullptr);
    lv_obj_set_size(statusbar, DISPLAY_WIDTH, 30);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(statusbar, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(statusbar, 0);
    lv_obj_set_style_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_parent(statusbar, screen);
    
    // Back button (chevron)
    lv_obj_t* btnBack = lv_btn_create(statusbar);
    lv_obj_set_size(btnBack, 24, 20);
    lv_obj_align(btnBack, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_opa(btnBack, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btnBack, 0, LV_PART_MAIN);
    lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
        Serial.println(F("[ScreenTemperature] Back button clicked"));
        if (g_uiManager) {
            g_uiManager->goBack();
        }
    }, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* backIcon = lv_label_create(btnBack);
    lv_label_set_text(backIcon, "◂");
    lv_obj_center(backIcon);
    lv_obj_set_style_text_font(backIcon, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(backIcon, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Temperature indicators (left side)
    lv_obj_t* tempContainer = lv_obj_create(statusbar);
    lv_obj_set_size(tempContainer, 100, 20);
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
    lv_obj_set_style_text_color(icon1, lv_color_hex(0xFF6B00), LV_PART_MAIN);
    
    lblExtruderTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblExtruderTemp, "%d°", (int)extruderTemp);
    lv_obj_set_style_text_font(lblExtruderTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblExtruderTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Spacer
    lv_obj_t* spacer1 = lv_obj_create(tempContainer);
    lv_obj_set_size(spacer1, 6, 1);
    lv_obj_set_style_bg_opa(spacer1, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Bed temp
    lv_obj_t* icon2 = lv_label_create(tempContainer);
    lv_label_set_text(icon2, "🔥");
    lv_obj_set_style_text_font(icon2, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon2, lv_color_hex(0x20B2AA), LV_PART_MAIN);
    
    lblBedTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblBedTemp, "%d°", (int)bedTemp);
    lv_obj_set_style_text_font(lblBedTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblBedTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Spacer
    lv_obj_t* spacer2 = lv_obj_create(tempContainer);
    lv_obj_set_size(spacer2, 6, 1);
    lv_obj_set_style_bg_opa(spacer2, LV_OPA_TRANSP, LV_PART_MAIN);
    
    // Mcu temp
    lv_obj_t* lblMcu = lv_label_create(tempContainer);
    lv_label_set_text(lblMcu, "M:");
    lv_obj_set_style_text_font(lblMcu, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblMcu, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    lblMcuTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblMcuTemp, "%d°", (int)mcuTemp);
    lv_obj_set_style_text_font(lblMcuTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblMcuTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Title (center)
    lblTitle = lv_label_create(statusbar);
    lv_label_set_text(lblTitle, "Mini | Temperature");
    lv_obj_align(lblTitle, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(lblTitle, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Time (right)
    lblTime = lv_label_create(statusbar);
    lv_label_set_text(lblTime, "20:34");
    lv_obj_align(lblTime, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(lblTime, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

void ScreenTemperature::createLeftPanel() {
    // Left panel takes 55% of width
    leftPanel = lv_obj_create(screen);
    lv_obj_set_size(leftPanel, 135, DISPLAY_HEIGHT - 30);
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

void ScreenTemperature::createTemperatureList() {
    // Container for temperature list
    tempList = lv_obj_create(leftPanel);
    lv_obj_set_size(tempList, 135, 100);
    lv_obj_set_style_bg_opa(tempList, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(tempList, 5, LV_PART_MAIN);
    
    // Title "Temp (°C)"
    lv_obj_t* titleLabel = lv_label_create(tempList);
    lv_label_set_text(titleLabel, "Temp (°C)");
    lv_obj_center(titleLabel);
    lv_obj_set_y(titleLabel, 0);
    applyLabelStyle(titleLabel, lv_color_hex(0xE0E0E0), 16);
    
    // Create temperature rows
    createTempRow(tempList, "🌡", "Extruder", extruderTemp, lv_color_hex(0xD32F2F), 20);
    createTempRow(tempList, "🔥", "Heater bed", bedTemp, lv_color_hex(0x20B2AA), 40);
    createTempRow(tempList, "🌡", "Mcu", mcuTemp, lv_color_hex(0xFF6B00), 60);
    createTempRow(tempList, "🌡", "Raspberry...", piTemp, lv_color_hex(0x1976D2), 80);
}

void ScreenTemperature::createTempRow(lv_obj_t* parent, const char* icon, const char* label, 
                                       float temp, lv_color_t barColor, lv_coord_t y) {
    // Row container
    lv_obj_t* row = lv_obj_create(parent);
    lv_obj_set_size(row, 125, 18);
    lv_obj_set_pos(row, 5, y);
    lv_obj_set_style_bg_opa(row, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(row, 0, LV_PART_MAIN);
    
    // Colored indicator bar (left side)
    lv_obj_t* bar = lv_obj_create(row);
    lv_obj_set_size(bar, 4, 16);
    lv_obj_set_pos(bar, 0, 1);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    
    // Icon
    lv_obj_t* iconLabel = lv_label_create(row);
    lv_label_set_text(iconLabel, icon);
    lv_obj_set_pos(iconLabel, 8, 2);
    lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(iconLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Temperature source name
    lv_obj_t* nameLabel = lv_label_create(row);
    lv_label_set_text(nameLabel, label);
    lv_obj_set_pos(nameLabel, 22, 2);
    applyLabelStyle(nameLabel, lv_color_hex(0xE0E0E0), 14);
    
    // Temperature value (right aligned)
    lv_obj_t* tempLabel = lv_label_create(row);
    lv_label_set_text_fmt(tempLabel, "%.1f", temp);
    lv_obj_align(tempLabel, LV_ALIGN_RIGHT_MID, -5, 0);
    applyLabelStyle(tempLabel, lv_color_hex(0xFFFFFF), 16);
}

void ScreenTemperature::createTemperatureGraph() {
    // Graph container
    graphContainer = lv_obj_create(leftPanel);
    lv_obj_set_size(graphContainer, 135, 130);
    lv_obj_set_style_bg_color(graphContainer, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(graphContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(graphContainer, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(graphContainer, 5, LV_PART_MAIN);
    
    // Grid lines background
    lv_obj_t* gridBg = lv_obj_create(graphContainer);
    lv_obj_set_size(gridBg, 120, 90);
    lv_obj_set_pos(gridBg, 5, 5);
    lv_obj_set_style_bg_color(gridBg, lv_color_hex(0x0D0D0D), LV_PART_MAIN);
    lv_obj_set_style_border_color(gridBg, lv_color_hex(0x2A2A2A), LV_PART_MAIN);
    lv_obj_set_style_border_width(gridBg, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(gridBg, 0, LV_PART_MAIN);
    
    // Y-axis labels
    for (int i = 0; i <= 4; i++) {
        lv_obj_t* yLabel = lv_label_create(gridBg);
        lv_label_set_text_fmt(yLabel, "%d", (4 - i) * 10);
        lv_obj_set_y(yLabel, i * 22 - 6);
        lv_obj_set_x(yLabel, 0);
        applyLabelStyle(yLabel, lv_color_hex(0x888888), 10);
    }
    
    // Time axis label
    lv_obj_t* timeLabel = lv_label_create(graphContainer);
    lv_label_set_text(timeLabel, "20:34");
    lv_obj_align(timeLabel, LV_ALIGN_BOTTOM_MID, 0, 0);
    applyLabelStyle(timeLabel, lv_color_hex(0x888888), 9);
}

void ScreenTemperature::createRightPanel() {
    // Right panel takes 45% of width
    rightPanel = lv_obj_create(screen);
    lv_obj_set_size(rightPanel, 105, DISPLAY_HEIGHT - 30);
    lv_obj_set_style_bg_color(rightPanel, lv_color_hex(0x121212), LV_PART_MAIN);
    lv_obj_set_style_pad_all(rightPanel, 5, LV_PART_MAIN);
    lv_obj_set_flex_grow(rightPanel, 0);
    
    // Vertical flex layout
    lv_obj_set_flex_flow(rightPanel, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(rightPanel, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_STRETCH, 
                         LV_FLEX_ALIGN_START);
    
    createPresetGrid();
}

void ScreenTemperature::createPresetGrid() {
    // Preset buttons grid container
    presetGrid = lv_obj_create(rightPanel);
    lv_obj_set_size(presetGrid, 95, DISPLAY_HEIGHT - 40);
    lv_obj_set_style_bg_opa(presetGrid, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(presetGrid, 0, LV_PART_MAIN);
    
    // Use grid layout: 2 columns x 4 rows
    lv_obj_set_layout(presetGrid, LV_LAYOUT_GRID);
    static lv_coord_t col_dsc[] = {45, 45, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {50, 50, 50, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(presetGrid, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_gap(presetGrid, 5, 0);
    lv_obj_set_style_grid_row_gap(presetGrid, 5, 0);
    lv_obj_set_grid_column_align(presetGrid, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_row_align(presetGrid, LV_GRID_ALIGN_CENTER);
    
    // Row 1: Cool + Settings icon
    createPresetButton(presetGrid, "❄", "Enfriar", lv_color_hex(0x20B2AA), 0, 0);
    createPresetButton(presetGrid, "", "", lv_color_hex(0xAEEA00), 1, 0);
    
    // Row 2: PLA + PETG
    createPresetButton(presetGrid, "", "PLA", lv_color_hex(0xFF6B00), 0, 1);
    createPresetButton(presetGrid, "", "PETG", lv_color_hex(0xE91E63), 1, 1);
    
    // Row 3: EPET + MAX
    createPresetButton(presetGrid, "", "EPET", lv_color_hex(0xAEEA00), 0, 2);
    createPresetButton(presetGrid, "", "MAX", lv_color_hex(0x20B2AA), 1, 2);
    
    // Row 4: Empty (optional)
}

void ScreenTemperature::createPresetButton(lv_obj_t* parent, const char* icon, const char* label,
                                             lv_color_t barColor, lv_coord_t col, lv_coord_t row) {
    // Button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 45, 45);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(btn, 8, LV_PART_MAIN);
    
    // Icon (if present)
    if (strlen(icon) > 0) {
        lv_obj_t* iconLabel = lv_label_create(btn);
        lv_label_set_text(iconLabel, icon);
        lv_obj_align(iconLabel, LV_ALIGN_TOP_MID, 0, 5);
        lv_obj_set_style_text_font(iconLabel, &lv_font_montserrat_20, LV_PART_MAIN);
        lv_obj_set_style_text_color(iconLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    }
    
    // Label (if present)
    if (strlen(label) > 0) {
        lv_obj_t* textLabel = lv_label_create(btn);
        lv_label_set_text(textLabel, label);
        lv_obj_align(textLabel, LV_ALIGN_CENTER, 0, strlen(icon) > 0 ? 8 : 0);
        lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_12, LV_PART_MAIN);
        lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    }
    
    // Colored bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 40, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback
    lv_obj_add_event_cb(btn, onPresetClick, LV_EVENT_CLICKED, (void*)label);
}

void ScreenTemperature::onCoolClick(lv_event_t* e) {
    Serial.println(F("[ScreenTemperature] Cool/Enfriar clicked"));
    // TODO: Send M104/M140 S0 (turn off heaters)
}

void ScreenTemperature::onPresetClick(lv_event_t* e) {
    const char* preset = (const char*)lv_event_get_user_data(e);
    Serial.printf("[ScreenTemperature] Preset clicked: %s\n", preset);
    
    if (strlen(preset) == 0) {
        // Icon-only button
        Serial.println(F("[ScreenTemperature] Settings icon clicked"));
    } else if (strcmp(preset, "Enfriar") == 0) {
        Serial.println(F("[ScreenTemperature] Cool/Enfriar clicked"));
        // TODO: Turn off all heaters
    } else if (strcmp(preset, "PLA") == 0) {
        Serial.println(F("[ScreenTemperature] PLA preset clicked"));
        // TODO: Set PLA temperatures (200°C extruder, 60°C bed)
    } else if (strcmp(preset, "PETG") == 0) {
        Serial.println(F("[ScreenTemperature] PETG preset clicked"));
        // TODO: Set PETG temperatures (240°C extruder, 80°C bed)
    } else if (strcmp(preset, "EPET") == 0) {
        Serial.println(F("[ScreenTemperature] EPET preset clicked"));
    } else if (strcmp(preset, "MAX") == 0) {
        Serial.println(F("[ScreenTemperature] MAX preset clicked"));
        // TODO: Set maximum temperatures
    }
}
