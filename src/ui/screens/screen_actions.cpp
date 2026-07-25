#include "ui/screens/screen_actions.h"
#include <Arduino.h>

extern UIManager* g_uiManager;

ScreenActions::ScreenActions()
    : extruderTemp(25.0)
    , bedTemp(27.0) {
    
    setScreenName("Actions");
}

void ScreenActions::load() {
    Serial.println(F("[ScreenActions] Loading..."));
    
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
    
    Serial.println(F("[ScreenActions] Loaded successfully"));
}

void ScreenActions::createLayout() {
    createStatusbar();
    createButtonGrid();
}

void ScreenActions::createStatusbar() {
    // Statusbar container
    statusbar = lv_obj_create(screen);
    lv_obj_set_size(statusbar, DISPLAY_WIDTH, 30);
    lv_obj_set_style_bg_color(statusbar, lv_color_hex(0x1A1A1A), LV_PART_MAIN);
    lv_obj_set_style_radius(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(statusbar, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(statusbar, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(statusbar, 0);
    
    // Back button (circle with arrow)
    lv_obj_t* btnBack = lv_btn_create(statusbar);
    lv_obj_set_size(btnBack, 20, 20);
    lv_obj_align(btnBack, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_bg_opa(btnBack, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btnBack, 1, LV_PART_MAIN);
    lv_obj_set_style_border_color(btnBack, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_obj_set_style_radius(btnBack, 10, LV_PART_MAIN);
    lv_obj_add_event_cb(btnBack, [](lv_event_t* e) {
        Serial.println(F("[ScreenActions] Back button clicked"));
        if (g_uiManager) {
            g_uiManager->goBack();
        }
    }, LV_EVENT_CLICKED, NULL);
    
    lv_obj_t* backIcon = lv_label_create(btnBack);
    lv_label_set_text(backIcon, "←");
    lv_obj_center(backIcon);
    lv_obj_set_style_text_font(backIcon, &lv_font_montserrat_14, LV_PART_MAIN);
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
    lv_obj_set_style_text_color(icon1, lv_color_hex(0xFF6B00), LV_PART_MAIN);
    
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
    lv_obj_set_style_text_color(icon2, lv_color_hex(0x20B2AA), LV_PART_MAIN);
    
    lblBedTemp = lv_label_create(tempContainer);
    lv_label_set_text_fmt(lblBedTemp, "%d°", (int)bedTemp);
    lv_obj_set_style_text_font(lblBedTemp, &lv_font_montserrat_12, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblBedTemp, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Title (center)
    lblTitle = lv_label_create(statusbar);
    lv_label_set_text(lblTitle, "Printer | Actions");
    lv_obj_align(lblTitle, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(lblTitle, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTitle, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Time (right)
    lblTime = lv_label_create(statusbar);
    lv_label_set_text(lblTime, "09:21");
    lv_obj_align(lblTime, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_font(lblTime, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(lblTime, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
}

void ScreenActions::createButtonGrid() {
    // Main button grid container
    buttonGrid = lv_obj_create(screen);
    lv_obj_set_size(buttonGrid, DISPLAY_WIDTH, DISPLAY_HEIGHT - 30);
    lv_obj_set_style_bg_opa(buttonGrid, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_pad_all(buttonGrid, 10, LV_PART_MAIN);
    lv_obj_set_flex_grow(buttonGrid, 1);
    
    // Use grid layout: 4 columns x 2 rows (adjustable)
    lv_obj_set_layout(buttonGrid, LV_LAYOUT_GRID);
    static lv_coord_t col_dsc[] = {55, 55, 55, 55, LV_GRID_TEMPLATE_LAST};
    static lv_coord_t row_dsc[] = {70, 70, 70, LV_GRID_TEMPLATE_LAST};
    lv_obj_set_grid_dsc_array(buttonGrid, col_dsc, row_dsc);
    lv_obj_set_style_grid_column_gap(buttonGrid, 5, 0);
    lv_obj_set_style_grid_row_gap(buttonGrid, 10, 0);
    lv_obj_set_grid_column_align(buttonGrid, LV_GRID_ALIGN_CENTER);
    lv_obj_set_grid_row_align(buttonGrid, LV_GRID_ALIGN_CENTER);
    
    // Row 1: Action buttons
    createActionButton(buttonGrid, "Move", lv_color_hex(0xFF6B00), 0, 0);
    createActionButton(buttonGrid, "Extrude", lv_color_hex(0xE91E63), 1, 0);
    createActionButton(buttonGrid, "Fan", lv_color_hex(0x20B2AA), 2, 0);
    createActionButton(buttonGrid, "Temperature", lv_color_hex(0xAEEA00), 3, 0);
    
    // Row 2: Utility buttons
    createActionButton(buttonGrid, "Macros", lv_color_hex(0xFF6B00), 0, 1);
    createActionButton(buttonGrid, "Disable\nMotors", lv_color_hex(0x20B2AA), 1, 1);
    createActionButton(buttonGrid, "Console", lv_color_hex(0xAEEA00), 2, 1);
    
    // Empty cell at (3,1) - optional 4th button
}

void ScreenActions::createActionButton(lv_obj_t* parent, const char* label, 
                                         lv_color_t barColor, lv_coord_t col, lv_coord_t row) {
    // Button container
    lv_obj_t* btn = lv_btn_create(parent);
    lv_obj_set_size(btn, 55, 65);
    lv_obj_set_grid_cell(btn, LV_GRID_ALIGN_CENTER, col, 1, LV_GRID_ALIGN_CENTER, row, 1);
    lv_obj_set_style_bg_opa(btn, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(btn, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(btn, 2, LV_PART_MAIN);
    
    // Icon (using Unicode characters as placeholders)
    lv_obj_t* icon = lv_label_create(btn);
    if (strcmp(label, "Move") == 0) {
        lv_label_set_text(icon, "");
    } else if (strcmp(label, "Extrude") == 0) {
        lv_label_set_text(icon, "◎");
    } else if (strcmp(label, "Fan") == 0) {
        lv_label_set_text(icon, "❂");
    } else if (strcmp(label, "Temperature") == 0) {
        lv_label_set_text(icon, "");
    } else if (strcmp(label, "Macros") == 0) {
        lv_label_set_text(icon, "📄");
    } else if (strcmp(label, "Disable\nMotors") == 0) {
        lv_label_set_text(icon, "⏻");
    } else if (strcmp(label, "Console") == 0) {
        lv_label_set_text(icon, ">_");
    } else {
        lv_label_set_text(icon, "?");
    }
    lv_obj_align(icon, LV_ALIGN_TOP_MID, 0, 5);
    lv_obj_set_style_text_font(icon, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(icon, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    
    // Label
    lv_obj_t* textLabel = lv_label_create(btn);
    lv_label_set_text(textLabel, label);
    lv_obj_align(textLabel, LV_ALIGN_CENTER, 0, 5);
    lv_obj_set_style_text_font(textLabel, &lv_font_montserrat_10, LV_PART_MAIN);
    lv_obj_set_style_text_color(textLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN);
    lv_label_set_long_mode(textLabel, LV_LABEL_LONG_WRAP);
    
    // Colored bar at bottom
    lv_obj_t* bar = lv_obj_create(btn);
    lv_obj_set_size(bar, 50, 4);
    lv_obj_align(bar, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_set_style_bg_color(bar, barColor, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 2, LV_PART_MAIN);
    lv_obj_set_style_border_width(bar, 0, LV_PART_MAIN);
    
    // Event callback
    lv_obj_add_event_cb(btn, [](lv_event_t* e) {
        const char* btnName = (const char*)lv_event_get_user_data(e);
        Serial.printf("[ScreenActions] Button clicked: %s\n", btnName);
        
        if (strcmp(btnName, "Move") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Move screen"));
            // TODO: g_uiManager->navigateTo(SCREEN_MOVE);
        } else if (strcmp(btnName, "Extrude") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Extrude screen"));
            // TODO: g_uiManager->navigateTo(SCREEN_EXTRUDE);
        } else if (strcmp(btnName, "Fan") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Fan screen"));
        } else if (strcmp(btnName, "Temperature") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Temperature screen"));
            // TODO: g_uiManager->navigateTo(SCREEN_TEMPERATURE);
        } else if (strcmp(btnName, "Macros") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Macros screen"));
        } else if (strcmp(btnName, "Disable\nMotors") == 0) {
            Serial.println(F("[ScreenActions] Disable motors clicked"));
            // TODO: Send G-code M18
        } else if (strcmp(btnName, "Console") == 0) {
            Serial.println(F("[ScreenActions] Navigate to Console screen"));
        }
    }, LV_EVENT_CLICKED, (void*)label);
}

void ScreenActions::onMoveClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Move clicked"));
}

void ScreenActions::onExtrudeClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Extrude clicked"));
}

void ScreenActions::onFanClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Fan clicked"));
}

void ScreenActions::onTemperatureClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Temperature clicked"));
}

void ScreenActions::onMacrosClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Macros clicked"));
}

void ScreenActions::onDisableMotorsClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Disable Motors clicked"));
}

void ScreenActions::onConsoleClick(lv_event_t* e) {
    Serial.println(F("[ScreenActions] Console clicked"));
}
