#include "ui/screens/screen_home.h"
#include "ui/ui_manager.h"
#include <Arduino.h>

// Global UI manager reference for navigation (set in main.cpp)
extern UIManager* g_uiManager;

ScreenHome::ScreenHome()
    : isConnected(false)
    , bedTemp(0.0)
    , bedTarget(0.0)
    , extruderTemp(0.0)
    , extruderTarget(0.0) {
    
    setScreenName("Home");
}

void ScreenHome::load() {
    Serial.println(F("[ScreenHome] Loading..."));
    
    // Create main screen object
    screen = lv_obj_create(nullptr);
    lv_obj_set_size(screen, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_obj_set_style_bg_color(screen, lv_color_hex(COLOR_BG_DARK), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    
    createLayout();
    
    Serial.println(F("[ScreenHome] Loaded successfully"));
}

void ScreenHome::update() {
    // Placeholder - will be updated with real temperature data from Klipper
}

void ScreenHome::createLayout() {
    createStatusbar();
    createTemperatureDisplay();
    createMenuGrid();
}

void ScreenHome::createStatusbar() {
    // Statusbar container at top
    statusbar = lv_obj_create(screen);
    lv_obj_set_size(statusbar, DISPLAY_WIDTH, 30);
    lv_obj_align(statusbar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(COLOR_BG_MEDIUM), LV_PART_MAIN);
    lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_left(statusbar, 10, LV_PART_MAIN);
    lv_obj_set_style_pad_right(statusbar, 10, LV_PART_MAIN);
    
    // Connection status
    lv_obj_t* statusLabel = lv_label_create(statusbar);
    lv_label_set_text(statusLabel, "Disconnected");
    lv_obj_set_pos(statusLabel, 10, 7);
    applyLabelStyle(statusLabel, lv_color_hex(COLOR_ERROR), 14);
    
    // Time display
    lv_obj_t* timeLabel = lv_label_create(statusbar);
    lv_label_set_text(timeLabel, "--:--");
    lv_obj_align(timeLabel, LV_ALIGN_RIGHT_MID, -10, 0);
    applyLabelStyle(timeLabel, lv_color_hex(COLOR_TEXT_LIGHT), 14);
}

void ScreenHome::createTemperatureDisplay() {
    // Temperature display container
    tempContainer = createContainer(screen, DISPLAY_WIDTH - 20, 80, 10, 40);
    
    // Grid layout: 2 columns x 2 rows
    lv_obj_set_layout(tempContainer, LV_LAYOUT_GRID);
    static lv_coord_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {35, 35, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(tempContainer, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_gap(tempContainer, 10, 0);
    lv_obj_set_style_grid_row_gap(tempContainer, 5, 0);
    
    // Bed temperature row
    lv_obj_t* bedLabel = lv_label_create(tempContainer);
    lv_label_set_text(bedLabel, "Bed");
    lv_obj_set_grid_cell(bedLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    applyLabelStyle(bedLabel, lv_color_hex(COLOR_TEXT_LIGHT), 16);
    
    lv_obj_t* bedTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(bedTempLabel, "25/60°C");
    lv_obj_set_grid_cell(bedTempLabel, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    applyLabelStyle(bedTempLabel, lv_color_hex(COLOR_WARNING), 18);
    
    // Extruder temperature row
    lv_obj_t* extLabel = lv_label_create(tempContainer);
    lv_label_set_text(extLabel, "Extruder");
    lv_obj_set_grid_cell(extLabel, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    applyLabelStyle(extLabel, lv_color_hex(COLOR_TEXT_LIGHT), 16);
    
    lv_obj_t* extTempLabel = lv_label_create(tempContainer);
    lv_label_set_text(extTempLabel, "200/210°C");
    lv_obj_set_grid_cell(extTempLabel, LV_GRID_ALIGN_END, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    applyLabelStyle(extTempLabel, lv_color_hex(COLOR_WARNING), 18);
}

void ScreenHome::createMenuGrid() {
    // Menu buttons container
    menuButtons = lv_obj_create(screen);
    lv_obj_set_size(menuButtons, DISPLAY_WIDTH - 20, DISPLAY_HEIGHT - 150);
    lv_obj_align(menuButtons, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_set_style_bg_opa(menuButtons, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(menuButtons, 0, LV_PART_MAIN);
    
    // Grid layout: 2 columns x 3 rows
    lv_obj_set_layout(menuButtons, LV_LAYOUT_GRID);
    static lv_coord_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {50, 50, 50, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(menuButtons, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_gap(menuButtons, 10, 0);
    lv_obj_set_style_grid_row_gap(menuButtons, 15, 0);
    
    // Extrude button
    lv_obj_t* btnExtrude = createButton(menuButtons, "Extrude", 100, 50, 0, 0);
    lv_obj_set_grid_cell(btnExtrude, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    applyButtonStyle(btnExtrude, lv_color_hex(BTN_COLOR_4));
    lv_obj_add_event_cb(btnExtrude, onExtrudeClick, LV_EVENT_CLICKED, NULL);
    
    // Move button
    lv_obj_t* btnMove = createButton(menuButtons, "Move", 100, 50, 0, 0);
    lv_obj_set_grid_cell(btnMove, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 0, 1);
    applyButtonStyle(btnMove, lv_color_hex(BTN_COLOR_2));
    lv_obj_add_event_cb(btnMove, onMoveClick, LV_EVENT_CLICKED, NULL);
    
    // Temperature button
    lv_obj_t* btnTemp = createButton(menuButtons, "Temp", 100, 50, 0, 0);
    lv_obj_set_grid_cell(btnTemp, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    applyButtonStyle(btnTemp, lv_color_hex(BTN_COLOR_1));
    lv_obj_add_event_cb(btnTemp, onTempClick, LV_EVENT_CLICKED, NULL);
    
    // Settings button
    lv_obj_t* btnSettings = createButton(menuButtons, "Settings", 100, 50, 0, 0);
    lv_obj_set_grid_cell(btnSettings, LV_GRID_ALIGN_CENTER, 1, 1, LV_GRID_ALIGN_CENTER, 1, 1);
    applyButtonStyle(btnSettings, lv_color_hex(COLOR_PRIMARY));
    lv_obj_add_event_cb(btnSettings, onSettingsClick, LV_EVENT_CLICKED, NULL);
}

void ScreenHome::onExtrudeClick(lv_event_t* e) {
    Serial.println(F("[ScreenHome] Extrude button clicked"));
    if (g_uiManager) {
        g_uiManager->navigateTo(SCREEN_EXTRUDE);
    }
}

void ScreenHome::onMoveClick(lv_event_t* e) {
    Serial.println(F("[ScreenHome] Move button clicked"));
    // TODO: Navigate to move screen when implemented
}

void ScreenHome::onTempClick(lv_event_t* e) {
    Serial.println(F("[ScreenHome] Temperature button clicked"));
    // TODO: Navigate to temperature screen when implemented
}

void ScreenHome::onSettingsClick(lv_event_t* e) {
    Serial.println(F("[ScreenHome] Settings button clicked"));
    // TODO: Navigate to settings screen when implemented
}
